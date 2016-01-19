"""Fire Simulation Library for Fire on the Fringe SESYNC Pursuit"""

import sys
sys.dont_write_bytecode = True
import numpy as np
import numba
import math


# Aliases for more readable code
BARE = 0
INFLAMMABLE = 0
UNBURNED = 0


def fill_L(n_biomes, B, p, L):
    """Fill a LandCover (L) map from a Biome map (B).
    
    [PARAMETERS]
    n_biomes - int:
        The number of biomes in the simulation.
    
    B - 2d numpy.ndarray(int)
        Spatial grid of Biome classes for each pixel.
    
    p - tuple(tuple(int))
        The proportion of different landcover classes within each Biome.
        p[b][l] is the proportion of Biome 'b' that is made up of landcover
        component 'l'. Req.: len(p) == biomes and 
        
    L* - 2d numpy.ndarray(int)
        Land cover drawn from the distributions in t corresponding to each
        Biome type. Identical in shape to B.
    """
    for biome in range(n_biomes):
        proportions = p[biome] # Proportion of each land cover class in this biome
        y,x = np.where(B==biome)
        L[y,x] = np.random.choice(len(proportions),y.size,p=proportions)


def fill_A(n_covers, L, rngs, A):
    """Fill the Activation Energy map (A) with randomly generated values.
    
    [PARAMETERS]
    L - 2d numpy.ndarray(int)
        2d map of andcover component type (e.g. bare, grass, etc.)
        
    rngs - dict(key=int,value=callable)
        A dict of callable objects (e.g. functions). There should be one
        entry in the dictionary for each landcover component class. The 
        dictionary contains callables that takes an integer n and
        returns a 1 dimensional numpy.ndarray(float) that has size n and 
        contains random values.
    
    A* - 2d numpy.ndarray(int)
        The array to be filled with random activation energies based
        on the land cover component.
    """
    for lc in range(n_covers):
        y,x = np.where(L==lc)
        if len(y):
            A[y,x] = rngs[lc](y.size)


def fill_R(n_covers, L, rngs, R):
    """Fill the Release Energy map (R) with randomly generated values.
    
    [PARAMETERS]
    n_covers - int
        Max number of unique cover classes in L
    
    L - 2d numpy.ndarray(int)
        Map of landcover component type (e.g. bare, grass, etc.)
    
    rngs - tuple(one parameter callables) of len(n_covers)
        A sequence of callable objects (e.g. functions). There should
        be one entry in rngs for each landcover component class. Each
        callable in rngs should take one parameter, an int n, and should
        generate n random values corresponding to the land cover class.
        
    R* - 2d numpy.ndarray(int)
        The array to be filled with random release energies based
        on the land cover component and the distribution.
    """
    fill_A(n_covers,L,rngs,R)  # Same functionality as fill_A


@numba.jit(nopython=True)
def euclidean_distance(y1,x1,y2,x2):
    '''Find the Euclidean distance between two points'''
    return math.sqrt( ((x1-x2)*(x1-x2)) + ((y1-y2)*(y1-y2)) )


@numba.jit(nopython=True)
def weight_distance(dist):
    """How do we weight transmitted energy with distance?"""
    return 1/(dist*dist)


_MAX_KERNEL_RADIUS_ = 10
def find_kernel_denominator():
    """Find the denominator so that energy release integrates to 1."""
    total = 0
    for iy in range(-_MAX_KERNEL_RADIUS_,_MAX_KERNEL_RADIUS_+1):
        for ix in range(-_MAX_KERNEL_RADIUS_,_MAX_KERNEL_RADIUS_+1):
            if iy==0 and ix == 0:
                continue
            dist = euclidean_distance(0,0,iy,ix)
            if dist > _MAX_KERNEL_RADIUS_:
                continue
            total += weight_distance(dist)
    return total

_KERNEL_DENOMINATOR_ = find_kernel_denominator()  # Calc done on library import


@numba.jit(nopython=True)
def burn_next_active_pixel(fires,active,L,E,A,R,F):
    """Apply the Released Energy in a burnt pixel to neighbors.
    
    Arguments
        
        fires - tuple(numpy.ndarray(dtype=int))
            2-tuple of y and x coordinates of active or past fires
        active - tuple(ints)
            (start,end] coordinates of the currently burning fires
        L,E,A,R,F - Environmental fields (see Variable Descriptions).
    """
    Y, X = E.shape
    y, x = fires[0][active[0]], fires[1][active[0]]
    fire_time_step = F[y, x]
    
    for iy in range(y-_MAX_KERNEL_RADIUS_,y+_MAX_KERNEL_RADIUS_+1):
        if iy<0 or iy>=Y: continue
        for ix in range(x-_MAX_KERNEL_RADIUS_,x+_MAX_KERNEL_RADIUS_+1):
            
            # Bounds checking (or if this pixel has already been burned)
            if ix < 0 or ix >= X or (y == iy and x == ix) or (L[iy,ix] == 0) or F[iy,ix]:
                continue
            
            # If we're inside of max radius
            dist = euclidean_distance(y,x,iy,ix)
            if dist <= _MAX_KERNEL_RADIUS_:  # Apply the energy to the neighbor
                E[iy,ix] +=  R[y,x] * weight_distance(dist)/_KERNEL_DENOMINATOR_
            
            # If we've exceeded Activation, add this pixel to the burn list
            if E[iy,ix] > A[iy,ix]:
                fires[0][active[1]] = iy
                fires[1][active[1]] = ix
                F[iy,ix] = fire_time_step + 1
                active[1] += 1
    
    active[0] += 1  # Set the next pixel as 'next'
    return True


@numba.jit(nopython=True)
def burn_all(fires, active, L, E, A, R, F, n=-1):
    """Burn until there are no more active fires."""
    while n and (active[0] != active[1]):
        burn_next_active_pixel(fires, active, L, E, A, R, F)
        if n: n -= 1
        else: break
    return abs(n)


@numba.jit(nopython=True)
def ignite_fires(ignitions, fires, active, L, F):
    """Add active fires to the simulation.
    
    Parameters:
        locations - 2-tuple(numpy.ndarray(int))
            y,x locations for N ignition points.
        
        fires* - 2-tuple(numpy.ndarray(int))
            Tuple of y,x pixel coordinates of all fires. Ignitions will
            be added to the end of this list. cursor will be modified so
            that it points to one past the last new fire added to this list.
        
        cursor* - numpy.ndarray(int,shape=(2,))
            First and last+1 indices of active fires.
        
        F* - 2d numpy.array(bool)
            Map of burnt and burning pixels.
    """
    N = len(ignitions[0])
    n_actual = 0
    offset = active[1]
    for i in range(N):
        y, x = ignitions[0][i], ignitions[1][i]
        if L[y, x] == BARE or F[y, x]:
            continue
        n_actual += 1
        fires[0][i+offset] = y
        fires[1][i+offset] = x
        F[y, x] = True
        active[1] += 1
    return n_actual


def fire_map_to_list(L,F):
    """Given a map of fires, build a list of fires and a active."""
    fires = np.zeros(shape=(F.size),dtype=int), np.zeros(shape=(F.size),dtype=int)
    F[L == BARE] = False # Cannot light bare ground
    y,x = np.where(F)
    N = y.size
    fires[:N,0] = y
    fires[:N,1] = x
    cursor = np.array([0,N])
    F[:] = False
    return fires, cursor


def parameterize_truncate_distribution(distr,p1,p2):
    """Parameterize a two-parameter distribution.
    
    Returns a parameterized distribution that takes one parameter:
        n - The number of samples to draw.
    """
    def truncated_rng(n):
        """Truncated, parameterized distribution."""
        x = distr(p1,p2,size=n)
        x[x<0] = 0
        return x
    
    return truncated_rng

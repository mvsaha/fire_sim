import sys
sys.dont_write_bytecode = True

from utils.index import *

import numpy as np
from numba import jit
import math

@jit(nopython=True)
def dot2d(v0,v1):
    '''Return the dot product of two 2-dimensional vectors.'''
    return (v0[0]*v1[0]) + (v0[1]+v1[1])

@jit(nopython=True)
def dot3d(v0,v1):
    '''Return the dot product of two 3-dimensional vectors.'''
    return (v0[0]*v1[0]) + (v0[1]+v1[1]) + (v0[2]+v1[2])

@jit(nopython=True)
def dot(v0,v1,dims):
    '''Return the dot product of two dims-dimensional vectors.'''
    total = 0
    for d in range(dims):
        print(total,v0[d],v1[d])
        total += v0[d]*v1[d]
    return total

@jit(nopython=True)
def distance(p0,p1,dims):
    '''Find the Euclidean distance between two d-dimensional points.'''
    total = 0
    for i in range(dims):
        total += p0[i] - p1[i]
    return math.sqrt(total)

@jit
def distance_idx(i0,i1,stride):
    '''
    Find the Euclidean distance between two linear indices.
    '''
    total = 0
    ndims = stride.shape[0]
    for d in range(0,ndims):
        di0, di1 = int(i0/stride[d]), int(i1/stride[d])
        #print(di0,di1)
        total += (di0-di1)**2
        i0 -= di0*stride[d]
        i1 -= di1*stride[d]
    return math.sqrt(total)

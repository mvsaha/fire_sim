import sys
sys.dont_write_bytecode = True

import numpy as np
from numba import jit

def dims2stride(dims):
    '''Return the stride of an n-dimensional array.

    [PARAMETERS]
    dims - 1-dimensional numpy.ndarray(dtype=int)
        The dimension of the array

    [RETURNS]
    stride - np.ndarray identical in size to dims
        The stride of an axis represents the number of elements
        that are transversed when the index of that axis is
        incremented by one.
    '''
    stride = np.zeros_like(dims)
    s = 1
    for i in range(len(stride)-1,-1,-1):
        stride[i] = s
        s *= dims[i]
    return stride

@jit(nopython=True)
def ind2sub(i,stride,sub):
    '''Convert a linear index into a subscripted index.

    [PARAMETERS]
    i - int
        The index to be converted

    stride - numpy.array(int) with n elements
        The stride of each axis (see dims2stride)s

    sub - numpy.array(int) with n elements
        The output subscripts where the result will be stored.

    [NOTES]
    This function does not bounds check. If i is greater than the number
    of elements in the matrix described by stride no error will be thrown.
    '''
    ndims = stride.shape[0]
    for d in range(0,ndims):
        sub[d] = i/stride[d]
        i -= sub[d]*stride[d]

@jit(nopython=True)
def update_idx( idx, shp, ndims):
    '''Iterate one step over a multidimensional index.'''
    for dim in range(ndims-1,-1,-1):
        idx[dim] += 1
        if idx[dim] == shp[dim]:
            idx[dim] = 0
            continue
        else:
            return

def indices(mat):
    '''Generate for indices over a numpy array.'''
    shp = np.array(mat.shape,dtype=int)
    n = np.prod(shp)
    ndims = len(shp)
    idx = np.zeros_like(shp,dtype=int)
    for x in range(n):
        yield tuple(idx)
        update_idx(idx,shp,ndims)

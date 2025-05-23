#!@PYTHON@

#
# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.
#

import os
import sys
from pathlib import Path

from imath import *

import imathnumpy
import numpy as np

testList = []

def numpyType(ia):
    podTypeDict = {
        SignedCharArray :    np.int8,
        UnsignedCharArray :  np.uint8,
        ShortArray :         np.int16,
        UnsignedShortArray : np.uint16,
        IntArray :           np.int32,
        UnsignedIntArray :   np.uint32,
        FloatArray :         np.float32,
        DoubleArray :        np.float64,
        V2sArray :           np.int16,
        V2iArray :           np.int32,
        V2fArray :           np.float32,
        V2dArray :           np.float64,
        V3sArray :           np.int16,
        V3iArray :           np.int32,
        V3fArray :           np.float32,
        V3dArray :           np.float64,
        V4sArray :           np.int16,
        V4iArray :           np.int32,
        V4fArray :           np.float32,
        V4dArray :           np.float64,
        C3cArray :           np.uint8,
        C3fArray :           np.float32,
        C4cArray :           np.uint8,
        C4fArray :           np.float32,
        IntArray2D :         np.int32,
        FloatArray2D :       np.float32,
        DoubleArray2D :      np.float64,
        Color4cArray2D :     np.uint8,
        Color4fArray2D :     np.float32,
    }

    return podTypeDict[type(ia)]

def baseTypeSize(ia):
    baseTypeSizeDict = {
        SignedCharArray :    1,
        UnsignedCharArray :  1,
        ShortArray :         1,
        UnsignedShortArray : 1,
        IntArray :           1,
        UnsignedIntArray :   1,
        FloatArray :         1,
        DoubleArray :        1,
        V2sArray :           2,
        V2iArray :           2,
        V2fArray :           2,
        V2dArray :           2,
        V3sArray :           3,
        V3iArray :           3,
        V3fArray :           3,
        V3dArray :           3,
        V4sArray :           4,
        V4iArray :           4,
        V4fArray :           4,
        V4dArray :           4,
        C3cArray :           3,
        C3fArray :           3,
        C4cArray :           4,
        C4fArray :           4,
        Color4cArray2D :     4,
        Color4fArray2D :     4
    }

    return baseTypeSizeDict[type(ia)]


###############################################################################

def testScalarArrayToNumpy():
    '''
    Tests the wrapping of PyImath scalar array types to numpy arrays.
    '''
    aDim = 20

    # Test all supported array types
    arrayTestTypes = [
        SignedCharArray,
        UnsignedCharArray,
        ShortArray,
        UnsignedShortArray,
        IntArray,
        UnsignedIntArray,
        FloatArray,
        DoubleArray]

    for arrayType in arrayTestTypes:
        print("\nTesting: %s conversion to numpy" % arrayType.__name__)

        ia = arrayType(aDim)
        refType  = numpyType(ia)
        refShape = (aDim,)
        
        for i in range(aDim):
            ia[i] = i
        na = imathnumpy.arrayToNumpy(ia)
        
        print("%s == %s" % (str(na.dtype), str(refType)))
        assert(na.dtype == refType)

        print("%s == %s" % (str(na.shape), str(refShape)))
        assert(na.shape == refShape)

        for i in range(aDim):
            assert(na[i] == ia[i])
        print(na)

testList.append(testScalarArrayToNumpy)

###############################################################################

def testVectorArrayToNumpy():
    '''
    Tests the wrapping of PyImath vector array types to numpy arrays.
    '''
    aDim = 20

    # Test all supported array types
    arrayTestTypes = [
        V2sArray,
        V2iArray,
        V2fArray,
        V2dArray,
        V3sArray,
        V3iArray,
        V3fArray,
        V3dArray,
        V4sArray,
        V4iArray,
        V4fArray,
        V4dArray,
        C3cArray,
        C3fArray,
        C4cArray,
        C4fArray]

    for arrayType in arrayTestTypes:
        print("\nTesting: %s conversion to numpy" % arrayType.__name__)

        ia = arrayType(aDim)
        refType         = numpyType(ia)
        refBaseTypeSize = baseTypeSize(ia)
        refShape        = (aDim, refBaseTypeSize)

        for i in range(aDim):
            for j in range(refBaseTypeSize):
                ia[i][j] = i + 2 ** j
        na = imathnumpy.arrayToNumpy(ia)
        
        print("%s == %s" % (str(na.dtype), str(refType)))
        assert(na.dtype == refType)

        print("%s == %s" % (str(na.shape), str(refShape)))
        assert(na.shape == refShape)

        for i in range(aDim):
            for j in range(refBaseTypeSize):
                assert(na[i][j] == ia[i][j])
        print(na)

testList.append(testVectorArrayToNumpy)

###############################################################################

def testScalarArray2DToNumpy():
    '''
    Tests the wrapping of PyImath scalar2D array types to numpy arrays.
    '''
    aDim1 = 10
    aDim2 = 5

    # Test all supported array types
    arrayTestTypes = [
        IntArray2D,
        FloatArray2D,
        DoubleArray2D]

    for arrayType in arrayTestTypes:
        print("\nTesting: %s conversion to numpy" % arrayType.__name__)

        ia = arrayType(aDim1, aDim2)
        refType  = numpyType(ia)
        refShape = (aDim2, aDim1)

        for y in range(aDim2):
            for x in range(aDim1):
                ia[(x, y)] = x + 2 * y
        na = imathnumpy.arrayToNumpy(ia)

        print("%s == %s" % (str(na.dtype), str(refType)))
        assert(na.dtype == refType)

        print("%s == %s" % (str(na.shape), str(refShape)))
        assert(na.shape == refShape)

        for y in range(aDim2):
            for x in range(aDim1):
                assert(na[y][x] == ia.item(x, y))
        print(na)

testList.append(testScalarArray2DToNumpy)

###############################################################################

def testVectorArray2DToNumpy():
    '''
    Tests the wrapping of PyImath vector2D array types to numpy arrays.
    '''
    aDim1 = 10
    aDim2 = 5

    # Test all supported array types
    arrayTestTypes = [
        Color4cArray2D,
        Color4fArray2D]

    for arrayType in arrayTestTypes:
        print("\nTesting: %s conversion to numpy" % arrayType.__name__)

        ia = arrayType(aDim1, aDim2)
        refType         = numpyType(ia)
        refBaseTypeSize = baseTypeSize(ia)
        refShape        = (aDim2, aDim1, refBaseTypeSize)

        for y in range(aDim2):
            for x in range(aDim1):
                for k in range(refBaseTypeSize):
                    ia.item(x, y)[k] = x + 2 * y + k
        na = imathnumpy.arrayToNumpy(ia)

        print("%s == %s" % (str(na.dtype), str(refType)))
        assert(na.dtype == refType)

        print("%s == %s" % (str(na.shape), str(refShape)))
        assert(na.shape == refShape)

        for y in range(aDim2):
            for x in range(aDim1):
                for k in range(refBaseTypeSize):
                    assert(na[y][x][k] == ia.item(x, y)[k])
        print(na)

testList.append(testVectorArray2DToNumpy)

###############################################################################

def testBufferProtocol():
    '''
    Numpy arrays and FixedArrays both now support the Python buffer protocol.
    This test will exercise the FixedArray implementation of the protocol
    against numpy arrays since only very limited testing of the protocol is
    possible in PyImathTest .
    '''
    def verify (x, X, factor=1., shared=True):

        assert (len(x) == len(X))
        for i in range(10):
            #  Make sure each element is the proper factor of the index, this
            # will help make sure all the indexing and striding is correct.
            assert (X[i] == i*factor)
            assert (X[i] == x[i])
        
        #  Buffers can be shared, so modifying the imath array should also
        # be operating on the numpy array...
        for i in range(10):
            x[i] *= 10
        for i in range(10):
            assert ((shared and (X[i] == x[i])) or (not shared and (X[i]*10 == x[i])))


    def verifyVector (v, V, cloned=False):

        assert (len(v) == len(V))
        for i in range(10):
            assert (len(v[i]) == len(V[i]))
            for j in range(len(v[i])): 
                assert (V[i][j] == v[i][j])
        
        #  Buffers should be shared, so modifying the imath array should also
        # be operating on the numpy array...
        for i in range(10):
            v[i] *= 10
        for i in range(10):
            for j in range(len(v[i])): 
                assert ((not cloned and (V[i][j] == v[i][j])) or (cloned and (V[i][j]*10 == v[i][j])))

        #  Verify that imath striding is properly mapped to numpy...
        # Note that we modified the elements by a factor of 10 above, so when
        # we check each element against the index we need to account for this.
        Vx = np.asarray (v.x)
        Vy = np.asarray (v.y)
        if cloned:
            # Factors will have already been applied...
            factor = 1000.
        else:
            factor = 10.
        verify (v.x, Vx, factor, shared=True)
        verify (v.y, Vy, factor*10., shared=True)

        if (len(v[0]) == 3):
            Vz = np.asarray (v.z)
            verify (v.z, Vz, factor*10.*10., shared=True)

    # .........................................................................

    a = UnsignedCharArray (10)
    for i in range(10):
        a[i] = i
    aNP = np.asarray (a)
    verify (a, aNP)

    a = IntArray (10)
    for i in range(10):
        a[i] = i
    aNP = np.asarray (a)
    verify (a, aNP)

    #  Use the BufferProtocol to construct a deepcopy of the numpy
    # array as a FixedArray.
    aNPclone = IntArrayFromBuffer (aNP)
    verify (aNPclone, aNP, factor=10., shared=False)

    a = FloatArray (10)
    for i in range(10):
        a[i] = float(i)
    aNP = np.asarray (a)
    verify (a, aNP)
    
    #  Use the BufferProtocol to construct a deepcopy of the numpy
    # array as a FixedArray.
    aNPclone = FloatArrayFromBuffer (aNP)
    verify (aNPclone, aNP, factor=10., shared=False)

    a = DoubleArray (10)
    for i in range(10):
        a[i] = float(i)
    aNP = np.asarray (a)
    verify (a, aNP)
    
    #  Use the BufferProtocol to construct a deepcopy of the numpy
    # array as a FixedArray.
    aNPclone = DoubleArrayFromBuffer (aNP)
    verify (aNPclone, aNP, factor=10., shared=False)

    v = V2iArray (10)
    for i in range(10):
        v[i] = V2i (i,i*10)
    vNP = np.asarray (v)
    verifyVector (v, vNP)

    #  Use the BufferProtocol to construct a deepcopy of the numpy
    # array as a FixedArray.
    vNPclone = V2iArrayFromBuffer (vNP)
    verifyVector (vNPclone, vNP, cloned=True)

    v = V2fArray (10)
    for i in range(10):
        v[i] = V2f (i,i*10)
    vNP = np.asarray (v)
    verifyVector (v, vNP)

    #  Use the BufferProtocol to construct a deepcopy of the numpy
    # array as a FixedArray.
    vNPclone = V2fArrayFromBuffer (vNP)
    verifyVector (vNPclone, vNP, cloned=True)

    v = V2dArray (10)
    for i in range(10):
        v[i] = V2d (i,i*10)
    vNP = np.asarray (v)
    verifyVector (v, vNP)

    #  Use the BufferProtocol to construct a deepcopy of the numpy
    # array as a FixedArray.
    vNPclone = V2dArrayFromBuffer (vNP)
    verifyVector (vNPclone, vNP, cloned=True)

    v = V3iArray (10)
    for i in range(10):
        v[i] = V3i (i,i*10,i*100)
    vNP = np.asarray (v)
    verifyVector (v, vNP)

    #  Use the BufferProtocol to construct a deepcopy of the numpy
    # array as a FixedArray.
    vNPclone = V3iArrayFromBuffer (vNP)
    verifyVector (vNPclone, vNP, cloned=True)

    v = V3fArray (10)
    for i in range(10):
        v[i] = V3f (i,i*10,i*100)
    vNP = np.asarray (v)
    verifyVector (v, vNP)

    #  Use the BufferProtocol to construct a deepcopy of the numpy
    # array as a FixedArray.
    vNPclone = V3fArrayFromBuffer (vNP)
    verifyVector (vNPclone, vNP, cloned=True)

    v = V3dArray (10)
    for i in range(10):
        v[i] = V3d (i,i*10,i*100)
    vNP = np.asarray (v)
    verifyVector (v, vNP)

    #  Use the BufferProtocol to construct a deepcopy of the numpy
    # array as a FixedArray.
    vNPclone = V3dArrayFromBuffer (vNP)
    verifyVector (vNPclone, vNP, cloned=True)

testList.append (testBufferProtocol)

# -------------------------------------------------------------------------
# Main loop

for test in testList:
    print("")
    print("Running %s" % test.__name__)
    test()

print("done.")

# Local Variables:
# mode:python
# End:

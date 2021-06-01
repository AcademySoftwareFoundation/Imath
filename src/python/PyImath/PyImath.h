//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef _PyImath_h_
#define _PyImath_h_

#include <ImathVec.h>
#include <ImathQuat.h>
#include <PyImathAPI.h>
#include "PyImathFixedArray.h"
#include "PyImathFixedMatrix.h"
#include "PyImathFixedArray2D.h"
#include "PyImathFixedVArray.h"

namespace PyImath {

typedef FixedArray<bool> BoolArray;
typedef FixedArray<signed char> SignedCharArray;
typedef FixedArray<unsigned char> UnsignedCharArray;
typedef FixedArray<short> ShortArray;
typedef FixedArray<unsigned short> UnsignedShortArray;
typedef FixedArray<int> IntArray;
typedef FixedArray<unsigned int> UnsignedIntArray;
typedef FixedArray<float> FloatArray;
typedef FixedArray<double> DoubleArray;

typedef FixedArray<IMATH_NAMESPACE::Quatf> QuatfArray;
typedef FixedArray<IMATH_NAMESPACE::Quatd> QuatdArray;

typedef FixedMatrix<int> IntMatrix;
typedef FixedMatrix<float> FloatMatrix;
typedef FixedMatrix<double> DoubleMatrix;

typedef FixedArray2D<float> FloatArray2D;
typedef FixedArray2D<int> IntArray2D;
typedef FixedArray2D<double> DoubleArray2D;

typedef FixedVArray<int> VIntArray;
typedef FixedVArray<float> VFloatArray;
typedef FixedVArray<Imath::Vec2<int> > VV2iArray;
typedef FixedVArray<Imath::Vec2<float> > VV2fArray;

}

#endif

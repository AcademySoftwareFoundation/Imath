//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifndef INCLUDED_IMATHFORWARD_H
#define INCLUDED_IMATHFORWARD_H

#include "ImathNamespace.h"
#include "ImathExport.h"

IMATH_INTERNAL_NAMESPACE_HEADER_ENTER

/// @cond Doxygen_Suppress

//
// Basic template type declarations.
//

template <class T> class IMATH_EXPORT_TEMPLATE_TYPE Box;
template <class T> class IMATH_EXPORT_TEMPLATE_TYPE Color3;
template <class T> class IMATH_EXPORT_TEMPLATE_TYPE Color4;
template <class T> class IMATH_EXPORT_TEMPLATE_TYPE Euler;
template <class T> class IMATH_EXPORT_TEMPLATE_TYPE Frustum;
template <class T> class IMATH_EXPORT_TEMPLATE_TYPE FrustumTest;
template <class T> class IMATH_EXPORT_TEMPLATE_TYPE Interval;
template <class T> class IMATH_EXPORT_TEMPLATE_TYPE Line3;
template <class T> class IMATH_EXPORT_TEMPLATE_TYPE Matrix33;
template <class T> class IMATH_EXPORT_TEMPLATE_TYPE Matrix44;
template <class T> class IMATH_EXPORT_TEMPLATE_TYPE Plane3;
template <class T> class IMATH_EXPORT_TEMPLATE_TYPE Quat;
template <class T> class IMATH_EXPORT_TEMPLATE_TYPE Shear6;
template <class T> class IMATH_EXPORT_TEMPLATE_TYPE Sphere3;
template <class T> class IMATH_EXPORT_TEMPLATE_TYPE TMatrix;
template <class T> class IMATH_EXPORT_TEMPLATE_TYPE TMatrixBase;
template <class T> class IMATH_EXPORT_TEMPLATE_TYPE TMatrixData;
template <class T> class IMATH_EXPORT_TEMPLATE_TYPE Vec2;
template <class T> class IMATH_EXPORT_TEMPLATE_TYPE Vec3;
template <class T> class IMATH_EXPORT_TEMPLATE_TYPE Vec4;

class Rand32;
class Rand48;

/// @endcond

IMATH_INTERNAL_NAMESPACE_HEADER_EXIT

#endif // INCLUDED_IMATHFORWARD_H

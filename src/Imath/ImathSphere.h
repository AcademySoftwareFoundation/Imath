//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifndef INCLUDED_IMATHSPHERE_H
#define INCLUDED_IMATHSPHERE_H

//-------------------------------------
//
//	A 3D sphere class template
//
//-------------------------------------

#include "ImathBox.h"
#include "ImathLine.h"
#include "ImathNamespace.h"
#include "ImathVec.h"

IMATH_INTERNAL_NAMESPACE_HEADER_ENTER

template <class T> class Sphere3
{
  public:
    Vec3<T> center;
    T radius;

    //---------------
    //	Constructors
    //---------------

    IMATH_HOSTDEVICE constexpr Sphere3() : center (0, 0, 0), radius (0) {}
    IMATH_HOSTDEVICE constexpr Sphere3 (const Vec3<T>& c, T r) : center (c), radius (r) {}

    //-------------------------------------------------------------------
    //	Utilities:
    //
    //	s.circumscribe(b)	sets center and radius of sphere s
    //				so that the s tightly encloses box b.
    //
    //	s.intersectT (l, t)	If sphere s and line l intersect, then
    //				intersectT() computes the smallest t,
    //				t >= 0, so that l(t) is a point on the
    //				sphere.  intersectT() then returns true.
    //
    //				If s and l do not intersect, intersectT()
    //				returns false.
    //
    //	s.intersect (l, i)	If sphere s and line l intersect, then
    //				intersect() calls s.intersectT(l,t) and
    //				computes i = l(t).
    //
    //				If s and l do not intersect, intersect()
    //				returns false.
    //
    //-------------------------------------------------------------------

    IMATH_HOSTDEVICE void circumscribe (const Box<Vec3<T>>& box);
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool
    intersect (const Line3<T>& l, Vec3<T>& intersection) const;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool intersectT (const Line3<T>& l, T& t) const;
};

//--------------------
// Convenient typedefs
//--------------------

typedef Sphere3<float> Sphere3f;
typedef Sphere3<double> Sphere3d;

//---------------
// Implementation
//---------------

template <class T>
inline void
Sphere3<T>::circumscribe (const Box<Vec3<T>>& box)
{
    center = T (0.5) * (box.min + box.max);
    radius = (box.max - center).length();
}

template <class T>
IMATH_CONSTEXPR14 bool
Sphere3<T>::intersectT (const Line3<T>& line, T& t) const
{
    bool doesIntersect = true;

    Vec3<T> v = line.pos - center;
    T B       = T (2.0) * (line.dir ^ v);
    T C       = (v ^ v) - (radius * radius);

    // compute discriminant
    // if negative, there is no intersection

    T discr = B * B - T (4.0) * C;

    if (discr < 0.0)
    {
        // line and Sphere3 do not intersect

        doesIntersect = false;
    }
    else
    {
        // t0: (-B - sqrt(B^2 - 4AC)) / 2A  (A = 1)

        T sqroot = std::sqrt (discr);
        t        = (-B - sqroot) * T (0.5);

        if (t < 0.0)
        {
            // no intersection, try t1: (-B + sqrt(B^2 - 4AC)) / 2A  (A = 1)

            t = (-B + sqroot) * T (0.5);
        }

        if (t < 0.0)
            doesIntersect = false;
    }

    return doesIntersect;
}

template <class T>
IMATH_CONSTEXPR14 bool
Sphere3<T>::intersect (const Line3<T>& line, Vec3<T>& intersection) const
{
    T t;

    if (intersectT (line, t))
    {
        intersection = line (t);
        return true;
    }
    else
    {
        return false;
    }
}

IMATH_INTERNAL_NAMESPACE_HEADER_EXIT

#endif // INCLUDED_IMATHSPHERE_H

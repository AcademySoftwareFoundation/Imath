///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2004-2012, Industrial Light & Magic, a division of Lucas
// Digital Ltd. LLC
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *       Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *       Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *       Neither the name of Industrial Light & Magic nor the names of
// its contributors may be used to endorse or promote products derived
// from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_IMATHBOX_H
#define INCLUDED_IMATHBOX_H

#include "ImathNamespace.h"
#include "ImathVec.h"

IMATH_INTERNAL_NAMESPACE_HEADER_ENTER

///
/// The `Box<V>` template represents an axis-aligned bounding box defined by
/// minimum and maximum values of type `V`. The `min` and `max` members are
/// public.
///
/// The type `V` is typically an Imath vector (i.e. `V2i`, `V3f`, etc) and must
/// implement an index `operator[]` that returns a type (typically as scalar)
/// that supports assignment, comparison, and arithmetic operators.
///
/// `V` must also provide a constructor that takes a float and/or double for
/// use in initializing the box.
///
/// `V` must also provide a function `V::dimensions()` which returns the
/// number of dimensions in the class (since its assumed its a vector) --
/// preferably, this returns a constant expression, typically `2` or `3`.
///

template <class V> class Box
{
  public:
    //-------------------------
    //  Data Members are public
    //-------------------------

    /// The minimum value of the box.
    V min;

    /// The maximum value of the box.
    V max;

    //-----------------------------------------------------
    //	Constructors - an "empty" box is created by default
    //-----------------------------------------------------

    /// Construct an empty bounding box. This initializes the mimimum to
    /// `V::baseTypeMax()` and the maximum to `V::baseTypeMin()`.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box() noexcept;

    /// Construct a bounding box that contains a single point.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box (const V& point) noexcept;

    /// Construct a bounding box with the given minimum and maximum values.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box (const V& minV, const V& maxV) noexcept;

    //--------------------
    //  Operators:  ==, !=
    //--------------------

    /// Compare two Box objects for equality.
    IMATH_HOSTDEVICE constexpr bool operator== (const Box<V>& src) const noexcept;

    /// Compare two Box objects for inequality.
    IMATH_HOSTDEVICE constexpr bool operator!= (const Box<V>& src) const noexcept;

    //------------------
    //	Box manipulation
    //------------------

    /// Set the Box to be empty. A Box is empty if the mimimum is greater
    /// than the maximum. makeEmpty() sets the mimimum to `V::baseTypeMax()`
    /// and the maximum to `V::baseTypeMin()`.
    IMATH_HOSTDEVICE void makeEmpty() noexcept;

    /// Extend the Box to include the given point.
    IMATH_HOSTDEVICE void extendBy (const V& point) noexcept;

    /// Extend the Box to include the given box.
    IMATH_HOSTDEVICE void extendBy (const Box<V>& box) noexcept;

    /// Make the box include the entire range of V.
    IMATH_HOSTDEVICE void makeInfinite() noexcept;

    //---------------------------------------------------
    //	Query functions - these compute results each time
    //---------------------------------------------------

    /// Return the size of the box. The size is of type `V`, defined as
    /// (max-min). An empty box has a size of V(0), i.e. 0 in each dimension.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 V size() const noexcept;

    /// Return the center of the box. The center is defined as
    /// (max+min)/2. The center of an empty box is undefined.
    IMATH_HOSTDEVICE constexpr V center() const noexcept;

    /// Return true if the given point is inside the box, false otherwise.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool intersects (const V& point) const noexcept;

    /// Return true if the given box is inside the box, false otherwise.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool intersects (const Box<V>& box) const noexcept;

    /// Return the major axis of the box. The major axis is the dimension with
    /// the greatest difference between maximum and minimum.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 unsigned int majorAxis() const noexcept;

    //----------------
    //	Classification
    //----------------

    /// Return true if the box is empty, false otherwise. An empty box's
    /// minimum is greater than its maximum.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool isEmpty() const noexcept;

    /// Return true if the box is larger than a single point, false otherwise.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool hasVolume() const noexcept;

    /// Return true if the box contains all points, false otherwise.
    /// An infinite box has a mimimum of`V::baseTypeMin()`
    /// and a maximum of `V::baseTypeMax()`.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool isInfinite() const noexcept;
};

//--------------------
// Convenient typedefs
//--------------------

/// 2D Box of base type `short`.
typedef Box<V2s> Box2s;

/// 2D Box of base type `int`.
typedef Box<V2i> Box2i;

/// 2D Box of base type `float`.
typedef Box<V2f> Box2f;

/// 2D Box of base type `double`.
typedef Box<V2d> Box2d;

/// 3D Box of base type `short`.
typedef Box<V3s> Box3s;

/// 3D Box of base type `int`.
typedef Box<V3i> Box3i;

/// 3D Box of base type `float`.
typedef Box<V3f> Box3f;

/// 3D Box of base type `double`.
typedef Box<V3d> Box3d;

//----------------
//  Implementation
//----------------

template <class V> IMATH_CONSTEXPR14 inline Box<V>::Box() noexcept
{
    makeEmpty();
}

template <class V> IMATH_CONSTEXPR14 inline Box<V>::Box (const V& point) noexcept
{
    min = point;
    max = point;
}

template <class V> IMATH_CONSTEXPR14 inline Box<V>::Box (const V& minV, const V& maxV) noexcept
{
    min = minV;
    max = maxV;
}

template <class V>
constexpr inline bool
Box<V>::operator== (const Box<V>& src) const noexcept
{
    return (min == src.min && max == src.max);
}

template <class V>
constexpr inline bool
Box<V>::operator!= (const Box<V>& src) const noexcept
{
    return (min != src.min || max != src.max);
}

template <class V>
inline void
Box<V>::makeEmpty() noexcept
{
    min = V (V::baseTypeMax());
    max = V (V::baseTypeMin());
}

template <class V>
inline void
Box<V>::makeInfinite() noexcept
{
    min = V (V::baseTypeMin());
    max = V (V::baseTypeMax());
}

template <class V>
inline void
Box<V>::extendBy (const V& point) noexcept
{
    for (unsigned int i = 0; i < min.dimensions(); i++)
    {
        if (point[i] < min[i])
            min[i] = point[i];

        if (point[i] > max[i])
            max[i] = point[i];
    }
}

template <class V>
inline void
Box<V>::extendBy (const Box<V>& box) noexcept
{
    for (unsigned int i = 0; i < min.dimensions(); i++)
    {
        if (box.min[i] < min[i])
            min[i] = box.min[i];

        if (box.max[i] > max[i])
            max[i] = box.max[i];
    }
}

template <class V>
IMATH_CONSTEXPR14 inline bool
Box<V>::intersects (const V& point) const noexcept
{
    for (unsigned int i = 0; i < min.dimensions(); i++)
    {
        if (point[i] < min[i] || point[i] > max[i])
            return false;
    }

    return true;
}

template <class V>
IMATH_CONSTEXPR14 inline bool
Box<V>::intersects (const Box<V>& box) const noexcept
{
    for (unsigned int i = 0; i < min.dimensions(); i++)
    {
        if (box.max[i] < min[i] || box.min[i] > max[i])
            return false;
    }

    return true;
}

template <class V>
IMATH_CONSTEXPR14 inline V
Box<V>::size() const noexcept
{
    if (isEmpty())
        return V (0);

    return max - min;
}

template <class V>
constexpr inline V
Box<V>::center() const noexcept
{
    return (max + min) / 2;
}

template <class V>
IMATH_CONSTEXPR14 inline bool
Box<V>::isEmpty() const noexcept
{
    for (unsigned int i = 0; i < min.dimensions(); i++)
    {
        if (max[i] < min[i])
            return true;
    }

    return false;
}

template <class V>
IMATH_CONSTEXPR14 inline bool
Box<V>::isInfinite() const noexcept
{
    for (unsigned int i = 0; i < min.dimensions(); i++)
    {
        if (min[i] != V::baseTypeMin() || max[i] != V::baseTypeMax())
            return false;
    }

    return true;
}

template <class V>
IMATH_CONSTEXPR14 inline bool
Box<V>::hasVolume() const noexcept
{
    for (unsigned int i = 0; i < min.dimensions(); i++)
    {
        if (max[i] <= min[i])
            return false;
    }

    return true;
}

template <class V>
IMATH_CONSTEXPR14 inline unsigned int
Box<V>::majorAxis() const noexcept
{
    unsigned int major = 0;
    V s                = size();

    for (unsigned int i = 1; i < min.dimensions(); i++)
    {
        if (s[i] > s[major])
            major = i;
    }

    return major;
}

//-------------------------------------------------------------------
//
//  Partial class specializations for Imath::Vec2<T> and Imath::Vec3<T>
//
//-------------------------------------------------------------------

template <typename V> class Box;

///
/// The Box<Vec2<T>> template represents a 2D bounding box defined by
/// minimum and maximum values of type Vec2<T>. The min and max members are
/// public.
///

template <class T> class Box<Vec2<T>>
{
  public:
    //-------------------------
    //  Data Members are public
    //-------------------------

    Vec2<T> min;
    Vec2<T> max;

    //-----------------------------------------------------
    //  Constructors - an "empty" box is created by default
    //-----------------------------------------------------

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box() noexcept;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box (const Vec2<T>& point) noexcept;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box (const Vec2<T>& minT, const Vec2<T>& maxT) noexcept;

    //--------------------
    //  Operators:  ==, !=
    //--------------------

    IMATH_HOSTDEVICE constexpr bool operator== (const Box<Vec2<T>>& src) const noexcept;
    IMATH_HOSTDEVICE constexpr bool operator!= (const Box<Vec2<T>>& src) const noexcept;

    //------------------
    //  Box manipulation
    //------------------

    IMATH_HOSTDEVICE void makeEmpty() noexcept;
    IMATH_HOSTDEVICE void extendBy (const Vec2<T>& point) noexcept;
    IMATH_HOSTDEVICE void extendBy (const Box<Vec2<T>>& box) noexcept;
    IMATH_HOSTDEVICE void makeInfinite() noexcept;

    //---------------------------------------------------
    //  Query functions - these compute results each time
    //---------------------------------------------------

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Vec2<T> size() const noexcept;
    IMATH_HOSTDEVICE constexpr Vec2<T> center() const noexcept;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool intersects (const Vec2<T>& point) const noexcept;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool intersects (const Box<Vec2<T>>& box) const noexcept;

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 unsigned int majorAxis() const noexcept;

    //----------------
    //  Classification
    //----------------

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool isEmpty() const noexcept;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool hasVolume() const noexcept;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool isInfinite() const noexcept;
};

//----------------
//  Implementation

template <class T> IMATH_CONSTEXPR14 inline Box<Vec2<T>>::Box() noexcept
{
    makeEmpty();
}

template <class T> IMATH_CONSTEXPR14 inline Box<Vec2<T>>::Box (const Vec2<T>& point) noexcept
{
    min = point;
    max = point;
}

template <class T>
IMATH_CONSTEXPR14 inline Box<Vec2<T>>::Box (const Vec2<T>& minT, const Vec2<T>& maxT) noexcept
{
    min = minT;
    max = maxT;
}

template <class T>
constexpr inline bool
Box<Vec2<T>>::operator== (const Box<Vec2<T>>& src) const noexcept
{
    return (min == src.min && max == src.max);
}

template <class T>
constexpr inline bool
Box<Vec2<T>>::operator!= (const Box<Vec2<T>>& src) const noexcept
{
    return (min != src.min || max != src.max);
}

template <class T>
inline void
Box<Vec2<T>>::makeEmpty() noexcept
{
    min = Vec2<T> (Vec2<T>::baseTypeMax());
    max = Vec2<T> (Vec2<T>::baseTypeMin());
}

template <class T>
inline void
Box<Vec2<T>>::makeInfinite() noexcept
{
    min = Vec2<T> (Vec2<T>::baseTypeMin());
    max = Vec2<T> (Vec2<T>::baseTypeMax());
}

template <class T>
inline void
Box<Vec2<T>>::extendBy (const Vec2<T>& point) noexcept
{
    if (point[0] < min[0])
        min[0] = point[0];

    if (point[0] > max[0])
        max[0] = point[0];

    if (point[1] < min[1])
        min[1] = point[1];

    if (point[1] > max[1])
        max[1] = point[1];
}

template <class T>
inline void
Box<Vec2<T>>::extendBy (const Box<Vec2<T>>& box) noexcept
{
    if (box.min[0] < min[0])
        min[0] = box.min[0];

    if (box.max[0] > max[0])
        max[0] = box.max[0];

    if (box.min[1] < min[1])
        min[1] = box.min[1];

    if (box.max[1] > max[1])
        max[1] = box.max[1];
}

template <class T>
IMATH_CONSTEXPR14 inline bool
Box<Vec2<T>>::intersects (const Vec2<T>& point) const noexcept
{
    if (point[0] < min[0] || point[0] > max[0] || point[1] < min[1] || point[1] > max[1])
        return false;

    return true;
}

template <class T>
IMATH_CONSTEXPR14 inline bool
Box<Vec2<T>>::intersects (const Box<Vec2<T>>& box) const noexcept
{
    if (box.max[0] < min[0] || box.min[0] > max[0] || box.max[1] < min[1] || box.min[1] > max[1])
        return false;

    return true;
}

template <class T>
IMATH_CONSTEXPR14 inline Vec2<T>
Box<Vec2<T>>::size() const noexcept
{
    if (isEmpty())
        return Vec2<T> (0);

    return max - min;
}

template <class T>
constexpr inline Vec2<T>
Box<Vec2<T>>::center() const noexcept
{
    return (max + min) / 2;
}

template <class T>
IMATH_CONSTEXPR14 inline bool
Box<Vec2<T>>::isEmpty() const noexcept
{
    if (max[0] < min[0] || max[1] < min[1])
        return true;

    return false;
}

template <class T>
IMATH_CONSTEXPR14 inline bool
Box<Vec2<T>>::isInfinite() const noexcept
{
    if (min[0] != limits<T>::min() || max[0] != limits<T>::max() || min[1] != limits<T>::min() ||
        max[1] != limits<T>::max())
        return false;

    return true;
}

template <class T>
IMATH_CONSTEXPR14 inline bool
Box<Vec2<T>>::hasVolume() const noexcept
{
    if (max[0] <= min[0] || max[1] <= min[1])
        return false;

    return true;
}

template <class T>
IMATH_CONSTEXPR14 inline unsigned int
Box<Vec2<T>>::majorAxis() const noexcept
{
    unsigned int major = 0;
    Vec2<T> s          = size();

    if (s[1] > s[major])
        major = 1;

    return major;
}

///
/// The Box<Vec3<T>> template represents a 3D bounding box defined by
/// minimum and maximum values of type Vec3<T>. The min and max members are
/// public.
///


template <class T> class Box<Vec3<T>>
{
  public:
    //-------------------------
    //  Data Members are public
    //-------------------------

    Vec3<T> min;
    Vec3<T> max;

    //-----------------------------------------------------
    //  Constructors - an "empty" box is created by default
    //-----------------------------------------------------

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box() noexcept;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box (const Vec3<T>& point) noexcept;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box (const Vec3<T>& minT, const Vec3<T>& maxT) noexcept;

    //--------------------
    //  Operators:  ==, !=
    //--------------------

    IMATH_HOSTDEVICE constexpr bool operator== (const Box<Vec3<T>>& src) const noexcept;
    IMATH_HOSTDEVICE constexpr bool operator!= (const Box<Vec3<T>>& src) const noexcept;

    //------------------
    //  Box manipulation
    //------------------

    IMATH_HOSTDEVICE void makeEmpty() noexcept;
    IMATH_HOSTDEVICE void extendBy (const Vec3<T>& point) noexcept;
    IMATH_HOSTDEVICE void extendBy (const Box<Vec3<T>>& box) noexcept;
    IMATH_HOSTDEVICE void makeInfinite() noexcept;

    //---------------------------------------------------
    //  Query functions - these compute results each time
    //---------------------------------------------------

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Vec3<T> size() const noexcept;
    IMATH_HOSTDEVICE constexpr Vec3<T> center() const noexcept;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool intersects (const Vec3<T>& point) const noexcept;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool intersects (const Box<Vec3<T>>& box) const noexcept;

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 unsigned int majorAxis() const noexcept;

    //----------------
    //  Classification
    //----------------

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool isEmpty() const noexcept;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool hasVolume() const noexcept;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool isInfinite() const noexcept;
};

//----------------
//  Implementation

template <class T> IMATH_CONSTEXPR14 inline Box<Vec3<T>>::Box() noexcept
{
    makeEmpty();
}

template <class T> IMATH_CONSTEXPR14 inline Box<Vec3<T>>::Box (const Vec3<T>& point) noexcept
{
    min = point;
    max = point;
}

template <class T>
IMATH_CONSTEXPR14 inline Box<Vec3<T>>::Box (const Vec3<T>& minT, const Vec3<T>& maxT) noexcept
{
    min = minT;
    max = maxT;
}

template <class T>
constexpr inline bool
Box<Vec3<T>>::operator== (const Box<Vec3<T>>& src) const noexcept
{
    return (min == src.min && max == src.max);
}

template <class T>
constexpr inline bool
Box<Vec3<T>>::operator!= (const Box<Vec3<T>>& src) const noexcept
{
    return (min != src.min || max != src.max);
}

template <class T>
inline void
Box<Vec3<T>>::makeEmpty() noexcept
{
    min = Vec3<T> (Vec3<T>::baseTypeMax());
    max = Vec3<T> (Vec3<T>::baseTypeMin());
}

template <class T>
inline void
Box<Vec3<T>>::makeInfinite() noexcept
{
    min = Vec3<T> (Vec3<T>::baseTypeMin());
    max = Vec3<T> (Vec3<T>::baseTypeMax());
}

template <class T>
inline void
Box<Vec3<T>>::extendBy (const Vec3<T>& point) noexcept
{
    if (point[0] < min[0])
        min[0] = point[0];

    if (point[0] > max[0])
        max[0] = point[0];

    if (point[1] < min[1])
        min[1] = point[1];

    if (point[1] > max[1])
        max[1] = point[1];

    if (point[2] < min[2])
        min[2] = point[2];

    if (point[2] > max[2])
        max[2] = point[2];
}

template <class T>
inline void
Box<Vec3<T>>::extendBy (const Box<Vec3<T>>& box) noexcept
{
    if (box.min[0] < min[0])
        min[0] = box.min[0];

    if (box.max[0] > max[0])
        max[0] = box.max[0];

    if (box.min[1] < min[1])
        min[1] = box.min[1];

    if (box.max[1] > max[1])
        max[1] = box.max[1];

    if (box.min[2] < min[2])
        min[2] = box.min[2];

    if (box.max[2] > max[2])
        max[2] = box.max[2];
}

template <class T>
IMATH_CONSTEXPR14 inline bool
Box<Vec3<T>>::intersects (const Vec3<T>& point) const noexcept
{
    if (point[0] < min[0] || point[0] > max[0] || point[1] < min[1] || point[1] > max[1] ||
        point[2] < min[2] || point[2] > max[2])
        return false;

    return true;
}

template <class T>
IMATH_CONSTEXPR14 inline bool
Box<Vec3<T>>::intersects (const Box<Vec3<T>>& box) const noexcept
{
    if (box.max[0] < min[0] || box.min[0] > max[0] || box.max[1] < min[1] || box.min[1] > max[1] ||
        box.max[2] < min[2] || box.min[2] > max[2])
        return false;

    return true;
}

template <class T>
IMATH_CONSTEXPR14 inline Vec3<T>
Box<Vec3<T>>::size() const noexcept
{
    if (isEmpty())
        return Vec3<T> (0);

    return max - min;
}

template <class T>
constexpr inline Vec3<T>
Box<Vec3<T>>::center() const noexcept
{
    return (max + min) / 2;
}

template <class T>
IMATH_CONSTEXPR14 inline bool
Box<Vec3<T>>::isEmpty() const noexcept
{
    if (max[0] < min[0] || max[1] < min[1] || max[2] < min[2])
        return true;

    return false;
}

template <class T>
IMATH_CONSTEXPR14 inline bool
Box<Vec3<T>>::isInfinite() const noexcept
{
    if (min[0] != limits<T>::min() || max[0] != limits<T>::max() || min[1] != limits<T>::min() ||
        max[1] != limits<T>::max() || min[2] != limits<T>::min() || max[2] != limits<T>::max())
        return false;

    return true;
}

template <class T>
IMATH_CONSTEXPR14 inline bool
Box<Vec3<T>>::hasVolume() const noexcept
{
    if (max[0] <= min[0] || max[1] <= min[1] || max[2] <= min[2])
        return false;

    return true;
}

template <class T>
IMATH_CONSTEXPR14 inline unsigned int
Box<Vec3<T>>::majorAxis() const noexcept
{
    unsigned int major = 0;
    Vec3<T> s          = size();

    if (s[1] > s[major])
        major = 1;

    if (s[2] > s[major])
        major = 2;

    return major;
}

IMATH_INTERNAL_NAMESPACE_HEADER_EXIT

#endif // INCLUDED_IMATHBOX_H

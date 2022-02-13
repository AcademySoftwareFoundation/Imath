//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

//
// Axis-aligned bounding box
//

#ifndef INCLUDED_IMATHBOX_H
#define INCLUDED_IMATHBOX_H

#include "ImathExport.h"
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
/// preferably, this returns a constant expression, typically 2 or 3.
///
/// NB: Because of the above operator requirements, we don't use a
/// base storage mechanism for passing to / from public API types
/// provided by \ref BaseTypes, instead favoring type cast operators
/// provided in the explict template specializations below.
///

template <typename V> class IMATH_EXPORT_TEMPLATE_TYPE Box
{
public:
    /// @{
    /// @name Direct access to bounds

    /// The minimum value of the box.
    V min;

    /// The maximum value of the box.
    V max;

    /// @}

    /// @{
    ///	@name Constructors

    /// Construct an empty bounding box. This initializes the mimimum to
    /// std::numeric_limits<V::baseType>::max() and the maximum to
    /// std::numeric_limits<V::baseType>::lowest().
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box () IMATH_NOEXCEPT;

    /// Construct a bounding box that contains a single point.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box (const V& point) IMATH_NOEXCEPT;

    /// Construct a bounding box with the given minimum and maximum values.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box (const V& minV, const V& maxV)
        IMATH_NOEXCEPT;

    /// Copy constructor
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box (const Box&) IMATH_NOEXCEPT;

    /// Move constructor
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box (Box&&) IMATH_NOEXCEPT;

    /// Copy assign
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box&
    operator= (const Box&) IMATH_NOEXCEPT;

    /// Move assign
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box& operator= (Box&&) IMATH_NOEXCEPT;

    IMATH_HOSTDEVICE ~Box () = default;
    /// @}

    /// @{
    /// @name Comparison

    /// Equality
    IMATH_HOSTDEVICE constexpr bool
    operator== (const Box& src) const IMATH_NOEXCEPT;

    /// Inequality
    IMATH_HOSTDEVICE constexpr bool
    operator!= (const Box& src) const IMATH_NOEXCEPT;

    /// @}

    /// @{
    /// @name Manipulation

    /// Set the box to be empty. A box is empty if the mimimum is greater
    /// than the maximum. makeEmpty() sets the mimimum to `V::baseTypeMax()`
    /// and the maximum to `V::baseTypeLowest()`.
    IMATH_HOSTDEVICE void makeEmpty () IMATH_NOEXCEPT;

    /// Extend the box to include the given point.
    IMATH_HOSTDEVICE void extendBy (const V& point) IMATH_NOEXCEPT;

    /// Extend the box to include the given box.
    IMATH_HOSTDEVICE void extendBy (const Box& box) IMATH_NOEXCEPT;

    /// Make the box include the entire range of `V`.
    IMATH_HOSTDEVICE void makeInfinite () IMATH_NOEXCEPT;

    /// @}

    /// @{
    /// @name Query

    /// Return the size of the box. The size is of type `V`, defined
    /// as `(max-min)`. An empty box has a size of `V(0)`, i.e. 0 in
    /// each dimension.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 V size () const IMATH_NOEXCEPT;

    /// Return the center of the box. The center is defined as
    /// `(max+min)/2`. The center of an empty box is undefined.
    IMATH_HOSTDEVICE constexpr V center () const IMATH_NOEXCEPT;

    /// Return true if the given point is inside the box, false otherwise.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool
    intersects (const V& point) const IMATH_NOEXCEPT;

    /// Return true if the given box is inside the box, false otherwise.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool
    intersects (const Box& box) const IMATH_NOEXCEPT;

    /// Return the major axis of the box. The major axis is the dimension with
    /// the greatest difference between maximum and minimum.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 unsigned int
    majorAxis () const IMATH_NOEXCEPT;

    /// Return true if the box is empty, false otherwise. An empty box's
    /// minimum is greater than its maximum.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool isEmpty () const IMATH_NOEXCEPT;

    /// Return true if the box is larger than a single point, false otherwise.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool hasVolume () const IMATH_NOEXCEPT;

    /// Return true if the box contains all points, false otherwise.
    /// An infinite box has a mimimum of`V::baseTypeLowest()`
    /// and a maximum of `V::baseTypeMax()`.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool isInfinite () const IMATH_NOEXCEPT;

    /// @}
};

//--------------------
// Convenient typedefs
//--------------------

/// 2D box of base type `short`.
typedef Box<V2s> Box2s;

/// 2D box of base type `int`.
typedef Box<V2i> Box2i;

/// 2D box of base type `int64_t`.
typedef Box<V2i64> Box2i64;

/// 2D box of base type `float`.
typedef Box<V2f> Box2f;

/// 2D box of base type `double`.
typedef Box<V2d> Box2d;

/// 3D box of base type `short`.
typedef Box<V3s> Box3s;

/// 3D box of base type `int`.
typedef Box<V3i> Box3i;

/// 3D box of base type `int64_t`.
typedef Box<V3i64> Box3i64;

/// 3D box of base type `float`.
typedef Box<V3f> Box3f;

/// 3D box of base type `double`.
typedef Box<V3d> Box3d;

////////////////////////////////////////

template <typename V>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline Box<V>::Box () IMATH_NOEXCEPT
{
    makeEmpty ();
}

template <typename V>
IMATH_HOSTDEVICE
    IMATH_CONSTEXPR14 inline Box<V>::Box (const V& point) IMATH_NOEXCEPT
{
    min = point;
    max = point;
}

template <typename V>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline Box<V>::Box (
    const V& minV, const V& maxV) IMATH_NOEXCEPT
{
    min = minV;
    max = maxV;
}

template <typename V>
IMATH_HOSTDEVICE
    IMATH_CONSTEXPR14 inline Box<V>::Box (const Box& b) IMATH_NOEXCEPT
{
    min = b.min;
    max = b.max;
}

template <typename V>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline Box<V>::Box (Box&& b) IMATH_NOEXCEPT
{
    min = std::move (b.min);
    max = std::move (b.max);
}

template <typename V>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline Box<V>&
Box<V>::operator= (const Box& src) IMATH_NOEXCEPT
{
    min = src.min;
    max = src.max;
    return *this;
}

template <typename V>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline Box<V>&
Box<V>::operator= (Box&& src) IMATH_NOEXCEPT
{
    min = std::move (src.min);
    max = std::move (src.max);
    return *this;
}

template <typename V>
IMATH_HOSTDEVICE constexpr inline bool
Box<V>::operator== (const Box<V>& src) const IMATH_NOEXCEPT
{
    return (min == src.min && max == src.max);
}

template <typename V>
IMATH_HOSTDEVICE constexpr inline bool
Box<V>::operator!= (const Box<V>& src) const IMATH_NOEXCEPT
{
    return (min != src.min || max != src.max);
}

template <typename V>
IMATH_HOSTDEVICE inline void
Box<V>::makeEmpty () IMATH_NOEXCEPT
{
    min = V (V::baseTypeMax ());
    max = V (V::baseTypeLowest ());
}

template <typename V>
IMATH_HOSTDEVICE inline void
Box<V>::makeInfinite () IMATH_NOEXCEPT
{
    min = V (V::baseTypeLowest ());
    max = V (V::baseTypeMax ());
}

template <typename V>
IMATH_HOSTDEVICE inline void
Box<V>::extendBy (const V& point) IMATH_NOEXCEPT
{
    for (unsigned int i = 0; i < min.dimensions (); i++)
    {
        if (point[i] < min[i]) min[i] = point[i];

        if (point[i] > max[i]) max[i] = point[i];
    }
}

template <typename V>
IMATH_HOSTDEVICE inline void
Box<V>::extendBy (const Box<V>& box) IMATH_NOEXCEPT
{
    for (unsigned int i = 0; i < min.dimensions (); i++)
    {
        if (box.min[i] < min[i]) min[i] = box.min[i];

        if (box.max[i] > max[i]) max[i] = box.max[i];
    }
}

template <typename V>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline bool
Box<V>::intersects (const V& point) const IMATH_NOEXCEPT
{
    for (unsigned int i = 0; i < min.dimensions (); i++)
    {
        if (point[i] < min[i] || point[i] > max[i]) return false;
    }

    return true;
}

template <typename V>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline bool
Box<V>::intersects (const Box<V>& box) const IMATH_NOEXCEPT
{
    for (unsigned int i = 0; i < min.dimensions (); i++)
    {
        if (box.max[i] < min[i] || box.min[i] > max[i]) return false;
    }

    return true;
}

template <typename V>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline V
Box<V>::size () const IMATH_NOEXCEPT
{
    if (isEmpty ()) return V (0);

    return max - min;
}

template <typename V>
IMATH_HOSTDEVICE constexpr inline V
Box<V>::center () const IMATH_NOEXCEPT
{
    return (max + min) / 2;
}

template <typename V>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline bool
Box<V>::isEmpty () const IMATH_NOEXCEPT
{
    for (unsigned int i = 0; i < min.dimensions (); i++)
    {
        if (max[i] < min[i]) return true;
    }

    return false;
}

template <typename V>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline bool
Box<V>::isInfinite () const IMATH_NOEXCEPT
{
    for (unsigned int i = 0; i < min.dimensions (); i++)
    {
        if (min[i] != V::baseTypeLowest () || max[i] != V::baseTypeMax ())
            return false;
    }

    return true;
}

template <typename V>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline bool
Box<V>::hasVolume () const IMATH_NOEXCEPT
{
    for (unsigned int i = 0; i < min.dimensions (); i++)
    {
        if (max[i] <= min[i]) return false;
    }

    return true;
}

template <typename V>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline unsigned int
Box<V>::majorAxis () const IMATH_NOEXCEPT
{
    unsigned int major = 0;
    V            s     = size ();

    for (unsigned int i = 1; i < min.dimensions (); i++)
    {
        if (s[i] > s[major]) major = i;
    }

    return major;
}

//-------------------------------------------------------------------
//
//  Partial class specializations for Imath::Vec2<T> and Imath::Vec3<T>
//
//-------------------------------------------------------------------

namespace detail
{
/// implementation detail to provide storage type for
/// conversion
template <typename T> struct BoxStorageAdapter
{};
// exposed / created by python, seems useless...
template <> struct BoxStorageAdapter<Vec2<uint8_t>>
{
    using type = imath_box2b_t;
};
template <> struct BoxStorageAdapter<V2s>
{
    using type = imath_box2s_t;
};
template <> struct BoxStorageAdapter<V2i>
{
    using type = imath_box2i_t;
};
template <> struct BoxStorageAdapter<V2i64>
{
    using type = imath_box2i64_t;
};
template <> struct BoxStorageAdapter<V2f>
{
    using type = imath_box2f_t;
};
template <> struct BoxStorageAdapter<V2d>
{
    using type = imath_box2d_t;
};
// exposed / created by python, seems useless...
template <> struct BoxStorageAdapter<Vec3<uint8_t>>
{
    using type = imath_box3b_t;
};
template <> struct BoxStorageAdapter<V3s>
{
    using type = imath_box3s_t;
};
template <> struct BoxStorageAdapter<V3i>
{
    using type = imath_box3i_t;
};
template <> struct BoxStorageAdapter<V3i64>
{
    using type = imath_box3i64_t;
};
template <> struct BoxStorageAdapter<V3f>
{
    using type = imath_box3f_t;
};
template <> struct BoxStorageAdapter<V3d>
{
    using type = imath_box3d_t;
};

} // namespace detail

///
/// The Box<Vec2<T>> template represents a 2D bounding box defined by
/// minimum and maximum values of type Vec2<T>. The min and max members are
/// public.
///
/// NB: we don't directly derive from the public API storage, such that min
///

template <typename T> class IMATH_EXPORT_TEMPLATE_TYPE Box<Vec2<T>>
{
public:
    using BaseVecType = Vec2<T>;

    /// @{
    /// @name Direct access to bounds

    /// The minimum value of the box.
    BaseVecType min;

    /// The maximum value of the box.
    BaseVecType max;

    /// @}

    /// @{
    /// @name Constructors and Assignment

    /// Empty by default
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box () IMATH_NOEXCEPT;

    /// Construct a bounding box that contains a single point.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box (const BaseVecType& point)
        IMATH_NOEXCEPT;

    /// Construct a bounding box with the given minimum and maximum points
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14
    Box (const BaseVecType& minT, const BaseVecType& maxT) IMATH_NOEXCEPT;

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box (const Box& src) IMATH_NOEXCEPT;

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box (Box&& src) IMATH_NOEXCEPT;

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box&
    operator= (const Box& src) IMATH_NOEXCEPT;

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box&
    operator= (Box&& src) IMATH_NOEXCEPT;

    ~Box () = default;
    /// @}

    /// @{
    /// @name API Conversion

    using APIStorageType =
        typename detail::BoxStorageAdapter<BaseVecType>::type;

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box (const APIStorageType& src)
        IMATH_NOEXCEPT;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box& operator= (const APIStorageType&) IMATH_NOEXCEPT;
    IMATH_HOSTDEVICE operator APIStorageType& () IMATH_NOEXCEPT;
    IMATH_HOSTDEVICE constexpr operator const APIStorageType& () const IMATH_NOEXCEPT;

    /// @}

    /// @{
    /// @name Comparison

    /// Equality
    IMATH_HOSTDEVICE constexpr bool
    operator== (const Box& src) const IMATH_NOEXCEPT;

    /// Inequality
    IMATH_HOSTDEVICE constexpr bool
    operator!= (const Box& src) const IMATH_NOEXCEPT;

    /// @}

    /// @{
    /// @name Manipulation

    /// Set the Box to be empty. A Box is empty if the mimimum is
    /// greater than the maximum. makeEmpty() sets the mimimum to
    /// std::numeric_limits<T>::max() and the maximum to
    /// std::numeric_limits<T>::lowest().
    IMATH_HOSTDEVICE void makeEmpty () IMATH_NOEXCEPT;

    /// Extend the Box to include the given point.
    IMATH_HOSTDEVICE void extendBy (const BaseVecType& point) IMATH_NOEXCEPT;

    /// Extend the Box to include the given box.
    IMATH_HOSTDEVICE void extendBy (const Box& box) IMATH_NOEXCEPT;

    /// Make the box include the entire range of T.
    IMATH_HOSTDEVICE void makeInfinite () IMATH_NOEXCEPT;

    /// @}

    /// @{
    /// @name Query

    /// Return the size of the box. The size is of type `V`, defined as
    /// `(max-min)`. An empty box has a size of `V(0)`, i.e. 0 in each dimension.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 BaseVecType size () const IMATH_NOEXCEPT;

    /// Return the center of the box. The center is defined as
    /// `(max+min)/2`. The center of an empty box is undefined.
    IMATH_HOSTDEVICE constexpr BaseVecType center () const IMATH_NOEXCEPT;

    /// Return true if the given point is inside the box, false otherwise.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool
    intersects (const BaseVecType& point) const IMATH_NOEXCEPT;

    /// Return true if the given box is inside the box, false otherwise.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool
    intersects (const Box& box) const IMATH_NOEXCEPT;

    /// Return the major axis of the box. The major axis is the dimension with
    /// the greatest difference between maximum and minimum.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 unsigned int
    majorAxis () const IMATH_NOEXCEPT;

    /// Return true if the box is empty, false otherwise. An empty box's
    /// minimum is greater than its maximum.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool isEmpty () const IMATH_NOEXCEPT;

    /// Return true if the box is larger than a single point, false otherwise.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool hasVolume () const IMATH_NOEXCEPT;

    /// Return true if the box contains all points, false otherwise.
    /// An infinite box has a mimimum of `V::baseTypeMin()`
    /// and a maximum of `V::baseTypeMax()`.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool isInfinite () const IMATH_NOEXCEPT;

    /// @}
};

//----------------
//  Implementation
//----------------

template <typename T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline Box<Vec2<T>>::Box () IMATH_NOEXCEPT
{
    makeEmpty ();
}

template <typename T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline Box<Vec2<T>>::Box (
    const Vec2<T>& point) IMATH_NOEXCEPT
{
    min = point;
    max = point;
}

template <typename T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline Box<Vec2<T>>::Box (
    const Vec2<T>& minT, const Vec2<T>& maxT) IMATH_NOEXCEPT
{
    min = minT;
    max = maxT;
}

template <typename T>
IMATH_HOSTDEVICE
    IMATH_CONSTEXPR14 inline Box<Vec2<T>>::Box (const Box& b) IMATH_NOEXCEPT
{
    min = b.min;
    max = b.max;
}

template <typename T>
IMATH_HOSTDEVICE
    IMATH_CONSTEXPR14 inline Box<Vec2<T>>::Box (Box&& b) IMATH_NOEXCEPT
{
    min = std::move (b.min);
    max = std::move (b.max);
}

template <typename T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline Box<Vec2<T>>&
Box<Vec2<T>>::operator= (const Box& src) IMATH_NOEXCEPT
{
    min = src.min;
    max = src.max;
    return *this;
}

template <typename T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline Box<Vec2<T>>&
Box<Vec2<T>>::operator= (Box&& src) IMATH_NOEXCEPT
{
    min = src.min;
    max = src.max;
    return *this;
}

template <typename T>
IMATH_HOSTDEVICE
    IMATH_CONSTEXPR14 inline Box<Vec2<T>>::Box (const APIStorageType& b) IMATH_NOEXCEPT
{
    min = b.min;
    max = b.max;
}

template <typename T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline Box<Vec2<T>>&
Box<Vec2<T>>::operator= (const APIStorageType& b) IMATH_NOEXCEPT
{
    min = b.min;
    max = b.max;
    return *this;
}

template <typename T>
IMATH_HOSTDEVICE inline
Box<Vec2<T>>::operator typename Box<Vec2<T>>::APIStorageType& () IMATH_NOEXCEPT
{
    static_assert (
        sizeof (Box) == sizeof (APIStorageType),
        "size must match with api storage type");
    static_assert (
        offsetof (Box, min) == offsetof (APIStorageType, min),
        "offset of min with api storage type");
    static_assert (
        offsetof (Box, max) == offsetof (APIStorageType, max),
        "offset of min with api storage type");

    return *(reinterpret_cast<APIStorageType *>(this));
}

template <typename T>
IMATH_HOSTDEVICE constexpr inline
Box<Vec2<T>>::operator const typename Box<Vec2<T>>::APIStorageType& () const IMATH_NOEXCEPT
{
    static_assert (
        sizeof (Box) == sizeof (APIStorageType),
        "size must match with api storage type");
    static_assert (
        offsetof (Box, min) == offsetof (APIStorageType, min),
        "offset of min with api storage type");
    static_assert (
        offsetof (Box, max) == offsetof (APIStorageType, max),
        "offset of min with api storage type");

    return *(reinterpret_cast<const APIStorageType *>(this));
}

template <typename T>
IMATH_HOSTDEVICE constexpr inline bool
Box<Vec2<T>>::operator== (const Box& src) const IMATH_NOEXCEPT
{
    return (min == src.min && max == src.max);
}

template <typename T>
IMATH_HOSTDEVICE constexpr inline bool
Box<Vec2<T>>::operator!= (const Box& src) const IMATH_NOEXCEPT
{
    return (min != src.min || max != src.max);
}

template <typename T>
IMATH_HOSTDEVICE inline void
Box<Vec2<T>>::makeEmpty () IMATH_NOEXCEPT
{
    min = Vec2<T> (Vec2<T>::baseTypeMax ());
    max = Vec2<T> (Vec2<T>::baseTypeLowest ());
}

template <typename T>
IMATH_HOSTDEVICE inline void
Box<Vec2<T>>::makeInfinite () IMATH_NOEXCEPT
{
    min = Vec2<T> (Vec2<T>::baseTypeLowest ());
    max = Vec2<T> (Vec2<T>::baseTypeMax ());
}

template <typename T>
IMATH_HOSTDEVICE inline void
Box<Vec2<T>>::extendBy (const Vec2<T>& point) IMATH_NOEXCEPT
{
    if (point[0] < min[0]) min[0] = point[0];

    if (point[0] > max[0]) max[0] = point[0];

    if (point[1] < min[1]) min[1] = point[1];

    if (point[1] > max[1]) max[1] = point[1];
}

template <typename T>
IMATH_HOSTDEVICE inline void
Box<Vec2<T>>::extendBy (const Box& box) IMATH_NOEXCEPT
{
    if (box.min[0] < min[0]) min[0] = box.min[0];

    if (box.max[0] > max[0]) max[0] = box.max[0];

    if (box.min[1] < min[1]) min[1] = box.min[1];

    if (box.max[1] > max[1]) max[1] = box.max[1];
}

template <typename T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline bool
Box<Vec2<T>>::intersects (const Vec2<T>& point) const IMATH_NOEXCEPT
{
    if (point[0] < min[0] || point[0] > max[0] || point[1] < min[1] ||
        point[1] > max[1])
        return false;

    return true;
}

template <typename T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline bool
Box<Vec2<T>>::intersects (const Box& box) const IMATH_NOEXCEPT
{
    if (box.max[0] < min[0] || box.min[0] > max[0] || box.max[1] < min[1] ||
        box.min[1] > max[1])
        return false;

    return true;
}

template <typename T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline Vec2<T>
                 Box<Vec2<T>>::size () const IMATH_NOEXCEPT
{
    if (isEmpty ()) return Vec2<T> (T (0));

    return max - min;
}

template <typename T>
IMATH_HOSTDEVICE constexpr inline Vec2<T>
Box<Vec2<T>>::center () const IMATH_NOEXCEPT
{
    return (max + min) / 2;
}

template <typename T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline bool
Box<Vec2<T>>::isEmpty () const IMATH_NOEXCEPT
{
    if (max[0] < min[0] || max[1] < min[1]) return true;

    return false;
}

template <typename T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline bool
Box<Vec2<T>>::isInfinite () const IMATH_NOEXCEPT
{
    if (min[0] != std::numeric_limits<T>::lowest () ||
        max[0] != std::numeric_limits<T>::max () ||
        min[1] != std::numeric_limits<T>::lowest () ||
        max[1] != std::numeric_limits<T>::max ())
        return false;

    return true;
}

template <typename T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline bool
Box<Vec2<T>>::hasVolume () const IMATH_NOEXCEPT
{
    if (max[0] <= min[0] || max[1] <= min[1]) return false;

    return true;
}

template <typename T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline unsigned int
Box<Vec2<T>>::majorAxis () const IMATH_NOEXCEPT
{
    unsigned int major = 0;
    Vec2<T>      s     = size ();

    if (s[1] > s[major]) major = 1;

    return major;
}

///
/// The Box<Vec3> template represents a 3D bounding box defined by
/// minimum and maximum values of type Vec3.
///
template <typename T>
class IMATH_EXPORT_TEMPLATE_TYPE Box<Vec3<T>>
{
public:
    using BaseVecType = Vec3<T>;

    /// @{
    /// @name Direct access to bounds

    /// The minimum value of the box.
    BaseVecType min;

    /// The maximum value of the box.
    BaseVecType max;

    /// @}

    /// @{
    /// @name Constructors

    /// Empty by default
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box () IMATH_NOEXCEPT;

    /// Construct a bounding box that contains a single point.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box (const BaseVecType& point)
        IMATH_NOEXCEPT;

    /// Construct a bounding box with the given minimum and maximum points
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14
    Box (const BaseVecType& minT, const BaseVecType& maxT) IMATH_NOEXCEPT;

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box (const Box& src) IMATH_NOEXCEPT;

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box (Box&& src) IMATH_NOEXCEPT;

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box&
    operator= (const Box& src) IMATH_NOEXCEPT;

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box&
    operator= (Box&& src) IMATH_NOEXCEPT;

    ~Box () = default;
    /// @}

    /// @{
    /// @name API Conversion

    using APIStorageType =
        typename detail::BoxStorageAdapter<BaseVecType>::type;

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box (const APIStorageType& src)
        IMATH_NOEXCEPT;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Box& operator= (const APIStorageType&) IMATH_NOEXCEPT;
    IMATH_HOSTDEVICE operator APIStorageType& () IMATH_NOEXCEPT;
    IMATH_HOSTDEVICE constexpr operator const APIStorageType& () const IMATH_NOEXCEPT;

    /// @}

    /// @{
    /// @name Comparison

    /// Equality
    IMATH_HOSTDEVICE constexpr bool
    operator== (const Box<BaseVecType>& src) const IMATH_NOEXCEPT;

    /// Inequality
    IMATH_HOSTDEVICE constexpr bool
    operator!= (const Box<BaseVecType>& src) const IMATH_NOEXCEPT;

    /// @}

    /// @{
    /// @name Manipulation

    /// Set the Box to be empty. A Box is empty if the mimimum is
    /// greater than the maximum. makeEmpty() sets the mimimum to
    /// std::numeric_limits<T>::max() and the maximum to
    /// std::numeric_limits<T>::lowest().
    IMATH_HOSTDEVICE void makeEmpty () IMATH_NOEXCEPT;

    /// Extend the Box to include the given point.
    IMATH_HOSTDEVICE void extendBy (const BaseVecType& point) IMATH_NOEXCEPT;
    /// Extend the Box to include the given box.

    IMATH_HOSTDEVICE void extendBy (const Box<BaseVecType>& box) IMATH_NOEXCEPT;

    /// Make the box include the entire range of T.
    IMATH_HOSTDEVICE void makeInfinite () IMATH_NOEXCEPT;

    /// @}

    /// @{
    /// @name Query

    /// Return the size of the box. The size is of type `V`, defined as
    /// (max-min). An empty box has a size of V(0), i.e. 0 in each dimension.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 BaseVecType size () const IMATH_NOEXCEPT;

    /// Return the center of the box. The center is defined as
    /// (max+min)/2. The center of an empty box is undefined.
    IMATH_HOSTDEVICE constexpr BaseVecType center () const IMATH_NOEXCEPT;

    /// Return true if the given point is inside the box, false otherwise.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool
    intersects (const BaseVecType& point) const IMATH_NOEXCEPT;

    /// Return true if the given box is inside the box, false otherwise.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool
    intersects (const Box<BaseVecType>& box) const IMATH_NOEXCEPT;

    /// Return the major axis of the box. The major axis is the dimension with
    /// the greatest difference between maximum and minimum.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 unsigned int
    majorAxis () const IMATH_NOEXCEPT;

    /// Return true if the box is empty, false otherwise. An empty box's
    /// minimum is greater than its maximum.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool isEmpty () const IMATH_NOEXCEPT;

    /// Return true if the box is larger than a single point, false otherwise.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool hasVolume () const IMATH_NOEXCEPT;

    /// Return true if the box contains all points, false otherwise.
    /// An infinite box has a mimimum of`V::baseTypeMin()`
    /// and a maximum of `V::baseTypeMax()`.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool isInfinite () const IMATH_NOEXCEPT;

    /// @}
};

//----------------
//  Implementation
//----------------

template <typename T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline Box<Vec3<T>>::Box () IMATH_NOEXCEPT
{
    makeEmpty ();
}

template <typename T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline Box<Vec3<T>>::Box (
    const Vec3<T>& point) IMATH_NOEXCEPT
{
    min = point;
    max = point;
}

template <typename T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline Box<Vec3<T>>::Box (
    const Vec3<T>& minT, const Vec3<T>& maxT) IMATH_NOEXCEPT
{
    min = minT;
    max = maxT;
}

template <typename T>
IMATH_HOSTDEVICE
    IMATH_CONSTEXPR14 inline Box<Vec3<T>>::Box (const Box& b) IMATH_NOEXCEPT
{
    min = b.min;
    max = b.max;
}

template <typename T>
IMATH_HOSTDEVICE
    IMATH_CONSTEXPR14 inline Box<Vec3<T>>::Box (Box&& b) IMATH_NOEXCEPT
{
    min = std::move (b.min);
    max = std::move (b.max);
}

template <typename T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline Box<Vec3<T>>&
Box<Vec3<T>>::operator= (const Box& src) IMATH_NOEXCEPT
{
    min = src.min;
    max = src.max;
    return *this;
}

template <typename T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline Box<Vec3<T>>&
Box<Vec3<T>>::operator= (Box&& src) IMATH_NOEXCEPT
{
    min = src.min;
    max = src.max;
    return *this;
}

template <typename T>
IMATH_HOSTDEVICE
    IMATH_CONSTEXPR14 inline Box<Vec3<T>>::Box (const APIStorageType& b) IMATH_NOEXCEPT
{
    min = b.min;
    max = b.max;
}

template <typename T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline Box<Vec3<T>>&
Box<Vec3<T>>::operator= (const APIStorageType& b) IMATH_NOEXCEPT
{
    min = b.min;
    max = b.max;
    return *this;
}

template <typename T>
IMATH_HOSTDEVICE inline
Box<Vec3<T>>::operator typename Box<Vec3<T>>::APIStorageType& () IMATH_NOEXCEPT
{
    static_assert (
        sizeof (Box) == sizeof (APIStorageType),
        "size must match with api storage type");
    static_assert (
        offsetof (Box, min) == offsetof (APIStorageType, min),
        "offset of min with api storage type");
    static_assert (
        offsetof (Box, max) == offsetof (APIStorageType, max),
        "offset of min with api storage type");

    return *(reinterpret_cast<APIStorageType *>(this));
}

template <typename T>
IMATH_HOSTDEVICE constexpr inline
Box<Vec3<T>>::operator const typename Box<Vec3<T>>::APIStorageType& () const IMATH_NOEXCEPT
{
    static_assert (
        sizeof (Box) == sizeof (APIStorageType),
        "size must match with api storage type");
    static_assert (
        offsetof (Box, min) == offsetof (APIStorageType, min),
        "offset of min with api storage type");
    static_assert (
        offsetof (Box, max) == offsetof (APIStorageType, max),
        "offset of min with api storage type");

    return *(reinterpret_cast<const APIStorageType *>(this));
}

template <typename T>
IMATH_HOSTDEVICE constexpr inline bool
Box<Vec3<T>>::operator== (const Box<Vec3<T>>& src) const IMATH_NOEXCEPT
{
    return (min == src.min && max == src.max);
}

template <typename T>
IMATH_HOSTDEVICE constexpr inline bool
Box<Vec3<T>>::operator!= (const Box<Vec3<T>>& src) const IMATH_NOEXCEPT
{
    return (min != src.min || max != src.max);
}

template <typename T>
IMATH_HOSTDEVICE inline void
Box<Vec3<T>>::makeEmpty () IMATH_NOEXCEPT
{
    min = Vec3<T> (Vec3<T>::baseTypeMax ());
    max = Vec3<T> (Vec3<T>::baseTypeLowest ());
}

template <typename T>
IMATH_HOSTDEVICE inline void
Box<Vec3<T>>::makeInfinite () IMATH_NOEXCEPT
{
    min = Vec3<T> (Vec3<T>::baseTypeLowest ());
    max = Vec3<T> (Vec3<T>::baseTypeMax ());
}

template <typename T>
IMATH_HOSTDEVICE inline void
Box<Vec3<T>>::extendBy (const Vec3<T>& point) IMATH_NOEXCEPT
{
    if (point[0] < min[0]) min[0] = point[0];

    if (point[0] > max[0]) max[0] = point[0];

    if (point[1] < min[1]) min[1] = point[1];

    if (point[1] > max[1]) max[1] = point[1];

    if (point[2] < min[2]) min[2] = point[2];

    if (point[2] > max[2]) max[2] = point[2];
}

template <typename T>
IMATH_HOSTDEVICE inline void
Box<Vec3<T>>::extendBy (const Box<Vec3<T>>& box) IMATH_NOEXCEPT
{
    if (box.min[0] < min[0]) min[0] = box.min[0];

    if (box.max[0] > max[0]) max[0] = box.max[0];

    if (box.min[1] < min[1]) min[1] = box.min[1];

    if (box.max[1] > max[1]) max[1] = box.max[1];

    if (box.min[2] < min[2]) min[2] = box.min[2];

    if (box.max[2] > max[2]) max[2] = box.max[2];
}

template <typename T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline bool
Box<Vec3<T>>::intersects (const Vec3<T>& point) const IMATH_NOEXCEPT
{
    if (point[0] < min[0] || point[0] > max[0] || point[1] < min[1] ||
        point[1] > max[1] || point[2] < min[2] || point[2] > max[2])
        return false;

    return true;
}

template <typename T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline bool
Box<Vec3<T>>::intersects (const Box<Vec3<T>>& box) const IMATH_NOEXCEPT
{
    if (box.max[0] < min[0] || box.min[0] > max[0] || box.max[1] < min[1] ||
        box.min[1] > max[1] || box.max[2] < min[2] || box.min[2] > max[2])
        return false;

    return true;
}

template <typename T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline Vec3<T>
                 Box<Vec3<T>>::size () const IMATH_NOEXCEPT
{
    if (isEmpty ()) return Vec3<T> (T (0));

    return max - min;
}

template <typename T>
IMATH_HOSTDEVICE constexpr inline Vec3<T>
Box<Vec3<T>>::center () const IMATH_NOEXCEPT
{
    return (max + min) / 2;
}

template <typename T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline bool
Box<Vec3<T>>::isEmpty () const IMATH_NOEXCEPT
{
    if (max[0] < min[0] || max[1] < min[1] || max[2] < min[2]) return true;

    return false;
}

template <typename T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline bool
Box<Vec3<T>>::isInfinite () const IMATH_NOEXCEPT
{
    if (min[0] != std::numeric_limits<T>::lowest () ||
        max[0] != std::numeric_limits<T>::max () ||
        min[1] != std::numeric_limits<T>::lowest () ||
        max[1] != std::numeric_limits<T>::max () ||
        min[2] != std::numeric_limits<T>::lowest () ||
        max[2] != std::numeric_limits<T>::max ())
        return false;

    return true;
}

template <typename T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline bool
Box<Vec3<T>>::hasVolume () const IMATH_NOEXCEPT
{
    if (max[0] <= min[0] || max[1] <= min[1] || max[2] <= min[2]) return false;

    return true;
}

template <typename T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline unsigned int
Box<Vec3<T>>::majorAxis () const IMATH_NOEXCEPT
{
    unsigned int major = 0;
    Vec3<T>      s     = size ();

    if (s[1] > s[major]) major = 1;

    if (s[2] > s[major]) major = 2;

    return major;
}

IMATH_INTERNAL_NAMESPACE_HEADER_EXIT

#endif // INCLUDED_IMATHBOX_H

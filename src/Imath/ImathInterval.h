//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

//
// An interval class
//

#ifndef INCLUDED_IMATHINTERVAL_H
#define INCLUDED_IMATHINTERVAL_H

#include "ImathExport.h"
#include "ImathNamespace.h"

#include "ImathVec.h"

IMATH_INTERNAL_NAMESPACE_HEADER_ENTER

///
/// An Interval has a min and a max and some miscellaneous
/// functions. It is basically a Box<T> that allows T to be a scalar.
///

template <class T> class IMATH_EXPORT_TEMPLATE_TYPE Interval
{
  public:

    /// @{
    /// @name Direct access to bounds
    
    /// The minimum value of the interval
    T min;

    /// The minimum value of the interval
    T max;

    /// @}
    
    /// @{
    /// @name Constructors

    /// Initialize to the empty interval
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Interval() noexcept;

    /// Intitialize to a single point
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Interval (const T& point) noexcept;

    /// Intitialize to a given (min,max)
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Interval (const T& minT, const T& maxT) noexcept;

    /// @}

    /// @{
    /// @name Comparison

    /// Equality
    IMATH_HOSTDEVICE constexpr bool operator== (const Interval<T>& src) const noexcept;
    /// Inequality
    IMATH_HOSTDEVICE constexpr bool operator!= (const Interval<T>& src) const noexcept;

    /// @}

    /// @{
    /// @name Manipulation

    /// Set the interval to be empty. An interval is empty if the
    /// minimum is greater than the maximum.
    IMATH_HOSTDEVICE void makeEmpty() noexcept;

    /// Extend the interval to include the given point.
    IMATH_HOSTDEVICE void extendBy (const T& point) noexcept;

    /// Extend the interval to include the given interval
    IMATH_HOSTDEVICE void extendBy (const Interval<T>& interval) noexcept;

    /// Make the interval include the entire range of the base type.
    IMATH_HOSTDEVICE void makeInfinite() noexcept;

    /// @}

    /// @{
    ///	@name Query

    /// Return the size of the interval. The size is (max-min). An empty box has a size of 0.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 T size() const noexcept;

    /// Return the center of the interval. The center is defined as
    /// (max+min)/2. The center of an empty interval is undefined.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 T center() const noexcept;

    /// Return true if the given point is inside the interval, false otherwise.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool intersects (const T& point) const noexcept;

    /// Return true if the given interval is inside the interval, false otherwise.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool intersects (const Interval<T>& interval) const noexcept;

    /// Return true if the interval is empty, false otherwise. An
    /// empty interval's minimum is greater than its maximum.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool isEmpty() const noexcept;

    /// Return true if the interval is larger than a single point,
    /// false otherwise.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool hasVolume() const noexcept;

    /// Return true if the interval contains all points, false
    /// otherwise.  An infinite box has a mimimum of `numeric_limits<T>::lowest()`
    /// and a maximum of `numeric_limits<T>::max()`
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool isInfinite() const noexcept;

    /// @}
};

/// Stream output
template <class T> std::ostream& operator<< (std::ostream& s, const Interval<T>& v);

/// Interval of type float
typedef Interval<float> Intervalf;

/// Interval of type double
typedef Interval<double> Intervald;

/// Interval of type short
typedef Interval<short> Intervals;

/// Interval of type integer
typedef Interval<int> Intervali;

template <class T> inline IMATH_CONSTEXPR14 Interval<T>::Interval() noexcept
{
    makeEmpty();
}

template <class T> IMATH_CONSTEXPR14 inline Interval<T>::Interval (const T& point) noexcept
{
    min = point;
    max = point;
}

template <class T> IMATH_CONSTEXPR14 inline Interval<T>::Interval (const T& minV, const T& maxV) noexcept
{
    min = minV;
    max = maxV;
}

template <class T>
constexpr inline bool
Interval<T>::operator== (const Interval<T>& src) const noexcept
{
    return (min == src.min && max == src.max);
}

template <class T>
constexpr inline bool
Interval<T>::operator!= (const Interval<T>& src) const noexcept
{
    return (min != src.min || max != src.max);
}

template <class T>
inline void
Interval<T>::makeEmpty() noexcept
{
    min = std::numeric_limits<T>::max();
    max = std::numeric_limits<T>::lowest();
}

template <class T>
inline void
Interval<T>::makeInfinite() noexcept
{
    min = std::numeric_limits<T>::lowest();
    max = std::numeric_limits<T>::max();
}


template <class T>
inline void
Interval<T>::extendBy (const T& point) noexcept
{
    if (point < min)
        min = point;

    if (point > max)
        max = point;
}

template <class T>
inline void
Interval<T>::extendBy (const Interval<T>& interval) noexcept
{
    if (interval.min < min)
        min = interval.min;

    if (interval.max > max)
        max = interval.max;
}

template <class T>
IMATH_CONSTEXPR14 inline bool
Interval<T>::intersects (const T& point) const noexcept
{
    return point >= min && point <= max;
}

template <class T>
IMATH_CONSTEXPR14 inline bool
Interval<T>::intersects (const Interval<T>& interval) const noexcept
{
    return interval.max >= min && interval.min <= max;
}

template <class T>
IMATH_CONSTEXPR14 inline T
Interval<T>::size() const noexcept
{
    if (isEmpty())
        return T(0);
    
    return max - min;
}

template <class T>
IMATH_CONSTEXPR14 inline T
Interval<T>::center() const noexcept
{
    return (max + min) / 2;
}

template <class T>
IMATH_CONSTEXPR14 inline bool
Interval<T>::isEmpty() const noexcept
{
    return max < min;
}

template <class T>
IMATH_CONSTEXPR14 inline bool
Interval<T>::hasVolume() const noexcept
{
    return max > min;
}

template <class T>
IMATH_CONSTEXPR14 inline bool
Interval<T>::isInfinite() const noexcept
{
    if (min != std::numeric_limits<T>::lowest() || max != std::numeric_limits<T>::max())
        return false;

    return true;
}

/// Stream output
template <class T>
std::ostream&
operator<< (std::ostream& s, const Interval<T>& v)
{
    return s << '(' << v.min << ' ' << v.max << ')';
}

IMATH_INTERNAL_NAMESPACE_HEADER_EXIT

#endif // INCLUDED_IMATHINTERVAL_H

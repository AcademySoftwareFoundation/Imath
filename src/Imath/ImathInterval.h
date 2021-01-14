//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifndef INCLUDED_IMATHINTERVAL_H
#define INCLUDED_IMATHINTERVAL_H

//-------------------------------------------------------------------
//
//	class Imath::Interval<class T>
//	--------------------------------
//
//	An Interval has a min and a max and some miscellaneous
//	functions. It is basically a Box<T> that allows T to be
//	a scalar.
//
//-------------------------------------------------------------------

#include "ImathNamespace.h"
#include "ImathVec.h"

IMATH_INTERNAL_NAMESPACE_HEADER_ENTER

template <class T> class Interval
{
  public:
    //-------------------------
    //  Data Members are public
    //-------------------------

    T min;
    T max;

    //-----------------------------------------------------
    //	Constructors - an "empty" Interval is created by default
    //-----------------------------------------------------

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Interval() noexcept;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Interval (const T& point) noexcept;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Interval (const T& minT, const T& maxT) noexcept;

    //--------------------------------
    //  Operators:  we get != from STL
    //--------------------------------

    IMATH_HOSTDEVICE constexpr bool operator== (const Interval<T>& src) const noexcept;
    IMATH_HOSTDEVICE constexpr bool operator!= (const Interval<T>& src) const noexcept;

    //------------------
    //	Interval manipulation
    //------------------

    IMATH_HOSTDEVICE void makeEmpty() noexcept;
    IMATH_HOSTDEVICE void extendBy (const T& point) noexcept;
    IMATH_HOSTDEVICE void extendBy (const Interval<T>& interval) noexcept;
    IMATH_HOSTDEVICE void makeInfinite() noexcept;

    //---------------------------------------------------
    //	Query functions - these compute results each time
    //---------------------------------------------------

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 T size() const noexcept;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 T center() const noexcept;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool intersects (const T& point) const noexcept;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool intersects (const Interval<T>& interval) const noexcept;

    //----------------
    //	Classification
    //----------------

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool hasVolume() const noexcept;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool isEmpty() const noexcept;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool isInfinite() const noexcept;
};

template <class T> std::ostream& operator<< (std::ostream& s, const Interval<T>& v);

//--------------------
// Convenient typedefs
//--------------------

typedef Interval<float> Intervalf;
typedef Interval<double> Intervald;
typedef Interval<short> Intervals;
typedef Interval<int> Intervali;

//----------------
//  Implementation
//----------------

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
    min = limits<T>::max();
    max = limits<T>::min();
}

template <class T>
inline void
Interval<T>::makeInfinite() noexcept
{
    min = limits<T>::min();
    max = limits<T>::max();
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
    if (min != limits<T>::min() || max != limits<T>::max())
        return false;

    return true;
}

template <class T>
std::ostream&
operator<< (std::ostream& s, const Interval<T>& v)
{
    return s << '(' << v.min << ' ' << v.max << ')';
}

IMATH_INTERNAL_NAMESPACE_HEADER_EXIT

#endif // INCLUDED_IMATHINTERVAL_H

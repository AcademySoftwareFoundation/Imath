///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2002-2012, Industrial Light & Magic, a division of Lucas
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

//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifndef INCLUDED_IMATHCOLOR_H
#define INCLUDED_IMATHCOLOR_H

//----------------------------------------------------
//
//	A three and four component color class template.
//
//----------------------------------------------------

#include "ImathNamespace.h"
#include "ImathVec.h"
#include "half.h"

IMATH_INTERNAL_NAMESPACE_HEADER_ENTER

template <class T> class Color3 : public Vec3<T>
{
  public:
    //-------------
    // Constructors
    //-------------

    IMATH_HOSTDEVICE Color3() noexcept;                         // no initialization
    IMATH_HOSTDEVICE constexpr explicit Color3 (T a) noexcept;  // (a a a)
    IMATH_HOSTDEVICE constexpr Color3 (T a, T b, T c) noexcept; // (a b c)
    ~Color3() = default;

    //---------------------------------
    // Copy constructors and assignment
    //---------------------------------

    IMATH_HOSTDEVICE constexpr Color3 (const Color3& c) noexcept;
    template <class S> IMATH_HOSTDEVICE constexpr Color3 (const Vec3<S>& v) noexcept;

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Color3& operator= (const Color3& c) noexcept;

    //------------------------
    // Component-wise addition
    //------------------------

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Color3& operator+= (const Color3& c) noexcept;
    IMATH_HOSTDEVICE constexpr Color3 operator+ (const Color3& c) const noexcept;

    //---------------------------
    // Component-wise subtraction
    //---------------------------

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Color3& operator-= (const Color3& c) noexcept;
    IMATH_HOSTDEVICE constexpr Color3 operator- (const Color3& c) const noexcept;

    //------------------------------------
    // Component-wise multiplication by -1
    //------------------------------------

    IMATH_HOSTDEVICE constexpr Color3 operator-() const noexcept;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Color3& negate() noexcept;

    //------------------------------
    // Component-wise multiplication
    //------------------------------

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Color3& operator*= (const Color3& c) noexcept;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Color3& operator*= (T a) noexcept;
    IMATH_HOSTDEVICE constexpr Color3 operator* (const Color3& c) const noexcept;
    IMATH_HOSTDEVICE constexpr Color3 operator* (T a) const noexcept;

    //------------------------
    // Component-wise division
    //------------------------

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Color3& operator/= (const Color3& c) noexcept;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Color3& operator/= (T a) noexcept;
    IMATH_HOSTDEVICE constexpr Color3 operator/ (const Color3& c) const noexcept;
    IMATH_HOSTDEVICE constexpr Color3 operator/ (T a) const noexcept;
};

template <class T> class Color4
{
  public:
    //-------------------
    // Access to elements
    //-------------------

    T r, g, b, a;

    IMATH_HOSTDEVICE T& operator[] (int i) noexcept;
    IMATH_HOSTDEVICE const T& operator[] (int i) const noexcept;

    //-------------
    // Constructors
    //-------------

    IMATH_HOSTDEVICE Color4() noexcept;                                      // no initialization
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 explicit Color4 (T a) noexcept;       // (a a a a)
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Color4 (T a, T b, T c, T d) noexcept; // (a b c d)
    ~Color4() = default;

    //---------------------------------
    // Copy constructors and assignment
    //---------------------------------

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Color4 (const Color4& v) noexcept;
    template <class S> IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Color4 (const Color4<S>& v) noexcept;

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Color4& operator= (const Color4& v) noexcept;

    //----------------------
    // Compatibility with Sb
    //----------------------

    template <class S> IMATH_HOSTDEVICE void setValue (S a, S b, S c, S d) noexcept;

    template <class S> IMATH_HOSTDEVICE void setValue (const Color4<S>& v) noexcept;

    template <class S> IMATH_HOSTDEVICE void getValue (S& a, S& b, S& c, S& d) const noexcept;

    template <class S> IMATH_HOSTDEVICE void getValue (Color4<S>& v) const noexcept;

    IMATH_HOSTDEVICE T* getValue() noexcept;
    IMATH_HOSTDEVICE const T* getValue() const noexcept;

    //---------
    // Equality
    //---------

    template <class S> IMATH_HOSTDEVICE constexpr bool operator== (const Color4<S>& v) const noexcept;

    template <class S> IMATH_HOSTDEVICE constexpr bool operator!= (const Color4<S>& v) const noexcept;

    //------------------------
    // Component-wise addition
    //------------------------

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Color4& operator+= (const Color4& v) noexcept;
    IMATH_HOSTDEVICE constexpr Color4 operator+ (const Color4& v) const noexcept;

    //---------------------------
    // Component-wise subtraction
    //---------------------------

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Color4& operator-= (const Color4& v) noexcept;
    IMATH_HOSTDEVICE constexpr Color4 operator- (const Color4& v) const noexcept;

    //------------------------------------
    // Component-wise multiplication by -1
    //------------------------------------

    IMATH_HOSTDEVICE constexpr Color4 operator-() const noexcept;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Color4& negate() noexcept;

    //------------------------------
    // Component-wise multiplication
    //------------------------------

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Color4& operator*= (const Color4& v) noexcept;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Color4& operator*= (T a) noexcept;
    IMATH_HOSTDEVICE constexpr Color4 operator* (const Color4& v) const noexcept;
    IMATH_HOSTDEVICE constexpr Color4 operator* (T a) const noexcept;

    //------------------------
    // Component-wise division
    //------------------------

    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Color4& operator/= (const Color4& v) noexcept;
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Color4& operator/= (T a) noexcept;
    IMATH_HOSTDEVICE constexpr Color4 operator/ (const Color4& v) const noexcept;
    IMATH_HOSTDEVICE constexpr Color4 operator/ (T a) const noexcept;

    //----------------------------------------------------------
    // Number of dimensions, i.e. number of elements in a Color4
    //----------------------------------------------------------

    IMATH_HOSTDEVICE constexpr static unsigned int dimensions() noexcept { return 4; }

    //-------------------------------------------------
    // Limitations of type T (see also class limits<T>)
    //-------------------------------------------------

    IMATH_HOSTDEVICE constexpr static T baseTypeMin() noexcept { return limits<T>::min(); }
    IMATH_HOSTDEVICE constexpr static T baseTypeMax() noexcept { return limits<T>::max(); }
    IMATH_HOSTDEVICE constexpr static T baseTypeSmallest() noexcept { return limits<T>::smallest(); }
    IMATH_HOSTDEVICE constexpr static T baseTypeEpsilon() noexcept { return limits<T>::epsilon(); }

    //--------------------------------------------------------------
    // Base type -- in templates, which accept a parameter, V, which
    // could be a Color4<T>, you can refer to T as
    // V::BaseType
    //--------------------------------------------------------------

    typedef T BaseType;
};

//--------------
// Stream output
//--------------

template <class T> std::ostream& operator<< (std::ostream& s, const Color4<T>& v);

//----------------------------------------------------
// Reverse multiplication: S * Color4<T>
//----------------------------------------------------

template <class S, class T> constexpr Color4<T> operator* (S a, const Color4<T>& v) noexcept;

//-------------------------
// Typedefs for convenience
//-------------------------

typedef Color3<float> Color3f;
typedef Color3<half> Color3h;
typedef Color3<unsigned char> Color3c;
typedef Color3<half> C3h;
typedef Color3<float> C3f;
typedef Color3<unsigned char> C3c;
typedef Color4<float> Color4f;
typedef Color4<half> Color4h;
typedef Color4<unsigned char> Color4c;
typedef Color4<float> C4f;
typedef Color4<half> C4h;
typedef Color4<unsigned char> C4c;
typedef unsigned int PackedColor;

//-------------------------
// Implementation of Color3
//-------------------------

template <class T> inline Color3<T>::Color3() noexcept : Vec3<T>()
{
    // empty
}

template <class T> constexpr inline Color3<T>::Color3 (T a) noexcept : Vec3<T> (a)
{
    // empty
}

template <class T> constexpr inline Color3<T>::Color3 (T a, T b, T c) noexcept : Vec3<T> (a, b, c)
{
    // empty
}

template <class T> constexpr inline Color3<T>::Color3 (const Color3& c) noexcept : Vec3<T> (c)
{
    // empty
}

template <class T>
template <class S>
constexpr inline Color3<T>::Color3 (const Vec3<S>& v) noexcept : Vec3<T> (v)
{
    //empty
}

template <class T>
IMATH_CONSTEXPR14 inline const Color3<T>&
Color3<T>::operator= (const Color3& c) noexcept
{
    *((Vec3<T>*) this) = c;
    return *this;
}

template <class T>
IMATH_CONSTEXPR14 inline const Color3<T>&
Color3<T>::operator+= (const Color3& c) noexcept
{
    *((Vec3<T>*) this) += c;
    return *this;
}

template <class T>
constexpr inline Color3<T>
Color3<T>::operator+ (const Color3& c) const noexcept
{
    return Color3 (*(Vec3<T>*) this + (const Vec3<T>&) c);
}

template <class T>
IMATH_CONSTEXPR14 inline const Color3<T>&
Color3<T>::operator-= (const Color3& c) noexcept
{
    *((Vec3<T>*) this) -= c;
    return *this;
}

template <class T>
constexpr inline Color3<T>
Color3<T>::operator- (const Color3& c) const noexcept
{
    return Color3 (*(Vec3<T>*) this - (const Vec3<T>&) c);
}

template <class T>
constexpr inline Color3<T>
Color3<T>::operator-() const noexcept
{
    return Color3 (-(*(Vec3<T>*) this));
}

template <class T>
IMATH_CONSTEXPR14 inline const Color3<T>&
Color3<T>::negate() noexcept
{
    ((Vec3<T>*) this)->negate();
    return *this;
}

template <class T>
IMATH_CONSTEXPR14 inline const Color3<T>&
Color3<T>::operator*= (const Color3& c) noexcept
{
    *((Vec3<T>*) this) *= c;
    return *this;
}

template <class T>
IMATH_CONSTEXPR14 inline const Color3<T>&
Color3<T>::operator*= (T a) noexcept
{
    *((Vec3<T>*) this) *= a;
    return *this;
}

template <class T>
constexpr inline Color3<T>
Color3<T>::operator* (const Color3& c) const noexcept
{
    return Color3 (*(Vec3<T>*) this * (const Vec3<T>&) c);
}

template <class T>
constexpr inline Color3<T>
Color3<T>::operator* (T a) const noexcept
{
    return Color3 (*(Vec3<T>*) this * a);
}

template <class T>
IMATH_CONSTEXPR14 inline const Color3<T>&
Color3<T>::operator/= (const Color3& c) noexcept
{
    *((Vec3<T>*) this) /= c;
    return *this;
}

template <class T>
IMATH_CONSTEXPR14 inline const Color3<T>&
Color3<T>::operator/= (T a) noexcept
{
    *((Vec3<T>*) this) /= a;
    return *this;
}

template <class T>
constexpr inline Color3<T>
Color3<T>::operator/ (const Color3& c) const noexcept
{
    return Color3 (*(Vec3<T>*) this / (const Vec3<T>&) c);
}

template <class T>
constexpr inline Color3<T>
Color3<T>::operator/ (T a) const noexcept
{
    return Color3 (*(Vec3<T>*) this / a);
}

//-----------------------
// Implementation of Color4
//-----------------------

template <class T>
inline T&
Color4<T>::operator[] (int i) noexcept
{
    return (&r)[i];
}

template <class T>
inline const T&
Color4<T>::operator[] (int i) const noexcept
{
    return (&r)[i];
}

template <class T> inline Color4<T>::Color4() noexcept
{
    // empty
}

template <class T> IMATH_CONSTEXPR14 inline Color4<T>::Color4 (T x) noexcept
{
    r = g = b = a = x;
}

template <class T> IMATH_CONSTEXPR14 inline Color4<T>::Color4 (T x, T y, T z, T w) noexcept
{
    r = x;
    g = y;
    b = z;
    a = w;
}

template <class T> IMATH_CONSTEXPR14 inline Color4<T>::Color4 (const Color4& v) noexcept
{
    r = v.r;
    g = v.g;
    b = v.b;
    a = v.a;
}

template <class T>
template <class S>
IMATH_CONSTEXPR14 inline Color4<T>::Color4 (const Color4<S>& v) noexcept
{
    r = T (v.r);
    g = T (v.g);
    b = T (v.b);
    a = T (v.a);
}

template <class T>
IMATH_CONSTEXPR14 inline const Color4<T>&
Color4<T>::operator= (const Color4& v) noexcept
{
    r = v.r;
    g = v.g;
    b = v.b;
    a = v.a;
    return *this;
}

template <class T>
template <class S>
inline void
Color4<T>::setValue (S x, S y, S z, S w) noexcept
{
    r = T (x);
    g = T (y);
    b = T (z);
    a = T (w);
}

template <class T>
template <class S>
inline void
Color4<T>::setValue (const Color4<S>& v) noexcept
{
    r = T (v.r);
    g = T (v.g);
    b = T (v.b);
    a = T (v.a);
}

template <class T>
template <class S>
inline void
Color4<T>::getValue (S& x, S& y, S& z, S& w) const noexcept
{
    x = S (r);
    y = S (g);
    z = S (b);
    w = S (a);
}

template <class T>
template <class S>
inline void
Color4<T>::getValue (Color4<S>& v) const noexcept
{
    v.r = S (r);
    v.g = S (g);
    v.b = S (b);
    v.a = S (a);
}

template <class T>
inline T*
Color4<T>::getValue() noexcept
{
    return (T*) &r;
}

template <class T>
inline const T*
Color4<T>::getValue() const noexcept
{
    return (const T*) &r;
}

template <class T>
template <class S>
constexpr inline bool
Color4<T>::operator== (const Color4<S>& v) const noexcept
{
    return r == v.r && g == v.g && b == v.b && a == v.a;
}

template <class T>
template <class S>
constexpr inline bool
Color4<T>::operator!= (const Color4<S>& v) const noexcept
{
    return r != v.r || g != v.g || b != v.b || a != v.a;
}

template <class T>
IMATH_CONSTEXPR14 inline const Color4<T>&
Color4<T>::operator+= (const Color4& v) noexcept
{
    r += v.r;
    g += v.g;
    b += v.b;
    a += v.a;
    return *this;
}

template <class T>
constexpr inline Color4<T>
Color4<T>::operator+ (const Color4& v) const noexcept
{
    return Color4 (r + v.r, g + v.g, b + v.b, a + v.a);
}

template <class T>
IMATH_CONSTEXPR14 inline const Color4<T>&
Color4<T>::operator-= (const Color4& v) noexcept
{
    r -= v.r;
    g -= v.g;
    b -= v.b;
    a -= v.a;
    return *this;
}

template <class T>
constexpr inline Color4<T>
Color4<T>::operator- (const Color4& v) const noexcept
{
    return Color4 (r - v.r, g - v.g, b - v.b, a - v.a);
}

template <class T>
constexpr inline Color4<T>
Color4<T>::operator-() const noexcept
{
    return Color4 (-r, -g, -b, -a);
}

template <class T>
IMATH_CONSTEXPR14 inline const Color4<T>&
Color4<T>::negate() noexcept
{
    r = -r;
    g = -g;
    b = -b;
    a = -a;
    return *this;
}

template <class T>
IMATH_CONSTEXPR14 inline const Color4<T>&
Color4<T>::operator*= (const Color4& v) noexcept
{
    r *= v.r;
    g *= v.g;
    b *= v.b;
    a *= v.a;
    return *this;
}

template <class T>
IMATH_CONSTEXPR14 inline const Color4<T>&
Color4<T>::operator*= (T x) noexcept
{
    r *= x;
    g *= x;
    b *= x;
    a *= x;
    return *this;
}

template <class T>
constexpr inline Color4<T>
Color4<T>::operator* (const Color4& v) const noexcept
{
    return Color4 (r * v.r, g * v.g, b * v.b, a * v.a);
}

template <class T>
constexpr inline Color4<T>
Color4<T>::operator* (T x) const noexcept
{
    return Color4 (r * x, g * x, b * x, a * x);
}

template <class T>
IMATH_CONSTEXPR14 inline const Color4<T>&
Color4<T>::operator/= (const Color4& v) noexcept
{
    r /= v.r;
    g /= v.g;
    b /= v.b;
    a /= v.a;
    return *this;
}

template <class T>
IMATH_CONSTEXPR14 inline const Color4<T>&
Color4<T>::operator/= (T x) noexcept
{
    r /= x;
    g /= x;
    b /= x;
    a /= x;
    return *this;
}

template <class T>
constexpr inline Color4<T>
Color4<T>::operator/ (const Color4& v) const noexcept
{
    return Color4 (r / v.r, g / v.g, b / v.b, a / v.a);
}

template <class T>
constexpr inline Color4<T>
Color4<T>::operator/ (T x) const noexcept
{
    return Color4 (r / x, g / x, b / x, a / x);
}

template <class T>
std::ostream&
operator<< (std::ostream& s, const Color4<T>& v)
{
    return s << '(' << v.r << ' ' << v.g << ' ' << v.b << ' ' << v.a << ')';
}

//-----------------------------------------
// Implementation of reverse multiplication
//-----------------------------------------

template <class S, class T>
constexpr inline Color4<T>
operator* (S x, const Color4<T>& v) noexcept
{
    return Color4<T> (x * v.r, x * v.g, x * v.b, x * v.a);
}

IMATH_INTERNAL_NAMESPACE_HEADER_EXIT

#endif // INCLUDED_IMATHCOLOR_H

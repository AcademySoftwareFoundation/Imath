//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifdef NDEBUG
#    undef NDEBUG
#endif

#include "testVec.h"
#include <ImathFun.h>
#include <ImathVec.h>
#include <cassert>
#include <cmath>
#include <iostream>
#include <type_traits>
#include <utility>

// Include ImathForward *after* other headers to validate forward declarations
#include <ImathForward.h>

using namespace std;
using namespace IMATH_INTERNAL_NAMESPACE;

namespace
{

//
// Compile-time detection of whether an expression is well-formed. This is
// used to confirm that length(), normalize(), normalizeExc(),
// normalizeNonNull(), normalized(), normalizedExc(), and
// normalizedNonNull() are visible (via SFINAE) on Vec2/Vec3/Vec4 for
// floating-point-like element types (float, double, half), and are not
// visible at all for integer element types (short, int, int64_t).
//

#define IMATH_TEST_HAS_METHOD(name)                                            \
    template <typename T, typename = void>                                     \
    struct has_##name : std::false_type                                        \
    {};                                                                        \
    template <typename T>                                                      \
    struct has_##name<                                                         \
        T,                                                                     \
        decltype ((void) std::declval<T> ().name ())> : std::true_type         \
    {}

IMATH_TEST_HAS_METHOD (length);
IMATH_TEST_HAS_METHOD (normalize);
IMATH_TEST_HAS_METHOD (normalizeExc);
IMATH_TEST_HAS_METHOD (normalizeNonNull);
IMATH_TEST_HAS_METHOD (normalized);
IMATH_TEST_HAS_METHOD (normalizedExc);
IMATH_TEST_HAS_METHOD (normalizedNonNull);

#undef IMATH_TEST_HAS_METHOD

// Confirm that the given VecN<T> supports all of length()/normalize()/etc.
#define IMATH_STATIC_ASSERT_HAS_ALL(Vec, T)                                    \
    static_assert (has_length<Vec<T>>::value, #Vec "<" #T ">::length");        \
    static_assert (has_normalize<Vec<T>>::value, #Vec "<" #T ">::normalize");  \
    static_assert (                                                            \
        has_normalizeExc<Vec<T>>::value, #Vec "<" #T ">::normalizeExc");       \
    static_assert (                                                            \
        has_normalizeNonNull<Vec<T>>::value,                                   \
        #Vec "<" #T ">::normalizeNonNull");                                    \
    static_assert (                                                            \
        has_normalized<Vec<T>>::value, #Vec "<" #T ">::normalized");           \
    static_assert (                                                            \
        has_normalizedExc<Vec<T>>::value, #Vec "<" #T ">::normalizedExc");     \
    static_assert (                                                            \
        has_normalizedNonNull<Vec<T>>::value,                                  \
        #Vec "<" #T ">::normalizedNonNull")

// Confirm that the given VecN<T> supports none of length()/normalize()/etc.
#define IMATH_STATIC_ASSERT_HAS_NONE(Vec, T)                                   \
    static_assert (!has_length<Vec<T>>::value, #Vec "<" #T ">::length");       \
    static_assert (                                                            \
        !has_normalize<Vec<T>>::value, #Vec "<" #T ">::normalize");            \
    static_assert (                                                            \
        !has_normalizeExc<Vec<T>>::value, #Vec "<" #T ">::normalizeExc");      \
    static_assert (                                                            \
        !has_normalizeNonNull<Vec<T>>::value,                                  \
        #Vec "<" #T ">::normalizeNonNull");                                    \
    static_assert (                                                            \
        !has_normalized<Vec<T>>::value, #Vec "<" #T ">::normalized");          \
    static_assert (                                                            \
        !has_normalizedExc<Vec<T>>::value, #Vec "<" #T ">::normalizedExc");    \
    static_assert (                                                            \
        !has_normalizedNonNull<Vec<T>>::value,                                 \
        #Vec "<" #T ">::normalizedNonNull")

// Floating-point-like element types: length()/normalize()/etc. must be
// present.
IMATH_STATIC_ASSERT_HAS_ALL (Vec2, float);
IMATH_STATIC_ASSERT_HAS_ALL (Vec2, double);
IMATH_STATIC_ASSERT_HAS_ALL (Vec2, half);
IMATH_STATIC_ASSERT_HAS_ALL (Vec3, float);
IMATH_STATIC_ASSERT_HAS_ALL (Vec3, double);
IMATH_STATIC_ASSERT_HAS_ALL (Vec3, half);
IMATH_STATIC_ASSERT_HAS_ALL (Vec4, float);
IMATH_STATIC_ASSERT_HAS_ALL (Vec4, double);
IMATH_STATIC_ASSERT_HAS_ALL (Vec4, half);

// Integer element types: length()/normalize()/etc. must not be present.
IMATH_STATIC_ASSERT_HAS_NONE (Vec2, short);
IMATH_STATIC_ASSERT_HAS_NONE (Vec2, int);
IMATH_STATIC_ASSERT_HAS_NONE (Vec2, int64_t);
IMATH_STATIC_ASSERT_HAS_NONE (Vec3, short);
IMATH_STATIC_ASSERT_HAS_NONE (Vec3, int);
IMATH_STATIC_ASSERT_HAS_NONE (Vec3, int64_t);
IMATH_STATIC_ASSERT_HAS_NONE (Vec4, short);
IMATH_STATIC_ASSERT_HAS_NONE (Vec4, int);
IMATH_STATIC_ASSERT_HAS_NONE (Vec4, int64_t);

//
// An application-defined scalar class, analogous to `half`, that behaves
// like a floating-point number but is not one of the builtin
// floating-point types. An application that wants Vec2/Vec3/Vec4 of this
// type to support length()/normalize()/etc. must specialize
// Imath::is_float_like<> for it, exactly as Imath itself does for `half`.
//
class CustomFloat
{
public:
    CustomFloat () IMATH_NOEXCEPT : _v (0.0) {}
    CustomFloat (double v) IMATH_NOEXCEPT : _v (v) {}
    operator double () const IMATH_NOEXCEPT { return _v; }

    CustomFloat& operator+= (const CustomFloat& o) IMATH_NOEXCEPT
    {
        _v += o._v;
        return *this;
    }
    CustomFloat& operator-= (const CustomFloat& o) IMATH_NOEXCEPT
    {
        _v -= o._v;
        return *this;
    }
    CustomFloat& operator*= (const CustomFloat& o) IMATH_NOEXCEPT
    {
        _v *= o._v;
        return *this;
    }
    CustomFloat& operator/= (const CustomFloat& o) IMATH_NOEXCEPT
    {
        _v /= o._v;
        return *this;
    }

    friend CustomFloat
    operator+ (CustomFloat a, const CustomFloat& b) IMATH_NOEXCEPT
    {
        a += b;
        return a;
    }
    friend CustomFloat
    operator- (CustomFloat a, const CustomFloat& b) IMATH_NOEXCEPT
    {
        a -= b;
        return a;
    }
    friend CustomFloat
    operator* (CustomFloat a, const CustomFloat& b) IMATH_NOEXCEPT
    {
        a *= b;
        return a;
    }
    friend CustomFloat
    operator/ (CustomFloat a, const CustomFloat& b) IMATH_NOEXCEPT
    {
        a /= b;
        return a;
    }
    friend CustomFloat operator- (const CustomFloat& a) IMATH_NOEXCEPT
    {
        return CustomFloat (-a._v);
    }

    // Mixed CustomFloat/double overloads: without these, expressions like
    // `customFloatValue * std::sqrt (x)` (which mixes CustomFloat with a
    // plain double) are ambiguous, since both the implicit
    // double-to-CustomFloat constructor and the implicit
    // CustomFloat-to-double conversion operator are viable, equally-ranked
    // conversions.
    friend CustomFloat operator* (const CustomFloat& a, double b) IMATH_NOEXCEPT
    {
        return CustomFloat (a._v * b);
    }
    friend CustomFloat operator* (double a, const CustomFloat& b) IMATH_NOEXCEPT
    {
        return CustomFloat (a * b._v);
    }

    friend bool
    operator== (const CustomFloat& a, const CustomFloat& b) IMATH_NOEXCEPT
    {
        return a._v == b._v;
    }
    friend bool
    operator!= (const CustomFloat& a, const CustomFloat& b) IMATH_NOEXCEPT
    {
        return a._v != b._v;
    }
    friend bool
    operator< (const CustomFloat& a, const CustomFloat& b) IMATH_NOEXCEPT
    {
        return a._v < b._v;
    }
    friend bool
    operator> (const CustomFloat& a, const CustomFloat& b) IMATH_NOEXCEPT
    {
        return a._v > b._v;
    }
    friend bool
    operator<= (const CustomFloat& a, const CustomFloat& b) IMATH_NOEXCEPT
    {
        return a._v <= b._v;
    }
    friend bool
    operator>= (const CustomFloat& a, const CustomFloat& b) IMATH_NOEXCEPT
    {
        return a._v >= b._v;
    }

private:
    double _v;
};

//
// An application-defined scalar class for which length()/normalize()/etc.
// should *not* be visible, because it does not represent a floating-point
// quantity (e.g. some kind of identifier, index, or fixed-point count) and
// the application does not specialize Imath::is_float_like<> for it.
//
class CustomNonFloat
{
public:
    CustomNonFloat () IMATH_NOEXCEPT : _v (0) {}
    CustomNonFloat (int v) IMATH_NOEXCEPT : _v (v) {}
    operator int () const IMATH_NOEXCEPT { return _v; }

    friend CustomNonFloat
    operator+ (CustomNonFloat a, const CustomNonFloat& b) IMATH_NOEXCEPT
    {
        return CustomNonFloat (a._v + b._v);
    }
    friend CustomNonFloat
    operator- (CustomNonFloat a, const CustomNonFloat& b) IMATH_NOEXCEPT
    {
        return CustomNonFloat (a._v - b._v);
    }
    friend CustomNonFloat
    operator* (CustomNonFloat a, const CustomNonFloat& b) IMATH_NOEXCEPT
    {
        return CustomNonFloat (a._v * b._v);
    }
    friend bool operator== (
        const CustomNonFloat& a, const CustomNonFloat& b) IMATH_NOEXCEPT
    {
        return a._v == b._v;
    }

private:
    int _v;
};

} // namespace

// Tell Imath that CustomFloat should be treated as float-like, exactly as
// an application would do for its own custom scalar type. CustomNonFloat
// is deliberately left unspecialized, so it defaults to
// std::is_floating_point<CustomNonFloat>::value, i.e. false.
IMATH_INTERNAL_NAMESPACE_SOURCE_ENTER
template <> struct is_float_like<CustomFloat> : public std::true_type
{};
IMATH_INTERNAL_NAMESPACE_SOURCE_EXIT

namespace
{

// Floating-point-like custom type: length()/normalize()/etc. must be
// present.
IMATH_STATIC_ASSERT_HAS_ALL (Vec2, CustomFloat);
IMATH_STATIC_ASSERT_HAS_ALL (Vec3, CustomFloat);
IMATH_STATIC_ASSERT_HAS_ALL (Vec4, CustomFloat);

// Non-floating-point custom type: length()/normalize()/etc. must not be
// present.
IMATH_STATIC_ASSERT_HAS_NONE (Vec2, CustomNonFloat);
IMATH_STATIC_ASSERT_HAS_NONE (Vec3, CustomNonFloat);
IMATH_STATIC_ASSERT_HAS_NONE (Vec4, CustomNonFloat);

#undef IMATH_STATIC_ASSERT_HAS_ALL
#undef IMATH_STATIC_ASSERT_HAS_NONE

void
testCustomFloatLike ()
{
    const double e = 1e-9;

    // Vec2<CustomFloat>
    {
        Vec2<CustomFloat> v (3, 4);
        assert (
            IMATH_INTERNAL_NAMESPACE::equal (double (v.length ()), 5.0, e));
        assert (IMATH_INTERNAL_NAMESPACE::equal (
            double (v.normalized ().length ()), 1.0, e));
        v.normalize ();
        assert (
            IMATH_INTERNAL_NAMESPACE::equal (double (v.length ()), 1.0, e));
    }

    // Vec3<CustomFloat>
    {
        Vec3<CustomFloat> v (0, 3, 4);
        assert (
            IMATH_INTERNAL_NAMESPACE::equal (double (v.length ()), 5.0, e));
        assert (IMATH_INTERNAL_NAMESPACE::equal (
            double (v.normalized ().length ()), 1.0, e));
        v.normalize ();
        assert (
            IMATH_INTERNAL_NAMESPACE::equal (double (v.length ()), 1.0, e));
    }

    // Vec4<CustomFloat>
    {
        Vec4<CustomFloat> v (0, 0, 3, 4);
        assert (
            IMATH_INTERNAL_NAMESPACE::equal (double (v.length ()), 5.0, e));
        assert (IMATH_INTERNAL_NAMESPACE::equal (
            double (v.normalized ().length ()), 1.0, e));
        v.normalize ();
        assert (
            IMATH_INTERNAL_NAMESPACE::equal (double (v.length ()), 1.0, e));
    }

    // CustomNonFloat still supports ordinary Vec operations, such as
    // arithmetic and equality, that are not gated by is_float_like<>.
    {
        Vec3<CustomNonFloat> a (1, 2, 3);
        Vec3<CustomNonFloat> b (4, 5, 6);
        Vec3<CustomNonFloat> c = a + b;
        assert (int (c.x) == 5 && int (c.y) == 7 && int (c.z) == 9);
        assert (a == a);
        assert (!(a == b));
    }
}

} // namespace

namespace
{

template <class T>
void
testLength2T ()
{
    const T s = std::sqrt (std::numeric_limits<T>::min ());
    const T e = 4 * std::numeric_limits<T>::epsilon ();

    Vec2<T> v;

    v = Vec2<T> (0, 0);
    assert (v.length () == 0);
    assert (v.normalized ().length () == 0);

    v = Vec2<T> (3, 4);
    assert (v.length () == 5);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    v = Vec2<T> (3000, 4000);
    assert (v.length () == 5000);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    T t = s * (1 << 4);

    v = Vec2<T> (t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec2<T> (0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec2<T> (-t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (
        v.length (), t * std::sqrt (2), t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    t = s / (1 << 4);

    v = Vec2<T> (t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec2<T> (0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec2<T> (-t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (
        v.length (), t * std::sqrt (2), t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    t = s / (1 << 20);

    v = Vec2<T> (t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec2<T> (0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec2<T> (-t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (
        v.length (), t * std::sqrt (2), t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
}

template <class T>
void
testLength3T ()
{
    const T s = std::sqrt (std::numeric_limits<T>::min ());
    const T e = 4 * std::numeric_limits<T>::epsilon ();

    Vec3<T> v;

    v = Vec3<T> (0, 0, 0);
    assert (v.length () == 0);
    assert (v.normalized ().length () == 0);

    v = Vec3<T> (3, 4, 0);
    assert (v.length () == 5);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    v = Vec3<T> (3000, 4000, 0);
    assert (v.length () == 5000);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    v = Vec3<T> (1, -1, 1);
    assert (
        IMATH_INTERNAL_NAMESPACE::equal (v.length (), 1 * std::sqrt (3), e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    v = Vec3<T> (1000, -1000, 1000);
    assert (IMATH_INTERNAL_NAMESPACE::equal (
        v.length (), 1000 * std::sqrt (3), 1000 * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    T t = s * (1 << 4);

    v = Vec3<T> (t, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec3<T> (0, t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec3<T> (0, 0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec3<T> (-t, -t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (
        v.length (), t * std::sqrt (3), t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    t = s / (1 << 4);

    v = Vec3<T> (t, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec3<T> (0, t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec3<T> (0, 0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec3<T> (-t, -t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (
        v.length (), t * std::sqrt (3), t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    t = s / (1 << 20);

    v = Vec3<T> (t, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec3<T> (0, t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec3<T> (0, 0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec3<T> (-t, -t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (
        v.length (), t * std::sqrt (3), t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
}

template <class T>
void
testLength4T ()
{
    const T s = std::sqrt (std::numeric_limits<T>::min ());
    const T e = 4 * std::numeric_limits<T>::epsilon ();

    Vec4<T> v;

    v = Vec4<T> (0, 0, 0, 0);
    assert (v.length () == 0);
    assert (v.normalized ().length () == 0);

    v = Vec4<T> (3, 4, 0, 0);
    assert (v.length () == 5);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    v = Vec4<T> (3000, 4000, 0, 0);
    assert (v.length () == 5000);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    v = Vec4<T> (1, -1, 1, 1);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), 2, e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    v = Vec4<T> (1000, -1000, 1000, 1000);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), 2000, 1000 * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    T t = s * (1 << 4);

    v = Vec4<T> (t, 0, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec4<T> (0, t, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec4<T> (0, 0, t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec4<T> (0, 0, 0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec4<T> (-t, -t, -t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t * 2, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    t = s / (1 << 4);

    v = Vec4<T> (t, 0, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec4<T> (0, t, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec4<T> (0, 0, t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec4<T> (0, 0, 0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec4<T> (-t, -t, -t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t * 2, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    t = s / (1 << 20);

    v = Vec4<T> (t, 0, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec4<T> (0, t, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec4<T> (0, 0, t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec4<T> (0, 0, 0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec4<T> (-t, -t, -t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t * 2, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
}

} // namespace

void
testVec ()
{
    cout << "Testing some basic vector operations" << endl;

    testLength2T<float> ();
    testLength2T<double> ();
    testLength3T<float> ();
    testLength3T<double> ();
    testLength4T<float> ();
    testLength4T<double> ();

    testCustomFloatLike ();

    cout << "ok\n" << endl;
}

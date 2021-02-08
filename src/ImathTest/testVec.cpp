//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifdef NDEBUG
#    undef NDEBUG
#endif

#include "ImathFun.h"
#include "ImathVec.h"
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include <testVec.h>

using namespace std;
using namespace IMATH_INTERNAL_NAMESPACE;

namespace
{

template <class T>
void
testLength2T()
{
    const T s = std::sqrt (std::numeric_limits<T>::min());
    const T e = 4 * std::numeric_limits<T>::epsilon();

    Vec2<T> v;

    v = Vec2<T> (0, 0);
    assert (v.length() == 0);
    assert (v.normalized().length() == 0);

    v = Vec2<T> (3, 4);
    assert (v.length() == 5);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));

    v = Vec2<T> (3000, 4000);
    assert (v.length() == 5000);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));

    T t = s * (1 << 4);

    v = Vec2<T> (t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec2<T> (0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec2<T> (-t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t * std::sqrt (2), t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));

    t = s / (1 << 4);

    v = Vec2<T> (t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec2<T> (0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec2<T> (-t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t * std::sqrt (2), t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));

    t = s / (1 << 20);

    v = Vec2<T> (t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec2<T> (0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec2<T> (-t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t * std::sqrt (2), t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
}

template <class T>
void
testLength3T()
{
    const T s = std::sqrt (std::numeric_limits<T>::min());
    const T e = 4 * std::numeric_limits<T>::epsilon();

    Vec3<T> v;

    v = Vec3<T> (0, 0, 0);
    assert (v.length() == 0);
    assert (v.normalized().length() == 0);

    v = Vec3<T> (3, 4, 0);
    assert (v.length() == 5);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));

    v = Vec3<T> (3000, 4000, 0);
    assert (v.length() == 5000);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));

    v = Vec3<T> (1, -1, 1);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), 1 * std::sqrt (3), e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));

    v = Vec3<T> (1000, -1000, 1000);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), 1000 * std::sqrt (3), 1000 * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));

    T t = s * (1 << 4);

    v = Vec3<T> (t, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec3<T> (0, t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec3<T> (0, 0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec3<T> (-t, -t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t * std::sqrt (3), t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));

    t = s / (1 << 4);

    v = Vec3<T> (t, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec3<T> (0, t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec3<T> (0, 0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec3<T> (-t, -t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t * std::sqrt (3), t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));

    t = s / (1 << 20);

    v = Vec3<T> (t, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec3<T> (0, t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec3<T> (0, 0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec3<T> (-t, -t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t * std::sqrt (3), t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
}

template <class T>
void
testLength4T()
{
    const T s = std::sqrt (std::numeric_limits<T>::min());
    const T e = 4 * std::numeric_limits<T>::epsilon();

    Vec4<T> v;

    v = Vec4<T> (0, 0, 0, 0);
    assert (v.length() == 0);
    assert (v.normalized().length() == 0);

    v = Vec4<T> (3, 4, 0, 0);
    assert (v.length() == 5);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));

    v = Vec4<T> (3000, 4000, 0, 0);
    assert (v.length() == 5000);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));

    v = Vec4<T> (1, -1, 1, 1);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), 2, e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));

    v = Vec4<T> (1000, -1000, 1000, 1000);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), 2000, 1000 * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));

    T t = s * (1 << 4);

    v = Vec4<T> (t, 0, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec4<T> (0, t, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec4<T> (0, 0, t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec4<T> (0, 0, 0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec4<T> (-t, -t, -t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t * 2, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));

    t = s / (1 << 4);

    v = Vec4<T> (t, 0, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec4<T> (0, t, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec4<T> (0, 0, t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec4<T> (0, 0, 0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec4<T> (-t, -t, -t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t * 2, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));

    t = s / (1 << 20);

    v = Vec4<T> (t, 0, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec4<T> (0, t, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec4<T> (0, 0, t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec4<T> (0, 0, 0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
    v = Vec4<T> (-t, -t, -t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length(), t * 2, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized().length(), 1, e));
}



// Extra simple vector that does nothing but allow element access, as an
// example of a vector type that an app might use and want interop with
// our vectors.
template <typename T, int N>
struct SimpleVec {
    T elements[N];

    SimpleVec(T val = T(0)) {
        for (int i = 0; i < N; ++i)
            elements[i] = val;
    }
    ~SimpleVec() = default;
    constexpr T operator[](int i) const { return elements[i]; }
    T& operator[](int i) { return elements[i]; }
};



void
testInteropPass3(const Vec3<float>& v, float a, float b, float c)
{
    assert(v[0] == a && v[1] == b && v[2] == c);
}

void
testInteropCtr3()
{
    // Test construction/assignment/paramater pass of a different vector type
    {
        SimpleVec<float,3> s;
        s[0] = 1;
        s[1] = 2;
        s[2] = 3;
        Vec3<float> v(s);
        assert(v[0] == 1 && v[1] == 2 && v[2] == 3);
        s[1] = 42;
        v = s;
        assert(v[0] == 1 && v[1] == 42 && v[2] == 3);

        testInteropPass3(s, 1, 42, 3);
    }
    // Test construction/assignment/paramater pass of a std::vector of length 3
    {
        std::vector<float> s { 1, 2, 3 };
        Vec3<float> v(s);
        assert(v[0] == 1 && v[1] == 2 && v[2] == 3);
        s[1] = 42;
        v = s;
        assert(v[0] == 1 && v[1] == 42 && v[2] == 3);
        testInteropPass3(s, 1, 42, 3);
    }
    // Test construction/assignment/paramater pass of a std::array of length 3
    {
        std::array<float, 3> s { 1, 2, 3 };
        Vec3<float> v(s);
        assert(v[0] == 1 && v[1] == 2 && v[2] == 3);
        s[1] = 42;
        v = s;
        assert(v[0] == 1 && v[1] == 42 && v[2] == 3);
        testInteropPass3(s, 1, 42, 3);
    }
    // Test construction/assignment/paramater pass of initializer lists.
    {
        Vec3<float> v({ 1.0f, 2.0f, 3.0f });
        assert(v[0] == 1 && v[1] == 2 && v[2] == 3);
        v = { 1.0f, 42.0f, 3.0f };
        assert(v[0] == 1 && v[1] == 42 && v[2] == 3);
        testInteropPass3({ 1.0f, 42.0f, 3.0f }, 1, 42, 3);
    }
#if 0
    // Test construction/assignment/paramater pass of a C array
    {
        float s[3] = { 1, 2, 3 };
        Vec3<float> v(s);
        assert(v[0] == 1 && v[1] == 2 && v[2] == 3);
        s[1] = 42;
        v = s;
        assert(v[0] == 1 && v[1] == 42 && v[2] == 3);
        testInteropPass3(s, 1, 42, 3);
    }
#endif
}


} // namespace

void
testVec()
{
    cout << "Testing some basic vector operations" << endl;

    testLength2T<float>();
    testLength2T<double>();
    testLength3T<float>();
    testLength3T<double>();
    testLength4T<float>();
    testLength4T<double>();

    testInteropCtr3();

    cout << "ok\n" << endl;
}

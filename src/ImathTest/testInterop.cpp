//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifdef NDEBUG
#    undef NDEBUG
#endif

#include <ImathFun.h>
#include <ImathVec.h>
#include <ImathMatrix.h>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include "testVec.h"

using namespace std;
using namespace IMATH_INTERNAL_NAMESPACE;


// Imath::has_subscript fails for std::vector because its length does not
// appear to be the length of N elements. Carve out an exception here that
// allows this to work.
IMATH_INTERNAL_NAMESPACE_SOURCE_ENTER
template<typename T, int N>
struct has_subscript<std::vector<T>, T, N> : public std::true_type { };
IMATH_INTERNAL_NAMESPACE_SOURCE_EXIT

#if IMATH_FOREIGN_VECTOR_INTEROP


namespace
{

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



// Extra simple matrix that does nothing but allow element access, as an
// example of a matrix type that an app might use and want interop with
// our vectors.
template <typename T, int N>
struct SimpleMx {
    T elements[N][N];

    SimpleMx(T val = T(0)) {
        for (int j = 0; j < N; ++j)
            for (int i = 0; i < N; ++i)
                elements[j][i] = val;
    }
    ~SimpleMx() = default;
    const T* operator[](int i) const { return elements[i]; }
    T* operator[](int i) { return elements[i]; }
};



// Small structs containing just x,y,z,w elements.
template<typename T>
struct xy {
    T x, y;
};


template<typename T>
struct xyz {
    T x, y, z;
};


template<typename T>
struct xyzw {
    T x, y, z, w;
};


template<typename T>
struct xyz_wrong {
    T x() { return 0; }
    T y() { return 1; }
    T z() { return 2; }
};



// A class that has *both* subscripting and named members
template<typename T>
struct ComboVec2 {
    union {
        T elements[2];
        struct { T x, y; };
    };

    ComboVec2(T val = T(0)) : x(val), y(val) { }
    ~ComboVec2() = default;
    constexpr T operator[](int i) const { return elements[i]; }
    T& operator[](int i) { return elements[i]; }
};



// A class that has *both* subscripting and named members
template<typename T>
struct ComboVec3 {
    union {
        T elements[3];
        struct { T x, y, z; };
    };

    ComboVec3(T val = T(0)) : x(val), y(val), z(val) { }
    ~ComboVec3() = default;
    constexpr T operator[](int i) const { return elements[i]; }
    T& operator[](int i) { return elements[i]; }
};



// A class that has *both* subscripting and named members
template<typename T>
struct ComboVec4 {
    union {
        T elements[4];
        struct { T x, y, z, w; };
    };

    ComboVec4(T val = T(0)) : x(val), y(val), z(val), w(val) { }
    ~ComboVec4() = default;
    constexpr T operator[](int i) const { return elements[i]; }
    T& operator[](int i) { return elements[i]; }
};



// Test whether a Vec contains the given elements.
void
testVecVal2f(const V2f& v, float a, float b)
{
    assert(v[0] == a && v[1] == b);
}

void
testVecVal3f(const V3f& v, float a, float b, float c)
{
    assert(v[0] == a && v[1] == b && v[2] == c);
}

void
testVecVal4f(const V4f& v, float a, float b, float c, float d)
{
    assert(v[0] == a && v[1] == b && v[2] == c && v[3] == d);
}




void
testInteropVec2()
{
    std::cout << "has_xy<SimpleVec<float,2>, float>::value = "
              << has_xy<SimpleVec<float,2>, float>::value << "\n";
    std::cout << "has_xy<SimpleVec<int,2>, float>::value = "
              << has_xy<SimpleVec<int,2>, float>::value << "\n";
    std::cout << "has_xy<SimpleVec<float,4>, float>::value = "
              << has_xy<SimpleVec<float,4>, float>::value << "\n";
    std::cout << "has_xy<xyz<float>, float>::value = "
              << has_xy<xyz<float>, float>::value << "\n";
    std::cout << "has_xy<xyzw<float>, float>::value = "
              << has_xy<xyzw<float>, float>::value << "\n";
    std::cout << "has_xy<xy<float>, float>::value = "
              << has_xy<xy<float>, float>::value << "\n";
    std::cout << "has_xy<xyz<int>, float>::value = "
              << has_xy<xyz<int>, float>::value << "\n";
    std::cout << "has_xy<xyz_wrong<float>, float>::value = "
              << has_xy<xyz_wrong<float>, float>::value << "\n";
    std::cout << "\n";
    std::cout << "has_subscript<SimpleVec<float,2>, float, 2>::value = "
              << has_subscript<SimpleVec<float,2>, float, 2>::value << "\n";
    std::cout << "has_subscript<SimpleVec<int,2>, float, 2>::value = "
              << has_subscript<SimpleVec<int,2>, float, 2>::value << "\n";
    std::cout << "has_subscript<SimpleVec<float,4>, float, 2>::value = "
              << has_subscript<SimpleVec<float,4>, float, 2>::value << "\n";
    std::cout << "has_subscript<xyz<float>, float, 2>::value = "
              << has_subscript<xyz<float>, float, 2>::value << "\n";
    std::cout << "has_subscript<xyzw<float>, float, 2>::value = "
              << has_subscript<xyzw<float>, float, 2>::value << "\n";
    std::cout << "has_subscript<xy<float>, float, 2>::value = "
              << has_subscript<xy<float>, float, 2>::value << "\n";
    std::cout << "has_subscript<xyz<int>, float, 2>::value = "
              << has_subscript<xyz<int>, float, 2>::value << "\n";
    std::cout << "has_subscript<xyz_wrong<float>, float, 2>::value = "
              << has_subscript<xyz_wrong<float>, float, 2>::value << "\n";
    std::cout << "has_subscript<std::array<float, 2>, float, 2>::value = "
              << has_subscript<std::array<float, 2>, float, 2>::value << "\n";
    std::cout << "has_subscript<std::array<int, 2>, float, 2>::value = "
              << has_subscript<std::array<int, 2>, float, 2>::value << "\n";
    std::cout << "has_subscript<std::array<float, 4>, float, 2>::value = "
              << has_subscript<std::array<float, 4>, float, 2>::value << "\n";
    std::cout << "has_subscript<std::vector<float>, float, 2>::value = "
              << has_subscript<std::vector<float>, float, 2>::value << "\n";
    std::cout << "\n";
    std::cout << "has_xy<ComboVec3<float>, float, 2>::value = "
              << has_xy<ComboVec3<float>, float>::value << "\n";
    std::cout << "has_subscript<ComboVec3<float>, float, 2>::value = "
              << has_subscript<ComboVec3<float>, float, 2>::value << "\n";
    std::cout << "\n";

    assert((!has_xy<SimpleVec<float,2>, float>::value));
    assert((has_xy<xy<float>, float>::value));
    assert((!has_xy<xyz<float>, float>::value));

    assert((has_subscript<SimpleVec<float,2>, float, 2>::value));
    assert((!has_subscript<SimpleVec<float,4>, float, 2>::value));
    assert((!has_subscript<SimpleVec<float,2>, int, 2>::value));
    assert((!has_subscript<xyz<float>, float, 2>::value));
    assert((!has_subscript<xy<float>, float, 2>::value));

    // Test construction/assignment/paramater pass of a vector type with
    // subscripting to access components.
    {
        SimpleVec<float,2> s;
        s[0] = 1;
        s[1] = 2;
        Vec2<float> v(s);
        assert(v[0] == 1 && v[1] == 2);
        s[1] = 42;
        v = s;
        assert(v[0] == 1 && v[1] == 42);
        testVecVal2f(V2f(s), 1.0f, 42.0f);
    }
    // Test construction/assignment/paramater pass of a vector type with
    // explicit .y, .y, .z components but no subscripting.
    {
        xy<float> s { 1, 2 };
        Vec2<float> v(s);
        assert(v[0] == 1 && v[1] == 2);
        s.y = 42;
        v = s;
        assert(v[0] == 1 && v[1] == 42);
        testVecVal2f(V2f(s), 1.0f, 42.0f);
    }
    // Test construction/assignment/paramater pass of a std::vector of length 3
    {
        std::vector<float> s { 1, 2 };
        Vec2<float> v(s);
        assert(v[0] == 1 && v[1] == 2);
        s[1] = 42;
        v = s;
        assert(v[0] == 1 && v[1] == 42);
        testVecVal2f(V2f(s), 1.0f, 42.0f);
    }
    // Test construction/assignment/paramater pass of a std::array of length 3
    {
        std::array<float, 2> s { 1, 2 };
        Vec2<float> v(s);
        assert(v[0] == 1 && v[1] == 2);
        s[1] = 42;
        v = s;
        assert(v[0] == 1 && v[1] == 42);
        testVecVal2f(V2f(s), 1.0f, 42.0f);
    }
    // Test construction/assignment/paramater pass of initializer lists.
    {
        Vec2<float> v({ 1.0f, 2.0f });
        assert(v[0] == 1 && v[1] == 2);
        v = { 1.0f, 42.0f };
        assert(v[0] == 1 && v[1] == 42);
        testVecVal2f({ 1.0f, 42.0f}, 1.0f, 42.0f);
    }
    // Test construction/assignment/paramater pass of a C array
    {
        float s[2] = { 1, 2 };
        Vec2<float> v(s);
        assert(v[0] == 1 && v[1] == 2);
        s[1] = 42;
        v = s;
        assert(v[0] == 1 && v[1] == 42);
        testVecVal2f(V2f(s), 1.0f, 42.0f);
    }
}


//---------

void
testInteropVec3()
{
    std::cout << "has_xyz<SimpleVec<float,3>, float>::value = "
              << has_xyz<SimpleVec<float,3>, float>::value << "\n";
    std::cout << "has_xyz<SimpleVec<int,3>, float>::value = "
              << has_xyz<SimpleVec<int,3>, float>::value << "\n";
    std::cout << "has_xyz<SimpleVec<float,4>, float>::value = "
              << has_xyz<SimpleVec<float,4>, float>::value << "\n";
    std::cout << "has_xyz<xyz<float>, float>::value = "
              << has_xyz<xyz<float>, float>::value << "\n";
    std::cout << "has_xyz<xyzw<float>, float>::value = "
              << has_xyz<xyzw<float>, float>::value << "\n";
    std::cout << "has_xyz<xy<float>, float>::value = "
              << has_xyz<xy<float>, float>::value << "\n";
    std::cout << "has_xyz<xyz<int>, float>::value = "
              << has_xyz<xyz<int>, float>::value << "\n";
    std::cout << "has_xyz<xyz_wrong<float>, float>::value = "
              << has_xyz<xyz_wrong<float>, float>::value << "\n";
    std::cout << "\n";
    std::cout << "has_subscript<SimpleVec<float,3>, float, 3>::value = "
              << has_subscript<SimpleVec<float,3>, float, 3>::value << "\n";
    std::cout << "has_subscript<SimpleVec<int,3>, float, 3>::value = "
              << has_subscript<SimpleVec<int,3>, float, 3>::value << "\n";
    std::cout << "has_subscript<SimpleVec<float,4>, float, 3>::value = "
              << has_subscript<SimpleVec<float,4>, float, 3>::value << "\n";
    std::cout << "has_subscript<xyz<float>, float, 3>::value = "
              << has_subscript<xyz<float>, float, 3>::value << "\n";
    std::cout << "has_subscript<xyzw<float>, float, 3>::value = "
              << has_subscript<xyzw<float>, float, 3>::value << "\n";
    std::cout << "has_subscript<xy<float>, float, 3>::value = "
              << has_subscript<xy<float>, float, 3>::value << "\n";
    std::cout << "has_subscript<xyz<int>, float, 3>::value = "
              << has_subscript<xyz<int>, float, 3>::value << "\n";
    std::cout << "has_subscript<xyz_wrong<float>, float, 3>::value = "
              << has_subscript<xyz_wrong<float>, float, 3>::value << "\n";
    std::cout << "has_subscript<std::array<float, 3>, float, 3>::value = "
              << has_subscript<std::array<float, 3>, float, 3>::value << "\n";
    std::cout << "has_subscript<std::array<int, 3>, float, 3>::value = "
              << has_subscript<std::array<int, 3>, float, 3>::value << "\n";
    std::cout << "has_subscript<std::array<float, 4>, float, 3>::value = "
              << has_subscript<std::array<float, 4>, float, 3>::value << "\n";
    std::cout << "has_subscript<std::vector<float>, float, 3>::value = "
              << has_subscript<std::vector<float>, float, 3>::value << "\n";
    std::cout << "\n";
    std::cout << "has_xyz<ComboVec3<float>, float, 3>::value = "
              << has_xyz<ComboVec3<float>, float>::value << "\n";
    std::cout << "has_subscript<ComboVec3<float>, float, 3>::value = "
              << has_subscript<ComboVec3<float>, float, 3>::value << "\n";
    std::cout << "\n";

    assert((!has_xyz<SimpleVec<float,3>, float>::value));
    assert((has_xyz<xyz<float>, float>::value));
    assert((!has_xyz<xy<float>, float>::value));

    assert((has_subscript<SimpleVec<float,3>, float, 3>::value));
    assert((!has_subscript<SimpleVec<float,4>, float, 3>::value));
    assert((!has_subscript<SimpleVec<float,3>, int, 3>::value));
    assert((!has_subscript<xyz<float>, float, 3>::value));
    assert((!has_subscript<xy<float>, float, 3>::value));

    // Test construction/assignment/paramater pass of a vector type with
    // subscripting to access components.
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
        testVecVal3f(V3f(s), 1.0f, 42.0f, 3.0f);
    }
    // Test construction/assignment/paramater pass of a vector type with
    // explicit .y, .y, .z components but no subscripting.
    {
        xyz<float> s { 1, 2, 3 };
        Vec3<float> v(s);
        assert(v[0] == 1 && v[1] == 2 && v[2] == 3);
        s.y = 42;
        v = s;
        assert(v[0] == 1 && v[1] == 42 && v[2] == 3);
        testVecVal3f(V3f(s), 1.0f, 42.0f, 3.0f);
    }
    // Test construction/assignment/paramater pass of a std::vector of length 3
    {
        std::vector<float> s { 1, 2, 3 };
        Vec3<float> v(s);
        assert(v[0] == 1 && v[1] == 2 && v[2] == 3);
        s[1] = 42;
        v = s;
        assert(v[0] == 1 && v[1] == 42 && v[2] == 3);
        testVecVal3f(V3f(s), 1.0f, 42.0f, 3.0f);
    }
    // Test construction/assignment/paramater pass of a std::array of length 3
    {
        std::array<float, 3> s { 1, 2, 3 };
        Vec3<float> v(s);
        assert(v[0] == 1 && v[1] == 2 && v[2] == 3);
        s[1] = 42;
        v = s;
        assert(v[0] == 1 && v[1] == 42 && v[2] == 3);
        testVecVal3f(V3f(s), 1.0f, 42.0f, 3.0f);
    }
    // Test construction/assignment/paramater pass of initializer lists.
    {
        Vec3<float> v({ 1.0f, 2.0f, 3.0f });
        assert(v[0] == 1 && v[1] == 2 && v[2] == 3);
        v = { 1.0f, 42.0f, 3.0f };
        assert(v[0] == 1 && v[1] == 42 && v[2] == 3);
        testVecVal3f({ 1.0f, 42.0f, 3.0f }, 1, 42, 3);
    }
    // Test construction/assignment/paramater pass of a C array
    {
        float s[3] = { 1, 2, 3 };
        Vec3<float> v(s);
        assert(v[0] == 1 && v[1] == 2 && v[2] == 3);
        s[1] = 42;
        v = s;
        assert(v[0] == 1 && v[1] == 42 && v[2] == 3);
        testVecVal3f(V3f(s), 1.0f, 42.0f, 3.0f);
    }
}


//---------

void
testInteropVec4()
{
    std::cout << "has_xyzw<SimpleVec<float,4>, float>::value = "
              << has_xyzw<SimpleVec<float,4>, float>::value << "\n";
    std::cout << "has_xyzw<SimpleVec<int,4>, float>::value = "
              << has_xyzw<SimpleVec<int,4>, float>::value << "\n";
    std::cout << "has_xyzw<SimpleVec<float,4>, float>::value = "
              << has_xyzw<SimpleVec<float,4>, float>::value << "\n";
    std::cout << "has_xyzw<xyzw<float>, float>::value = "
              << has_xyzw<xyzw<float>, float>::value << "\n";
    std::cout << "has_xyzw<xyzw<float>, float>::value = "
              << has_xyzw<xyzw<float>, float>::value << "\n";
    std::cout << "has_xyzw<xy<float>, float>::value = "
              << has_xyzw<xy<float>, float>::value << "\n";
    std::cout << "has_xyzw<xyzw<int>, float>::value = "
              << has_xyzw<xyzw<int>, float>::value << "\n";
    std::cout << "has_xyzw<xyz_wrong<float>, float>::value = "
              << has_xyzw<xyz_wrong<float>, float>::value << "\n";
    std::cout << "\n";
    std::cout << "has_subscript<SimpleVec<float,4>, float, 4>::value = "
              << has_subscript<SimpleVec<float,4>, float, 4>::value << "\n";
    std::cout << "has_subscript<SimpleVec<int,4>, float, 4>::value = "
              << has_subscript<SimpleVec<int,4>, float, 4>::value << "\n";
    std::cout << "has_subscript<SimpleVec<float,4>, float, 4>::value = "
              << has_subscript<SimpleVec<float,4>, float, 4>::value << "\n";
    std::cout << "has_subscript<xyzw<float>, float, 4>::value = "
              << has_subscript<xyzw<float>, float, 4>::value << "\n";
    std::cout << "has_subscript<xyzw<float>, float, 4>::value = "
              << has_subscript<xyzw<float>, float, 4>::value << "\n";
    std::cout << "has_subscript<xy<float>, float, 4>::value = "
              << has_subscript<xy<float>, float, 4>::value << "\n";
    std::cout << "has_subscript<xyzw<int>, float, 4>::value = "
              << has_subscript<xyzw<int>, float, 4>::value << "\n";
    std::cout << "has_subscript<xyz_wrong<float>, float, 4>::value = "
              << has_subscript<xyz_wrong<float>, float, 4>::value << "\n";
    std::cout << "has_subscript<std::array<float, 3>, float, 4>::value = "
              << has_subscript<std::array<float, 3>, float, 4>::value << "\n";
    std::cout << "has_subscript<std::array<int, 3>, float, 4>::value = "
              << has_subscript<std::array<int, 3>, float, 4>::value << "\n";
    std::cout << "has_subscript<std::array<float, 4>, float, 4>::value = "
              << has_subscript<std::array<float, 4>, float, 4>::value << "\n";
    std::cout << "has_subscript<std::vector<float>, float, 4>::value = "
              << has_subscript<std::vector<float>, float, 4>::value << "\n";
    std::cout << "\n";
    std::cout << "has_xyzw<ComboVec4<float>, float, 4>::value = "
              << has_xyzw<ComboVec4<float>, float>::value << "\n";
    std::cout << "has_subscript<ComboVec4<float>, float, 4>::value = "
              << has_subscript<ComboVec4<float>, float, 4>::value << "\n";
    std::cout << "\n";

    assert((!has_xyzw<SimpleVec<float,4>, float>::value));
    assert((has_xyzw<xyzw<float>, float>::value));
    assert((!has_xyzw<xyz<float>, float>::value));

    assert((has_subscript<SimpleVec<float,4>, float, 4>::value));
    assert((!has_subscript<SimpleVec<float,4>, float, 3>::value));
    assert((!has_subscript<SimpleVec<float,4>, int, 4>::value));
    assert((!has_subscript<xyzw<float>, float, 4>::value));
    assert((!has_subscript<xy<float>, float, 4>::value));

    // Test construction/assignment/paramater pass of a vector type with
    // subscripting to access components.
    {
        SimpleVec<float,4> s;
        s[0] = 1;
        s[1] = 2;
        s[2] = 3;
        s[3] = 4;
        Vec4<float> v(s);
        assert(v[0] == 1 && v[1] == 2 && v[2] == 3 && v[3] == 4);
        s[1] = 42;
        v = s;
        assert(v[0] == 1 && v[1] == 42 && v[2] == 3 && v[3] == 4);
        testVecVal4f(V4f(s), 1.0f, 42.0f, 3.0f, 4.0f);
    }
    // Test construction/assignment/paramater pass of a vector type with
    // explicit .y, .y, .z components but no subscripting.
    {
        xyzw<float> s { 1, 2, 3, 4 };
        Vec4<float> v(s);
        assert(v[0] == 1 && v[1] == 2 && v[2] == 3 && v[3] == 4);
        s.y = 42;
        v = s;
        assert(v[0] == 1 && v[1] == 42 && v[2] == 3 && v[3] == 4);
        testVecVal4f(V4f(s), 1.0f, 42.0f, 3.0f, 4.0f);
    }
    // Test construction/assignment/paramater pass of a std::vector of length 3
    {
        std::vector<float> s { 1, 2, 3, 4 };
        Vec4<float> v(s);
        assert(v[0] == 1 && v[1] == 2 && v[2] == 3 && v[3] == 4);
        s[1] = 42;
        v = s;
        assert(v[0] == 1 && v[1] == 42 && v[2] == 3 && v[3] == 4);
        testVecVal4f(V4f(s), 1.0f, 42.0f, 3.0f, 4.0f);
    }
    // Test construction/assignment/paramater pass of a std::array of length 3
    {
        std::array<float, 4> s { 1, 2, 3, 4 };
        Vec4<float> v(s);
        assert(v[0] == 1 && v[1] == 2 && v[2] == 3 && v[3] == 4);
        s[1] = 42;
        v = s;
        assert(v[0] == 1 && v[1] == 42 && v[2] == 3 && v[3] == 4);
        testVecVal4f(V4f(s), 1.0f, 42.0f, 3.0f, 4.0f);
    }
    // Test construction/assignment/paramater pass of initializer lists.
    {
        Vec4<float> v({ 1.0f, 2.0f, 3.0f, 4.0f });
        assert(v[0] == 1 && v[1] == 2 && v[2] == 3 && v[3] == 4);
        v = { 1.0f, 42.0f, 3.0f, 4.0f };
        assert(v[0] == 1 && v[1] == 42 && v[2] == 3 && v[3] == 4);
        testVecVal4f({ 1.0f, 42.0f, 3.0f, 4.0f }, 1, 42, 3, 4);
    }
    // Test construction/assignment/paramater pass of a C array
    {
        float s[4] = { 1, 2, 3, 4 };
        Vec4<float> v(s);
        assert(v[0] == 1 && v[1] == 2 && v[2] == 3 && v[3] == 4);
        s[1] = 42;
        v = s;
        assert(v[0] == 1 && v[1] == 42 && v[2] == 3 && v[3] == 4);
        testVecVal4f(V4f(s), 1.0f, 42.0f, 3.0f, 4.0f);
    }
}


//---------

void
testInteropMx2()
{
    std::cout << "has_double_subscript<SimpleMx<float,2>, float, 2, 2>::value = "
              << has_double_subscript<SimpleMx<float,2>, float, 2, 2>::value << "\n";
    std::cout << "has_double_subscript<SimpleMx<int,2>, float, 2, 2>::value = "
              << has_double_subscript<SimpleMx<int,2>, float, 2, 2>::value << "\n";
    std::cout << "has_double_subscript<SimpleMx<float,2>, float, 4, 4>::value = "
              << has_double_subscript<SimpleMx<float,2>, float, 4, 4>::value << "\n";
    std::cout << "has_double_subscript<xyzw<float>, float, 2, 2>::value = "
              << has_double_subscript<xyzw<float>, float, 2, 2>::value << "\n";
    std::cout << "has_double_subscript<xyz_wrong<float>, float, 2, 2>::value = "
              << has_double_subscript<xyz_wrong<float>, float, 2, 2>::value << "\n";
    std::cout << "\n";

    assert((has_double_subscript<SimpleMx<float,2>, float, 2, 2>::value));
    assert((!has_double_subscript<SimpleMx<float,2>, float, 4, 4>::value));
    assert((!has_double_subscript<SimpleMx<float,2>, int, 2, 2>::value));
    assert((!has_double_subscript<xyzw<float>, float, 2, 2>::value));
    assert((!has_double_subscript<xy<float>, float, 2, 2>::value));

    // Test construction/assignment/paramater pass of a vector type with
    // subscripting to access components.
    {
        Matrix22<float> ref;
        SimpleMx<float,2> s;
        for (int j = 0; j < 2; ++j)
            for (int i = 0; i < 2; ++i) {
                s[j][i] = i + j * 2;
                ref[j][i] = i + j * 2;
            }
        Matrix22<float> v(s);
        assert(v[0][0] == 0 && v[0][1] == 1 &&
               v[1][0] == 2 && v[1][1] == 3);
        s[1][1] = 42;
        v = s;
        assert(v[0][0] == 0 && v[0][1] == 1 &&
               v[1][0] == 2 && v[1][1] == 42);
    }
}



void
testInteropMx3()
{
    std::cout << "has_double_subscript<SimpleMx<float,3>, float, 3, 3>::value = "
              << has_double_subscript<SimpleMx<float,3>, float, 3, 3>::value << "\n";
    std::cout << "has_double_subscript<SimpleMx<int,3>, float, 3, 3>::value = "
              << has_double_subscript<SimpleMx<int,3>, float, 3, 3>::value << "\n";
    std::cout << "has_double_subscript<SimpleMx<float,3>, float, 4, 4>::value = "
              << has_double_subscript<SimpleMx<float,3>, float, 4, 4>::value << "\n";
    std::cout << "has_double_subscript<xyzw<float>, float, 3, 3>::value = "
              << has_double_subscript<xyzw<float>, float, 3, 3>::value << "\n";
    std::cout << "has_double_subscript<xyz_wrong<float>, float, 3, 3>::value = "
              << has_double_subscript<xyz_wrong<float>, float, 3, 3>::value << "\n";
    std::cout << "\n";

    assert((has_double_subscript<SimpleMx<float,3>, float, 3, 3>::value));
    assert((!has_double_subscript<SimpleMx<float,3>, float, 4, 4>::value));
    assert((!has_double_subscript<SimpleMx<float,3>, int, 3, 3>::value));
    assert((!has_double_subscript<xyzw<float>, float, 3, 3>::value));
    assert((!has_double_subscript<xy<float>, float, 3, 3>::value));

    // Test construction/assignment/paramater pass of a vector type with
    // subscripting to access components.
    {
        Matrix33<float> ref;
        SimpleMx<float,3> s;
        for (int j = 0; j < 3; ++j)
            for (int i = 0; i < 3; ++i) {
                s[j][i] = i + j * 3;
                ref[j][i] = i + j * 3;
            }
        Matrix33<float> v(s);
        assert(v[0][0] == 0 && v[0][1] == 1 && v[0][2] == 2 &&
               v[1][0] == 3 && v[1][1] == 4 && v[1][2] == 5 &&
               v[2][0] == 6 && v[2][1] == 7 && v[2][2] == 8);
        s[1][1] = 42;
        v = s;
        assert(v[0][0] == 0 && v[0][1] == 1 && v[0][2] == 2 &&
               v[1][0] == 3 && v[1][1] == 42 && v[1][2] == 5 &&
               v[2][0] == 6 && v[2][1] == 7 && v[2][2] == 8);
    }
}



void
testInteropMx4()
{
    std::cout << "has_double_subscript<SimpleMx<float,4>, float, 4, 4>::value = "
              << has_double_subscript<SimpleMx<float,4>, float, 4, 4>::value << "\n";
    std::cout << "has_double_subscript<SimpleMx<int,4>, float, 4, 4>::value = "
              << has_double_subscript<SimpleMx<int,4>, float, 4, 4>::value << "\n";
    std::cout << "has_double_subscript<SimpleMx<float,4>, float, 4, 4>::value = "
              << has_double_subscript<SimpleMx<float,4>, float, 4, 4>::value << "\n";
    std::cout << "has_double_subscript<xyzw<float>, float, 4, 4>::value = "
              << has_double_subscript<xyzw<float>, float, 4, 4>::value << "\n";
    std::cout << "has_double_subscript<xyz_wrong<float>, float, 4, 4>::value = "
              << has_double_subscript<xyz_wrong<float>, float, 4, 4>::value << "\n";
    std::cout << "\n";

    assert((has_double_subscript<SimpleMx<float,4>, float, 4, 4>::value));
    assert((!has_double_subscript<SimpleMx<float,4>, float, 3, 3>::value));
    assert((!has_double_subscript<SimpleMx<float,4>, int, 4, 4>::value));
    assert((!has_double_subscript<xyzw<float>, float, 4, 4>::value));
    assert((!has_double_subscript<xy<float>, float, 4, 4>::value));

    // Test construction/assignment/paramater pass of a vector type with
    // subscripting to access components.
    {
        Matrix44<float> ref;
        SimpleMx<float,4> s;
        for (int j = 0; j < 4; ++j)
            for (int i = 0; i < 4; ++i) {
                s[j][i] = i + j * 4;
                ref[j][i] = i + j * 4;
            }
        Matrix44<float> v(s);
        assert(v[0][0] == 0 && v[0][1] == 1 && v[0][2] == 2 && v[0][3] == 3 &&
               v[1][0] == 4 && v[1][1] == 5 && v[1][2] == 6 && v[1][3] == 7 &&
               v[2][0] == 8 && v[2][1] == 9 && v[2][2] == 10 && v[2][3] == 11 &&
               v[3][0] == 12 && v[3][1] == 13 && v[3][2] == 14 && v[3][3] == 15);
        s[1][1] = 42;
        v = s;
        assert(v[0][0] == 0 && v[0][1] == 1 && v[0][2] == 2 && v[0][3] == 3 &&
               v[1][0] == 4 && v[1][1] == 42 && v[1][2] == 6 && v[1][3] == 7 &&
               v[2][0] == 8 && v[2][1] == 9 && v[2][2] == 10 && v[2][3] == 11 &&
               v[3][0] == 12 && v[3][1] == 13 && v[3][2] == 14 && v[3][3] == 15);
    }
}

} // namespace
#endif


void
testInterop()
{
    cout << "Testing interoperability with foreign types" << endl;

#if IMATH_FOREIGN_VECTOR_INTEROP
    testInteropVec2();
    testInteropVec3();
    testInteropVec4();

    testInteropMx2();
    testInteropMx3();
    testInteropMx4();

    cout << "ok\n" << endl;
#else
    cout << "Foreign vector type interopability is disabled.\n";
#endif
}

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


// Imath::has_subscript fails for std::vector because its length does not
// appear to be the length of N elements. Carve out an exception here that
// allows this to work.
IMATH_INTERNAL_NAMESPACE_SOURCE_ENTER
template<typename T, int N>
struct has_subscript<std::vector<T>, T, N> : public std::true_type { };
IMATH_INTERNAL_NAMESPACE_SOURCE_EXIT



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
        testVecVal3f(s, 1.0f, 42.0f, 3.0f);
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
        testVecVal3f(s, 1.0f, 42.0f, 3.0f);
    }
    // Test construction/assignment/paramater pass of a std::vector of length 3
    {
        std::vector<float> s { 1, 2, 3 };
        Vec3<float> v(s);
        assert(v[0] == 1 && v[1] == 2 && v[2] == 3);
        s[1] = 42;
        v = s;
        assert(v[0] == 1 && v[1] == 42 && v[2] == 3);
        testVecVal3f(s, 1.0f, 42.0f, 3.0f);
    }
    // Test construction/assignment/paramater pass of a std::array of length 3
    {
        std::array<float, 3> s { 1, 2, 3 };
        Vec3<float> v(s);
        assert(v[0] == 1 && v[1] == 2 && v[2] == 3);
        s[1] = 42;
        v = s;
        assert(v[0] == 1 && v[1] == 42 && v[2] == 3);
        testVecVal3f(s, 1.0f, 42.0f, 3.0f);
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
        testVecVal3f(s, 1.0f, 42.0f, 3.0f);
    }
}


} // namespace

void
testInterop()
{
    cout << "Testing interoperability with foreign types" << endl;

    testInteropVec3();

    cout << "ok\n" << endl;
}

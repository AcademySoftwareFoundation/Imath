//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifdef NDEBUG
#    undef NDEBUG
#endif

#include <ImathFun.h>
#include <ImathVec.h>
#include <cassert>
#include <cmath>
#include <iostream>
#include "testVec.h"

// Include ImathForward *after* other headers to validate forward declarations
#include <ImathForward.h>

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

    cout << "ok\n" << endl;
}

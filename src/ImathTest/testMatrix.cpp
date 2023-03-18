//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifdef NDEBUG
#    undef NDEBUG
#endif

#include <ImathInt64.h>
#include <ImathMath.h>
#include <ImathMatrix.h>
#include <ImathMatrixAlgo.h>
#include <ImathRandom.h>
#include <ImathVec.h>
#include <assert.h>
#include <iostream>
#include "testMatrix.h"
#include <sstream>

// Include ImathForward *after* other headers to validate forward declarations
#include <ImathForward.h>

using namespace std;
using IMATH_INTERNAL_NAMESPACE::Int64;

//
// This file is not currently intended to exhaustively test
// the Imath Matrix33<T> and Matrix44<T> classes.  We leave
// that to PyImathTest.
//
// Instead, in this file we test only those aspects of the
// Imath Matrix33<T> and Matrix44<T> classes that must be
// or are more convenient to test from C++.
//

void
testMatrix22ArrayConstructor(const float a[2][2])
{
    IMATH_INTERNAL_NAMESPACE::M22f m(a);
    assert(m == IMATH_INTERNAL_NAMESPACE::M22f());
}

void
testMatrix33ArrayConstructor(const float a[3][3])
{
    IMATH_INTERNAL_NAMESPACE::M33f m(a);
    assert(m == IMATH_INTERNAL_NAMESPACE::M33f());
}

void
testMatrix44ArrayConstructor(const float a[4][4])
{
    IMATH_INTERNAL_NAMESPACE::M44f m(a);
    assert(m == IMATH_INTERNAL_NAMESPACE::M44f());
}


void
testMatrix ()
{
    cout << "Testing functions in ImathMatrix.h" << endl;

    union
    {
        float f;
        int i;
    } nanf;
    nanf.i = 0x7f800001; //  NAN

    union
    {
        double d;
        uint64_t i;
    } nand;
    nand.i = 0x7ff0000000000001ULL; //  NAN

    {
        cout << "Imath::M22f constructors and equality operators" << endl;

        IMATH_INTERNAL_NAMESPACE::M22f m1;
        m1[0][0] = 99.0f;
        m1[1][1] = 101.0f;

        const IMATH_INTERNAL_NAMESPACE::M22f test (m1);
        assert (test == m1);

        IMATH_INTERNAL_NAMESPACE::M22f test2;
        assert (test != test2);

        IMATH_INTERNAL_NAMESPACE::M22f test3;
        test3.makeIdentity();
        assert (test2 == test3);

        const float a[2][2] = {
            { 1.0f, 0.0f },
            { 0.0f, 1.0f }
        };
        testMatrix22ArrayConstructor(a);
        
        m1 = 42;
        assert (m1[0][0] == 42 && m1[0][1] == 42 && m1[1][0] == 42 && m1[1][1] == 42);

        const float* i1 = test.getValue();
        assert(i1[0] == 99.0f);
        assert(i1[3] == 101.0f);
        
        float* i2 = m1.getValue();
        assert(i2[0] == 42.0f);
        assert(i2[1] == 42.0f);
        assert(i2[2] == 42.0f);
        assert(i2[3] == 42.0f);

        IMATH_INTERNAL_NAMESPACE::M22f test4;
        test.getValue(test4);
        assert (test == test4);

        test4.setTheMatrix(test3);
        assert(test4 == test3);
    }

    {
        cout << "M22d constructors and equality operators" << endl;

        IMATH_INTERNAL_NAMESPACE::M22d m2;
        m2[0][0] = 99.0f;
        m2[1][1] = 101.0f;

        IMATH_INTERNAL_NAMESPACE::M22d test (m2);
        assert (test == m2);

        IMATH_INTERNAL_NAMESPACE::M22d test2;
        assert (test != test2);

        IMATH_INTERNAL_NAMESPACE::M22d test3;
        test3.makeIdentity();
        assert (test2 == test3);

        IMATH_INTERNAL_NAMESPACE::M22f test4 (1.0f, 2.0f, 3.0f, 4.0f);

        IMATH_INTERNAL_NAMESPACE::M22d test5 = IMATH_INTERNAL_NAMESPACE::M22d (test4);

        assert (test5[0][0] == 1.0);
        assert (test5[0][1] == 2.0);

        assert (test5[1][0] == 3.0);
        assert (test5[1][1] == 4.0);

        const float a[3][3] = {
            { 1.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f }
        };
        testMatrix33ArrayConstructor(a);
    }

    {
        cout << "M22f inversion operators" << endl;

        IMATH_INTERNAL_NAMESPACE::M22f m1 (3.0f, 3.0f, 5.0f, 5.0f);
        IMATH_INTERNAL_NAMESPACE::M22f m2 = m1;
        assert(m1.inverse(false) == m1.inverse());
        m2.invert(false);
        m1.invert();
        assert(m1 == m2);

        IMATH_INTERNAL_NAMESPACE::M22f m3 (4.0f, 7.0f, 2.0f, 6.0f);
        m2 = m3;
        assert(m2.inverse(true) == m2.inverse());
        m3.invert(true);
        m2.invert();
        assert(m3 == m2);
    }

    {
        cout << "Imath::M33f shear functions" << endl;

        IMATH_INTERNAL_NAMESPACE::M33f m1, m2;
        m1.setShear (2.0f);
        assert (m1[0][0] == 1.0f && m1[0][1] == 0.0f && m1[0][2] == 0.0f && m1[1][0] == 2.0f &&
                m1[1][1] == 1.0f && m1[1][2] == 0.0f && m1[2][0] == 0.0f && m1[2][1] == 0.0f &&
                m1[2][2] == 1.0f);

        m2.setShear (IMATH_INTERNAL_NAMESPACE::V2f (3.0f, 4.0f));
        assert (m2[0][0] == 1.0f && m2[0][1] == 4.0f && m2[0][2] == 0.0f && m2[1][0] == 3.0f &&
                m2[1][1] == 1.0f && m2[1][2] == 0.0f && m2[2][0] == 0.0f && m2[2][1] == 0.0f &&
                m2[2][2] == 1.0f);

        m1.shear (IMATH_INTERNAL_NAMESPACE::V2f (5.0f, 6.0f));
        assert (m1[0][0] == 13.0f && m1[0][1] == 6.0f && m1[0][2] == 0.0f && m1[1][0] == 7.0f &&
                m1[1][1] == 1.0f && m1[1][2] == 0.0f && m1[2][0] == 0.0f && m1[2][1] == 0.0f &&
                m1[2][2] == 1.0f);

        m2.shear (7.0f);
        assert (m2[0][0] == 1.0f && m2[0][1] == 4.0f && m2[0][2] == 0.0f && m2[1][0] == 10.0f &&
                m2[1][1] == 29.0f && m2[1][2] == 0.0f && m2[2][0] == 0.0f && m2[2][1] == 0.0f &&
                m2[2][2] == 1.0f);

        cout << "M33f constructors and equality operators" << endl;

        const IMATH_INTERNAL_NAMESPACE::M33f test (m2);
        assert (test == m2);

        IMATH_INTERNAL_NAMESPACE::M33f test2;
        assert (test != test2);

        IMATH_INTERNAL_NAMESPACE::M33f test3;
        test3.makeIdentity();
        assert (test2 == test3);

        m1 = 42;
        assert (m1[0][0] == 42 && m1[0][1] == 42 && m1[0][2] == 42 &&
                m1[1][0] == 42 && m1[1][1] == 42 && m1[1][2] == 42 &&
                m1[2][0] == 42 && m1[2][1] == 42 && m1[2][2] == 42);

        const float* i1 = test.getValue();
        assert (i1[0] == 1.0f  && i1[1] == 4.0f  && i1[2] == 0.0f &&
                i1[3] == 10.0f && i1[4] == 29.0f && i1[5] == 0.0f &&
                i1[6] == 0.0f  && i1[7] == 0.0f  && i1[8] == 1.0f);
        
        float* i2 = m1.getValue();
        assert(i2[0] == 42.0f);
        assert(i2[1] == 42.0f);
        assert(i2[2] == 42.0f);
        assert(i2[3] == 42.0f);

        IMATH_INTERNAL_NAMESPACE::M33f test4;
        test.getValue(test4);
        assert (test == test4);

        test4.setTheMatrix(test3);
        assert(test4 == test3);

        IMATH_INTERNAL_NAMESPACE::V3f v(2.0f);
        IMATH_INTERNAL_NAMESPACE::M33f m(2.0f);
        v *= m;
        assert (IMATH_INTERNAL_NAMESPACE::equal(v[0], 12.0f, 0.0001f));
        assert (IMATH_INTERNAL_NAMESPACE::equal(v[1], 12.0f, 0.0001f));
        assert (IMATH_INTERNAL_NAMESPACE::equal(v[2], 12.0f, 0.0001f));
    }

    {
      cout << "M33f inversion operators" << endl;

      IMATH_INTERNAL_NAMESPACE::M33f m1 (0.0f, 2.0f, -1.0f, 3.0f, -2.0f, 1.0f, 3.0f, 2.0f, -1.0f);
      IMATH_INTERNAL_NAMESPACE::M33f m2 = m1;
      assert(m1.inverse(false) == m1.inverse());
      m2.invert(false);
      m1.invert();
      assert(m1 == m2);

      IMATH_INTERNAL_NAMESPACE::M33f m3 (1.0f, 0.0f, 5.0f, 2.0f, 1.0f, 6.0f, 3.0f, 4.0f, 0.0f);
      m2 = m3;
      assert(m3.inverse(true) == m3.inverse());
      m3.invert(true);
      m2.invert();
      assert(m3 == m2);

      IMATH_INTERNAL_NAMESPACE::M33f m4 (0.0f, 2.0f, -1.0f, 3.0f, -2.0f, 1.0f, 3.0f, 2.0f, -1.0f);
      m2 = m4;
      assert(m4.gjInverse(false) == m4.gjInverse());
      m2.gjInvert(false);
      m4.gjInvert();
      assert(m4 == m2);

      IMATH_INTERNAL_NAMESPACE::M33f m5 (1.0f, 0.0f, 5.0f, 2.0f, 1.0f, 6.0f, 3.0f, 4.0f, 0.0f);
      m2 = m5;
      assert(m5.gjInverse(true) == m5.gjInverse());
      m5.gjInvert(true);
      m2.gjInvert();
      assert(m5 == m2);
    }

    {
        cout << "M33d constructors and equality operators" << endl;

        IMATH_INTERNAL_NAMESPACE::M33d m2;
        m2[0][0] = 99.0f;
        m2[1][2] = 101.0f;

        IMATH_INTERNAL_NAMESPACE::M33d test (m2);
        assert (test == m2);

        IMATH_INTERNAL_NAMESPACE::M33d test2;
        assert (test != test2);

        IMATH_INTERNAL_NAMESPACE::M33d test3;
        test3.makeIdentity();
        assert (test2 == test3);

        IMATH_INTERNAL_NAMESPACE::M33f test4 (1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f);

        IMATH_INTERNAL_NAMESPACE::M33d test5 = IMATH_INTERNAL_NAMESPACE::M33d (test4);

        assert (test5[0][0] == 1.0);
        assert (test5[0][1] == 2.0);
        assert (test5[0][2] == 3.0);

        assert (test5[1][0] == 4.0);
        assert (test5[1][1] == 5.0);
        assert (test5[1][2] == 6.0);

        assert (test5[2][0] == 7.0);
        assert (test5[2][1] == 8.0);
        assert (test5[2][2] == 9.0);
    }

    {
        IMATH_INTERNAL_NAMESPACE::M44f m2;
        m2[0][0] = 99.0f;
        m2[1][2] = 101.0f;

        cout << "M44f constructors and equality operators" << endl;

        IMATH_INTERNAL_NAMESPACE::M44f test (m2);
        assert (test == m2);

        IMATH_INTERNAL_NAMESPACE::M44f test2;
        assert (test != test2);

        IMATH_INTERNAL_NAMESPACE::M44f test3;
        test3.makeIdentity();
        assert (test2 == test3);

        //
        // Test non-equality when a NAN is in the same
        // place in two identical matrices
        //

        test2[0][0] = nanf.f;
        test3       = test2;
        assert (test2 != test3);
    }

    {
        IMATH_INTERNAL_NAMESPACE::M44d m2;
        m2[0][0] = 99.0f;
        m2[1][2] = 101.0f;

        cout << "M44d constructors and equality operators" << endl;

        IMATH_INTERNAL_NAMESPACE::M44d test (m2);
        assert (test == m2);

        IMATH_INTERNAL_NAMESPACE::M44d test2;
        assert (test != test2);

        IMATH_INTERNAL_NAMESPACE::M44d test3;
        test3.makeIdentity();
        assert (test2 == test3);

        const float a[4][4] = {
            { 1.0f, 0.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f }
        };
        testMatrix44ArrayConstructor(a);

        //
        // Test non-equality when a NAN is in the same
        // place in two identical matrices
        //

        test2[0][0] = nand.d;
        test3       = test2;
        assert (test2 != test3);

        IMATH_INTERNAL_NAMESPACE::M44f test4 (1.0f,
                                              2.0f,
                                              3.0f,
                                              4.0f,
                                              5.0f,
                                              6.0f,
                                              7.0f,
                                              8.0f,
                                              9.0f,
                                              10.0f,
                                              11.0f,
                                              12.0f,
                                              13.0f,
                                              14.0f,
                                              15.0f,
                                              16.0f);

        IMATH_INTERNAL_NAMESPACE::M44d test5 = IMATH_INTERNAL_NAMESPACE::M44d (test4);

        assert (test5[0][0] == 1.0);
        assert (test5[0][1] == 2.0);
        assert (test5[0][2] == 3.0);
        assert (test5[0][3] == 4.0);

        assert (test5[1][0] == 5.0);
        assert (test5[1][1] == 6.0);
        assert (test5[1][2] == 7.0);
        assert (test5[1][3] == 8.0);

        assert (test5[2][0] == 9.0);
        assert (test5[2][1] == 10.0);
        assert (test5[2][2] == 11.0);
        assert (test5[2][3] == 12.0);

        assert (test5[3][0] == 13.0);
        assert (test5[3][1] == 14.0);
        assert (test5[3][2] == 15.0);
        assert (test5[3][3] == 16.0);
    }

    {
        cout << "M44f *= operators" << endl;

        IMATH_INTERNAL_NAMESPACE::V3f v(2.0f);
        IMATH_INTERNAL_NAMESPACE::M44f m(2.0f);
        m.setScale(2.0f);
        v *= m;
        assert (IMATH_INTERNAL_NAMESPACE::equal(v[0], 4.0f, 0.0001f));
        assert (IMATH_INTERNAL_NAMESPACE::equal(v[1], 4.0f, 0.0001f));
        assert (IMATH_INTERNAL_NAMESPACE::equal(v[2], 4.0f, 0.0001f));

        IMATH_INTERNAL_NAMESPACE::V4f v4f(2.0f);
        IMATH_INTERNAL_NAMESPACE::V4f v4f2 = v4f * m;
        
        assert (IMATH_INTERNAL_NAMESPACE::equal(v4f2[0], 4.0f, 0.0001f));
        assert (IMATH_INTERNAL_NAMESPACE::equal(v4f2[1], 4.0f, 0.0001f));
        assert (IMATH_INTERNAL_NAMESPACE::equal(v4f2[2], 4.0f, 0.0001f));
        assert (IMATH_INTERNAL_NAMESPACE::equal(v4f2[3], 2.0f, 0.0001f));

        v4f *= m;
        assert (v4f == v4f2);
        
        IMATH_INTERNAL_NAMESPACE::M44f a(2.0f);
        IMATH_INTERNAL_NAMESPACE::M44f b(3.0f);
        IMATH_INTERNAL_NAMESPACE::M44f c;

        IMATH_INTERNAL_NAMESPACE::M44f::multiply(a, b, c);
        assert (IMATH_INTERNAL_NAMESPACE::equal(c[0][0], 24.0f, 0.0001f));
        assert (IMATH_INTERNAL_NAMESPACE::equal(c[1][1], 24.0f, 0.0001f));
        assert (IMATH_INTERNAL_NAMESPACE::equal(c[2][2], 24.0f, 0.0001f));
        assert (IMATH_INTERNAL_NAMESPACE::equal(c[3][3], 24.0f, 0.0001f));
    }
    
    cout << "Matrix << operators" << endl;
    
    {
        std::stringstream s;
        s << IMATH_INTERNAL_NAMESPACE::identity22f;
        const char v[] = "(  1.000000e+00   0.000000e+00\n   0.000000e+00   1.000000e+00)\n";
        assert (s.str() == v);
    }
        
    {
        std::stringstream s;
        s << IMATH_INTERNAL_NAMESPACE::identity33f;
        const char v[] = "(  1.000000e+00   0.000000e+00   0.000000e+00\n   0.000000e+00   1.000000e+00   0.000000e+00\n   0.000000e+00   0.000000e+00   1.000000e+00)\n";
        assert (s.str() == v);
    }

    {
        std::stringstream s;
        s << IMATH_INTERNAL_NAMESPACE::identity44f;
        const char v[] = "(  1.000000e+00   0.000000e+00   0.000000e+00   0.000000e+00\n   0.000000e+00   1.000000e+00   0.000000e+00   0.000000e+00\n   0.000000e+00   0.000000e+00   1.000000e+00   0.000000e+00\n   0.000000e+00   0.000000e+00   0.000000e+00   1.000000e+00)\n";
        assert (s.str() == v);
    }

    {
        cout << "M44f inversion operators" << endl;

      IMATH_INTERNAL_NAMESPACE::M44f m1 (1.0f,
                                         0.0f,
                                         0.0f,
                                         0.0f,
                                         0.0f,
                                         1.0f,
                                         0.0f,
                                         0.0f,
                                         0.0f,
                                         0.0f,
                                         1.0f,
                                         0.0f,
                                         0.0f,
                                         0.0f,
                                         0.0f,
                                         0.0f);
      IMATH_INTERNAL_NAMESPACE::M44f m2 = m1;
      assert(m1.inverse(false) == m1.inverse());
      m2.invert(false);
      m1.invert();
      assert(m1 == m2);

      IMATH_INTERNAL_NAMESPACE::M44f m3 (5.0f,
                                         6.0f,
                                         6.0f,
                                         8.0f,
                                         2.0f,
                                         2.0f,
                                         2.0f,
                                         8.0f,
                                         6.0f,
                                         6.0f,
                                         2.0f,
                                         8.0f,
                                         2.0f,
                                         3.0f,
                                         6.0f,
                                         7.0f);
      m2 = m3;
      assert(m3.inverse(true) == m3.inverse());
      m3.invert(true);
      m2.invert();
      assert(m3 == m2);

      IMATH_INTERNAL_NAMESPACE::M44f m4 (1.0f,
                                         0.0f,
                                         0.0f,
                                         0.0f,
                                         0.0f,
                                         1.0f,
                                         0.0f,
                                         0.0f,
                                         0.0f,
                                         0.0f,
                                         1.0f,
                                         0.0f,
                                         0.0f,
                                         0.0f,
                                         0.0f,
                                         0.0f);
      m2 = m4;
      assert(m4.gjInverse(false) == m4.gjInverse());
      m2.gjInvert(false);
      m4.gjInvert();
      assert(m4 == m2);

      IMATH_INTERNAL_NAMESPACE::M44f m5 (5.0f,
                                         6.0f,
                                         6.0f,
                                         8.0f,
                                         2.0f,
                                         2.0f,
                                         2.0f,
                                         8.0f,
                                         6.0f,
                                         6.0f,
                                         2.0f,
                                         8.0f,
                                         2.0f,
                                         3.0f,
                                         6.0f,
                                         7.0f);
      m2 = m5;
      assert(m5.gjInverse(true) == m5.gjInverse());
      m5.gjInvert(true);
      m2.gjInvert();
      assert(m5 == m2);
    }

    {
        cout << "Converting between M33 and M44" << endl;

        IMATH_INTERNAL_NAMESPACE::M44d m1;
        m1[0][0] = 99;
        IMATH_INTERNAL_NAMESPACE::M44f m2;
        m2.setValue (m1);
        assert (m2[0][0] == (float) m1[0][0]);
        m1[0][0] = 101;
        m1.setValue (m2);
        assert (m2[0][0] == (float) m1[0][0]);
    }

    // Matrix minors
    {
        cout << "3x3 Matrix minors" << endl;

        IMATH_INTERNAL_NAMESPACE::M33f a (1, 2, 3, 4, 5, 6, 7, 8, 9);

        assert (a.minorOf (0, 0) == a.fastMinor (1, 2, 1, 2));
        assert (a.minorOf (0, 1) == a.fastMinor (1, 2, 0, 2));
        assert (a.minorOf (0, 2) == a.fastMinor (1, 2, 0, 1));
        assert (a.minorOf (1, 0) == a.fastMinor (0, 2, 1, 2));
        assert (a.minorOf (1, 1) == a.fastMinor (0, 2, 0, 2));
        assert (a.minorOf (1, 2) == a.fastMinor (0, 2, 0, 1));
        assert (a.minorOf (2, 0) == a.fastMinor (0, 1, 1, 2));
        assert (a.minorOf (2, 1) == a.fastMinor (0, 1, 0, 2));
        assert (a.minorOf (2, 2) == a.fastMinor (0, 1, 0, 1));
    }
    {
        IMATH_INTERNAL_NAMESPACE::M33d a (1, 2, 3, 4, 5, 6, 7, 8, 9);

        assert (a.minorOf (0, 0) == a.fastMinor (1, 2, 1, 2));
        assert (a.minorOf (0, 1) == a.fastMinor (1, 2, 0, 2));
        assert (a.minorOf (0, 2) == a.fastMinor (1, 2, 0, 1));
        assert (a.minorOf (1, 0) == a.fastMinor (0, 2, 1, 2));
        assert (a.minorOf (1, 1) == a.fastMinor (0, 2, 0, 2));
        assert (a.minorOf (1, 2) == a.fastMinor (0, 2, 0, 1));
        assert (a.minorOf (2, 0) == a.fastMinor (0, 1, 1, 2));
        assert (a.minorOf (2, 1) == a.fastMinor (0, 1, 0, 2));
        assert (a.minorOf (2, 2) == a.fastMinor (0, 1, 0, 1));
    }

    // Determinants (by building a random singular value decomposition)

    {
        cout << "2x2 determinant" << endl;

        IMATH_INTERNAL_NAMESPACE::Rand32 random;

        IMATH_INTERNAL_NAMESPACE::M22f u;
        IMATH_INTERNAL_NAMESPACE::M22f v;
        IMATH_INTERNAL_NAMESPACE::M22f s;

        u.setRotation (random.nextf());
        v.setRotation (random.nextf());
        s[0][0] = random.nextf();
        s[1][1] = random.nextf();

        IMATH_INTERNAL_NAMESPACE::M22f c = u * s * v.transpose();
        assert (fabsf (c.determinant() - s[0][0] * s[1][1]) <= u.baseTypeEpsilon());
    }
    {
        IMATH_INTERNAL_NAMESPACE::Rand32 random;

        IMATH_INTERNAL_NAMESPACE::M22d u;
        IMATH_INTERNAL_NAMESPACE::M22d v;
        IMATH_INTERNAL_NAMESPACE::M22d s;

        u.setRotation ((double) random.nextf());
        v.setRotation ((double) random.nextf());
        s[0][0] = (double) random.nextf();
        s[1][1] = (double) random.nextf();

        IMATH_INTERNAL_NAMESPACE::M22d c = u * s * v.transpose();
        assert (fabs (c.determinant() - s[0][0] * s[1][1]) <= u.baseTypeEpsilon());
    }

    {
        cout << "3x3 determinant" << endl;

        IMATH_INTERNAL_NAMESPACE::Rand32 random;

        IMATH_INTERNAL_NAMESPACE::M33f u;
        IMATH_INTERNAL_NAMESPACE::M33f v;
        IMATH_INTERNAL_NAMESPACE::M33f s;

        u.setRotation (random.nextf());
        v.setRotation (random.nextf());
        s[0][0] = random.nextf();
        s[1][1] = random.nextf();
        s[2][2] = random.nextf();

        IMATH_INTERNAL_NAMESPACE::M33f c = u * s * v.transpose();
        assert (fabsf (c.determinant() - s[0][0] * s[1][1] * s[2][2]) <= u.baseTypeEpsilon());
    }
    {
        IMATH_INTERNAL_NAMESPACE::Rand32 random;

        IMATH_INTERNAL_NAMESPACE::M33d u;
        IMATH_INTERNAL_NAMESPACE::M33d v;
        IMATH_INTERNAL_NAMESPACE::M33d s;

        u.setRotation ((double) random.nextf());
        v.setRotation ((double) random.nextf());
        s[0][0] = (double) random.nextf();
        s[1][1] = (double) random.nextf();
        s[2][2] = (double) random.nextf();

        IMATH_INTERNAL_NAMESPACE::M33d c = u * s * v.transpose();
        assert (fabs (c.determinant() - s[0][0] * s[1][1] * s[2][2]) <= u.baseTypeEpsilon());
    }

    // Outer product of two 3D vectors
    {
        cout << "Outer product of two 3D vectors" << endl;

        IMATH_INTERNAL_NAMESPACE::V3f a (1, 2, 3);
        IMATH_INTERNAL_NAMESPACE::V3f b (4, 5, 6);
        IMATH_INTERNAL_NAMESPACE::M33f p = IMATH_INTERNAL_NAMESPACE::outerProduct (a, b);

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                assert (p[i][j] == a[i] * b[j]);
            }
        }
    }
    {
        IMATH_INTERNAL_NAMESPACE::V3d a (1, 2, 3);
        IMATH_INTERNAL_NAMESPACE::V3d b (4, 5, 6);
        IMATH_INTERNAL_NAMESPACE::M33d p = IMATH_INTERNAL_NAMESPACE::outerProduct (a, b);

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                assert (p[i][j] == a[i] * b[j]);
            }
        }
    }

    // Determinants (by building a random singular value decomposition)
    {
        cout << "4x4 determinants" << endl;

        IMATH_INTERNAL_NAMESPACE::Rand32 random;

        IMATH_INTERNAL_NAMESPACE::M44f u = IMATH_INTERNAL_NAMESPACE::rotationMatrix (
            IMATH_INTERNAL_NAMESPACE::V3f (random.nextf(), random.nextf(), random.nextf())
                .normalize(),
            IMATH_INTERNAL_NAMESPACE::V3f (random.nextf(), random.nextf(), random.nextf())
                .normalize());
        IMATH_INTERNAL_NAMESPACE::M44f v = IMATH_INTERNAL_NAMESPACE::rotationMatrix (
            IMATH_INTERNAL_NAMESPACE::V3f (random.nextf(), random.nextf(), random.nextf())
                .normalize(),
            IMATH_INTERNAL_NAMESPACE::V3f (random.nextf(), random.nextf(), random.nextf())
                .normalize());
        IMATH_INTERNAL_NAMESPACE::M44f s;

        s[0][0] = random.nextf();
        s[1][1] = random.nextf();
        s[2][2] = random.nextf();
        s[3][3] = random.nextf();

        IMATH_INTERNAL_NAMESPACE::M44f c = u * s * v.transpose();
        assert (fabsf (c.determinant() - s[0][0] * s[1][1] * s[2][2] * s[3][3]) <=
                u.baseTypeEpsilon());
    }
    {
        IMATH_INTERNAL_NAMESPACE::Rand32 random;

        IMATH_INTERNAL_NAMESPACE::M44d u = IMATH_INTERNAL_NAMESPACE::rotationMatrix (
            IMATH_INTERNAL_NAMESPACE::V3d (random.nextf(), random.nextf(), random.nextf())
                .normalize(),
            IMATH_INTERNAL_NAMESPACE::V3d (random.nextf(), random.nextf(), random.nextf())
                .normalize());
        IMATH_INTERNAL_NAMESPACE::M44d v = IMATH_INTERNAL_NAMESPACE::rotationMatrix (
            IMATH_INTERNAL_NAMESPACE::V3d (random.nextf(), random.nextf(), random.nextf())
                .normalize(),
            IMATH_INTERNAL_NAMESPACE::V3d (random.nextf(), random.nextf(), random.nextf())
                .normalize());
        IMATH_INTERNAL_NAMESPACE::M44d s;

        s[0][0] = random.nextf();
        s[1][1] = random.nextf();
        s[2][2] = random.nextf();
        s[3][3] = random.nextf();

        IMATH_INTERNAL_NAMESPACE::M44d c = u * s * v.transpose();
        assert (fabs (c.determinant() - s[0][0] * s[1][1] * s[2][2] * s[3][3]) <=
                u.baseTypeEpsilon());
    }

    // Trace
    {
        cout << "2x2 trace" << endl;

        IMATH_INTERNAL_NAMESPACE::Rand32 random;

        IMATH_INTERNAL_NAMESPACE::M22f u;
        float                          trace = 0;
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                const float randomNum = random.nextf ();
                u[i][j]               = randomNum;
                if (i == j) { trace += randomNum; }
            }
        }

        assert (fabsf (u.trace () - trace) <= u.baseTypeEpsilon ());
    }
    {
        IMATH_INTERNAL_NAMESPACE::Rand32 random;

        IMATH_INTERNAL_NAMESPACE::M22d u;
        double                         trace = 0;
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                const double randomNum = random.nextf ();
                u[i][j]                = randomNum;
                if (i == j) { trace += randomNum; }
            }
        }

        assert (fabs (u.trace () - trace) <= u.baseTypeEpsilon ());
    }
    {
        cout << "3x3 trace" << endl;

        IMATH_INTERNAL_NAMESPACE::Rand32 random;

        IMATH_INTERNAL_NAMESPACE::M33f u;
        float                          trace = 0;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                const float randomNum = random.nextf ();
                u[i][j]               = randomNum;
                if (i == j) { trace += randomNum; }
            }
        }

        assert (fabsf (u.trace () - trace) <= u.baseTypeEpsilon ());
    }
    {
        IMATH_INTERNAL_NAMESPACE::Rand32 random;

        IMATH_INTERNAL_NAMESPACE::M33d u;
        double                         trace = 0;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                const double randomNum = random.nextf ();
                u[i][j]                = randomNum;
                if (i == j) { trace += randomNum; }
            }
        }

        assert (fabs (u.trace () - trace) <= u.baseTypeEpsilon ());
    }
    {
        cout << "4x4 trace" << endl;

        IMATH_INTERNAL_NAMESPACE::Rand32 random;

        IMATH_INTERNAL_NAMESPACE::M44f u;
        float                          trace = 0;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                const float randomNum = random.nextf ();
                u[i][j]               = randomNum;
                if (i == j) { trace += randomNum; }
            }
        }

        assert (fabsf (u.trace () - trace) <= u.baseTypeEpsilon ());
    }
    {
        IMATH_INTERNAL_NAMESPACE::Rand32 random;

        IMATH_INTERNAL_NAMESPACE::M44d u;
        double                         trace = 0;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                const double randomNum = random.nextf ();
                u[i][j]                = randomNum;
                if (i == j) { trace += randomNum; }
            }
        }

        assert (fabs (u.trace () - trace) <= u.baseTypeEpsilon ());
    }

    // Matrix minors
    {
        cout << "4x4 matrix minors" << endl;

        IMATH_INTERNAL_NAMESPACE::M44d a (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

        assert (a.minorOf (0, 0) == a.fastMinor (1, 2, 3, 1, 2, 3));
        assert (a.minorOf (0, 1) == a.fastMinor (1, 2, 3, 0, 2, 3));
        assert (a.minorOf (0, 2) == a.fastMinor (1, 2, 3, 0, 1, 3));
        assert (a.minorOf (0, 3) == a.fastMinor (1, 2, 3, 0, 1, 2));
        assert (a.minorOf (1, 0) == a.fastMinor (0, 2, 3, 1, 2, 3));
        assert (a.minorOf (1, 1) == a.fastMinor (0, 2, 3, 0, 2, 3));
        assert (a.minorOf (1, 2) == a.fastMinor (0, 2, 3, 0, 1, 3));
        assert (a.minorOf (1, 3) == a.fastMinor (0, 2, 3, 0, 1, 2));
        assert (a.minorOf (2, 0) == a.fastMinor (0, 1, 3, 1, 2, 3));
        assert (a.minorOf (2, 1) == a.fastMinor (0, 1, 3, 0, 2, 3));
        assert (a.minorOf (2, 2) == a.fastMinor (0, 1, 3, 0, 1, 3));
        assert (a.minorOf (2, 3) == a.fastMinor (0, 1, 3, 0, 1, 2));
        assert (a.minorOf (3, 0) == a.fastMinor (0, 1, 2, 1, 2, 3));
        assert (a.minorOf (3, 1) == a.fastMinor (0, 1, 2, 0, 2, 3));
        assert (a.minorOf (3, 2) == a.fastMinor (0, 1, 2, 0, 1, 3));
        assert (a.minorOf (3, 3) == a.fastMinor (0, 1, 2, 0, 1, 2));
    }
    {
        IMATH_INTERNAL_NAMESPACE::M44f a (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

        assert (a.minorOf (0, 0) == a.fastMinor (1, 2, 3, 1, 2, 3));
        assert (a.minorOf (0, 1) == a.fastMinor (1, 2, 3, 0, 2, 3));
        assert (a.minorOf (0, 2) == a.fastMinor (1, 2, 3, 0, 1, 3));
        assert (a.minorOf (0, 3) == a.fastMinor (1, 2, 3, 0, 1, 2));
        assert (a.minorOf (1, 0) == a.fastMinor (0, 2, 3, 1, 2, 3));
        assert (a.minorOf (1, 1) == a.fastMinor (0, 2, 3, 0, 2, 3));
        assert (a.minorOf (1, 2) == a.fastMinor (0, 2, 3, 0, 1, 3));
        assert (a.minorOf (1, 3) == a.fastMinor (0, 2, 3, 0, 1, 2));
        assert (a.minorOf (2, 0) == a.fastMinor (0, 1, 3, 1, 2, 3));
        assert (a.minorOf (2, 1) == a.fastMinor (0, 1, 3, 0, 2, 3));
        assert (a.minorOf (2, 2) == a.fastMinor (0, 1, 3, 0, 1, 3));
        assert (a.minorOf (2, 3) == a.fastMinor (0, 1, 3, 0, 1, 2));
        assert (a.minorOf (3, 0) == a.fastMinor (0, 1, 2, 1, 2, 3));
        assert (a.minorOf (3, 1) == a.fastMinor (0, 1, 2, 0, 2, 3));
        assert (a.minorOf (3, 2) == a.fastMinor (0, 1, 2, 0, 1, 3));
        assert (a.minorOf (3, 3) == a.fastMinor (0, 1, 2, 0, 1, 2));
    }

    // VC 2005 64 bits compiler has a bug with __restrict keword.
    // Pointers with __restrict should not alias the same symbol.
    // But, with optimization on, VC removes intermediate temp variable
    // and ignores __restrict.
    {
        cout << "M44 multiplicaftion test" << endl;
        IMATH_INTERNAL_NAMESPACE::M44f M (1.0f,
                                          2.0f,
                                          3.0f,
                                          4.0f,
                                          5.0f,
                                          6.0f,
                                          7.0f,
                                          8.0f,
                                          9.0f,
                                          10.0f,
                                          11.0f,
                                          12.0f,
                                          13.0f,
                                          14.0f,
                                          15.0f,
                                          16.0f);

        IMATH_INTERNAL_NAMESPACE::M44f N;
        N.makeIdentity();

        // N should be equal to M
        // This typical test fails
        // when __restrict is used for pointers in "multiply" function.
        N = N * M;

        assert (N == M);

        if (N != M)
        {
            cout << "M44 multiplication test has failed, error." << endl
                 << "M" << endl
                 << M << endl
                 << "N" << endl
                 << N << endl;
        }
    }

    cout << "ok\n" << endl;
}

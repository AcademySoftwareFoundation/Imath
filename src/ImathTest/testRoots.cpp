//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifdef NDEBUG
#    undef NDEBUG
#endif

#include <ImathFun.h>
#include <ImathRoots.h>
#include <algorithm>
#include <assert.h>
#include <iomanip>
#include <iostream>
#include "testRoots.h"

using namespace std;

void
sort (int nx, double& x0, double& x1, double& x2)
{
    if (nx == 2)
    {
        if (x0 > x1)
            swap (x0, x1);
    }

    if (nx == 3)
    {
        if (x0 > x1)
            swap (x0, x1);
        if (x1 > x2)
            swap (x1, x2);
        if (x0 > x1)
            swap (x0, x1);
    }
}

void
sort (int nx, double x[])
{
    if (nx == 2)
    {
        if (x[0] > x[1])
            swap (x[0], x[1]);
    }

    if (nx == 3)
    {
        if (x[0] > x[1])
            swap (x[0], x[1]);
        if (x[1] > x[2])
            swap (x[1], x[2]);
        if (x[0] > x[1])
            swap (x[0], x[1]);
    }
}

void
solve (double a,
       double b,
       double c,
       double d, // coefficients
       int nx,   // number of expected solutions
       double x0,
       double x1,
       double x2) // expected solutions
{
    cout << "coefficients: " << setw (3) << a << ' ' << setw (3) << b << ' ' << setw (3) << c << ' '
         << setw (3) << d << ' ';

    //
    // Solve the equation a*x^3 + b*x^2 + c*x +d
    //

    double x[3];
    int n = IMATH_INTERNAL_NAMESPACE::solveCubic (a, b, c, d, x);

    //
    // Sort the numerical solutions.
    // Sorte the expected solutions.
    //
    sort (nx, x0, x1, x2);
    sort (n, x);

    //
    // Compare the numerical and the expected solutions.
    //

    assert (n == nx);

    cout << " solutions: ";

    if (n == -1)
        cout << "[-inf, inf]";

    if (n == 0)
        cout << "none";

    const double e = 0.0000001; // maximum expected error for
                                // the test cases listed below
    if (n >= 1)
    {
        cout << x[0];
        assert (IMATH_INTERNAL_NAMESPACE::equal (x[0], x0, e));
    }
    if (n >= 2)
    {
        cout << ' ' << x[1];
        assert (IMATH_INTERNAL_NAMESPACE::equal (x[1], x1, e));
    }
    if (n >= 3)
    {
        cout << ' ' << x[2];
        assert (IMATH_INTERNAL_NAMESPACE::equal (x[2], x2, e));
    }

    cout << endl;
}

void
solve (double a,
       double b,
       double c, // coefficients
       int nx,   // number of expected solutions
       double x0,
       double x1) // expected solutions
{
    cout << "coefficients: " << setw (3) << a << ' ' << setw (3) << b << ' ' << setw (3) << c
         << ' ';
    //
    // Solve the equation a*x^2 + b*x^1 + c*x
    //

    double x[2] = { 0.0, 0.0 };
    int n       = IMATH_INTERNAL_NAMESPACE::solveQuadratic (a, b, c, x);

    //
    // Sort the numerical solutions.
    // Sort the expected solutions.
    //
    // Dummy variable for sort
    double x2 = 0;
    sort (nx, x0, x1, x2);
    sort (n, x);

    //
    // Compare the numerical and the expected solutions.
    //

    assert (n == nx);
    cout << " solutions: ";

    if (n == -1)
        cout << "[-inf, inf]";

    if (n == 0)
        cout << "none";

    const double e = 0.0000001; // maximum expected error for
                                // the test cases listed below

    if (n >= 1)
    {
        cout << x[0];
        assert (IMATH_INTERNAL_NAMESPACE::equal (x[0], x0, e));
    }

    if (n >= 2)
    {
        cout << ' ' << x[1];
        assert (IMATH_INTERNAL_NAMESPACE::equal (x[1], x1, e));
    }

    cout << endl;
}

void
testRoots()
{
    cout << "Testing functions in ImathRoots.h" << endl;

    cout << endl << "solveCubic" << endl;
    // Solve cubiec equations
    //
    //    coefficients         number of expected solutions
    //            |            |
    //            |            |   expected solutions
    //            |            |         |
    //    +-------+--------+   |  +------+-----+
    //    |                |   |  |            |
    solve (1, 6, 11, 6, 3, -1, -2, -3);  // real solutions: -1, -2, -3
    solve (2, 2, -20, 16, 3, 1, -4, 2);  // real solutions: 1, -4, 2
    solve (3, -3, 1, -1, 1, 1, 0, 0);    // real solutions: 1
    solve (2, 0, -24, -32, 2, 4, -2, 0); // real solutions: 4, -2
    solve (1, 0, 0, 0, 1, 0, 0, 0);      // real solutions: 0
    solve (8, -24, 24, -8, 1, 1, 0, 0);  // real solutions: 1
    solve (0, 2, -10, 12, 2, 2, 3, 0);   // real solutions: 2, 3
    solve (0, 1, -1, -20, 2, 5, -4, 0);  // real solutions: 5, -4
    solve (0, 3, -12, 12, 1, 2, 0, 0);   // real solutions: 2
    solve (0, 1, 0, 0, 1, 0, 0, 0);      // real solutions: 0
    solve (0, 1, 0, 1, 0, 0, 0, 0);      // real solutions: none
    solve (0, 0, 3, -6, 1, 2, 0, 0);     // real solutions: 2
    solve (0, 0, 5, 15, 1, -3, 0, 0);    // real solutions: -3
    solve (0, 0, 1, 0, 1, 0, 0, 0);      // real solutions: 0
    solve (0, 0, 0, 1, 0, 0, 0, 0);      // real solutions: none
    solve (0, 0, 0, 0, -1, 0, 0, 0);     // real solutions: [-inf, inf]

    cout << endl << "solveQuadratic" << endl;
    // Solve quadratic equations
    //
    //    coefficients    number of expected solutions
    //          |         |
    //          |         |  expected solutions
    //          |         |      |
    //    +-----+-----+   |  +---+---+
    //    |           |   |  |       |
    solve (1, 3, 2, 2, -1, -2); // real solutions: -1, -2
    solve (1, 0, -9, 2, -3, 3); // real solutions: -3, 3
    solve (1, -4, 0, 2, 4, 0);  // real solutions: 0, 4
    solve (2, -4, 2, 1, 1, 0);  // real solutions: 1
    solve (0, -4, 8, 1, 2, 0);  // real solutions: 2
    solve (0, 7, 0, 1, 0, 0);   // real solutions: 0
    solve (10, 0, 0, 1, 0, 0);  // real solutions: 0
    solve (0, 0, 0, -1, 0, 0);  // real solutions: [-inf, inf]
    solve (0, 0, 1, 0, 0, 0);   // real solutions: none
    solve (3, -6, 30, 0, 0, 0); // real solutions: none
    cout << "ok\n" << endl;
}

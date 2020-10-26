//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifdef NDEBUG
#    undef NDEBUG
#endif

#include "../CImath/chalf.h"
#include "../Imath/half.h"
#include <assert.h>
#include <cstring>
#include <iostream>
#include <testHalf.h>
#include <vector>

using namespace std;

static const vector<float> test_values = { 1.0, 0.0, -1.0, 123.0, 123456789.0, 0.123456789 };

union CpptoC
{
    Imath::half cpp;
    imath_half c;
};

void
testHalfCreate()
{
    cout << "Create half from float" << endl;
    cout << "h1 == h2" << endl;

    for (float f : test_values)
    {
        Imath::half h1 (f);
        imath_half h2 = imath_half_create (f);
        char* h1_c    = reinterpret_cast<char*> (&h1);
        char* h2_c    = reinterpret_cast<char*> (&h2);

        assert (memcmp (h1_c, h2_c, sizeof (Imath::half)) == 0);
    }

    cout << "ok\n\n" << flush;
}

void
testHalfCopy()
{
    cout << "Copy half" << endl;
    cout << "h1 == h2" << endl;

    for (float f : test_values)
    {
        Imath::half h1 (f);
        CpptoC cpptoc;
        cpptoc.cpp    = h1;
        imath_half h2 = imath_half_copy (&cpptoc.c);
        char* h1_c    = reinterpret_cast<char*> (&h1);
        char* h2_c    = reinterpret_cast<char*> (&h2);

        assert (memcmp (h1_c, h2_c, sizeof (Imath::half)) == 0);
    }

    cout << "ok\n\n" << flush;
}

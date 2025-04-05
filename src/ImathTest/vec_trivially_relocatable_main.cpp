//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifdef NDEBUG
#    undef NDEBUG
#endif

#include "testVecTriviallyRelocatable.h"

#include <iostream>
#include <string.h>

#define TEST(x)                                                                \
    if (argc < 2 || !strcmp (argv[1], #x)) x ();

int
main (int argc, char* argv[])
{
    // NB: If you add a test here, make sure to enumerate it in the
    // CMakeLists.txt so it runs as part of the test suite
    TEST (testVecTriviallyRelocatable)
    // NB: If you add a test here, make sure to enumerate it in the
    // CMakeLists.txt so it runs as part of the test suite

    return 0;
}

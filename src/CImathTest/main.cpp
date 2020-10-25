//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifdef NDEBUG
#    undef NDEBUG
#endif

#include <testHalf.h>

#include <iostream>
#include <string.h>

#define TEST(x)                                                                                    \
    if (argc < 2 || !strcmp (argv[1], #x))                                                         \
        x();

int
main (int argc, char* argv[])
{
    std::cout << "\ntesting type half:\n\n" << std::flush;

    TEST (testHalfCreateFromFloat);
    TEST (testHalfCopy);

    return 0;
}

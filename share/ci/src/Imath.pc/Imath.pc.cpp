//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

//
// A simple program for use in validating that Imath.pc is configured
// properly.
//

#include <ImathConfig.h>
#include <ImathVec.h>

using namespace IMATH_NAMESPACE;

int
main (int argc, char* argv[])
{

    V2f v(0);

    std::cout << argv[0] << ": ok" << std::endl;
    
    return 0;
}

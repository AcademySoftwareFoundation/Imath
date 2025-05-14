//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

//
// A simple program for use in validating that Imath's configuration properly
// exports the information require by downstream projects using Imath via
// CMake's FetchContent
//

#include <ImathConfig.h>
#include <ImathVec.h>

using namespace IMATH_NAMESPACE;

int
main (int argc, char* argv[])
{

    V2f v(0);

    std::cout << "ImathFetchTest: v=" << v << std::endl;
    
    return 0;
}

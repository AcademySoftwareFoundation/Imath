//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

//
// A simple program for use in validating that PyImath.pc is
// configured properly.
//

#include <PyImathStringTable.h>
#include <PyImath.h>

int
main (int argc, char* argv[])
{
    Py_Initialize();

    PyImath::StringTable st;

    std::cout << argv[0] << ": ok" << std::endl;
    
    return 0;
}

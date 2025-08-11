#!/usr/bin/env bash

# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

#
# Validate that the given source file compiles and links with the
# given pkg-config file.
#
# First argument is the directory where Imath is installed
# Second argument is the pkg name, i.e. Imath or PyImath
# Third argument is the test program source 
#

set -x

if [ "$#" -ne 3 ]; then
    echo "Error: Expected 4 arguments, got $#." >&2
    echo "Usage: $0 <install dir> <pc file> <src file>" >&2
    exit 1
fi

INSTALL=$1
PKG=$2
SRC=$3

export PKG_CONFIG_PATH=$INSTALL/lib/pkgconfig
cflags=$(pkg-config --cflags $PKG)
libs=$(pkg-config --libs $PKG)
bin=$SRC.bin
g++ -std=c++17 $cflags $SRC $libs -o $bin -Wl,-rpath,$INSTALL/lib
./$bin

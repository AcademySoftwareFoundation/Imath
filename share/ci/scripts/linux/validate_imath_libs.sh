#!/usr/bin/env bash

#
# Validate the libary symlinks:
#   * The actual elf binary is, e.g. libImath-3_1.so.29.0.0
#   * The symlinks are:
#       libImath.so -> libImath-3_1.so
#       libImath-3_1.so -> libImath-3_1.so.29
#       libImath-3_1.so.29 -> libImath-3_1.so.29.0.0
#
# Extract the lib version by compiling a program that prints the
# IMATH_LIB_VERSION_STRING. This also validates that the program
# compiles and executes with the info from pkg-config.
#

if [[ $# == "0" ]]; then
    echo "usage: $0 BUILD_ROOT [SRC_ROOT]"
    exit -1
fi

BUILD_ROOT=$1
SRC_ROOT=$2

# Locate Imath.pc and set PKG_CONFIG_PATH accordingly

pkgconfig=$(find $BUILD_ROOT -name Imath.pc)
if [[ "$pkgconfig" == "" ]]; then
    echo "Can't find Imath.pc"
    exit -1
fi    
export PKG_CONFIG_PATH=$(dirname $pkgconfig)

CXX_FLAGS=$(pkg-config Imath --cflags)
LD_FLAGS=$(pkg-config Imath --libs --static)

VALIDATE_CPP=$(mktemp --tmpdir "validate_XXX.cpp")
VALIDATE_BIN=$(mktemp --tmpdir "validate_XXX")
trap "rm -rf $VALIDATE_CPP $VALIDATE_BIN" exit

# Invoke succf (which is not inline) in the program so it links against the library, to validate the link phase.

echo -e '#include <ImathFun.h>\n#include <stdio.h>\nint main() { printf("%s %s\\n", IMATH_PACKAGE_STRING, IMATH_LIB_VERSION_STRING); Imath::succf(0.0f); return 0; }' > $VALIDATE_CPP

g++ $CXX_FLAGS $VALIDATE_CPP -o $VALIDATE_BIN $LD_FLAGS

# Execute the program

LIB_DIR=$(pkg-config Imath --variable=libdir)
export LD_LIBRARY_PATH=$LIB_DIR

validate=`$VALIDATE_BIN`
status=$?

echo $validate

if [[ "$status" != "0" ]]; then
   echo "validate failed: $validate"
   exit -1
fi

# Get the suffix, e.g. -2_5_d, and determine if there's also a _d
libsuffix=$(pkg-config Imath --variable=libsuffix)
if [[ $libsuffix != $(basename ./$libsuffix _d) ]]; then
    _d="_d"
else
    _d=""
fi

#
# Validate each of the symlinks, which should be changed together like:
#
# libImath.so -> libImath-3_1.so -> libImath-3_1.so.29 -> libImath-3_1.so.29.0.0
#

libs=$(pkg-config Imath --libs-only-l | sed -e s/-l//g)
for lib in $libs; do

    base=`echo $lib | cut -d- -f1` # Imath
    suffix=`echo $lib | cut -d- -f2` # 3_1
    
    if [[ -f $LIB_DIR/lib$base$_d.so ]]; then       # libImath.so
        libbase=`readlink $LIB_DIR/lib$base$_d.so`  # libImath-3.1.so
        libcurrent=`readlink $LIB_DIR/$libbase`     # libImath-3.1.so.29
        libversion=`readlink $LIB_DIR/$libcurrent`  # libImath-3.1.so.29.0.0
        file $LIB_DIR/$libversion | grep -q "ELF"

        if [[ "$?" != 0 ]]; then
            echo "Broken libs: lib$base.so -> $libbase -> $libcurrent -> $libversion"
            exit -1
        fi

        echo "lib$base.so -> $libbase -> $libcurrent -> $libversion"

    elif [[ ! -f $LIB_DIR/lib$lib.a ]]; then
        echo "No static lib: $LIB_DIR/lib$lib.a"
    else
        echo "Static lib lib$lib.a"
    fi

done

pkg-config --validate --silence-errors PyImath
if [[ $? == 1 ]]; then
    echo "No PyImath pkg-config"
else

    pylibs=`pkg-config PyImath --libs-only-l | sed -e s/-l//g`
    for lib in $pylibs; do

        base=`echo $lib | cut -d- -f1`
        suffix=`echo $lib | cut -d- -f2`

        if [[ -f $LIB_DIR/lib$lib.so ]]; then 
            libcurrent=`readlink $LIB_DIR/lib$lib.so`
            libversion=`readlink $LIB_DIR/$libcurrent`
            file $LIB_DIR/$libversion | grep -q "ELF"

            if [[ "$?" != 0 ]]; then
                echo "Broken libs: lib$base.so -> $libbase -> $libcurrent -> $libversion"
                exit -1
            fi

            echo "lib$lib.so -> $libcurrent -> $libversion"

        elif [[ ! -f $LIB_DIR/lib$lib.a ]]; then
            echo "No static lib: $LIB_DIR/lib$lib.a"
        else
            echo "Static lib lib$lib.a"
        fi
    done
fi

# Confirm no broken .so symlinks 
file $LIB_DIR/lib* | grep -q broken 
if [[ "$?" == "0" ]]; then
  echo "Broken symbolic link."
  exit -1
fi

# If the SRC_ROOT is specified, check if there are release notes.
if [[ "$SRC_ROOT" != "" ]]; then
    version=$(pkg-config Imath --modversion)
    notes=$(grep "\* \[Version $version\]" $SRC_ROOT/CHANGES.md | head -1)
    if [[ "$notes" == "" ]]; then
        echo "No release notes."
    else
        echo "Release notes: $notes"
    fi
fi
   
echo "ok."


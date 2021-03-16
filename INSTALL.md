# Building and Installation

## Download

To build the latest release of Imath, download the source from the
releases page
https://github.com/AcademySoftwareFoundation/Imath/releases.

To build from the latest development version, which may not be stable,
clone the GitHub repo directly via:

    % git clone https://github.com/AcademySoftwareFoundation/Imath.git

Alternatively, you can download the master branch via
https://github.com/AcademySoftwareFoundation/Imath/tarball/master, and
extract the contents via ``tar``:

    % curl -L https://github.com/AcademySoftwareFoundation/Imath/tarball/master | tar xv

which will produce a source directory named
``AcademySoftwareFoundation-Imath-<abbreviated-SHA-1-checksum>``.

In the instructions that follow, we will refer to the top-level
directory of the source code tree as ``$source_directory``.

## Prerequisites

Make sure these are installed on your system before building Imath:

* Imath requires CMake version 3.12 or newer 
* C++ compiler that supports C++11

## Linux/macOS Quick Start

To build via CMake, first choose a location for the build directory,
which we will refer to as ``$build_directory``.

    % mkdir $build_directory
    % cd $build_directory
    % cmake $source_directory
    % make
    % make install

Note that the CMake configuration prefers to apply an out-of-tree
build process, since there may be multiple build configurations
(i.e. debug and release), one per folder, all pointing at once source
tree, hence the ``$build_directory`` noted above, referred to in CMake
parlance as the *build directory*. You can place this directory
wherever you like.

See the CMake Configuration Options section below for the most common
configuration options especially the install directory. Note that with
no arguments, as above, ``make install`` installs the header files in
``/usr/local/include``, the object libraries in ``/usr/local/lib``, and the
executable programs in ``/usr/local/bin``.

## Python Bindings

If you wish to install the optional python bindings included in this
repository, this must first be toggled on by appending -DPYTHON=ON to the
cmake command before compiling.

    % cmake $source_directory -DPYTHON=ON

From here forward PyImath will be compiled until it is toggled back. Using:

    % cmake $source_directory -DPYTHON=OFF

## Windows Quick Start

Under Windows, if you are using a command line-based setup, such as
cygwin, you can of course follow the above. For Visual Studio, cmake
generators are "multiple configuration", so you don't even have to set
the build type, although you will most likely need to specify the
install location.  Install Directory By default, ``make install``
installs the headers, libraries, and programs into ``/usr/local``, but you
can specify a local install directory to cmake via the
``CMAKE_INSTALL_PREFIX`` variable:

    % cmake .. -DCMAKE_INSTALL_PREFIX=$install_directory

## Library Names

By default the installed libraries follow a pattern for how they are
named. This is done to enable multiple versions of the library to be
installed and targeted by different builds depending on the needs of
the project. A simple example of this would be to have different
versions of the library installed to allow for applications targeting
different VFX Platform years to co-exist.

If you are building dynamic libraries, once you have configured, built,
and installed the libraries, you should see the following pattern of
symlinks and files in the install lib folder:

    libHalf.so -> libHalf-$LIB_SUFFIX.so
    libHalf-$LIB_SUFFIX.so -> libHalf-$LIB_SUFFIX.so.$SO_MAJOR_VERSION
    libHalf-$LIB_SUFFIX.so.$SO_MAJOR_VERSION -> libHalf-$LIB_SUFFIX.so.$SO_FULL_VERSION
    libHalf-$LIB_SUFFIX.so.$SO_FULL_VERSION (actual file)

You can configure the LIB_SUFFIX, although it defaults to the library
major and minor version, so in the case of a 2.3 library, it would default
to 2_3. You would then link your programs against this versioned library
to have maximum safety (i.e. `-lHalf-2_3`), and the pkg-config and cmake
configuration files included with find_package should set this up.

## Custom Namespaces

Normally, all Imath symbols are in the ``Imath`` namespace, but you
can control this at cmake time via the ``IMATH_NAMESPACE`` and
``IMATH_INTERNAL_NAMESPACE`` cmake settings.

These settings specify an ``IMATH_INTERNAL_NAMESPACE`` preprocessor
definition that places all of the Imath symbols within the given
namespace rather than the standard ``Imath`` namespace.  Those symbols
are made available to client code through the ``IMATH_NAMESPACE`` in
addition to the ``IMATH_INTERNAL_NAMESPACE``. See
``Imath/ImathConfig.h`` for details.

To ensure source code compatibility, the ``IMATH_NAMESPACE`` defaults
to ``Imath`` and then ``using namespace IMATH_INTERNAL_NAMESPACE;``
brings all of the declarations from the ``IMATH_INTERNAL_NAMESPACE``
into the ``IMATH_NAMESPACE``.  This means that client code can
continue to use syntax like ``Imath::V3f``, but at link time it will
resolve to a mangled symbol based on the ``IMATH_INTERNAL_NAMESPACE``.

As an example, if one needed to build against a newer version of Imath
and have it run alongside an older version in the same application, it
is possible to use an internal namespace to prevent collisions between
the older versions of Imath symbols and the newer ones.  To do this,
the following could be defined at build time:

    cmake -DIMATH_INTERNAL_NAMESPACE=Imath_v2 $source_directory

This means that declarations inside Imath headers look like this (after
the preprocessor has done its work):

    namespace Imath_v2 {
        ...
        class declarations
        ...
    }

    namespace Imath {
        using namespace Imath_v2;
    }

## Cross Compiling / Specifying Specific Compilers

When trying to either cross-compile for a different platform, or for
tasks such as specifying a compiler set to match the VFX reference
platform (https://vfxplatform.com/), cmake provides the idea of a
toolchain which may be useful instead of having to remember a chain of
configuration options. It also means that platform-specific compiler
names and options are out of the main cmake file, providing better
isolation.

A toolchain file is simply just a cmake script that sets all the
compiler and related flags and is run very early in the configuration
step to be able to set all the compiler options and such for the
discovery that cmake performs automatically. These options can be set
on the command line still if that is clearer, but a theoretical
toolchain file for compiling for VFX Platform 2015 is provided in the
source tree at cmake/Toolchain-Linux-VFX_Platform15.cmake which will
hopefully provide a guide how this might work.

For cross-compiling for additional platforms, there is also an
included sample script in cmake/Toolchain-mingw.cmake which shows how
cross compiling from Linux for Windows may work. The compiler names
and paths may need to be changed for your environment.

More documentation:

* Toolchains: https://cmake.org/cmake/help/v3.12/manual/cmake-toolchains.7.html
* Cross compiling: https://gitlab.kitware.com/cmake/community/wikis/doc/cmake/

## CMake Configuration Options

The default CMake configuration options are stored in
``Imath/config/ImathSetup.cmake``.  To see a complete set of option
variables, run:

    % cmake -LAH $source_directory

Via standard cmake operation, you can customize these options three
ways:

1. Modify the ``.cmake`` files in place.
2. Use the UI ``cmake-gui`` or ``ccmake``.
2. Specify them as command-line arguments via -D when you invoke cmake.

Common cmake configuration settings:

* ``IMATH_BUILD_BOTH_STATIC_SHARED`` - Build both static and shared
  libraries in one step (otherwise follows BUILD_SHARED_LIBS). Default
  is ``OFF``.

* ``IMATH_CXX_STANDARD`` - C++ standard to compile against. Default is
  ``14``.

* ``IMATH_ENABLE_LARGE_STACK`` - Enables code to take advantage of
  large stack support.  Default is ``OFF``.

* ``IMATH_INSTALL_PKG_CONFIG`` - Install Imath.pc file. Default is
  ``ON``.

* ``IMATH_NAMESPACE`` - Public namespace alias for
  Imath. Default is ``Imath``.

* ``IMATH_INTERNAL_NAMESPACE`` - Real namespace for Imath that
  will end up in compiled symbols. Default is ``Imath_<major>_<minor>``.

* ``IMATH_NAMESPACE_CUSTOM`` - Whether the namespace has been
  customized (so external users know). Default is ``NO``.

* ``IMATH_LIB_SUFFIX`` - String added to the end of all the
  libraries. Default is ``-<major>_<minor>``

* ``IMATH_OUTPUT_SUBDIR`` - Destination sub-folder of the include path
  for install. Default is ``Imath``.

## Cmake Tips and Tricks:

If you have ninja (https://ninja-build.org/) installed, it is faster than make. You can generate ninja files using cmake when doing the initial generation:

    % cmake -G “Ninja” ..

If you would like to confirm compile flags, you don’t have to specify the verbose configuration up front, you can instead run

    % make VERBOSE=1


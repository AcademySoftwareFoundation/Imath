# Building and Installation

## Download

To build the latest release of Imath, download the source (.zip or
.tar.gz) from the releases page
https://github.com/AcademySoftwareFoundation/Imath/releases.

To build from the latest development version, clone the GitHub repo directly via:

    % git clone https://github.com/AcademySoftwareFoundation/Imath.git

The ``master`` branch is the most recent development version, which
may be unstable, but the ``release`` branch always points to the most
recent and most modern, stable, released version.

**Note:** The GitHub repository identifies a "latest" release, which
GitHub defines as the release containing the most recent commit, which
may correspond to a patch for an earlier minor release. Therefore, the
release labled "latest" is not always the most modern or preferable
version. If you want the most modern release, use the ``releases``
branch.

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

See the [CMake Configuration Options](#CMake-Configuration-Options)
section below for the most common configuration options especially the
install directory. Note that with no arguments, as above, ``make
install`` installs the header files in ``/usr/local/include``, the
object libraries in ``/usr/local/lib``, and the executable programs in
``/usr/local/bin``.

## Windows Quick Start

Under Windows, if you are using a command line-based setup, such as
cygwin, you can of course follow the above. For Visual Studio, CMake
generators are "multiple configuration", so you don't even have to set
the build type, although you will most likely need to specify the
install location.  Install Directory By default, ``make install``
installs the headers, libraries, and programs into ``/usr/local``, but you
can specify a local install directory to CMake via the
``CMAKE_INSTALL_PREFIX`` variable:

    % cmake .. -DCMAKE_INSTALL_PREFIX=$install_directory


## Documentation

The Imath documentation at
[imath.readthedocs.io](https://imath.readthedocs.io) is generated via
[Sphinx](https://www.sphinx-doc.org) with the
[Breathe](https://breathe.readthedocs.io) extension using information
extracted from header comments by [Doxgen](https://www.doxygen.nl).

To build the documentation locally from the source headers and
``.rst`` files, set the CMake option ``DOCS=ON``. This adds
``Doxygen`` and ``Sphinx`` CMake targets. Local documentation
generation is off by default.

Building the documentation requires that sphinx, breathe, and doxygen
are installed.

## Python Bindings

To build and install the optional Python bindings for Imath, set the
CMake option ``PYTHON=ON``.

The Python bindings require that ``boost_python`` is installed. By
default, the bindings build for Python 3. To build with python 2, set
the CMake option ``USE_PYTHON2=ON``.

## Library Names

By default the installed libraries follow a pattern for how they are
named. This is done to enable multiple versions of the library to be
installed and targeted by different builds depending on the needs of
the project. A simple example of this would be to have different
versions of the library installed to allow for applications targeting
different VFX Platform years to co-exist.

If you are building dynamic libraries, once you have configured,
built, and installed the libraries, you should see something like the
following pattern of symlinks and files in the install lib folder:

    libImath.so -> libImath-3_1.so
    libImath-3_1.so -> libImath-3_1.so.29
    libImath-3_1.so.29 -> libImath-3_1.29.0.1

The actual shared object file is appended with the library interface
version name, formed according to the [libtool versioning
scheme](https://www.gnu.org/software/libtool/manual/html_node/Libtool-versioning.html):
``current.revision.age``.

Note that the ``libImath.so`` symlink is ommitted if the
``IMATH_INSTALL_SYM_LINK`` option is disabled.

You can configure the ``IMATH_LIB_SUFFIX``, although it defaults to
the library major and minor version, so in the case of a ``3.1`` release,
it would default to ``-3_1``. You would then link your programs against
this versioned library to have maximum safety (i.e. `-lImath-3_1`),
and the pkg-config and CMake configuration files included with
find_package should set this up.

The versioned libraries will have the ``-d`` suffix when the
``CMAKE_BUILD_TYPE`` is ``Debug``.

## Custom Namespaces

Normally, all Imath symbols are in the ``Imath`` namespace, but you
can control this at CMake time via the ``IMATH_NAMESPACE`` and
``IMATH_INTERNAL_NAMESPACE`` CMake settings.

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

    % cmake -DIMATH_INTERNAL_NAMESPACE=Imath_v2 $source_directory

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

When either cross-compiling for a different platform, or when
specifying a compiler set to match the VFX reference platform
(https://vfxplatform.com/), CMake provides the idea of a *toolchain*,
which may be useful instead of having to remember a chain of
configuration options. This also means that platform-specific compiler
names and options are kept out of the main ``CMakeList.txt`` file,
which provides better isolation.

A toolchain file is simply a CMake script that sets compiler and
related flags and is run early in the configuration step, prior to
CMake's automatic discovery step. These options can still be set on
the command line if that is clearer, but a theoretical toolchain file
for compiling for VFX Platform 2021 is provided in the source tree at
``cmake/Toolchain-Linux-VFX_Platform21.cmake`` which will hopefully
provide a guide how this might work.

For cross-compiling for additional platforms, there is also an
included sample script in ``cmake/Toolchain-mingw.cmake`` which shows how
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

### Imath Configuration Settings:

``IMATH_CXX_STANDARD``
  C++ standard to compile against. Default is ``14``.

``IMATH_HALF_USE_LOOKUP_TABLE``
  Use the half-to-float conversion lookup table. Default is ``ON`` for
  backwards compatibility. With the value of ``OFF``, use a bit-shift
  conversion algorithm. Note that this setting is overriden when
  compiler flags enable the F16C SSE instruction set.

``IMATH_USE_DEFAULT_VISIBILITY``
  Use default visibility, which makes all symbols visible in compiled
  objects.  Default is ``OFF``, in which case only designated
  necessary symbols are marked for export.

``IMATH_USE_NOEXCEPT``
  Use the ``noexcept`` specifier of appropriate methods. Default is
  ``ON``.  With the value of ``OFF``, the ``noexcept`` specifier is
  omitted, for situations in which it is not desireable.

``IMATH_ENABLE_LARGE_STACK``
  Enables the ``halfFunction`` object to place the lookup tables on
  the stack rather than allocating heap memory. Default is ``OFF``.

``IMATH_VERSION_RELEASE_TYPE``
  A string to append to the version
  number in the internal package name macro
  IMATH_PACKAGE_STRING. Default is the empty string, but can be set
  to, for example, "-dev" during development (e.g. "3.1.0-dev").

``IMATH_INSTALL_SYM_LINK``
  Install an unversioned symbolic link (i.e. libImath.so) to the
  versioned library.

``IMATH_INSTALL_PKG_CONFIG``
  Install Imath.pc file. Default is ``ON``.

``IMATH_NAMESPACE``
  Public namespace alias for Imath. Default is ``Imath``.

``IMATH_INTERNAL_NAMESPACE``
  Real namespace for Imath that will end up in compiled
  symbols. Default is ``Imath_<major>_<minor>``.

``IMATH_NAMESPACE_CUSTOM``
  Whether the namespace has been customized (so external users
  know). Default is ``NO``.

``IMATH_LIB_SUFFIX``
  String added to the end of all the versioned libraries. Default is
  ``-<major>_<minor>``

``IMATH_OUTPUT_SUBDIR``
  Destination sub-folder of the include path for install. Default is ``Imath``.

``DOCS``
  Build the html documentation. Default is ``OFF``.
  
``PYTHON``
  Build the optional Imath python bindings. Default is ``OFF``.
  
  The Python bindings require that ``boost_python`` is installed.

``USE_PYTHON2`` If ``ON`` and ``PYTHON`` is also ``ON``, build the
  bindings for Python 2. Default is ``OFF``, implying that the default
  bindings are built for Python 3.

``PYIMATH_OVERRIDE_PYTHON_INSTALL_DIR``
  Custom destination for installatation of ``imath.so`` and
  ``imathnumpy.so`` modules. By default, they go into either
  ``site-packages`` or ``dist-packages`.

To enable half-to-float conversion using the F16C SSE instruction set
for g++ and clang when installing Imath, add the ``-mf16c`` compiler
option:

      % cmake <Imath source directory> -DCMAKE_CXX_FLAGS="-mf16c"

See the [Imath Technical
Documentation](https://imath.readthedocs.io/en/latest) for more
information about the half-float conversion process.

### Common CMake Settings:

* ``BUILD_SHARED_LIBS`` - CMake standard variable to select a static or shared
  build. Default is ``ON``.

* ``BUILD_TESTING`` - Build the runtime test suite. Default is ``ON``.

## CMake Tips and Tricks:

If you have ninja (https://ninja-build.org/) installed, it is faster
than make. You can generate ninja files using CMake when doing the
initial generation:

    % cmake -G “Ninja” ..

If you would like to confirm compile flags, you don’t have to specify
the verbose configuration up front, you can instead run

    % make VERBOSE=1


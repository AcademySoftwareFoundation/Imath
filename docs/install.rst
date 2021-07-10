Install Options
###############

When installing Imath from source, take note of the following CMake options:

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

To enable half-to-float conversion using the F16C SSE instruction set
for g++ and clang when installing Imath, add the ``-mf16c`` compiler
option:
::
   
    % cmake <Imath source directory> -DCMAKE_CXX_FLAGS="-mf16c"

See :ref:`half-float Conversion Configuration Options <half-float-conversion-configuration-options>` for more
information about the half-float conversion process.


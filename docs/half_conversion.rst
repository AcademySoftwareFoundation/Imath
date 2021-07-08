half-float Conversion Configuration Options
###########################################

The Imath library supports three options for conversion between 16-bit
half and 32-bit float:

1. F16C SSE instructions - single-instruction conversion for machine
   architectures that support it. When available, this is the fastest
   option by far.

2. Bit-shift conversion algorithm.

3. Conversion from half to float via a 16-bit lookup table. Prior to
   Imath v3.1, this was the only method supported.

To use the F16C instruction set on an architecture that supports it,
simply provide the appropriate compiler flags. For g++ and clang, for example:

    % cmake -dCMAKE_CXX_FLAGS="-m16fc"

When code including ``half.h`` is compiled with F16C enabled, it will
automatically perform conversions using the instruction set, no
additional settings required.

On architectures that do not support F16C, you may choose at
compile-time between the bit-shift conversion and lookup table
conversion via the ``IMATH_ENABLE_HALF_LOOKUP_TABLES`` CMake option:

    % cmake -d IMATH_ENABLE_HALF_LOOKUP_TABLES=OFF

Note that when building the Imath library itself, the lookup table
will be compiled into the library regardless of the
``IMATH_ENABLE_HALF_LOOKUP_TABLES`` setting. This subsequently allows
programs using Imath to choose at compile time which conversion to
use.

Applications with memory limitations that cannot accomodate the
conversion lookup tables can eliminate them from the library by
building Imath and with the C macro ``IMATH_HALF_TABLES_AT_ALL``
defined. Note that this is a compile-time option, not a CMake setting.

Furthermore, an implementation wishing to receive floating point
exceptions on underflow / overflow when converting float to half can
include ``half.h`` with ``IMATH_HALF_EXCEPTIONS_ENABLED``
defined. This setting is off by default.







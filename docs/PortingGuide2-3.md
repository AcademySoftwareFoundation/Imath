OpenEXR/Imath 2.x to 3.x Porting Guide
======================================

This porting guide outlines the several areas where switching from OpenEXR
2.x to OpenEXR 3.x + Imath 3.x will require source code or build changes of
downstream software.

In each case, we will often explain both how to change if you are expecting
3.x only hereafter, or usually a more complex accommodation if you want to
keep compatibility with both 2.x and 3.x.


OpenEXR and Imath are different packages now
--------------------------------------------

If your use of OpenEXR was only for the sake of using the math classes and
utilities, maybe you were unhappy that you needed to download and build the
full OpenEXR dependency. You are in luck -- now Imath is a separate, very
lightweight open source package. You can use Imath functionality without
needing any of OpenEXR, which as of 3.x only includes the parts you need to
read and write OpenEXR image files.

The parts of "IlmBase" that were Imath and `half` are now repackaged as the
`Imath` library. The IlmThread and Iex libraries have been folded into the
OpenEXR package, since they were were not necessary to the rest of Imath.

When building OpenEXR 3.x, note that if Imath 3.x library is not found
already installed at build time, it will be automatically downloaded and
built as part of the OpenEXR build.



Finding and using OpenEXR and Imath CMake configs
-------------------------------------------------

**OpenEXR/Imath 3.x only**

If you are *only* concerned with OpenEXR/Imath 3.x going forward, this is
the recommended way to find the libraries in a downstream project that uses
the CMake build system:

    find_package(Imath CONFIG)
    find_package(OpenEXR CONFIG)

Note that the second line may be omitted if you only need the Imath
portions.

And then your project can reference the imported targets like this:

    target_link_libraries (my_target
        PRIVATE
            OpenEXR::OpenEXR
            Imath::Imath
            Imath::Half
        )

You only need the parts you use, so for example, if you only need Half and
Imath, you can omit the OpenEXR target. Also note that in our example above,
we have used the `PRIVATE` label, but you should specify them as `PUBLIC` if
you are exposing those classes in your own package's public interface.


**Accommodating OpenEXR/Imath 3.x or OpenEXR 2.x**

On the other hand, to accommodate both 2.x and 3.x, it's admittedly
inconvenient because the packages and the import targets have changed their
names.  We have found the following idioms to work:

Finding either/both packages:

    # First, try to find just the right config files
    find_package(Imath CONFIG)
    if (NOT TARGET Imath::Imath)
        # Couldn't find Imath::Imath, maybe it's older and has IlmBase?
        find_package(IlmBase CONFIG)
    endif ()
    find_package(OpenEXR CONFIG)


To link against them, we use CMake generator expressions so that we can
referencing *both* sets of targets, but it will only use the ones
corresponding to the package version that was found.

    target_link_libraries (my_target
        PRIVATE
            # For OpenEXR/Imath 3.x:
              $<$<TARGET_EXISTS:OpenEXR::OpenEXR>:OpenEXR::OpenEXR>
              $<$<TARGET_EXISTS:Imath::Imath>:Imath::Imath>
              $<$<TARGET_EXISTS:Imath::Half>:Imath::Half>
            # For OpenEXR 2.4/2.5:
              $<$<TARGET_EXISTS:OpenEXR::IlmImf>:OpenEXR::IlmImf>
              $<$<TARGET_EXISTS:IlmBase::Imath>:IlmBase::Imath>
              $<$<TARGET_EXISTS:IlmBase::Half>:IlmBase::Half>
              $<$<TARGET_EXISTS:IlmBase::IlmThread>:IlmBase::IlmThread>
              $<$<TARGET_EXISTS:IlmBase::Iex>:IlmBase::Iex>
        )

Again, you can eliminate the references to any of the individual libaries
that you don't actually need for your application.


Imath include files are in a different subdirectory
---------------------------------------------------

Imath 3.0 will copy its headers to some `include/Imath` subdirectory
instead of the old `include/OpenEXR`.

**OpenEXR/Imath 3.x only**

If you know that you are only using Imath 3.x, then just change any
include directions, like this:

    #include <OpenEXR/ImathVec.h>
    #include <OpenEXR/half.h>

to the new locations:

    #include <Imath/ImathVec.h>
    #include <Imath/half.h>

**Accommodating OpenEXR/Imath 3.x or OpenEXR 2.x**

If you want your software to be able to build against either OpenEXR 2.x or
3.x (depending on which dependency is available at build time), we recommend
using a more complicated idiom:

    // The version can reliably be found in this header file from OpenEXR,
    // for both 2.x and 3.x:
    #include <OpenEXR/OpenEXRConfig.h>
    #define COMBINED_OPENEXR_VERSION ((10000*OPENEXR_VERSION_MAJOR) + \
                                      (100*OPENEXR_VERSION_MINOR) + \
                                      OPENEXR_VERSION_PATCH)

    // There's just no easy way to have an `#include` that works in both
    // cases, so we use the version to switch which set of include files we
    // use.
    #if COMBINED_OPENEXR_VERSION >= 20599 /* 2.5.99: pre-3.0 */
    #   include <Imath/ImathVec.h>
    #   include <Imath/half.h>
    #else
        // OpenEXR 2.x, use the old locations
    #   include <OpenEXR/ImathVec.h>
    #   include <OpenEXR/half.h>
    #endif


Imath now uses standard C++ exceptions and uses `noexcept`
----------------------------------------------------------

In OpenEXR 2.x, the Imath functions that threw exceptions used to throw
various Iex varieties.

In Imath 3.x, these functions just throw `std::exception` varieties that
correspond to the failure (e.g., `std::invalid_argument`,
`std::domain_error`, etc.). For that reason, all of the Iex exceptions are
now only part of the OpenEXR library (where they are still used in the same
manner they were for OpenEXR 2.x).

Imath 3.x has very few functions that throw exceptions. Each is are clearly
marked as such, and each has a version that does not throw exceptions (so
that it may be used from code where exceptions are avoided). The functions
that do not throw exceptions are now marked `noexcept`.


Some headers and classes have been removed from Imath 3.x
---------------------------------------------------------

The `Math<T>` class (and `ImathMath.h` header file) are deprecated. All of
the `Math<T>` functionality is subsumed by C++11 `std::` math functions. For
example, calls to `Imath::Math<T>::abs(x)` should be replaced with
`std::abs(x)`.

The `Limits<T>` class (and the `ImathLimits.h` and `ImathHalfLimits.h`
headers) have been removed entirely. All uses of `Limits<>` should be
replaced with the appropriate `std::numeric_limits<>` method call. The
Imath-specific versions predated C++11, and were not only redundant in a
C++11 world, but also potentially confusing because some of their functions
behaved quite differently than the `std::numeric_limits` method with the
same name. We are following the precept that if C++11 does something in a
standard way, we should not define our own equivalent function (and
especially not define it in a way that doesn't match the standard behavior).

`Vec<T>::normalize()` and `length()` methods, for integer `T` types, have
been removed. Also the standalone `projec()` and `orthogonal()` functions
are no longer defined for vectors made of integer elements. These all had
behavior that was hard to understand and probably useless. They still work
as expected for vectors of floating-point types.

# Imath

[![License](https://img.shields.io/github/license/AcademySoftwareFoundation/Imath)](LICENSE.md)
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/2799/badge)](https://bestpractices.coreinfrastructure.org/projects/2799)
[![Build Status](https://dev.azure.com/academysoftwarefoundation/Academy%20Software%20Foundation/_apis/build/status/academysoftwarefoundation.Imath)](https://dev.azure.com/academysoftwarefoundation/Academy%20Software%20Foundation/_build?definitionId=4&_a=summary)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=AcademySoftwareFoundation_Imath&metric=alert_status)](https://sonarcloud.io/dashboard?id=AcademySoftwareFoundation_Imath)

Imath is a basic, light-weight, and efficient C++ representation of 2D
and 3D vectors and matrices and other simple but useful mathematical
objects, functions, and data types common in computer graphics
applications, including the “half” 16-bit floating-point type.

Imath also includes optional python bindings for all types and
functions, including optimized implementations of vector and matrix
arrays.

## Project Mission

The goals of the Imath project are simplicity, ease of use,
correctness and verifiability, performance, and breadth of
adoption. Imath is not intended to be a comprehensive linear algebra
or numerical analysis package.

## Features

* half: 16-bit floating-point type
* Vector: V2s, V2i, V2i64, V2f, V2d, V3s, V3i, V4i64, V3f, V3d, V4s, V4i, V4i64, V4f, V4d
* Matrix: M22f, M22d, M33f, M33d, M44f, M44d
* Bounding box: Box2s, Box2i, Box2i64, Box2f, Box2d, Box3s, Box3i, Box3i64, Box3f, Box3d
* Color: C3h, C3f, C3c, C4f, C4h, C4c
* Euler angles: Eulerf, Eulerd
* Quaternion: Quatf, Quatd
* Viewing frustum: Frustrumf, Frustumd
* Interval: Intervals, Intervali, Intervalf, Intervald
* Line: Line3f, Line3d
* Plane: Plane3f, Plane3d
* Sphere: Sphere3f, Sphere3d
* Shear: Shear3f, Shear3d, Shear6f, Shear6
* Miscellaneous math functions
  
## New Features in 3.1

The 3.1 release of Imath introduces optimized half-to-float and
float-to-half conversion using the F16C SSE instruction set extension,
if available. These single-instruction conversions offer a 5-10x
speedup for float-to-half and 2x speedup for half-to-float over
Imath/half's traditional table-based conversion (timings depend on the
data).

In the absence of the F16C instruction set, the lookup-table-based
conversion from half to float is still the default, but Imath 3.1 also
introduces an optimized bit-shift conversion algorithm as a
compile-time option that does not require lookup tables, for
architectures where memory is limited. The float-to-half conversion
also no longer requires an exponent lookup table, further reducing
memory requirements.

These new conversions generate the same values as the traditional
methods, which ensures backwards compatibility.  See
[INSTALL.md](INSTALL.md#imath-configuration-settings) for more
installation and configuration options.

Also, ``half.h`` can now be included in pure C code for a definition
of the type and for conversions between half and float.

## Supported Platforms

Imath builds on Linux, macOS, Microsoft Windows, and is
cross-compilable on other systems.

## About Imath

Imath was originally developed at Industrial Light & Magic in the
early 2000's and was originally distributed as open source as a part
of the [OpenEXR](https://github.com/AcademySoftwareFoundation/openexr)
project.

Imath continues to be maintained as a sub-project of OpenEXR, which is
now a project of the [Academy Software
Foundation](https://www.aswf.io).  See
the OpenEXR project's [GOVERNANCE.md](https://github.com/AcademySoftwareFoundation/openexr/blob/main/GOVERNANCE.md)
for more information about how the project operates.

The OpenEXR project is dedicated to promoting a harassment-free
community. Read our [code of conduct](CODE_OF_CONDUCT.md).

## Developer Quick Start

Technical documentation for the Imath classes and functions can be found at
[imath.readthedocs.io](https://imath.readthedocs.io).

See [INSTALL.md](INSTALL.md) for instructions on downloading and building Imath
from source.

If you encounter problems compiling code or building projects written
with an earlier release of Imath, the [porting
guide](https://github.com/AcademySoftwareFoundation/Imath/blob/main/docs/PortingGuide2-3.md)
explains some of the differences and how to address them.

## A Note about Versioning

Because Imath was originally distributed as a part of OpenEXR, it has
already had two major release versions, as a part of OpenEXR v1 and
v2. To avoid confusion with these original releases, the first
version of Imath released independently of OpenEXR is Version v3.0. To
be clear, the versioning and release of Imath is no longer tied to
OpenEXR.

## Getting Help

Connect with the project through:

* The openexr-dev@lists.aswf.io mail list: This is a development
  focused mail list with a deep history of technical conversations and
  decisions that have shaped the project. Subscribe at
  [openexr-dev@lists.aswf.io](https://lists.aswf.io/g/openexr-dev).

* Slack: [#openexr](https://academysoftwarefdn.slack.com/archives/CMLRW4N73)
  
* GitHub Issues: GitHub issues are used both to track bugs and to
  discuss feature requests. Submit an issue here:
  <https://github.com/AcademySoftwareFoundation/imath/issues>.

## Getting Involved

The OpenEXR developer community welcomes contributions to the
project. See [CONTRIBUTING.md](CONTRIBUTING.md) for more information
about contributing to Imath and OpenEXR.

## License

Imath is released under OpenEXR's [BSD-3-Clause](LICENSE.md) license.

---

![aswf](https://github.com/AcademySoftwareFoundation/openexr/blob/main/ASWF/images/aswf.png)

[![License](https://img.shields.io/github/license/AcademySoftwareFoundation/Imath)](LICENSE.md)
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/2799/badge)](https://bestpractices.coreinfrastructure.org/projects/2799)
[![Build Status](https://dev.azure.com/academysoftwarefoundation/Academy%20Software%20Foundation/_apis/build/status/academysoftwarefoundation.Imath)](https://dev.azure.com/academysoftwarefoundation/Academy%20Software%20Foundation/_build?definitionId=4&_a=summary)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=AcademySoftwareFoundation_Imath&metric=alert_status)](https://sonarcloud.io/dashboard?id=AcademySoftwareFoundation_Imath)

# Imath

Imath is a basic, light-weight, and efficient C++ representation of 2D
and 3D vectors and matrices and other simple but useful mathematical
objects, functions, and data types common in computer graphics
applications, including the “half” 16-bit floating-point type.

Imath also includes option python bindings for all types and
functions, including optimized implementations of vector and matrix
arrays. 

### Project Mission

The goals of the Imath project are simplicity, ease of use,
correctness and verifiability, performance, and breadth of
adoption. Imath is not intended to be a comprehensive linear algebra
or numerical analysis package.

### Features

* half: 16-bit floating-point type
* Vector: V2s, V2i, V2f, V2d, V3s, V3i, V3f, V3d, V4s, V4i, V4f, V4d 
* Matrix: M22f, M22d, M33f, M33d, M44f, M44d 
* Bounding box: Box2s, Box2i, Box2f, Box2d, Box3s, Box3i, Box3f, Box3d
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
  
### Supported Platforms

Imath builds on Linux, macOS, Microsoft Windows, and is
cross-compilable on other systems.

### About Imath

Imath was originally developed at Industrial Light & Magic in the
early 2000's and was originally distributed as open source as a part
of the [OpenEXR](https://github.com/AcademySoftwareFoundation/openexr)
project.

Imath continues to be maintained as a sub-project of OpenEXR, which is
now a project of the [Academy Software
Foundation](https://www.aswf.io).  See
the OpenEXR project's [GOVERNANCE.md](https://github.com/AcademySoftwareFoundation/openexr/blob/master/GOVERNANCE.md)
for more information about how the project operates.

The OpenEXR project is dedicated to promoting a harassment-free
community. Read our [code of conduct](CODE_OF_CONDUCT.md).

### Developer Quick Start

See [INSTALL.md](INSTALL.md) for instructions on downloading and building Imath
from source.

### Getting Help

There are two primary ways to connect with the project:

* The openexr-dev@lists.aswf.io mail list: This is a development
  focused mail list with a deep history of technical conversations and
  decisions that have shaped the project. Subscribe at
  [openexr-dev@lists.aswf.io](https://lists.aswf.io/g/openexr-dev).

* GitHub Issues: GitHub issues are used both to track bugs and to
  discuss feature requests.

### Getting Involved

The OpenEXR developer community welcomes contributions to the
project. See [CONTRIBUTING.md](CONTRIBUTING.md) for more information
about contributing to Imath and OpenEXR.

## License

Imath continues to be released under its original OpenEXR [BSD-3-Clause](LICENSE.md) license.

---

![aswf](https://github.com/AcademySoftwareFoundation/openexr/blob/master/ASWF/images/aswf.png)

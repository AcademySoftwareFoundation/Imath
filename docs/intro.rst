Overview
--------

Imath is a basic, light-weight, and efficient C++ representation of 2D
and 3D vectors and matrices and other simple but useful mathematical
objects, functions, and data types common in computer graphics
applications, including the ``half`` 16-bit floating-point type.

Imath also includes optional python bindings for all types and
functions, including optimized implementations of vector and matrix
arrays.

The Imath library emphasizes simplicity, ease of use, correctness and
verifiability, performance, and breadth of adoption. Imath is not
intended to be a comprehensive linear algebra or numerical analysis
package.

Imath is not `Eigen <https://eigen.tuxfamily.org>`_! It's not a
full-featured linear algebra package, and it doesn't represent vectors
and matrices of arbitrary dimension. Its greatest utility is as a
geometric data representation, primarily for 2D images and 3D scenes
and coordinate transformations, along with an accompanying set of
utility methods and functions.

Quick Links
-----------

- Download: https://github.com/AcademySoftwareFoundation/Imath
- Install Help: `INSTALL.md <https://github.com/AcademySoftwareFoundation/Imath/blob/master/INSTALL.md>`_
- Porting Help: `Imath/OpenEXR Version 2->3 Porting Guide <https://github.com/AcademySoftwareFoundation/Imath/blob/master/docs/PortingGuide2-3.md>`_
- License: `BSD License <https://github.com/AcademySoftwareFoundation/Imath/blob/master/LICENSE.md>`_

Install Options
---------------

When installing Imath from source, take note of the following CMake options:

``IMATH_CXX_STANDARD``
  C++ standard to compile against. Default is ``14``.

``IMATH_HALF_USE_LOOKUP_TABLE``
  Use the half-to-float conversion lookup table. Default is ``ON`` for
  backwards compatibility. With the value of ``OFF``, use a bit-shif
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

      % cmake <Imath source directory> -DCMAKE_CXX_FLAGS="-mf16c"

See `half-float Conversion Configuration Options`_ for more
information about the half-float conversion process.

History
-------

Imath originated at Industrial Light & Magic in the late 1990's and
early 2000's, and it was originally distributed publicly as a
component of
`OpenEXR <https:://github.com/AcademySoftwareFoundation/openexr>`_.

Imath is Version 3 because it was previously distributed as a
component of OpenEXR v1 and v2.

Example
-------

A basic program:

.. code-block::

   #include <Imath/ImathVec.h>
   #include <iostream>
   
   int
   main()
   {
       Imath::V3f v (3.0, 4.0, 5.0);
   
       v.normalize();

       Imath::M33f M;
       M.translate (1.0, 2.0, 3.0);
       Imath::V3f p = v * M;

       std::cout << "What's your vector, Victor? " << p << std::endl;

       return 0;
   }

Matrices Are Row-Major
----------------------

Imath stores matrices in row-major layout, originally inspired by
compatibility with OpenGL matrices. 

A matrix described as:

.. math::
  \begin{bmatrix}
  m_{00} & m_{01} & m_{02} & m_{03} \\
  m_{10} & m_{11} & m_{12} & m_{13} \\
  m_{20} & m_{21} & m_{22} & m_{23} \\
  m_{30} & m_{31} & m_{32} & m_{33} \\
  \end{bmatrix}

is laid out in memory as:

.. list-table::
   :widths: 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10
   :header-rows: 1

   * - 0
     - 1
     - 2
     - 3
     - 4
     - 5
     - 6
     - 7
     - 8
     - 9
     - 10
     - 11
     - 12
     - 13
     - 14
     - 15
   * - :math:`m_{00}`
     - :math:`m_{01}`
     - :math:`m_{02}`
     - :math:`m_{03}`
     - :math:`m_{10}`
     - :math:`m_{11}`
     - :math:`m_{12}`
     - :math:`m_{13}`
     - :math:`m_{20}`
     - :math:`m_{21}`
     - :math:`m_{22}`
     - :math:`m_{23}`
     - :math:`m_{30}`
     - :math:`m_{31}`
     - :math:`m_{32}`
     - :math:`m_{33}`

A matrix representing a homogeneous transform has a right-hand column
of :math:`\begin{bmatrix} 0 & 0 & 0 & 1\end{bmatrix}` and the
translation component across the bottom row.

As a result, it is best to think of Imath vectors as row-vectors, and
vector-matrix multiplication with the vector on the left and matrix on
the right:

.. math::
  \begin{bmatrix} v_{0}' & v_{1}' & v_{2}' & 1' \end{bmatrix}
  =
  \begin{bmatrix} v_{0} & v_{1} & v_{2} & 1 \end{bmatrix}
  \begin{bmatrix} m_{00} & m_{01} & m_{02} & 0 \\
  m_{10} & m_{11} & m_{12} & 0 \\
  m_{20} & m_{21} & m_{22} & 0 \\
  m_{30} & m_{31} & m_{32} & 1
  \end{bmatrix}

This further implies that you should interpret local transformations
as pre-multiplication:

.. code-block::

   M44f M;
   M.translate (tx, ty, tz);
   m.rotate (r, 0, 0);
   m.scale (s);

.. math::
  \begin{bmatrix}
  m_{00} & m_{01} & m_{02} & m_{03} \\
  m_{10} & m_{11} & m_{12} & m_{13} \\
  m_{20} & m_{21} & m_{22} & m_{23} \\
  m_{30} & m_{31} & m_{32} & m_{33} \\
  \end{bmatrix}
  = 
  \begin{bmatrix}
  s & 0 & 0 & 0 \\
  0 & s & 0 & 0 \\
  0 & 0 & s & 0 \\
  0 & 0 & 0 & 1 \\
  \end{bmatrix}
  \begin{bmatrix}
  1 & 0 & 0 & 0 \\
  0 & \cos(r) & \sin(r) & 0 \\
  0 & -\sin(r) & \cos(r) & 0 \\
  0 & 0 & 0 & 1 \\
  \end{bmatrix}
  \begin{bmatrix}
  1 & 0 & 0 & 0 \\
  0 & 1 & 0 & 0 \\
  0 & 0 & 1 & 0 \\
  tx & ty & tz & 1 \\
  \end{bmatrix}


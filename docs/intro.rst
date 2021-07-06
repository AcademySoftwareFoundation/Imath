===============
Imath |version|
===============

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

``IMATH_USE_HALF_LOOKUP_TABLES``
  The 3.1 release of Imath introduces optimized half-to-float and
  float-to-half conversion, using the F16C SSE extension if available,
  or otherwise using an optimized bit-manipulation algorithm that does
  not require lookup tables. Performance of both options is generally
  significantly faster than the lookup-table based conversions that
  Imath has traditionally used, although performance may vary
  depending on the nature of your data. The new optimized conversions
  generate the same values as the tranditional methods.

  For backwards compatibility and ensured stability in the 3.1
  release, the optimized conversion is off by default, but it can be
  enabled at compile-time by disabling the
  ``IMATH_USE_HALF_LOOKUP_TABLES`` CMake option:

      % cmake <source directory> -DIMATH_USE_HALF_LOOKUP_TABLES=OFF

``IMATH_HALF_NO_TABLES_AT_ALL``
  Furthermore, the ``IMATH_HALF_NO_TABLES_AT_ALL`` CMake option forces
  elimination of all half-conversion lookup tables. This forces either
  the SSE extension instructions or the bit-manipulation conversion.

``IMATH_HALF_EXCEPTIONS_ENABLED``
  An implementation wishing to receive floating point exceptions on
  underflow / overflow when converting float to half can include
  ``half.h`` with ``IMATH_HALF_EXCEPTIONS_ENABLED`` defined.

``IMATH_USE_NOEXCEPT``
  Also, the ``IMATH_USE_NOEXCEPT`` CMake option disables the use of
  the ``noexcept`` specifier. Earlier versions of the OpenEXR library
  provided the ability to catch floating point errors through signal
  handlers and throw corresponding C++ exceptions.  Code using this
  mechanism is incompatible with the ``noexcept`` specifier.

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

The half Type
------------------------

.. doxygenfile:: half.h
   :sections: detaileddescription

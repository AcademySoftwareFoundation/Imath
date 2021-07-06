Matrix33
########

.. code-block::

   #include <Imath/ImathMatrix.h>
   
The ``Matrix33`` class template represents a 3x3 matrix, with
predefined typedefs for ``float`` and ``double``.

There are also various utility functions that operate on matrices
defined in ``ImathMatrixAlgo.h`` and described in :ref:`Matrix
Functions <matrix-functions>`.

Example:

.. literalinclude:: ../examples/Matrix33.cpp
   :language: c++                 

.. doxygentypedef:: M33f

.. doxygentypedef:: M33d

.. doxygenclass:: Imath::Matrix33
   :undoc-members:
   :members:

.. doxygenfunction:: operator<<(std::ostream& s, const Matrix33<T>& m)


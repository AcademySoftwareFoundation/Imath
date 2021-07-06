Matrix44
########

.. code-block::

   #include <Imath/ImathMatrix.h>
   
The ``Matrix44`` class template represents a 4x4 matrix, with
predefined typedefs for ``float`` and ``double``.

There are also various utility functions that operate on matrices
defined in ``ImathMatrixAlgo.h`` and described in :ref:`Matrix
Functions <matrix-functions>`.

Example:

.. literalinclude:: ../examples/Matrix44.cpp
   :language: c++
               
.. doxygentypedef:: M44f

.. doxygentypedef:: M44d

.. doxygenclass:: Imath::Matrix44
   :undoc-members:
   :members:

.. doxygenfunction:: operator<<(std::ostream& s, const Matrix44<T>& m)
      

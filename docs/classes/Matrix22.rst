Matrix22
########

.. code-block::

   #include <Imath/ImathMatrix.h>
   
The ``Matrix22`` class template represents a 2x2 matrix, with
predefined typedefs for ``float`` and ``double``.

There are also various utility functions that operate on matrices
defined in ``ImathMatrixAlgo.h`` and described in :ref:`Matrix
Functions <matrix-functions>`.

Example:

.. literalinclude:: ../examples/Matrix22.cpp
   :language: c++                 

.. doxygentypedef:: M22f

.. doxygentypedef:: M22d

.. doxygenclass:: Imath::Matrix22
   :undoc-members:
   :members:

.. doxygenfunction:: operator<<(std::ostream& s, const Matrix22<T>& m)

      

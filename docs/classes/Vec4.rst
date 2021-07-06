Vec4
####

.. code-block::

   #include <Imath/ImathVec.h>
   
The ``Vec4`` class template represents a 4D vector, with predefined
typedefs for vectors of type ``short``, ``int``, ``int64_t``,
``float``, and ``double``.

Note that the integer specializations of ``Vec4`` lack the
``length()`` and ``normalize()`` methods that are present in the
``float`` and ``double`` versions, because the results don't fit into
integer quantities.

There are also various utility functions that operate on vectors
defined in ``ImathVecAlgo.h`` and described in :ref:`Vector Functions
<vector-functions>`.

Example:

.. literalinclude:: ../examples/Vec4.cpp
   :language: c++
              
.. doxygentypedef:: V4s

.. doxygentypedef:: V4i
                    
.. doxygentypedef:: V4i64
                    
.. doxygentypedef:: V4f
                    
.. doxygentypedef:: V4d
                    
.. doxygenclass:: Imath::Vec4
   :undoc-members:
   :members:

.. doxygenfunction:: operator<<(std::ostream& s, const Vec4<T>& v)
      

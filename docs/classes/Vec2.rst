Vec2
####

.. code-block::

   #include <Imath/ImathVec.h>
   
The ``Vec2`` class template represents a 2D vector, with predefined
typedefs for vectors of type ``short``, ``int``, ``int64_t``, ``float``, and
``double``.

Note that the integer specializations of ``Vec2`` lack the
``length()`` and ``normalize()`` methods that are present in the
``float`` and ``double`` versions, because the results don't fit into
integer quantities.

There are also various utility functions that operate on vectors
defined in ``ImathVecAlgo.h`` and described in :ref:`Vector Functions
<vector-functions>`.


Example:

.. literalinclude:: ../examples/Vec2.cpp
   :language: c++
              
.. doxygentypedef:: V2s

.. doxygentypedef:: V2i
                    
.. doxygentypedef:: V2i64
                    
.. doxygentypedef:: V2f
                    
.. doxygentypedef:: V2d
                    
.. doxygenclass:: Imath::Vec2
   :undoc-members:
   :members:

.. doxygenfunction:: operator<<(std::ostream& s, const Vec2<T>& v)

      

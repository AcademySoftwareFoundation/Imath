Vec3
####

.. code-block::

   #include <Imath/ImathVec.h>
   
The ``Vec3`` class template represents a 3D vector, with predefined
typedefs for vectors of type ``short``, ``int``, ``int64_t``,
``float``, and ``double``.

Note that the integer specializations of ``Vec3`` lack the
``length()`` and ``normalize()`` methods that are present in the
``float`` and ``double`` versions, because the results don't fit into
integer quantities.

There are also various utility functions that operate on vectors
defined in ``ImathVecAlgo.h`` and described in :ref:`Vector Functions
<vector-functions>`.

Example:

.. literalinclude:: ../examples/Vec3.cpp
   :language: c++
              
.. doxygentypedef:: V3s

.. doxygentypedef:: V3i
                    
.. doxygentypedef:: V3i64
                    
.. doxygentypedef:: V3f
                    
.. doxygentypedef:: V3d
                    
.. doxygenclass:: Imath::Vec3
   :undoc-members:
   :members:

.. doxygenfunction:: operator<<(std::ostream& s, const Vec3<T>& v)

      

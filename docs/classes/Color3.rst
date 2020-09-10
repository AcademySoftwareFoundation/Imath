Color3
######

The ``Color3`` class template represents a 3-component color, with
pre-defined typedefs of ``unsigned char``, ``half``, and ``float``.

The ``Color3`` class is derived from ``Imath::Vec3`` and thus has
fields named ``x``, ``y``, and ``z``. The class itself implies no
specific interpretation of the values.

Example:

.. literalinclude:: ../examples/Color3.cpp
   :language: c++

.. doxygentypedef:: Color3c

.. doxygentypedef:: Color3h

.. doxygentypedef:: Color3f

.. doxygentypedef:: C3c

.. doxygentypedef:: C3h

.. doxygentypedef:: C3f
                    
.. doxygenclass:: Imath::Color3
   :members:
   :undoc-members:


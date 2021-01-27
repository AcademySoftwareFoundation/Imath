Color4
######

The ``Color4`` class template represents a 4-component color (red,
green, blue, and alpha), with pre-defined typedefs of ``unsigned
char``, ``half``, and ``float``.

The ``Color4`` class is *not* derived from ``Vec4``. Its fields are
named ``r``, ``g``, ``b``, and ``a``. The class itself implies no
specific interpretation of the values.

Example:

.. literalinclude:: ../examples/Color4.cpp
   :language: c++

.. doxygentypedef:: Color4c

.. doxygentypedef:: Color4h

.. doxygentypedef:: Color4f

.. doxygentypedef:: C4c

.. doxygentypedef:: C4h

.. doxygentypedef:: C4f
                    
.. doxygenclass:: Imath::Color4
   :undoc-members:
   :members:


The ``half`` Class
##################

.. code-block::

   #include <Imath/half.h>

``half`` is a 16-bit floating point number. See `The half Type`_ for
an explanation of the representation.

Also, see `C-language half Conversion`_ for C-language support for
conversion between ``half`` and ``float``.

Example:

.. literalinclude:: ../examples/half.cpp
   :language: c++
              
.. doxygenclass:: Imath::half
   :undoc-members:
   :members:

.. doxygenfunction:: operator<<(std::ostream& os, Imath::half h)

.. doxygenfunction:: operator>>(std::ostream& is, Imath::half& h)

                     

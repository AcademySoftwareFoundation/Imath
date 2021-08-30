The half Class
##############

.. code-block::

   #include <Imath/half.h>

``half`` is a 16-bit floating point number. See :doc:`../float` for an
explanation of the representation.

See :doc:`../functions/half_c` for C-language functions for conversion
between ``half`` and ``float``. Also, see :doc:`../half_conversion`
for information about building Imath with support for the F16C SSE
instruction set.

Example:

.. literalinclude:: ../examples/half.cpp
   :language: c++
              
.. doxygenclass:: Imath::half
   :undoc-members:
   :members:

.. doxygenfunction:: operator<<(std::ostream& os, Imath::half h)

.. doxygenfunction:: operator>>(std::ostream& is, Imath::half& h)

                     

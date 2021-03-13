Interval
########

.. code-block::

   #include <Imath/ImathInterval.h>
   
The ``Interval`` class template represents a scalar interval, with
predefined typedefs for ``short``, ``int``, ``float``, and ``double``.

An ``Interval`` is essentially a ``Box<T>`` that allows ``T`` to be a
scalar.

Example:

.. literalinclude:: ../examples/Interval.cpp
   :language: c++                 

.. doxygentypedef:: Intervals

.. doxygentypedef:: Intervali

.. doxygentypedef:: Intervalf

.. doxygentypedef:: Intervald

.. doxygenclass:: Imath::Interval
   :undoc-members:
   :members:

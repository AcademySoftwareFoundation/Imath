Box
###

The ``Box`` class template represents 2D and 3D bounding boxes, with
predefined typedefs for boxes of type ``short``, ``int``, ``float``,
and ``double``.

The ``Box`` class represents an axis-aligned bounding box defined in
terms of minimum and maximum values along each axis, represented by
``Vec2<T>`` for the ``Box2`` types and by ``Vec3<T>`` for ``Box3``
types.

Example:

.. literalinclude:: ../examples/Box.cpp
   :language: c++

.. doxygentypedef:: Box2s

.. doxygentypedef:: Box2i

.. doxygentypedef:: Box2f

.. doxygentypedef:: Box2d
      
.. doxygentypedef:: Box3s

.. doxygentypedef:: Box3i

.. doxygentypedef:: Box3f

.. doxygentypedef:: Box3d

.. doxygenclass:: Imath::Box
   :members:
   :undoc-members:

.. doxygenclass:: Imath::Box< Vec2< T > >
   :members:
   :undoc-members:
      
.. doxygenclass:: Imath::Box< Vec3< T > >
   :members:
   :undoc-members:




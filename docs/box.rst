Box
###

2D and 3D bounding boxes of type ``short``, ``int``, ``float``, and ``double``.

The ``Box`` class represents an axis-aligned bounding box defined in
terms of minimum and maximum values along each axis, represented by
``Vec2<T>`` for the ``Box2`` types and by ``Vec3<T>`` for ``Box3``
types.

Example: ::

        Imath::V3f   a (0, 0, 0);
        Imath::V3f   b (1, 1, 1);
        Imath::V3f   c (2, 2, 2);

        Imath::Box3f bbox (a);
    
        bbox.extendBy (c);

        bool b_inside = box.intersects (b);
       
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




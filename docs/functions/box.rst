Box
###

Functions to operate on bounding boxes.

.. doxygenfunction:: clip
                     
.. doxygenfunction:: closestPointInBox
                     
.. doxygenfunction:: template< class S, class T > Box< Vec3< S > > transform(const Box< Vec3< S > > &box, const Matrix44< T > &m) noexcept

.. doxygenfunction:: template< class S, class T > void affineTransform(const Box< Vec3< S > > &box, const Matrix44< T > &m, Box< Vec3< S > > &result) noexcept

.. doxygenfunction:: findEntryAndExitPoints

.. doxygenfunction:: template< class T > constexpr bool intersects(const Box< Vec3< T > > &box, const Line3< T > &ray) noexcept


   
                     

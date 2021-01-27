Color
#####

Functions that operate on colors.

.. doxygenfunction:: hsv2rgb

.. doxygenfunction:: template<class T> constexpr Color4<T> hsv2rgb(const Color4<T> &hsv) noexcept

.. doxygenfunction:: template<class T> constexpr Vec3<T> hsv2rgb(const Vec3<T> &hsv) noexcept
                     
.. doxygenfunction:: template<class T> constexpr Color4<T> rgb2hsv(const Color4<T> &rgb) noexcept
                     
.. doxygenfunction:: template<class T> constexpr Vec3<T> rgb2hsv(const Vec3<T> &rgb) noexcept

.. doxygenfunction:: template<class T> constexpr PackedColor rgb2packed(const Color4<T> &c) noexcept

.. doxygenfunction:: template<class T> constexpr PackedColor rgb2packed(const Vec3<T> &c) noexcept

.. doxygenfunction:: template<class T> void packed2rgb(PackedColor packed, Color4<T> &out) noexcept

.. doxygenfunction:: template<class T> void packed2rgb(PackedColor packed, Vec3<T> &out) noexcept

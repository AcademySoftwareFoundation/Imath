//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

//
// 2D, 3D and 4D point/vector class templates
//

#ifndef INCLUDED_IMATHVEC_H
#define INCLUDED_IMATHVEC_H

#include "ImathExport.h"
#include "ImathNamespace.h"

#include "ImathBaseTypes.h"
#include "ImathTypeTraits.h"

#include "ImathMath.h"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <initializer_list>

#if (defined _WIN32 || defined _WIN64) && defined _MSC_VER
// suppress exception specification warnings
#    pragma warning(push)
#    pragma warning(disable : 4290)
#endif

IMATH_INTERNAL_NAMESPACE_HEADER_ENTER

template <typename T, typename Storage> class Vec2;
template <typename T, typename Storage> class Vec3;
template <typename T, typename Storage> class Vec4;

/// Enum for the Vec4 to Vec3 conversion constructor
enum IMATH_EXPORT_ENUM InfException
{
    INF_EXCEPTION
};

///
/// 2-element vector
///

namespace detail
{
template <typename T> struct Vec2StorageAdapter
{
    struct generated_store
    {
        union
        {
            struct
            {
                T x, y;
            };
            T arr[2];
        };
    };
    using type = generated_store;
};
template <> struct Vec2StorageAdapter<uint8_t>
{
    using type = imath_v2b_t;
};
template <> struct Vec2StorageAdapter<int16_t>
{
    using type = imath_v2s_t;
};
template <> struct Vec2StorageAdapter<int32_t>
{
    using type = imath_v2i_t;
};
template <> struct Vec2StorageAdapter<int64_t>
{
    using type = imath_v2i64_t;
};
template <> struct Vec2StorageAdapter<float>
{
    using type = imath_v2f_t;
};
template <> struct Vec2StorageAdapter<double>
{
    using type = imath_v2d_t;
};
} // namespace detail

template <
    class T,
    typename Storage = typename detail::Vec2StorageAdapter<T>::type>
class IMATH_EXPORT_TEMPLATE_TYPE Vec2 : public Storage
{
public:
    /// @{
    /// @name Direct access to elements

    using Storage::x;
    using Storage::y;

    /// @}

    /// Element access by index.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 T& operator[] (int i) IMATH_NOEXCEPT;

    /// Element access by index.
    IMATH_HOSTDEVICE constexpr const T& operator[] (int i) const IMATH_NOEXCEPT;

    /// @{
    ///	@name Constructors and Assignment

    /// Uninitialized by default
    IMATH_HOSTDEVICE Vec2 () IMATH_NOEXCEPT;

    /// Initialize to a scalar `(a,a)`
    IMATH_HOSTDEVICE constexpr explicit Vec2 (T a) IMATH_NOEXCEPT;

    /// Initialize to given elements `(a,b)`
    IMATH_HOSTDEVICE constexpr Vec2 (T a, T b) IMATH_NOEXCEPT;

    /// Copy constructor
    IMATH_HOSTDEVICE constexpr Vec2 (const Vec2& v) IMATH_NOEXCEPT;

    /// Move constructor
    IMATH_HOSTDEVICE constexpr Vec2 (Vec2&& v) IMATH_NOEXCEPT;

    /// public api constructor
    IMATH_HOSTDEVICE constexpr Vec2 (const Storage& v) IMATH_NOEXCEPT;

    /// bare C array constructor
    IMATH_HOSTDEVICE constexpr Vec2 (const T v[2]) IMATH_NOEXCEPT;

    /// initializer_list constructor
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Vec2 (std::initializer_list<T> v)
        IMATH_NOEXCEPT;

    /// Construct from Vec2 of another base type
    template <typename S, typename SStorage>
    IMATH_HOSTDEVICE constexpr Vec2 (const Vec2<S, SStorage>& v) IMATH_NOEXCEPT;

    /// Assignment
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Vec2&
    operator= (const Vec2& v) IMATH_NOEXCEPT;

    /// Assignment
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Vec2&
    operator= (Vec2&& v) IMATH_NOEXCEPT;

    /// public api assignment
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Vec2&
    operator= (const Storage& v) IMATH_NOEXCEPT;

    /// initializer_list assignment
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Vec2&
    operator= (std::initializer_list<T> v) IMATH_NOEXCEPT;

    /// Destructor
    ~Vec2 () IMATH_NOEXCEPT = default;

    /// @}

#if IMATH_FOREIGN_VECTOR_INTEROP
    /// @{
    /// @name Interoperability with other vector types
    ///
    /// Construction and assignment are allowed from other classes that
    /// appear to be equivalent vector types, provided that they have either
    /// a subscripting operator, or data members .x and .y, that are of the
    /// same type as the elements of this vector, and their size appears to
    /// be the right number of elements.
    ///
    /// This functionality is disabled for gcc 4.x, which seems to have a
    /// compiler bug that results in spurious errors. It can also be
    /// disabled by defining IMATH_FOREIGN_VECTOR_INTEROP to be 0 prior to
    /// including any Imath header files.
    ///

    /// Initialize to a scalar `(a,a)` with type coersion
    template <typename S, IMATH_ENABLE_IF (std::is_arithmetic<S>::value)>
    IMATH_HOSTDEVICE explicit constexpr Vec2 (S a) IMATH_NOEXCEPT
        : Vec2 (T (a), T (a))
    {}

    template <typename V, IMATH_ENABLE_IF (has_xy<V, T>::value)>
    IMATH_HOSTDEVICE explicit constexpr Vec2 (const V& v) IMATH_NOEXCEPT
        : Vec2 (T (v.x), T (v.y))
    {}

    template <
        typename V,
        IMATH_ENABLE_IF (has_subscript<V, T, 2>::value && !has_xy<V, T>::value)>
    IMATH_HOSTDEVICE explicit Vec2 (const V& v) : Vec2 (T (v[0]), T (v[1]))
    {}

    template <typename V, IMATH_ENABLE_IF (has_xy<V, T>::value)>
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec2&
    operator= (const V& v) IMATH_NOEXCEPT
    {
        x = T (v.x);
        y = T (v.y);
        return *this;
    }

    template <
        typename V,
        IMATH_ENABLE_IF (has_subscript<V, T, 2>::value && !has_xy<V, T>::value)>
    IMATH_HOSTDEVICE const Vec2& operator= (const V& v)
    {
        x = T (v[0]);
        y = T (v[1]);
        return *this;
    }
#endif

    /// @{
    /// @name Compatibility with Sb

    /// Set the value
    template <class S> IMATH_HOSTDEVICE void setValue (S a, S b) IMATH_NOEXCEPT;

    /// Set the value
    IMATH_HOSTDEVICE void setValue (const Storage& v) IMATH_NOEXCEPT;

    /// Set the value
    template <typename S, typename SStorage>
    IMATH_HOSTDEVICE void setValue (const Vec2<S, SStorage>& v) IMATH_NOEXCEPT;

    /// Return the value in `a` and `b`
    template <class S>
    IMATH_HOSTDEVICE void getValue (S& a, S& b) const IMATH_NOEXCEPT;

    /// Set the value
    IMATH_HOSTDEVICE void getValue (Storage& v) const IMATH_NOEXCEPT;

    /// Return the value in `v`
    template <typename S, typename SStorage>
    IMATH_HOSTDEVICE void getValue (Vec2<S, SStorage>& v) const IMATH_NOEXCEPT;

    /// Return a raw pointer to the array of values
    IMATH_HOSTDEVICE T* getValue () IMATH_NOEXCEPT;

    /// Return a raw pointer to the array of values
    IMATH_HOSTDEVICE const T* getValue () const IMATH_NOEXCEPT;

    /// @}

    /// @{
    /// @name Arithmetic and Comparison

    /// Equality
    template <typename S, typename SStorage>
    IMATH_HOSTDEVICE constexpr bool
    operator== (const Vec2<S, SStorage>& v) const IMATH_NOEXCEPT;

    /// Inequality
    template <typename S, typename SStorage>
    IMATH_HOSTDEVICE constexpr bool
    operator!= (const Vec2<S, SStorage>& v) const IMATH_NOEXCEPT;

    /// Compare two matrices and test if they are "approximately equal":
    /// @return True if the coefficients of this and `m` are the same
    /// with an absolute error of no more than e, i.e., for all i, j:
    ///
    ///     abs (this[i][j] - m[i][j]) <= e
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool
    equalWithAbsError (const Vec2& v, T e) const IMATH_NOEXCEPT;

    /// Compare two matrices and test if they are "approximately equal":
    /// @return True if the coefficients of this and m are the same with
    /// a relative error of no more than e, i.e., for all i, j:
    ///
    ///     abs (this[i] - v[i][j]) <= e * abs (this[i][j])
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool
    equalWithRelError (const Vec2& v, T e) const IMATH_NOEXCEPT;

    /// Dot product
    IMATH_HOSTDEVICE constexpr T dot (const Vec2& v) const IMATH_NOEXCEPT;

    /// Dot product
    IMATH_HOSTDEVICE constexpr T operator^ (const Vec2& v) const IMATH_NOEXCEPT;

    /// Right-handed cross product, i.e. z component of
    /// Vec3 (this->x, this->y, 0) % Vec3 (v.x, v.y, 0)
    IMATH_HOSTDEVICE constexpr T cross (const Vec2& v) const IMATH_NOEXCEPT;

    /// Right-handed cross product, i.e. z component of
    /// Vec3 (this->x, this->y, 0) % Vec3 (v.x, v.y, 0)
    IMATH_HOSTDEVICE constexpr T operator% (const Vec2& v) const IMATH_NOEXCEPT;

    /// Component-wise addition
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec2&
    operator+= (const Vec2& v) IMATH_NOEXCEPT;

    /// Component-wise addition
    IMATH_HOSTDEVICE constexpr Vec2
    operator+ (const Vec2& v) const IMATH_NOEXCEPT;

    /// Component-wise subtraction
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec2&
    operator-= (const Vec2& v) IMATH_NOEXCEPT;

    /// Component-wise subtraction
    IMATH_HOSTDEVICE constexpr Vec2
    operator- (const Vec2& v) const IMATH_NOEXCEPT;

    /// Component-wise multiplication by -1
    IMATH_HOSTDEVICE constexpr Vec2 operator- () const IMATH_NOEXCEPT;

    /// Component-wise multiplication by -1
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec2& negate () IMATH_NOEXCEPT;

    /// Component-wise multiplication
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec2&
    operator*= (const Vec2& v) IMATH_NOEXCEPT;

    /// Component-wise multiplication
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec2&
    operator*= (T a) IMATH_NOEXCEPT;

    /// Component-wise multiplication
    IMATH_HOSTDEVICE constexpr Vec2
    operator* (const Vec2& v) const IMATH_NOEXCEPT;

    /// Component-wise multiplication
    IMATH_HOSTDEVICE constexpr Vec2 operator* (T a) const IMATH_NOEXCEPT;

    /// Component-wise division
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec2&
    operator/= (const Vec2& v) IMATH_NOEXCEPT;

    /// Component-wise division
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec2&
    operator/= (T a) IMATH_NOEXCEPT;

    /// Component-wise division
    IMATH_HOSTDEVICE constexpr Vec2
    operator/ (const Vec2& v) const IMATH_NOEXCEPT;

    /// Component-wise division
    IMATH_HOSTDEVICE constexpr Vec2 operator/ (T a) const IMATH_NOEXCEPT;

    /// @}

    /// @{
    /// @name Query and Manipulation

    /// Return the Euclidean norm
    IMATH_HOSTDEVICE T length () const IMATH_NOEXCEPT;

    /// Return the square of the Euclidean norm, i.e. the dot product
    /// with itself.
    IMATH_HOSTDEVICE constexpr T length2 () const IMATH_NOEXCEPT;

    /// Normalize in place. If length()==0, return a null vector.
    IMATH_HOSTDEVICE const Vec2& normalize () IMATH_NOEXCEPT;

    /// Normalize in place. If length()==0, throw an exception.
    const Vec2& normalizeExc ();

    /// Normalize without any checks for length()==0. Slightly faster
    /// than the other normalization routines, but if v.length() is
    /// 0.0, the result is undefined.
    IMATH_HOSTDEVICE const Vec2& normalizeNonNull () IMATH_NOEXCEPT;

    /// Return a normalized vector. Does not modify *this.
    IMATH_HOSTDEVICE Vec2 normalized () const IMATH_NOEXCEPT;

    /// Return a normalized vector. Does not modify *this. Throw an
    /// exception if length()==0.
    Vec2 normalizedExc () const;

    /// Return a normalized vector. Does not modify *this, and does
    /// not check for length()==0. Slightly faster than the other
    /// normalization routines, but if v.length() is 0.0, the result
    /// is undefined.
    IMATH_HOSTDEVICE Vec2 normalizedNonNull () const IMATH_NOEXCEPT;

    /// @}

    /// @{
    /// @name Numeric Limits

    /// Largest possible negative value
    IMATH_HOSTDEVICE constexpr static T baseTypeLowest () IMATH_NOEXCEPT
    {
        return std::numeric_limits<T>::lowest ();
    }

    /// Largest possible positive value
    IMATH_HOSTDEVICE constexpr static T baseTypeMax () IMATH_NOEXCEPT
    {
        return std::numeric_limits<T>::max ();
    }

    /// Smallest possible positive value
    IMATH_HOSTDEVICE constexpr static T baseTypeSmallest () IMATH_NOEXCEPT
    {
        return std::numeric_limits<T>::min ();
    }

    /// Smallest possible e for which 1+e != 1
    IMATH_HOSTDEVICE constexpr static T baseTypeEpsilon () IMATH_NOEXCEPT
    {
        return std::numeric_limits<T>::epsilon ();
    }

    /// @}

    /// Return the number of dimensions, i.e. 2
    IMATH_HOSTDEVICE constexpr static unsigned int dimensions () IMATH_NOEXCEPT
    {
        return 2;
    }

    /// The base type: In templates that accept a parameter `V`, you
    /// can refer to `T` as `V::BaseType`
    using BaseType = T;
    /// The base storage: In templates that accept a parameter `V`, you
    /// can refer to the storage for a `T` as `V::BaseStorage`
    using BaseStorage = Storage;

private:
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 T lengthTiny () const IMATH_NOEXCEPT;
};

///
/// 3-element vector
///

namespace detail
{
template <typename T> struct Vec3StorageAdapter
{
    struct generated_store
    {
        union
        {
            struct
            {
                T x, y, z;
            };
            T arr[3];
        };
    };
    using type = generated_store;
};
template <> struct Vec3StorageAdapter<uint8_t>
{
    using type = imath_v3b_t;
};
template <> struct Vec3StorageAdapter<int16_t>
{
    using type = imath_v3s_t;
};
template <> struct Vec3StorageAdapter<int32_t>
{
    using type = imath_v3i_t;
};
template <> struct Vec3StorageAdapter<int64_t>
{
    using type = imath_v3i64_t;
};
template <> struct Vec3StorageAdapter<float>
{
    using type = imath_v3f_t;
};
template <> struct Vec3StorageAdapter<double>
{
    using type = imath_v3d_t;
};
} // namespace detail

template <
    class T,
    typename Storage = typename detail::Vec3StorageAdapter<T>::type>
class IMATH_EXPORT_TEMPLATE_TYPE Vec3 : public Storage
{
public:
    /// @{
    /// @name Direct access to elements

    using Storage::x;
    using Storage::y;
    using Storage::z;

    /// @}

    /// Element access by index.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 T& operator[] (int i) IMATH_NOEXCEPT;

    /// Element access by index.
    IMATH_HOSTDEVICE constexpr const T& operator[] (int i) const IMATH_NOEXCEPT;

    /// @{
    ///	@name Constructors and Assignment

    /// Uninitialized by default
    IMATH_HOSTDEVICE Vec3 () IMATH_NOEXCEPT;

    /// Initialize to a scalar `(a,a,a)`
    IMATH_HOSTDEVICE constexpr explicit Vec3 (T a) IMATH_NOEXCEPT;

    /// Initialize to given elements `(a,b,c)`
    IMATH_HOSTDEVICE constexpr Vec3 (T a, T b, T c) IMATH_NOEXCEPT;

    /// Copy constructor
    IMATH_HOSTDEVICE constexpr Vec3 (const Vec3& v) IMATH_NOEXCEPT;

    /// Move constructor
    IMATH_HOSTDEVICE constexpr Vec3 (Vec3&& v) IMATH_NOEXCEPT;

    /// public api constructor
    IMATH_HOSTDEVICE constexpr Vec3 (const Storage& v) IMATH_NOEXCEPT;

    /// bare C array constructor
    IMATH_HOSTDEVICE constexpr Vec3 (const T v[3]) IMATH_NOEXCEPT;

    /// initializer_list constructor
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Vec3 (std::initializer_list<T> v)
        IMATH_NOEXCEPT;

    /// Construct from Vec3 of another base type
    template <typename S, typename SStorage>
    IMATH_HOSTDEVICE constexpr Vec3 (const Vec3<S, SStorage>& v) IMATH_NOEXCEPT;

    /// Vec4 to Vec3 conversion: divide x, y and z by w, even if w is
    /// 0.  The result depends on how the environment handles
    /// floating-point exceptions.
    template <typename S, typename SStorage>
    IMATH_HOSTDEVICE explicit constexpr Vec3 (const Vec4<S, SStorage>& v)
        IMATH_NOEXCEPT;

    /// Vec4 to Vec3 conversion: divide x, y and z by w.  Throws an
    /// exception if w is zero or if division by w would overflow.
    template <typename S, typename SStorage>
    explicit IMATH_HOSTDEVICE IMATH_CONSTEXPR14
    Vec3 (const Vec4<S, SStorage>& v, InfException);

    /// Assignment
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Vec3&
    operator= (const Vec3& v) IMATH_NOEXCEPT;

    /// Move Assignment
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Vec3&
    operator= (Vec3&& v) IMATH_NOEXCEPT;

    /// public api assignment
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Vec3&
    operator= (const Storage& v) IMATH_NOEXCEPT;

    /// initializer_list assignment
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Vec3&
    operator= (std::initializer_list<T> v) IMATH_NOEXCEPT;

    /// Destructor
    ~Vec3 () IMATH_NOEXCEPT = default;

    /// @}

#if IMATH_FOREIGN_VECTOR_INTEROP
    /// @{
    /// @name Interoperability with other vector types
    ///
    /// Construction and assignment are allowed from other classes that
    /// appear to be equivalent vector types, provided that they have either
    /// a subscripting operator, or data members .x, .y, .z, that are of the
    /// same type as the elements of this vector, and their size appears to
    /// be the right number of elements.
    ///
    /// This functionality is disabled for gcc 4.x, which seems to have a
    /// compiler bug that results in spurious errors. It can also be
    /// disabled by defining IMATH_FOREIGN_VECTOR_INTEROP to be 0 prior to
    /// including any Imath header files.
    ///

    /// Initialize to a scalar `(a,a)` with type coersion
    template <typename S, IMATH_ENABLE_IF (std::is_arithmetic<S>::value)>
    IMATH_HOSTDEVICE explicit constexpr Vec3 (S a) IMATH_NOEXCEPT
        : Vec3 (T (a), T (a), T (a))
    {}

    template <typename V, IMATH_ENABLE_IF (has_xyz<V, T>::value)>
    IMATH_HOSTDEVICE explicit constexpr Vec3 (const V& v) IMATH_NOEXCEPT
        : Vec3 (T (v.x), T (v.y), T (v.z))
    {}

    template <
        typename V,
        IMATH_ENABLE_IF (
            has_subscript<V, T, 3>::value && !has_xyz<V, T>::value)>
    IMATH_HOSTDEVICE explicit Vec3 (const V& v)
        : Vec3 (T (v[0]), T (v[1]), T (v[2]))
    {}

    /// Interoperability assignment from another type that behaves as if it
    /// were an equivalent vector.
    template <typename V, IMATH_ENABLE_IF (has_xyz<V, T>::value)>
    IMATH_HOSTDEVICE const Vec3& operator= (const V& v) IMATH_NOEXCEPT
    {
        x = T (v.x);
        y = T (v.y);
        z = T (v.z);
        return *this;
    }

    template <
        typename V,
        IMATH_ENABLE_IF (
            has_subscript<V, T, 3>::value && !has_xyz<V, T>::value)>
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec3& operator= (const V& v)
    {
        x = T (v[0]);
        y = T (v[1]);
        z = T (v[2]);
        return *this;
    }
    /// @}
#endif

    /// @{
    /// @name Compatibility with Sb

    /// Set the value
    template <class S>
    IMATH_HOSTDEVICE void setValue (S a, S b, S c) IMATH_NOEXCEPT;

    /// Set the value
    IMATH_HOSTDEVICE void setValue (const Storage& v) IMATH_NOEXCEPT;

    /// Set the value
    template <typename S, typename SStorage>
    IMATH_HOSTDEVICE void setValue (const Vec3<S, SStorage>& v) IMATH_NOEXCEPT;

    /// Return the value in `a`, `b`, and `c`
    template <class S>
    IMATH_HOSTDEVICE void getValue (S& a, S& b, S& c) const IMATH_NOEXCEPT;

    /// Set the value
    IMATH_HOSTDEVICE void getValue (Storage& v) const IMATH_NOEXCEPT;

    /// Return the value in `v`
    template <typename S, typename SStorage>
    IMATH_HOSTDEVICE void getValue (Vec3<S, SStorage>& v) const IMATH_NOEXCEPT;

    /// Return a raw pointer to the array of values
    IMATH_HOSTDEVICE T* getValue () IMATH_NOEXCEPT;

    /// Return a raw pointer to the array of values
    IMATH_HOSTDEVICE const T* getValue () const IMATH_NOEXCEPT;

    /// @}

    /// @{
    /// @name Arithmetic and Comparison

    /// Equality
    template <typename S, typename SStorage>
    IMATH_HOSTDEVICE constexpr bool
    operator== (const Vec3<S, SStorage>& v) const IMATH_NOEXCEPT;

    /// Inequality
    template <typename S, typename SStorage>
    IMATH_HOSTDEVICE constexpr bool
    operator!= (const Vec3<S, SStorage>& v) const IMATH_NOEXCEPT;

    /// Compare two matrices and test if they are "approximately equal":
    /// @return True if the coefficients of this and `m` are the same
    /// with an absolute error of no more than e, i.e., for all i, j:
    ///
    ///     abs (this[i][j] - m[i][j]) <= e
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool
    equalWithAbsError (const Vec3& v, T e) const IMATH_NOEXCEPT;

    /// Compare two matrices and test if they are "approximately equal":
    /// @return True if the coefficients of this and m are the same with
    /// a relative error of no more than e, i.e., for all i, j:
    ///
    ///     abs (this[i] - v[i][j]) <= e * abs (this[i][j])
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool
    equalWithRelError (const Vec3& v, T e) const IMATH_NOEXCEPT;

    /// Dot product
    IMATH_HOSTDEVICE constexpr T dot (const Vec3& v) const IMATH_NOEXCEPT;

    /// Dot product
    IMATH_HOSTDEVICE constexpr T operator^ (const Vec3& v) const IMATH_NOEXCEPT;

    /// Right-handed cross product
    IMATH_HOSTDEVICE constexpr Vec3 cross (const Vec3& v) const IMATH_NOEXCEPT;

    /// Right-handed cross product
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec3&
    operator%= (const Vec3& v) IMATH_NOEXCEPT;

    /// Right-handed cross product
    IMATH_HOSTDEVICE constexpr Vec3
    operator% (const Vec3& v) const IMATH_NOEXCEPT;

    /// Component-wise addition
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec3&
    operator+= (const Vec3& v) IMATH_NOEXCEPT;

    /// Component-wise addition
    IMATH_HOSTDEVICE constexpr Vec3
    operator+ (const Vec3& v) const IMATH_NOEXCEPT;

    /// Component-wise subtraction
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec3&
    operator-= (const Vec3& v) IMATH_NOEXCEPT;

    /// Component-wise subtraction
    IMATH_HOSTDEVICE constexpr Vec3
    operator- (const Vec3& v) const IMATH_NOEXCEPT;

    /// Component-wise multiplication by -1
    IMATH_HOSTDEVICE constexpr Vec3 operator- () const IMATH_NOEXCEPT;

    /// Component-wise multiplication by -1
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec3& negate () IMATH_NOEXCEPT;

    /// Component-wise multiplication
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec3&
    operator*= (const Vec3& v) IMATH_NOEXCEPT;

    /// Component-wise multiplication
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec3&
    operator*= (T a) IMATH_NOEXCEPT;

    /// Component-wise multiplication
    IMATH_HOSTDEVICE constexpr Vec3
    operator* (const Vec3& v) const IMATH_NOEXCEPT;

    /// Component-wise multiplication
    IMATH_HOSTDEVICE constexpr Vec3 operator* (T a) const IMATH_NOEXCEPT;

    /// Component-wise division
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec3&
    operator/= (const Vec3& v) IMATH_NOEXCEPT;

    /// Component-wise division
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec3&
    operator/= (T a) IMATH_NOEXCEPT;

    /// Component-wise division
    IMATH_HOSTDEVICE constexpr Vec3
    operator/ (const Vec3& v) const IMATH_NOEXCEPT;

    /// Component-wise division
    IMATH_HOSTDEVICE constexpr Vec3 operator/ (T a) const IMATH_NOEXCEPT;

    /// @}

    /// @{
    /// @name Query and Manipulation

    /// Return the Euclidean norm
    IMATH_HOSTDEVICE T length () const IMATH_NOEXCEPT;

    /// Return the square of the Euclidean norm, i.e. the dot product
    /// with itself.
    IMATH_HOSTDEVICE constexpr T length2 () const IMATH_NOEXCEPT;

    /// Normalize in place. If length()==0, return a null vector.
    IMATH_HOSTDEVICE const Vec3& normalize () IMATH_NOEXCEPT;

    /// Normalize in place. If length()==0, throw an exception.
    const Vec3& normalizeExc ();

    /// Normalize without any checks for length()==0. Slightly faster
    /// than the other normalization routines, but if v.length() is
    /// 0.0, the result is undefined.
    IMATH_HOSTDEVICE const Vec3& normalizeNonNull () IMATH_NOEXCEPT;

    /// Return a normalized vector. Does not modify *this.
    IMATH_HOSTDEVICE Vec3
    normalized () const IMATH_NOEXCEPT; // does not modify *this

    /// Return a normalized vector. Does not modify *this. Throw an
    /// exception if length()==0.
    Vec3 normalizedExc () const;

    /// Return a normalized vector. Does not modify *this, and does
    /// not check for length()==0. Slightly faster than the other
    /// normalization routines, but if v.length() is 0.0, the result
    /// is undefined.
    IMATH_HOSTDEVICE Vec3 normalizedNonNull () const IMATH_NOEXCEPT;

    /// @}

    /// @{
    /// @name Numeric Limits

    /// Largest possible negative value
    IMATH_HOSTDEVICE constexpr static T baseTypeLowest () IMATH_NOEXCEPT
    {
        return std::numeric_limits<T>::lowest ();
    }

    /// Largest possible positive value
    IMATH_HOSTDEVICE constexpr static T baseTypeMax () IMATH_NOEXCEPT
    {
        return std::numeric_limits<T>::max ();
    }

    /// Smallest possible positive value
    IMATH_HOSTDEVICE constexpr static T baseTypeSmallest () IMATH_NOEXCEPT
    {
        return std::numeric_limits<T>::min ();
    }

    /// Smallest possible e for which 1+e != 1
    IMATH_HOSTDEVICE constexpr static T baseTypeEpsilon () IMATH_NOEXCEPT
    {
        return std::numeric_limits<T>::epsilon ();
    }

    /// @}

    /// Return the number of dimensions, i.e. 3
    IMATH_HOSTDEVICE constexpr static unsigned int dimensions () IMATH_NOEXCEPT
    {
        return 3;
    }

    /// The base type: In templates that accept a parameter `V`, you
    /// can refer to `T` as `V::BaseType`
    using BaseType = T;
    /// The base storage: In templates that accept a parameter `V`, you
    /// can refer to the storage for a `T` as `V::BaseStorage`
    using BaseStorage = Storage;

private:
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 T lengthTiny () const IMATH_NOEXCEPT;
};

///
/// 4-element vector
///

namespace detail
{
template <typename T> struct Vec4StorageAdapter
{
    struct generated_store
    {
        union
        {
            struct
            {
                T x, y, z, w;
            };
            T arr[4];
        };
    };
    using type = generated_store;
};
template <> struct Vec4StorageAdapter<uint8_t>
{
    using type = imath_v4b_t;
};
template <> struct Vec4StorageAdapter<int16_t>
{
    using type = imath_v4s_t;
};
template <> struct Vec4StorageAdapter<int32_t>
{
    using type = imath_v4i_t;
};
template <> struct Vec4StorageAdapter<int64_t>
{
    using type = imath_v4i64_t;
};
template <> struct Vec4StorageAdapter<float>
{
    using type = imath_v4f_t;
};
template <> struct Vec4StorageAdapter<double>
{
    using type = imath_v4d_t;
};
} // namespace detail

template <
    class T,
    typename Storage = typename detail::Vec4StorageAdapter<T>::type>
class IMATH_EXPORT_TEMPLATE_TYPE Vec4 : public Storage
{
public:
    /// @{
    /// @name Direct access to elements

    // clang-format off
    using Storage::x;
    using Storage::y;
    using Storage::z;
    using Storage::w;
    // clang-format on

    /// @}

    /// Element access by index.
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 T& operator[] (int i) IMATH_NOEXCEPT;

    /// Element access by index.
    IMATH_HOSTDEVICE constexpr const T& operator[] (int i) const IMATH_NOEXCEPT;

    /// @{
    ///	@name Constructors and Assignment

    /// Uninitialized by default
    IMATH_HOSTDEVICE Vec4 () IMATH_NOEXCEPT; // no initialization

    /// Initialize to a scalar `(a,a,a,a)`
    IMATH_HOSTDEVICE constexpr explicit Vec4 (T a) IMATH_NOEXCEPT;

    /// Initialize to given elements `(a,b,c,d)`
    IMATH_HOSTDEVICE constexpr Vec4 (T a, T b, T c, T d) IMATH_NOEXCEPT;

    /// Copy constructor
    IMATH_HOSTDEVICE constexpr Vec4 (const Vec4& v) IMATH_NOEXCEPT;

    /// Move constructor
    IMATH_HOSTDEVICE constexpr Vec4 (Vec4&& v) IMATH_NOEXCEPT;

    /// public api constructor
    IMATH_HOSTDEVICE constexpr Vec4 (const Storage& v) IMATH_NOEXCEPT;

    /// bare C array constructor
    IMATH_HOSTDEVICE constexpr Vec4 (const T v[4]) IMATH_NOEXCEPT;

    /// initializer_list constructor
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Vec4 (std::initializer_list<T> v)
        IMATH_NOEXCEPT;

    /// Construct from Vec4 of another base type
    template <typename S, typename SStorage>
    IMATH_HOSTDEVICE constexpr Vec4 (const Vec4<S, SStorage>& v) IMATH_NOEXCEPT;

    /// Vec3 to Vec4 conversion, sets w to 1.
    template <typename S, typename SStorage>
    IMATH_HOSTDEVICE explicit constexpr Vec4 (const Vec3<S, SStorage>& v)
        IMATH_NOEXCEPT;

    /// Assignment
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Vec4&
    operator= (const Vec4& v) IMATH_NOEXCEPT;

    /// Move Assignment
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Vec4&
    operator= (Vec4&& v) IMATH_NOEXCEPT;

    /// public api assignment
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Vec4&
    operator= (const Storage& v) IMATH_NOEXCEPT;

    /// initializer_list assignment
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Vec4&
    operator= (std::initializer_list<T> v) IMATH_NOEXCEPT;

    /// Destructor
    ~Vec4 () IMATH_NOEXCEPT = default;

    /// @}

#if IMATH_FOREIGN_VECTOR_INTEROP
    /// @{
    /// @name Interoperability with other vector types
    ///
    /// Construction and assignment are allowed from other classes that
    /// appear to be equivalent vector types, provided that they have either
    /// a subscripting operator, or data members .x, .y, .z, .w that are of
    /// the same type as the elements of this vector, and their size appears
    /// to be the right number of elements.
    ///
    /// This functionality is disabled for gcc 4.x, which seems to have a
    /// compiler bug that results in spurious errors. It can also be
    /// disabled by defining IMATH_FOREIGN_VECTOR_INTEROP to be 0 prior to
    /// including any Imath header files.
    ///

    /// Initialize to a scalar `(a,a)` with type coersion
    template <typename S, IMATH_ENABLE_IF (std::is_arithmetic<S>::value)>
    IMATH_HOSTDEVICE explicit constexpr Vec4 (S a) IMATH_NOEXCEPT
        : Vec4 (T (a), T (a), T (a), T (a))
    {}

    template <typename V, IMATH_ENABLE_IF (has_xyzw<V, T>::value)>
    IMATH_HOSTDEVICE explicit constexpr Vec4 (const V& v) IMATH_NOEXCEPT
        : Vec4 (T (v.x), T (v.y), T (v.z), T (v.w))
    {}

    template <
        typename V,
        IMATH_ENABLE_IF (
            has_subscript<V, T, 4>::value && !has_xyzw<V, T>::value)>
    IMATH_HOSTDEVICE explicit Vec4 (const V& v)
        : Vec4 (T (v[0]), T (v[1]), T (v[2]), T (v[3]))
    {}

    template <typename V, IMATH_ENABLE_IF (has_xyzw<V, T>::value)>
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec4&
    operator= (const V& v) IMATH_NOEXCEPT
    {
        x = T (v.x);
        y = T (v.y);
        z = T (v.z);
        w = T (v.w);
        return *this;
    }

    template <
        typename V,
        IMATH_ENABLE_IF (
            has_subscript<V, T, 4>::value && !has_xyzw<V, T>::value)>
    IMATH_HOSTDEVICE const Vec4& operator= (const V& v)
    {
        x = T (v[0]);
        y = T (v[1]);
        z = T (v[2]);
        w = T (v[3]);
        return *this;
    }
    /// @}
#endif

    /// @{
    /// @name Arithmetic and Comparison

    /// Equality
    template <typename S, typename SStorage>
    IMATH_HOSTDEVICE constexpr bool
    operator== (const Vec4<S, SStorage>& v) const IMATH_NOEXCEPT;

    /// Inequality
    template <typename S, typename SStorage>
    IMATH_HOSTDEVICE constexpr bool
    operator!= (const Vec4<S, SStorage>& v) const IMATH_NOEXCEPT;

    /// Compare two matrices and test if they are "approximately equal":
    /// @return True if the coefficients of this and `m` are the same
    /// with an absolute error of no more than e, i.e., for all i, j:
    ///
    ///     abs (this[i][j] - m[i][j]) <= e
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool
    equalWithAbsError (const Vec4& v, T e) const IMATH_NOEXCEPT;

    /// Compare two matrices and test if they are "approximately equal":
    /// @return True if the coefficients of this and m are the same with
    /// a relative error of no more than e, i.e., for all i, j:
    ///
    ///     abs (this[i] - v[i][j]) <= e * abs (this[i][j])
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool
    equalWithRelError (const Vec4& v, T e) const IMATH_NOEXCEPT;

    /// Dot product
    IMATH_HOSTDEVICE constexpr T dot (const Vec4& v) const IMATH_NOEXCEPT;

    /// Dot product
    IMATH_HOSTDEVICE constexpr T operator^ (const Vec4& v) const IMATH_NOEXCEPT;

    /// Component-wise addition
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec4&
    operator+= (const Vec4& v) IMATH_NOEXCEPT;

    /// Component-wise addition
    IMATH_HOSTDEVICE constexpr Vec4
    operator+ (const Vec4& v) const IMATH_NOEXCEPT;

    /// Component-wise subtraction
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec4&
    operator-= (const Vec4& v) IMATH_NOEXCEPT;

    /// Component-wise subtraction
    IMATH_HOSTDEVICE constexpr Vec4
    operator- (const Vec4& v) const IMATH_NOEXCEPT;

    /// Component-wise multiplication by -1
    IMATH_HOSTDEVICE constexpr Vec4 operator- () const IMATH_NOEXCEPT;

    /// Component-wise multiplication by -1
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec4& negate () IMATH_NOEXCEPT;

    /// Component-wise multiplication
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec4&
    operator*= (const Vec4& v) IMATH_NOEXCEPT;

    /// Component-wise multiplication
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec4&
    operator*= (T a) IMATH_NOEXCEPT;

    /// Component-wise multiplication
    IMATH_HOSTDEVICE constexpr Vec4
    operator* (const Vec4& v) const IMATH_NOEXCEPT;

    /// Component-wise multiplication
    IMATH_HOSTDEVICE constexpr Vec4 operator* (T a) const IMATH_NOEXCEPT;

    /// Component-wise division
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec4&
    operator/= (const Vec4& v) IMATH_NOEXCEPT;

    /// Component-wise division
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec4&
    operator/= (T a) IMATH_NOEXCEPT;

    /// Component-wise division
    IMATH_HOSTDEVICE constexpr Vec4
    operator/ (const Vec4& v) const IMATH_NOEXCEPT;

    /// Component-wise division
    IMATH_HOSTDEVICE constexpr Vec4 operator/ (T a) const IMATH_NOEXCEPT;

    /// @}

    /// @{
    /// @name Query and Manipulation

    /// Return the Euclidean norm
    IMATH_HOSTDEVICE T length () const IMATH_NOEXCEPT;

    /// Return the square of the Euclidean norm, i.e. the dot product
    /// with itself.
    IMATH_HOSTDEVICE constexpr T length2 () const IMATH_NOEXCEPT;

    /// Normalize in place. If length()==0, return a null vector.
    IMATH_HOSTDEVICE const Vec4& normalize () IMATH_NOEXCEPT; // modifies *this

    /// Normalize in place. If length()==0, throw an exception.
    const Vec4& normalizeExc ();

    /// Normalize without any checks for length()==0. Slightly faster
    /// than the other normalization routines, but if v.length() is
    /// 0.0, the result is undefined.
    IMATH_HOSTDEVICE const Vec4& normalizeNonNull () IMATH_NOEXCEPT;

    /// Return a normalized vector. Does not modify *this.
    IMATH_HOSTDEVICE Vec4
    normalized () const IMATH_NOEXCEPT; // does not modify *this

    /// Return a normalized vector. Does not modify *this. Throw an
    /// exception if length()==0.
    Vec4 normalizedExc () const;

    /// Return a normalized vector. Does not modify *this, and does
    /// not check for length()==0. Slightly faster than the other
    /// normalization routines, but if v.length() is 0.0, the result
    /// is undefined.
    IMATH_HOSTDEVICE Vec4 normalizedNonNull () const IMATH_NOEXCEPT;

    /// @}

    /// @{
    /// @name Numeric Limits

    /// Largest possible negative value
    IMATH_HOSTDEVICE constexpr static T baseTypeLowest () IMATH_NOEXCEPT
    {
        return std::numeric_limits<T>::lowest ();
    }

    /// Largest possible positive value
    IMATH_HOSTDEVICE constexpr static T baseTypeMax () IMATH_NOEXCEPT
    {
        return std::numeric_limits<T>::max ();
    }

    /// Smallest possible positive value
    IMATH_HOSTDEVICE constexpr static T baseTypeSmallest () IMATH_NOEXCEPT
    {
        return std::numeric_limits<T>::min ();
    }

    /// Smallest possible e for which 1+e != 1
    IMATH_HOSTDEVICE constexpr static T baseTypeEpsilon () IMATH_NOEXCEPT
    {
        return std::numeric_limits<T>::epsilon ();
    }

    /// @}

    /// Return the number of dimensions, i.e. 4
    IMATH_HOSTDEVICE constexpr static unsigned int dimensions () IMATH_NOEXCEPT
    {
        return 4;
    }

    /// The base type: In templates that accept a parameter `V`, you
    /// can refer to `T` as `V::BaseType`
    using BaseType = T;
    /// The base storage: In templates that accept a parameter `V`, you
    /// can refer to the storage for a `T` as `V::BaseStorage`
    using BaseStorage = Storage;

private:
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 T lengthTiny () const IMATH_NOEXCEPT;
};

/// Stream output, as "(x y)"
template <typename T, typename Storage>
std::ostream& operator<< (std::ostream& s, const Vec2<T, Storage>& v);

/// Stream output, as "(x y z)"
template <typename T, typename Storage>
std::ostream& operator<< (std::ostream& s, const Vec3<T, Storage>& v);

/// Stream output, as "(x y z w)"
template <typename T, typename Storage>
std::ostream& operator<< (std::ostream& s, const Vec4<T, Storage>& v);

/// Reverse multiplication: S * Vec2<T>
template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr Vec2<T, Storage>
operator* (T a, const Vec2<T, Storage>& v) IMATH_NOEXCEPT;

/// Reverse multiplication: S * Vec3<T>
template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr Vec3<T, Storage>
operator* (T a, const Vec3<T, Storage>& v) IMATH_NOEXCEPT;

/// Reverse multiplication: S * Vec4<T>
template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr Vec4<T, Storage>
operator* (T a, const Vec4<T, Storage>& v) IMATH_NOEXCEPT;

//-------------------------
// Typedefs for convenience
//-------------------------

/// Vec2 of short / int16_t
typedef Vec2<short> V2s;

/// Vec2 of integer
typedef Vec2<int> V2i;

/// Vec2 of int64_t
typedef Vec2<int64_t> V2i64;

/// Vec2 of float
typedef Vec2<float> V2f;

/// Vec2 of double
typedef Vec2<double> V2d;

/// Vec3 of short
typedef Vec3<short> V3s;

/// Vec3 of integer
typedef Vec3<int> V3i;

/// Vec3 of int64_t
typedef Vec3<int64_t> V3i64;

/// Vec3 of float
typedef Vec3<float> V3f;

/// Vec3 of double
typedef Vec3<double> V3d;

/// Vec4 of short
typedef Vec4<short> V4s;

/// Vec4 of integer
typedef Vec4<int> V4i;

/// Vec4 of int64_t
typedef Vec4<int64_t> V4i64;

/// Vec4 of float
typedef Vec4<float> V4f;

/// Vec4 of double
typedef Vec4<double> V4d;

//----------------------------------------------------------------------------
// Specializations for VecN<short>, VecN<int>
//
// Normalize and length don't make sense for integer vectors, so disable them.
//----------------------------------------------------------------------------

/// @cond Doxygen_Suppress

// Vec2<short>
template <>
IMATH_HOSTDEVICE short
Vec2<short, imath_v2s_t>::length () const IMATH_NOEXCEPT = delete;
template <>
IMATH_HOSTDEVICE const Vec2<short, imath_v2s_t>&
Vec2<short, imath_v2s_t>::normalize () IMATH_NOEXCEPT = delete;
template <>
const Vec2<short, imath_v2s_t>&
Vec2<short, imath_v2s_t>::normalizeExc () = delete;
template <>
IMATH_HOSTDEVICE const Vec2<short, imath_v2s_t>&
Vec2<short, imath_v2s_t>::normalizeNonNull () IMATH_NOEXCEPT = delete;
template <>
IMATH_HOSTDEVICE Vec2<short, imath_v2s_t>
Vec2<short, imath_v2s_t>::normalized () const IMATH_NOEXCEPT = delete;
template <>
Vec2<short, imath_v2s_t>
Vec2<short, imath_v2s_t>::normalizedExc () const = delete;
template <>
IMATH_HOSTDEVICE Vec2<short, imath_v2s_t>
Vec2<short, imath_v2s_t>::normalizedNonNull () const IMATH_NOEXCEPT = delete;

// Vec2<int>
template <>
IMATH_HOSTDEVICE int
Vec2<int, imath_v2i_t>::length () const IMATH_NOEXCEPT = delete;
template <>
IMATH_HOSTDEVICE const Vec2<int, imath_v2i_t>&
Vec2<int, imath_v2i_t>::normalize () IMATH_NOEXCEPT = delete;
template <>
const Vec2<int, imath_v2i_t>& Vec2<int, imath_v2i_t>::normalizeExc () = delete;
template <>
IMATH_HOSTDEVICE const Vec2<int, imath_v2i_t>&
Vec2<int, imath_v2i_t>::normalizeNonNull () IMATH_NOEXCEPT = delete;
template <>
IMATH_HOSTDEVICE Vec2<int, imath_v2i_t>
Vec2<int, imath_v2i_t>::normalized () const IMATH_NOEXCEPT = delete;
template <>
Vec2<int, imath_v2i_t> Vec2<int, imath_v2i_t>::normalizedExc () const = delete;
template <>
IMATH_HOSTDEVICE Vec2<int, imath_v2i_t>
Vec2<int, imath_v2i_t>::normalizedNonNull () const IMATH_NOEXCEPT = delete;

// Vec2<int64_t>
template <>
IMATH_HOSTDEVICE int64_t
Vec2<int64_t, imath_v2i64_t>::length () const IMATH_NOEXCEPT = delete;
template <>
IMATH_HOSTDEVICE const Vec2<int64_t, imath_v2i64_t>&
Vec2<int64_t, imath_v2i64_t>::normalize () IMATH_NOEXCEPT = delete;
template <>
const Vec2<int64_t, imath_v2i64_t>&
Vec2<int64_t, imath_v2i64_t>::normalizeExc () = delete;
template <>
IMATH_HOSTDEVICE const Vec2<int64_t, imath_v2i64_t>&
Vec2<int64_t, imath_v2i64_t>::normalizeNonNull () IMATH_NOEXCEPT = delete;
template <>
IMATH_HOSTDEVICE Vec2<int64_t, imath_v2i64_t>
Vec2<int64_t, imath_v2i64_t>::normalized () const IMATH_NOEXCEPT = delete;
template <>
Vec2<int64_t, imath_v2i64_t>
Vec2<int64_t, imath_v2i64_t>::normalizedExc () const = delete;
template <>
IMATH_HOSTDEVICE Vec2<int64_t, imath_v2i64_t>
Vec2<int64_t, imath_v2i64_t>::normalizedNonNull () const IMATH_NOEXCEPT =
    delete;

// Vec3<short>
template <>
IMATH_HOSTDEVICE short
Vec3<short, imath_v3s_t>::length () const IMATH_NOEXCEPT = delete;
template <>
IMATH_HOSTDEVICE const Vec3<short, imath_v3s_t>&
Vec3<short, imath_v3s_t>::normalize () IMATH_NOEXCEPT = delete;
template <>
const Vec3<short, imath_v3s_t>&
Vec3<short, imath_v3s_t>::normalizeExc () = delete;
template <>
IMATH_HOSTDEVICE const Vec3<short, imath_v3s_t>&
Vec3<short, imath_v3s_t>::normalizeNonNull () IMATH_NOEXCEPT = delete;
template <>
IMATH_HOSTDEVICE Vec3<short, imath_v3s_t>
Vec3<short, imath_v3s_t>::normalized () const IMATH_NOEXCEPT = delete;
template <>
Vec3<short, imath_v3s_t>
Vec3<short, imath_v3s_t>::normalizedExc () const = delete;
template <>
IMATH_HOSTDEVICE Vec3<short, imath_v3s_t>
Vec3<short, imath_v3s_t>::normalizedNonNull () const IMATH_NOEXCEPT = delete;

// Vec3<int>
template <>
IMATH_HOSTDEVICE int
Vec3<int, imath_v3i_t>::length () const IMATH_NOEXCEPT = delete;
template <>
IMATH_HOSTDEVICE const Vec3<int, imath_v3i_t>&
Vec3<int, imath_v3i_t>::normalize () IMATH_NOEXCEPT = delete;
template <>
const Vec3<int, imath_v3i_t>& Vec3<int, imath_v3i_t>::normalizeExc () = delete;
template <>
IMATH_HOSTDEVICE const Vec3<int, imath_v3i_t>&
Vec3<int, imath_v3i_t>::normalizeNonNull () IMATH_NOEXCEPT = delete;
template <>
IMATH_HOSTDEVICE Vec3<int, imath_v3i_t>
Vec3<int, imath_v3i_t>::normalized () const IMATH_NOEXCEPT = delete;
template <>
Vec3<int, imath_v3i_t> Vec3<int, imath_v3i_t>::normalizedExc () const = delete;
template <>
IMATH_HOSTDEVICE Vec3<int, imath_v3i_t>
Vec3<int, imath_v3i_t>::normalizedNonNull () const IMATH_NOEXCEPT = delete;

// Vec3<int64_t>
template <>
IMATH_HOSTDEVICE int64_t
Vec3<int64_t, imath_v3i64_t>::length () const IMATH_NOEXCEPT = delete;
template <>
IMATH_HOSTDEVICE const Vec3<int64_t, imath_v3i64_t>&
Vec3<int64_t, imath_v3i64_t>::normalize () IMATH_NOEXCEPT = delete;
template <>
const Vec3<int64_t, imath_v3i64_t>&
Vec3<int64_t, imath_v3i64_t>::normalizeExc () = delete;
template <>
IMATH_HOSTDEVICE const Vec3<int64_t, imath_v3i64_t>&
Vec3<int64_t, imath_v3i64_t>::normalizeNonNull () IMATH_NOEXCEPT = delete;
template <>
IMATH_HOSTDEVICE Vec3<int64_t, imath_v3i64_t>
Vec3<int64_t, imath_v3i64_t>::normalized () const IMATH_NOEXCEPT = delete;
template <>
Vec3<int64_t, imath_v3i64_t>
Vec3<int64_t, imath_v3i64_t>::normalizedExc () const = delete;
template <>
IMATH_HOSTDEVICE Vec3<int64_t, imath_v3i64_t>
Vec3<int64_t, imath_v3i64_t>::normalizedNonNull () const IMATH_NOEXCEPT =
    delete;

// Vec4<short>
template <>
IMATH_HOSTDEVICE short
Vec4<short, imath_v4s_t>::length () const IMATH_NOEXCEPT = delete;
template <>
IMATH_HOSTDEVICE const Vec4<short, imath_v4s_t>&
Vec4<short, imath_v4s_t>::normalize () IMATH_NOEXCEPT = delete;
template <>
const Vec4<short, imath_v4s_t>&
Vec4<short, imath_v4s_t>::normalizeExc () = delete;
template <>
IMATH_HOSTDEVICE const Vec4<short, imath_v4s_t>&
Vec4<short, imath_v4s_t>::normalizeNonNull () IMATH_NOEXCEPT = delete;
template <>
IMATH_HOSTDEVICE Vec4<short, imath_v4s_t>
Vec4<short, imath_v4s_t>::normalized () const IMATH_NOEXCEPT = delete;
template <>
Vec4<short, imath_v4s_t>
Vec4<short, imath_v4s_t>::normalizedExc () const = delete;
template <>
IMATH_HOSTDEVICE Vec4<short, imath_v4s_t>
Vec4<short, imath_v4s_t>::normalizedNonNull () const IMATH_NOEXCEPT = delete;

// Vec4<int>
template <>
IMATH_HOSTDEVICE int
Vec4<int, imath_v4i_t>::length () const IMATH_NOEXCEPT = delete;
template <>
IMATH_HOSTDEVICE const Vec4<int, imath_v4i_t>&
Vec4<int, imath_v4i_t>::normalize () IMATH_NOEXCEPT = delete;
template <>
const Vec4<int, imath_v4i_t>& Vec4<int, imath_v4i_t>::normalizeExc () = delete;
template <>
IMATH_HOSTDEVICE const Vec4<int, imath_v4i_t>&
Vec4<int, imath_v4i_t>::normalizeNonNull () IMATH_NOEXCEPT = delete;
template <>
IMATH_HOSTDEVICE Vec4<int, imath_v4i_t>
Vec4<int, imath_v4i_t>::normalized () const IMATH_NOEXCEPT = delete;
template <>
Vec4<int, imath_v4i_t> Vec4<int, imath_v4i_t>::normalizedExc () const = delete;
template <>
IMATH_HOSTDEVICE Vec4<int, imath_v4i_t>
Vec4<int, imath_v4i_t>::normalizedNonNull () const IMATH_NOEXCEPT = delete;

// Vec4<int64_t>
template <>
IMATH_HOSTDEVICE int64_t
Vec4<int64_t, imath_v4i64_t>::length () const IMATH_NOEXCEPT = delete;
template <>
IMATH_HOSTDEVICE const Vec4<int64_t, imath_v4i64_t>&
Vec4<int64_t, imath_v4i64_t>::normalize () IMATH_NOEXCEPT = delete;
template <>
const Vec4<int64_t, imath_v4i64_t>&
Vec4<int64_t, imath_v4i64_t>::normalizeExc () = delete;
template <>
IMATH_HOSTDEVICE const Vec4<int64_t, imath_v4i64_t>&
Vec4<int64_t, imath_v4i64_t>::normalizeNonNull () IMATH_NOEXCEPT = delete;
template <>
IMATH_HOSTDEVICE Vec4<int64_t, imath_v4i64_t>
Vec4<int64_t, imath_v4i64_t>::normalized () const IMATH_NOEXCEPT = delete;
template <>
Vec4<int64_t, imath_v4i64_t>
Vec4<int64_t, imath_v4i64_t>::normalizedExc () const = delete;
template <>
IMATH_HOSTDEVICE Vec4<int64_t, imath_v4i64_t>
Vec4<int64_t, imath_v4i64_t>::normalizedNonNull () const IMATH_NOEXCEPT =
    delete;

/// @endcond Doxygen_Suppress

//------------------------
// Implementation of Vec2:
//------------------------

template <typename T, typename Storage>
IMATH_CONSTEXPR14 IMATH_HOSTDEVICE inline T&
Vec2<T, Storage>::operator[] (int i) IMATH_NOEXCEPT
{
    return this->arr[i];
}

template <typename T, typename Storage>
constexpr IMATH_HOSTDEVICE inline const T&
Vec2<T, Storage>::operator[] (int i) const IMATH_NOEXCEPT
{
    return this->arr[i];
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE inline Vec2<T, Storage>::Vec2 () IMATH_NOEXCEPT
{
    // empty, and not constexpr because data is uninitialized.
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec2<T, Storage>::Vec2 (T a) IMATH_NOEXCEPT
    : BaseStorage ({a, a})
{}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec2<T, Storage>::Vec2 (T a, T b)
    IMATH_NOEXCEPT : BaseStorage ({a, b})
{}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec2<T, Storage>::Vec2 (const T v[2])
    IMATH_NOEXCEPT : BaseStorage ({v[0], v[1]})
{}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec2<T, Storage>::Vec2 (const Vec2& v)
    IMATH_NOEXCEPT : BaseStorage ({v.x, v.y})
{}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec2<T, Storage>::Vec2 (Vec2&& v)
    IMATH_NOEXCEPT : BaseStorage ({std::move (v.x), std::move (v.y)})
{}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec2<T, Storage>::Vec2 (const Storage& v)
    IMATH_NOEXCEPT : BaseStorage (v)
{}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline Vec2<T, Storage>::Vec2 (
    std::initializer_list<T> v) IMATH_NOEXCEPT
{
    int i = 0;
    for (auto vv: v)
    {
        (*this)[i++] = vv;
        if (i == 2) break;
    }
}

template <typename T, typename Storage>
template <typename S, typename SStorage>
IMATH_HOSTDEVICE constexpr inline Vec2<T, Storage>::Vec2 (
    const Vec2<S, SStorage>& v) IMATH_NOEXCEPT
    : BaseStorage ({T (v.x), T (v.y)})
{}

template <typename T, typename Storage>
IMATH_CONSTEXPR14 IMATH_HOSTDEVICE inline Vec2<T, Storage>&
Vec2<T, Storage>::operator= (const Vec2& v) IMATH_NOEXCEPT
{
    x = v.x;
    y = v.y;
    return *this;
}

template <typename T, typename Storage>
IMATH_CONSTEXPR14 IMATH_HOSTDEVICE inline Vec2<T, Storage>&
Vec2<T, Storage>::operator= (Vec2&& v) IMATH_NOEXCEPT
{
    x = std::move (v.x);
    y = std::move (v.y);
    return *this;
}

template <typename T, typename Storage>
IMATH_CONSTEXPR14 IMATH_HOSTDEVICE inline Vec2<T, Storage>&
Vec2<T, Storage>::operator= (const Storage& v) IMATH_NOEXCEPT
{
    x = v.x;
    y = v.y;
    return *this;
}

template <typename T, typename Storage>
IMATH_CONSTEXPR14 IMATH_HOSTDEVICE inline Vec2<T, Storage>&
Vec2<T, Storage>::operator= (std::initializer_list<T> v) IMATH_NOEXCEPT
{
    int i = 0;
    for (auto vv: v)
    {
        (*this)[i++] = vv;
        if (i == 2) break;
    }
    return *this;
}

template <typename T, typename Storage>
template <class S>
IMATH_HOSTDEVICE inline void
Vec2<T, Storage>::setValue (S a, S b) IMATH_NOEXCEPT
{
    x = T (a);
    y = T (b);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE inline void
Vec2<T, Storage>::setValue (const Storage& v) IMATH_NOEXCEPT
{
    x = v.x;
    y = v.y;
}

template <typename T, typename Storage>
template <typename S, typename SStorage>
IMATH_HOSTDEVICE inline void
Vec2<T, Storage>::setValue (const Vec2<S, SStorage>& v) IMATH_NOEXCEPT
{
    x = T (v.x);
    y = T (v.y);
}

template <typename T, typename Storage>
template <class S>
IMATH_HOSTDEVICE inline void
Vec2<T, Storage>::getValue (S& a, S& b) const IMATH_NOEXCEPT
{
    a = S (x);
    b = S (y);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE inline void
Vec2<T, Storage>::getValue (Storage& v) const IMATH_NOEXCEPT
{
    v.x = x;
    v.y = y;
}

template <typename T, typename Storage>
template <typename S, typename SStorage>
IMATH_HOSTDEVICE inline void
Vec2<T, Storage>::getValue (Vec2<S, SStorage>& v) const IMATH_NOEXCEPT
{
    v.x = S (x);
    v.y = S (y);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE inline T*
Vec2<T, Storage>::getValue () IMATH_NOEXCEPT
{
    return this->arr;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE inline const T*
Vec2<T, Storage>::getValue () const IMATH_NOEXCEPT
{
    return this->arr;
}

template <typename T, typename Storage>
template <typename S, typename SStorage>
IMATH_HOSTDEVICE constexpr inline bool
Vec2<T, Storage>::operator== (const Vec2<S, SStorage>& v) const IMATH_NOEXCEPT
{
    return x == v.x && y == v.y;
}

template <typename T, typename Storage>
template <typename S, typename SStorage>
IMATH_HOSTDEVICE constexpr inline bool
Vec2<T, Storage>::operator!= (const Vec2<S, SStorage>& v) const IMATH_NOEXCEPT
{
    return x != v.x || y != v.y;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline bool
Vec2<T, Storage>::equalWithAbsError (const Vec2<T, Storage>& v, T e) const
    IMATH_NOEXCEPT
{
    for (int i = 0; i < 2; i++)
        if (!IMATH_INTERNAL_NAMESPACE::equalWithAbsError ((*this)[i], v[i], e))
            return false;

    return true;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline bool
Vec2<T, Storage>::equalWithRelError (const Vec2<T, Storage>& v, T e) const
    IMATH_NOEXCEPT
{
    for (int i = 0; i < 2; i++)
        if (!IMATH_INTERNAL_NAMESPACE::equalWithRelError ((*this)[i], v[i], e))
            return false;

    return true;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline T
Vec2<T, Storage>::dot (const Vec2& v) const IMATH_NOEXCEPT
{
    return x * v.x + y * v.y;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline T
Vec2<T, Storage>::operator^ (const Vec2& v) const IMATH_NOEXCEPT
{
    return dot (v);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline T
Vec2<T, Storage>::cross (const Vec2& v) const IMATH_NOEXCEPT
{
    return x * v.y - y * v.x;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline T
Vec2<T, Storage>::operator% (const Vec2& v) const IMATH_NOEXCEPT
{
    return x * v.y - y * v.x;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec2<T, Storage>&
Vec2<T, Storage>::operator+= (const Vec2& v) IMATH_NOEXCEPT
{
    x += v.x;
    y += v.y;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec2<T, Storage>
Vec2<T, Storage>::operator+ (const Vec2& v) const IMATH_NOEXCEPT
{
    return Vec2 (x + v.x, y + v.y);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec2<T, Storage>&
Vec2<T, Storage>::operator-= (const Vec2& v) IMATH_NOEXCEPT
{
    x -= v.x;
    y -= v.y;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec2<T, Storage>
Vec2<T, Storage>::operator- (const Vec2& v) const IMATH_NOEXCEPT
{
    return Vec2 (x - v.x, y - v.y);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec2<T, Storage>
Vec2<T, Storage>::operator- () const IMATH_NOEXCEPT
{
    return Vec2 (-x, -y);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec2<T, Storage>&
                 Vec2<T, Storage>::negate () IMATH_NOEXCEPT
{
    x = -x;
    y = -y;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec2<T, Storage>&
Vec2<T, Storage>::operator*= (const Vec2& v) IMATH_NOEXCEPT
{
    x *= v.x;
    y *= v.y;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec2<T, Storage>&
Vec2<T, Storage>::operator*= (T a) IMATH_NOEXCEPT
{
    x *= a;
    y *= a;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec2<T, Storage>
Vec2<T, Storage>::operator* (const Vec2& v) const IMATH_NOEXCEPT
{
    return Vec2 (x * v.x, y * v.y);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec2<T, Storage>
Vec2<T, Storage>::operator* (T a) const IMATH_NOEXCEPT
{
    return Vec2 (x * a, y * a);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec2<T, Storage>&
Vec2<T, Storage>::operator/= (const Vec2& v) IMATH_NOEXCEPT
{
    x /= v.x;
    y /= v.y;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec2<T, Storage>&
Vec2<T, Storage>::operator/= (T a) IMATH_NOEXCEPT
{
    x /= a;
    y /= a;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec2<T, Storage>
Vec2<T, Storage>::operator/ (const Vec2& v) const IMATH_NOEXCEPT
{
    return Vec2 (x / v.x, y / v.y);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec2<T, Storage>
Vec2<T, Storage>::operator/ (T a) const IMATH_NOEXCEPT
{
    return Vec2 (x / a, y / a);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline T
Vec2<T, Storage>::lengthTiny () const IMATH_NOEXCEPT
{
    T absX = std::abs (x);
    T absY = std::abs (y);

    T max = absX;

    if (max < absY) max = absY;

    if (IMATH_UNLIKELY (max == T (0))) return T (0);

    //
    // Do not replace the divisions by max with multiplications by 1/max.
    // Computing 1/max can overflow but the divisions below will always
    // produce results less than or equal to 1.
    //

    absX /= max;
    absY /= max;

    return max * std::sqrt (absX * absX + absY * absY);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE inline T
Vec2<T, Storage>::length () const IMATH_NOEXCEPT
{
    T length2 = dot (*this);

    if (IMATH_UNLIKELY (length2 < T (2) * std::numeric_limits<T>::min ()))
        return lengthTiny ();

    return std::sqrt (length2);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline T
Vec2<T, Storage>::length2 () const IMATH_NOEXCEPT
{
    return dot (*this);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE inline const Vec2<T, Storage>&
Vec2<T, Storage>::normalize () IMATH_NOEXCEPT
{
    T l = length ();

    if (IMATH_LIKELY (l != T (0)))
    {
        //
        // Do not replace the divisions by l with multiplications by 1/l.
        // Computing 1/l can overflow but the divisions below will always
        // produce results less than or equal to 1.
        //

        x /= l;
        y /= l;
    }

    return *this;
}

template <typename T, typename Storage>
inline const Vec2<T, Storage>&
Vec2<T, Storage>::normalizeExc ()
{
    T l = length ();

    if (IMATH_UNLIKELY (l == T (0)))
        throw std::domain_error ("Cannot normalize null vector.");

    x /= l;
    y /= l;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE inline const Vec2<T, Storage>&
Vec2<T, Storage>::normalizeNonNull () IMATH_NOEXCEPT
{
    T l = length ();
    x /= l;
    y /= l;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE inline Vec2<T, Storage>
Vec2<T, Storage>::normalized () const IMATH_NOEXCEPT
{
    T l = length ();

    if (IMATH_UNLIKELY (l == T (0))) return Vec2 (T (0));

    return Vec2 (x / l, y / l);
}

template <typename T, typename Storage>
inline Vec2<T, Storage>
Vec2<T, Storage>::normalizedExc () const
{
    T l = length ();

    if (IMATH_UNLIKELY (l == T (0)))
        throw std::domain_error ("Cannot normalize null vector.");

    return Vec2 (x / l, y / l);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE inline Vec2<T, Storage>
Vec2<T, Storage>::normalizedNonNull () const IMATH_NOEXCEPT
{
    T l = length ();
    return Vec2 (x / l, y / l);
}

//-----------------------
// Implementation of Vec3
//-----------------------

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline T&
Vec3<T, Storage>::operator[] (int i) IMATH_NOEXCEPT
{
    return this->arr[i];
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline const T&
Vec3<T, Storage>::operator[] (int i) const IMATH_NOEXCEPT
{
    return this->arr[i];
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE inline Vec3<T, Storage>::Vec3 () IMATH_NOEXCEPT
{
    // empty, and not constexpr because data is uninitialized.
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec3<T, Storage>::Vec3 (T a) IMATH_NOEXCEPT
    : BaseStorage ({a, a, a})
{}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec3<T, Storage>::Vec3 (T a, T b, T c)
    IMATH_NOEXCEPT : BaseStorage ({a, b, c})
{}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec3<T, Storage>::Vec3 (const T v[3])
    IMATH_NOEXCEPT : BaseStorage ({v[0], v[1], v[2]})
{}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec3<T, Storage>::Vec3 (const Vec3& v)
    IMATH_NOEXCEPT : BaseStorage ({v.x, v.y, v.z})
{}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec3<T, Storage>::Vec3 (Vec3&& v)
    IMATH_NOEXCEPT
    : BaseStorage ({std::move (v.x), std::move (v.y), std::move (v.z)})
{}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec3<T, Storage>::Vec3 (const Storage& v)
    IMATH_NOEXCEPT : BaseStorage (v)
{}

template <typename T, typename Storage>
template <typename S, typename SStorage>
IMATH_HOSTDEVICE constexpr inline Vec3<T, Storage>::Vec3 (
    const Vec3<S, SStorage>& v) IMATH_NOEXCEPT
    : BaseStorage ({T (v.x), T (v.y), T (v.z)})
{}

template <typename T, typename Storage>
IMATH_CONSTEXPR14 IMATH_HOSTDEVICE inline Vec3<T, Storage>::Vec3 (
    std::initializer_list<T> v) IMATH_NOEXCEPT
{
    int i = 0;
    for (auto vv: v)
    {
        (*this)[i++] = vv;
        if (i == 3) break;
    }
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline Vec3<T, Storage>&
Vec3<T, Storage>::operator= (const Vec3& v) IMATH_NOEXCEPT
{
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline Vec3<T, Storage>&
Vec3<T, Storage>::operator= (Vec3&& v) IMATH_NOEXCEPT
{
    x = std::move (v.x);
    y = std::move (v.y);
    z = std::move (v.z);
    return *this;
}

template <typename T, typename Storage>
IMATH_CONSTEXPR14 IMATH_HOSTDEVICE inline Vec3<T, Storage>&
Vec3<T, Storage>::operator= (const Storage& v) IMATH_NOEXCEPT
{
    x = v.x;
    y = v.y;
    y = v.z;
    return *this;
}

template <typename T, typename Storage>
IMATH_CONSTEXPR14 IMATH_HOSTDEVICE inline Vec3<T, Storage>&
Vec3<T, Storage>::operator= (std::initializer_list<T> v) IMATH_NOEXCEPT
{
    int i = 0;
    for (auto vv: v)
    {
        (*this)[i++] = vv;
        if (i == 3) break;
    }
    return *this;
}

template <typename T, typename Storage>
template <typename S, typename SStorage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline Vec3<T, Storage>::Vec3 (
    const Vec4<S, SStorage>& v) IMATH_NOEXCEPT
    : BaseStorage (T (v.x / v.w), T (v.y / v.w), T (v.z / v.w))
{}

template <typename T, typename Storage>
template <typename S, typename SStorage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline Vec3<T, Storage>::Vec3 (
    const Vec4<S, SStorage>& v, InfException)
{
    T vx = T (v.x);
    T vy = T (v.y);
    T vz = T (v.z);
    T vw = T (v.w);

    T absW = (vw >= T (0)) ? vw : -vw;

    if (absW < 1)
    {
        T m = baseTypeMax () * absW;

        if (vx <= -m || vx >= m || vy <= -m || vy >= m || vz <= -m || vz >= m)
            throw std::domain_error ("Cannot normalize point at infinity.");
    }

    x = vx / vw;
    y = vy / vw;
    z = vz / vw;
}

template <typename T, typename Storage>
template <class S>
IMATH_HOSTDEVICE inline void
Vec3<T, Storage>::setValue (S a, S b, S c) IMATH_NOEXCEPT
{
    x = T (a);
    y = T (b);
    z = T (c);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE inline void
Vec3<T, Storage>::setValue (const Storage& v) IMATH_NOEXCEPT
{
    x = v.x;
    y = v.y;
    z = v.z;
}

template <typename T, typename Storage>
template <typename S, typename SStorage>
IMATH_HOSTDEVICE inline void
Vec3<T, Storage>::setValue (const Vec3<S, SStorage>& v) IMATH_NOEXCEPT
{
    x = T (v.x);
    y = T (v.y);
    z = T (v.z);
}

template <typename T, typename Storage>
template <class S>
IMATH_HOSTDEVICE inline void
Vec3<T, Storage>::getValue (S& a, S& b, S& c) const IMATH_NOEXCEPT
{
    a = S (x);
    b = S (y);
    c = S (z);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE inline void
Vec3<T, Storage>::getValue (Storage& v) const IMATH_NOEXCEPT
{
    v.x = x;
    v.y = y;
    v.z = z;
}

template <typename T, typename Storage>
template <typename S, typename SStorage>
IMATH_HOSTDEVICE inline void
Vec3<T, Storage>::getValue (Vec3<S, SStorage>& v) const IMATH_NOEXCEPT
{
    v.x = S (x);
    v.y = S (y);
    v.z = S (z);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE inline T*
Vec3<T, Storage>::getValue () IMATH_NOEXCEPT
{
    return this->arr;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE inline const T*
Vec3<T, Storage>::getValue () const IMATH_NOEXCEPT
{
    return this->arr;
}

template <typename T, typename Storage>
template <typename S, typename SStorage>
IMATH_HOSTDEVICE constexpr inline bool
Vec3<T, Storage>::operator== (const Vec3<S, SStorage>& v) const IMATH_NOEXCEPT
{
    return x == v.x && y == v.y && z == v.z;
}

template <typename T, typename Storage>
template <typename S, typename SStorage>
IMATH_HOSTDEVICE constexpr inline bool
Vec3<T, Storage>::operator!= (const Vec3<S, SStorage>& v) const IMATH_NOEXCEPT
{
    return x != v.x || y != v.y || z != v.z;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline bool
Vec3<T, Storage>::equalWithAbsError (const Vec3<T, Storage>& v, T e) const
    IMATH_NOEXCEPT
{
    for (int i = 0; i < 3; i++)
        if (!IMATH_INTERNAL_NAMESPACE::equalWithAbsError ((*this)[i], v[i], e))
            return false;

    return true;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline bool
Vec3<T, Storage>::equalWithRelError (const Vec3<T, Storage>& v, T e) const
    IMATH_NOEXCEPT
{
    for (int i = 0; i < 3; i++)
        if (!IMATH_INTERNAL_NAMESPACE::equalWithRelError ((*this)[i], v[i], e))
            return false;

    return true;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline T
Vec3<T, Storage>::dot (const Vec3& v) const IMATH_NOEXCEPT
{
    return x * v.x + y * v.y + z * v.z;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline T
Vec3<T, Storage>::operator^ (const Vec3& v) const IMATH_NOEXCEPT
{
    return dot (v);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec3<T, Storage>
Vec3<T, Storage>::cross (const Vec3& v) const IMATH_NOEXCEPT
{
    return Vec3 (y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec3<T, Storage>&
Vec3<T, Storage>::operator%= (const Vec3& v) IMATH_NOEXCEPT
{
    T a = y * v.z - z * v.y;
    T b = z * v.x - x * v.z;
    T c = x * v.y - y * v.x;
    x   = a;
    y   = b;
    z   = c;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec3<T, Storage>
Vec3<T, Storage>::operator% (const Vec3& v) const IMATH_NOEXCEPT
{
    return Vec3 (y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec3<T, Storage>&
Vec3<T, Storage>::operator+= (const Vec3& v) IMATH_NOEXCEPT
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec3<T, Storage>
Vec3<T, Storage>::operator+ (const Vec3& v) const IMATH_NOEXCEPT
{
    return Vec3 (x + v.x, y + v.y, z + v.z);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec3<T, Storage>&
Vec3<T, Storage>::operator-= (const Vec3& v) IMATH_NOEXCEPT
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec3<T, Storage>
Vec3<T, Storage>::operator- (const Vec3& v) const IMATH_NOEXCEPT
{
    return Vec3 (x - v.x, y - v.y, z - v.z);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec3<T, Storage>
Vec3<T, Storage>::operator- () const IMATH_NOEXCEPT
{
    return Vec3 (-x, -y, -z);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec3<T, Storage>&
                 Vec3<T, Storage>::negate () IMATH_NOEXCEPT
{
    x = -x;
    y = -y;
    z = -z;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec3<T, Storage>&
Vec3<T, Storage>::operator*= (const Vec3& v) IMATH_NOEXCEPT
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec3<T, Storage>&
Vec3<T, Storage>::operator*= (T a) IMATH_NOEXCEPT
{
    x *= a;
    y *= a;
    z *= a;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec3<T, Storage>
Vec3<T, Storage>::operator* (const Vec3& v) const IMATH_NOEXCEPT
{
    return Vec3 (x * v.x, y * v.y, z * v.z);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec3<T, Storage>
Vec3<T, Storage>::operator* (T a) const IMATH_NOEXCEPT
{
    return Vec3 (x * a, y * a, z * a);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec3<T, Storage>&
Vec3<T, Storage>::operator/= (const Vec3& v) IMATH_NOEXCEPT
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec3<T, Storage>&
Vec3<T, Storage>::operator/= (T a) IMATH_NOEXCEPT
{
    x /= a;
    y /= a;
    z /= a;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec3<T, Storage>
Vec3<T, Storage>::operator/ (const Vec3& v) const IMATH_NOEXCEPT
{
    return Vec3 (x / v.x, y / v.y, z / v.z);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec3<T, Storage>
Vec3<T, Storage>::operator/ (T a) const IMATH_NOEXCEPT
{
    return Vec3 (x / a, y / a, z / a);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline T
Vec3<T, Storage>::lengthTiny () const IMATH_NOEXCEPT
{
    T absX = (x >= T (0)) ? x : -x;
    T absY = (y >= T (0)) ? y : -y;
    T absZ = (z >= T (0)) ? z : -z;

    T max = absX;

    if (max < absY) max = absY;

    if (max < absZ) max = absZ;

    if (IMATH_UNLIKELY (max == T (0))) return T (0);

    //
    // Do not replace the divisions by max with multiplications by 1/max.
    // Computing 1/max can overflow but the divisions below will always
    // produce results less than or equal to 1.
    //

    absX /= max;
    absY /= max;
    absZ /= max;

    return max * std::sqrt (absX * absX + absY * absY + absZ * absZ);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE inline T
Vec3<T, Storage>::length () const IMATH_NOEXCEPT
{
    T length2 = dot (*this);

    if (IMATH_UNLIKELY (length2 < T (2) * std::numeric_limits<T>::min ()))
        return lengthTiny ();

    return std::sqrt (length2);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline T
Vec3<T, Storage>::length2 () const IMATH_NOEXCEPT
{
    return dot (*this);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE inline const Vec3<T, Storage>&
Vec3<T, Storage>::normalize () IMATH_NOEXCEPT
{
    T l = length ();

    if (IMATH_LIKELY (l != T (0)))
    {
        //
        // Do not replace the divisions by l with multiplications by 1/l.
        // Computing 1/l can overflow but the divisions below will always
        // produce results less than or equal to 1.
        //

        x /= l;
        y /= l;
        z /= l;
    }

    return *this;
}

template <typename T, typename Storage>
inline const Vec3<T, Storage>&
Vec3<T, Storage>::normalizeExc ()
{
    T l = length ();

    if (IMATH_UNLIKELY (l == T (0)))
        throw std::domain_error ("Cannot normalize null vector.");

    x /= l;
    y /= l;
    z /= l;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE inline const Vec3<T, Storage>&
Vec3<T, Storage>::normalizeNonNull () IMATH_NOEXCEPT
{
    T l = length ();
    x /= l;
    y /= l;
    z /= l;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE inline Vec3<T, Storage>
Vec3<T, Storage>::normalized () const IMATH_NOEXCEPT
{
    T l = length ();

    if (IMATH_UNLIKELY ((l == T (0)))) return Vec3 (T (0));

    return Vec3 (x / l, y / l, z / l);
}

template <typename T, typename Storage>
inline Vec3<T, Storage>
Vec3<T, Storage>::normalizedExc () const
{
    T l = length ();

    if (IMATH_UNLIKELY (l == T (0)))
        throw std::domain_error ("Cannot normalize null vector.");

    return Vec3 (x / l, y / l, z / l);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE inline Vec3<T, Storage>
Vec3<T, Storage>::normalizedNonNull () const IMATH_NOEXCEPT
{
    T l = length ();
    return Vec3 (x / l, y / l, z / l);
}

//-----------------------
// Implementation of Vec4
//-----------------------

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline T&
Vec4<T, Storage>::operator[] (int i) IMATH_NOEXCEPT
{
    return this->arr[i];
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline const T&
Vec4<T, Storage>::operator[] (int i) const IMATH_NOEXCEPT
{
    return this->arr[i];
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE inline Vec4<T, Storage>::Vec4 () IMATH_NOEXCEPT
{
    // empty, and not constexpr because data is uninitialized.
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec4<T, Storage>::Vec4 (T a) IMATH_NOEXCEPT
    : BaseStorage ({a, a, a, a})
{}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec4<T, Storage>::Vec4 (T a, T b, T c, T d)
    IMATH_NOEXCEPT : BaseStorage ({a, b, c, d})
{}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec4<T, Storage>::Vec4 (const T v[4])
    IMATH_NOEXCEPT : BaseStorage ({v[0], v[1], v[2], v[3]})
{}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec4<T, Storage>::Vec4 (const Vec4& v)
    IMATH_NOEXCEPT : BaseStorage ({v.x, v.y, v.z, v.w})
{}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec4<T, Storage>::Vec4 (Vec4&& v)
    IMATH_NOEXCEPT
    : BaseStorage (
          {std::move (v.x), std::move (v.y), std::move (v.z), std::move (v.w)})
{}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec4<T, Storage>::Vec4 (const Storage& v)
    IMATH_NOEXCEPT : BaseStorage (v)
{}

template <typename T, typename Storage>
IMATH_CONSTEXPR14 IMATH_HOSTDEVICE inline Vec4<T, Storage>::Vec4 (
    std::initializer_list<T> v) IMATH_NOEXCEPT
{
    int i = 0;
    for (auto vv: v)
    {
        (*this)[i++] = vv;
        if (i == 4) break;
    }
}

template <typename T, typename Storage>
template <typename S, typename SStorage>
IMATH_HOSTDEVICE constexpr inline Vec4<T, Storage>::Vec4 (
    const Vec4<S, SStorage>& v) IMATH_NOEXCEPT
    : BaseStorage ({T (v.x), T (v.y), T (v.z), T (v.w)})
{}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline Vec4<T, Storage>&
Vec4<T, Storage>::operator= (const Vec4& v) IMATH_NOEXCEPT
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline Vec4<T, Storage>&
Vec4<T, Storage>::operator= (Vec4&& v) IMATH_NOEXCEPT
{
    x = std::move (v.x);
    y = std::move (v.y);
    z = std::move (v.z);
    w = std::move (v.w);
    return *this;
}

template <typename T, typename Storage>
IMATH_CONSTEXPR14 IMATH_HOSTDEVICE inline Vec4<T, Storage>&
Vec4<T, Storage>::operator= (const Storage& v) IMATH_NOEXCEPT
{
    x = v.x;
    y = v.y;
    y = v.z;
    w = v.w;
    return *this;
}

template <typename T, typename Storage>
IMATH_CONSTEXPR14 IMATH_HOSTDEVICE inline Vec4<T, Storage>&
Vec4<T, Storage>::operator= (std::initializer_list<T> v) IMATH_NOEXCEPT
{
    int i = 0;
    for (auto vv: v)
    {
        (*this)[i++] = vv;
        if (i == 4) break;
    }
    return *this;
}

template <typename T, typename Storage>
template <typename S, typename SStorage>
IMATH_HOSTDEVICE constexpr inline Vec4<T, Storage>::Vec4 (
    const Vec3<S, SStorage>& v) IMATH_NOEXCEPT
    : BaseStorage ({T (v.x), T (v.y), T (v.z), T (1)})
{}

template <typename T, typename Storage>
template <typename S, typename SStorage>
IMATH_HOSTDEVICE constexpr inline bool
Vec4<T, Storage>::operator== (const Vec4<S, SStorage>& v) const IMATH_NOEXCEPT
{
    return x == v.x && y == v.y && z == v.z && w == v.w;
}

template <typename T, typename Storage>
template <typename S, typename SStorage>
IMATH_HOSTDEVICE constexpr inline bool
Vec4<T, Storage>::operator!= (const Vec4<S, SStorage>& v) const IMATH_NOEXCEPT
{
    return x != v.x || y != v.y || z != v.z || w != v.w;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline bool
Vec4<T, Storage>::equalWithAbsError (const Vec4<T, Storage>& v, T e) const
    IMATH_NOEXCEPT
{
    for (int i = 0; i < 4; i++)
        if (!IMATH_INTERNAL_NAMESPACE::equalWithAbsError ((*this)[i], v[i], e))
            return false;

    return true;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline bool
Vec4<T, Storage>::equalWithRelError (const Vec4<T, Storage>& v, T e) const
    IMATH_NOEXCEPT
{
    for (int i = 0; i < 4; i++)
        if (!IMATH_INTERNAL_NAMESPACE::equalWithRelError ((*this)[i], v[i], e))
            return false;

    return true;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline T
Vec4<T, Storage>::dot (const Vec4& v) const IMATH_NOEXCEPT
{
    return x * v.x + y * v.y + z * v.z + w * v.w;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline T
Vec4<T, Storage>::operator^ (const Vec4& v) const IMATH_NOEXCEPT
{
    return dot (v);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec4<T, Storage>&
Vec4<T, Storage>::operator+= (const Vec4& v) IMATH_NOEXCEPT
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec4<T, Storage>
Vec4<T, Storage>::operator+ (const Vec4& v) const IMATH_NOEXCEPT
{
    return Vec4 (x + v.x, y + v.y, z + v.z, w + v.w);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec4<T, Storage>&
Vec4<T, Storage>::operator-= (const Vec4& v) IMATH_NOEXCEPT
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec4<T, Storage>
Vec4<T, Storage>::operator- (const Vec4& v) const IMATH_NOEXCEPT
{
    return Vec4 (x - v.x, y - v.y, z - v.z, w - v.w);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec4<T, Storage>
Vec4<T, Storage>::operator- () const IMATH_NOEXCEPT
{
    return Vec4 (-x, -y, -z, -w);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec4<T, Storage>&
                 Vec4<T, Storage>::negate () IMATH_NOEXCEPT
{
    x = -x;
    y = -y;
    z = -z;
    w = -w;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec4<T, Storage>&
Vec4<T, Storage>::operator*= (const Vec4& v) IMATH_NOEXCEPT
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec4<T, Storage>&
Vec4<T, Storage>::operator*= (T a) IMATH_NOEXCEPT
{
    x *= a;
    y *= a;
    z *= a;
    w *= a;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec4<T, Storage>
Vec4<T, Storage>::operator* (const Vec4& v) const IMATH_NOEXCEPT
{
    return Vec4 (x * v.x, y * v.y, z * v.z, w * v.w);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec4<T, Storage>
Vec4<T, Storage>::operator* (T a) const IMATH_NOEXCEPT
{
    return Vec4 (x * a, y * a, z * a, w * a);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec4<T, Storage>&
Vec4<T, Storage>::operator/= (const Vec4& v) IMATH_NOEXCEPT
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec4<T, Storage>&
Vec4<T, Storage>::operator/= (T a) IMATH_NOEXCEPT
{
    x /= a;
    y /= a;
    z /= a;
    w /= a;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec4<T, Storage>
Vec4<T, Storage>::operator/ (const Vec4& v) const IMATH_NOEXCEPT
{
    return Vec4 (x / v.x, y / v.y, z / v.z, w / v.w);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec4<T, Storage>
Vec4<T, Storage>::operator/ (T a) const IMATH_NOEXCEPT
{
    return Vec4 (x / a, y / a, z / a, w / a);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline T
Vec4<T, Storage>::lengthTiny () const IMATH_NOEXCEPT
{
    T absX = (x >= T (0)) ? x : -x;
    T absY = (y >= T (0)) ? y : -y;
    T absZ = (z >= T (0)) ? z : -z;
    T absW = (w >= T (0)) ? w : -w;

    T max = absX;

    if (max < absY) max = absY;

    if (max < absZ) max = absZ;

    if (max < absW) max = absW;

    if (IMATH_UNLIKELY (max == T (0))) return T (0);

    //
    // Do not replace the divisions by max with multiplications by 1/max.
    // Computing 1/max can overflow but the divisions below will always
    // produce results less than or equal to 1.
    //

    absX /= max;
    absY /= max;
    absZ /= max;
    absW /= max;

    return max *
           std::sqrt (absX * absX + absY * absY + absZ * absZ + absW * absW);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE inline T
Vec4<T, Storage>::length () const IMATH_NOEXCEPT
{
    T length2 = dot (*this);

    if (IMATH_UNLIKELY (length2 < T (2) * std::numeric_limits<T>::min ()))
        return lengthTiny ();

    return std::sqrt (length2);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline T
Vec4<T, Storage>::length2 () const IMATH_NOEXCEPT
{
    return dot (*this);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE const inline Vec4<T, Storage>&
Vec4<T, Storage>::normalize () IMATH_NOEXCEPT
{
    T l = length ();

    if (IMATH_LIKELY (l != T (0)))
    {
        //
        // Do not replace the divisions by l with multiplications by 1/l.
        // Computing 1/l can overflow but the divisions below will always
        // produce results less than or equal to 1.
        //

        x /= l;
        y /= l;
        z /= l;
        w /= l;
    }

    return *this;
}

template <typename T, typename Storage>
const inline Vec4<T, Storage>&
Vec4<T, Storage>::normalizeExc ()
{
    T l = length ();

    if (IMATH_UNLIKELY (l == T (0)))
        throw std::domain_error ("Cannot normalize null vector.");

    x /= l;
    y /= l;
    z /= l;
    w /= l;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE inline const Vec4<T, Storage>&
Vec4<T, Storage>::normalizeNonNull () IMATH_NOEXCEPT
{
    T l = length ();
    x /= l;
    y /= l;
    z /= l;
    w /= l;
    return *this;
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE inline Vec4<T, Storage>
Vec4<T, Storage>::normalized () const IMATH_NOEXCEPT
{
    T l = length ();

    if (IMATH_UNLIKELY (l == T (0))) return Vec4 (T (0));

    return Vec4 (x / l, y / l, z / l, w / l);
}

template <typename T, typename Storage>
inline Vec4<T, Storage>
Vec4<T, Storage>::normalizedExc () const
{
    T l = length ();

    if (IMATH_UNLIKELY (l == T (0)))
        throw std::domain_error ("Cannot normalize null vector.");

    return Vec4 (x / l, y / l, z / l, w / l);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE inline Vec4<T, Storage>
Vec4<T, Storage>::normalizedNonNull () const IMATH_NOEXCEPT
{
    T l = length ();
    return Vec4 (x / l, y / l, z / l, w / l);
}

//-----------------------------
// Stream output implementation
//-----------------------------

template <typename T, typename Storage>
inline std::ostream&
operator<< (std::ostream& s, const Vec2<T, Storage>& v)
{
    return s << '(' << v.x << ' ' << v.y << ')';
}

template <typename T, typename Storage>
inline std::ostream&
operator<< (std::ostream& s, const Vec3<T, Storage>& v)
{
    return s << '(' << v.x << ' ' << v.y << ' ' << v.z << ')';
}

template <typename T, typename Storage>
inline std::ostream&
operator<< (std::ostream& s, const Vec4<T, Storage>& v)
{
    return s << '(' << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w << ')';
}

//-----------------------------------------
// Implementation of reverse multiplication
//-----------------------------------------

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec2<T, Storage>
operator* (T a, const Vec2<T, Storage>& v) IMATH_NOEXCEPT
{
    return Vec2<T, Storage> (a * v.x, a * v.y);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec3<T, Storage>
operator* (T a, const Vec3<T, Storage>& v) IMATH_NOEXCEPT
{
    return Vec3<T, Storage> (a * v.x, a * v.y, a * v.z);
}

template <typename T, typename Storage>
IMATH_HOSTDEVICE constexpr inline Vec4<T, Storage>
operator* (T a, const Vec4<T, Storage>& v) IMATH_NOEXCEPT
{
    return Vec4<T, Storage> (a * v.x, a * v.y, a * v.z, a * v.w);
}

#if (defined _WIN32 || defined _WIN64) && defined _MSC_VER
#    pragma warning(pop)
#endif

IMATH_INTERNAL_NAMESPACE_HEADER_EXIT

#endif // INCLUDED_IMATHVEC_H

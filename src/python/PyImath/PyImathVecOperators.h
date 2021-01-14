//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef _PyImathVecOperators_h_
#define _PyImathVecOperators_h_

namespace PyImath {

template <class T>
struct op_vecDot {
    static inline typename T::BaseType apply(const T &a, const T &b) { return a.dot(b); }
};

template <class T,
          IMATH_ENABLE_IF(!std::is_integral<typename T::BaseType>::value)>
struct op_vecLength {
    static inline typename T::BaseType apply(const T &v) { return v.length(); }
};

template <class T>
struct op_vecLength2 {
    static inline typename T::BaseType apply(const T &v) { return v.length2(); }
};

template <class T,
          IMATH_ENABLE_IF(!std::is_integral<typename T::BaseType>::value)>
struct op_vecNormalize {
    static inline void apply(T &v) { v.normalize(); }
};

template <class T,
          IMATH_ENABLE_IF(!std::is_integral<typename T::BaseType>::value)>
struct op_vecNormalized {
    static inline T apply(const T &v) { return v.normalized(); }
};

template <class T,
          IMATH_ENABLE_IF(!std::is_integral<typename T::BaseType>::value)>
struct op_vecNormalizeExc {
    static inline void apply(T &v) { v.normalizeExc(); }
};

template <class T,
          IMATH_ENABLE_IF(!std::is_integral<typename T::BaseType>::value)>
struct op_vecNormalizedExc {
    static inline T apply(const T &v) { return v.normalizedExc(); }
};
  
template <class T>
struct op_vec3Cross {
    static inline IMATH_NAMESPACE::Vec3<T> apply(const IMATH_NAMESPACE::Vec3<T> &a, const IMATH_NAMESPACE::Vec3<T> &b) { return a.cross(b); }
};

template <class T>
struct op_vec2Cross {
    static inline T apply(const IMATH_NAMESPACE::Vec2<T> &a, const IMATH_NAMESPACE::Vec2<T> &b) { return a.cross(b); }
};

}  // namespace PyImath

#endif // _PyImathVecOperators_h_

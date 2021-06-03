//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef _PyImathQuatOperators_h_
#define _PyImathQuatOperators_h_

#include <ImathQuat.h>

namespace PyImath {

template <class T>
struct op_quatDot {
    static inline typename T::BaseType apply (const T &self, const T &qB)
    { return self.euclideanInnerProduct (qB); }
};

template <class T>
struct op_quatNormalize {
    static inline void apply (T &self)
    { self.normalize(); }
};

template <class T>
struct op_quatNormalized {
    static inline T apply (const T &self)
    { return self.normalized(); }
};

template <class T>
struct op_quatSlerp {
    static inline T apply (const T &self, const T &qB, const typename T::BaseType t)
        { return Imath::slerpShortestArc (self, qB, t); }
};


}  // namespace PyImath

#endif // _PyImathQuatOperators_h_

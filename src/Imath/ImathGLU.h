//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifndef INCLUDED_IMATHGLU_H
#define INCLUDED_IMATHGLU_H

#include <GL/gl.h>
#include <GL/glu.h>

#include "ImathVec.h"

inline void
gluLookAt (const IMATH_INTERNAL_NAMESPACE::V3f& pos,
           const IMATH_INTERNAL_NAMESPACE::V3f& interest,
           const IMATH_INTERNAL_NAMESPACE::V3f& up)
{
    gluLookAt (pos.x, pos.y, pos.z, interest.x, interest.y, interest.z, up.x, up.y, up.z);
}

#endif

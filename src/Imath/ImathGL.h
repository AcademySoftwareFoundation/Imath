//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifndef INCLUDED_IMATHGL_H
#define INCLUDED_IMATHGL_H

#include <GL/gl.h>

#include "ImathFun.h"
#include "ImathMatrix.h"
#include "ImathNamespace.h"
#include "ImathVec.h"

inline void
glVertex (const IMATH_INTERNAL_NAMESPACE::V3f& v)
{
    glVertex3f (v.x, v.y, v.z);
}

inline void
glVertex (const IMATH_INTERNAL_NAMESPACE::V2f& v)
{
    glVertex2f (v.x, v.y);
}

inline void
glNormal (const IMATH_INTERNAL_NAMESPACE::V3f& n)
{
    glNormal3f (n.x, n.y, n.z);
}

inline void
glColor (const IMATH_INTERNAL_NAMESPACE::V3f& c)
{
    glColor3f (c.x, c.y, c.z);
}

inline void
glTranslate (const IMATH_INTERNAL_NAMESPACE::V3f& t)
{
    glTranslatef (t.x, t.y, t.z);
}

inline void
glTexCoord (const IMATH_INTERNAL_NAMESPACE::V2f& t)
{
    glTexCoord2f (t.x, t.y);
}

inline void
glDisableTexture()
{
    glActiveTexture (GL_TEXTURE1);
    glBindTexture (GL_TEXTURE_2D, 0);
    glDisable (GL_TEXTURE_2D);

    glActiveTexture (GL_TEXTURE0);
}

namespace
{

const float GL_FLOAT_MAX = 1.8e+19; // sqrt (FLT_MAX)

inline bool
badFloat (float f)
{
    return !IMATH_INTERNAL_NAMESPACE::finitef (f) || f < -GL_FLOAT_MAX || f > GL_FLOAT_MAX;
}

} // namespace

inline void
throwBadMatrix (const IMATH_INTERNAL_NAMESPACE::M44f& m)
{
    if (badFloat (m[0][0]) || badFloat (m[0][1]) || badFloat (m[0][2]) || badFloat (m[0][3]) ||
        badFloat (m[1][0]) || badFloat (m[1][1]) || badFloat (m[1][2]) || badFloat (m[1][3]) ||
        badFloat (m[2][0]) || badFloat (m[2][1]) || badFloat (m[2][2]) || badFloat (m[2][3]) ||
        badFloat (m[3][0]) || badFloat (m[3][1]) || badFloat (m[3][2]) || badFloat (m[3][3]))
        throw IEX_NAMESPACE::OverflowExc ("GL matrix overflow");
}

inline void
glMultMatrix (const IMATH_INTERNAL_NAMESPACE::M44f& m)
{
    throwBadMatrix (m);
    glMultMatrixf ((GLfloat*) m[0]);
}

inline void
glMultMatrix (const IMATH_INTERNAL_NAMESPACE::M44f* m)
{
    throwBadMatrix (*m);
    glMultMatrixf ((GLfloat*) (*m)[0]);
}

inline void
glLoadMatrix (const IMATH_INTERNAL_NAMESPACE::M44f& m)
{
    throwBadMatrix (m);
    glLoadMatrixf ((GLfloat*) m[0]);
}

inline void
glLoadMatrix (const IMATH_INTERNAL_NAMESPACE::M44f* m)
{
    throwBadMatrix (*m);
    glLoadMatrixf ((GLfloat*) (*m)[0]);
}

IMATH_INTERNAL_NAMESPACE_HEADER_ENTER

//
// Class objects that push/pop the GL state. These objects assist with
// proper cleanup of the state when exceptions are thrown.
//

class GLPushMatrix
{
  public:
    GLPushMatrix() { glPushMatrix(); }
    ~GLPushMatrix() { glPopMatrix(); }
};

class GLPushAttrib
{
  public:
    GLPushAttrib (GLbitfield mask) { glPushAttrib (mask); }
    ~GLPushAttrib() { glPopAttrib(); }
};

class GLBegin
{
  public:
    GLBegin (GLenum mode) { glBegin (mode); }
    ~GLBegin() { glEnd(); }
};

IMATH_INTERNAL_NAMESPACE_HEADER_EXIT

#endif

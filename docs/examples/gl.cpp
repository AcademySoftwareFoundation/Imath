#include <OpenEXR/ImathGL.h>

void
example()
{
    Imath::M44f M;
    glPushMatrix (M);

    Imath::V3f v (0.0f, 1.0f, 2.0f);
    glVertex (v);
}


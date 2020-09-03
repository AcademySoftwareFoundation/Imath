#include <ImathPlane.h>

void
plane3_example()
{
    Imath::V3f a (1.0f, 0.0f, 0.0f);
    Imath::V3f b (0.0f, 1.0f, 0.0f);
    Imath::V3f c (0.0f, 0.0f, 1.0f);

    Imath::V3f o (0.0f, 0.0f, 0.0f);

    Imath::Plane3f p (a, b, c);
    float d = p.distanceTo (o);
}

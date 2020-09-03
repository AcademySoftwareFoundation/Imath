#include <ImathSphere.h>

void
sphere3_example()
{
    Imath::V3f center (1.0f, 1.0f, 1.0f); 
    float radius = 2.0f;
    Imath::Sphere3f s (center, radius);

    Imath::Line3f l (Imath::V3f (0.0f, 0.0f, 0.0f),
                     Imath::V3f (1.0f, 1.0f, 1.0f));

    Imath::V3f v;
    bool ans = s.intersect (l, v);
}

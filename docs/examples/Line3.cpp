#include <ImathLine.h>

void
line3_example()
{
    Imath::V3f   a (0.0f, 0.0f, 0.0f);
    Imath::V3f   b (1.0f, 1.0f, 1.0f);

    Imath::Line3f l (a, b);
  
    Imath::V3f   c (0.5f, 0.5f, 0.5f);

    float f = l.distanceTo (c);

    Imath::V3f p = l (0.5f); // midpoint, i.e. 0.5 between a and b
}

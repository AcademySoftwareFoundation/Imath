#include <Imath/ImathColor.h>

void
color3_example()
{
    Imath::C3c   r (255, 0, 0);
    Imath::C3c   g (0, 255, 0);
    Imath::C3c   b (0, 0, 255);
    
    Imath::C3c   w = (r + g + b) / 3;
}

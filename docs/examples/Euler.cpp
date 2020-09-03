#include <ImathEuler.h>

void
euler_example()
{
    Imath::Eulerf f (0.0f, M_PI, 0.0f, Imath::Eulerf::XYZ);

    Imath::M44f M = f.toMatrix44();
}
  

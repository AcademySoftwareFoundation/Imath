#include <OpenEXR/ImathEuler.h>

int
main()
{
    Imath::Eulerf f (0.0f, M_PI, 0.0f, Eulerf::XYZ);

    Imath::M44f M = f.toMatrix44();

    return 0;
}
  

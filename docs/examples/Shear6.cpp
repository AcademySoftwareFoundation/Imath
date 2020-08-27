#include <OpenEXR/ImathShear.h>

int
main()
{
    Imath::Shear6f s (0.330f, 0.710f, 0.010f, 0.999f, -0.531f, -0.012f);

    Imath::M44f M;
    M.setShear (s);

    return 0;
}

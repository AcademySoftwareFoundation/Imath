#include <OpenEXR/ImathMatrix.h>

int
main()
{
    Imath::M22f M; // uninitialized

    M.makeIdentity();
    assert (M[0][0] == 1.0f);

    Imath::M33f Minv = M.inverse();
  
    return 0;
}

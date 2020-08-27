#include <OpenEXR/ImathMatrix.h>

int
main()
{
    Imath::M44f M; // uninitialized

    M.makeIdentity();
    assert (M[0][0] == 1.0f);

    Imath::M44f Minv = M.inverse();

    return 0;
}

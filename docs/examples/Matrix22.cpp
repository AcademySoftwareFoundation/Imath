#include <ImathMatrix.h>

void
matrix22_example()
{
    Imath::M22f M; // uninitialized

    M.makeIdentity();
    assert (M[0][0] == 1.0f);

    Imath::M22f Minv = M.inverse();
}

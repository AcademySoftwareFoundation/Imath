#include <ImathMatrix.h>

void
matrix44_example()
{
    Imath::M44f M; // uninitialized

    M.makeIdentity();
    assert (M[0][0] == 1.0f);

    Imath::M44f Minv = M.inverse();
}

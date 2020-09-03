#include <ImathMatrix.h>

void
matrix33_example()
{
    Imath::M33f M; // uninitialized

    M.makeIdentity();
    assert (M[0][0] == 1.0f);

    Imath::M33f Minv = M.inverse();
}

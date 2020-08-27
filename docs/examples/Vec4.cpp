#include <OpenEXR/ImathVec.h>

int
main()
{
    Imath::V4f   a (1.0f, 2.0f, 3.0f, 4.0f);
    Imath::V4f   b; // b is uninitialized

    b.x = a[0];
    b.y = a[1];
    b.z = a[2];
    b.w = a[3];

    assert (a == b);

    return 0;
}

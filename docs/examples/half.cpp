#include <OpenEXR/half.h>

int
main()
{
    half a (3.5);
    float b (a + sqrt (a));
    a += b;
    b += a;
    b = a + 7;

    return 0;
}


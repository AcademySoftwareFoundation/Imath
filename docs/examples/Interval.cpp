#include <OpenEXR/ImathInterval.h>

int
main()
{
    Imath::Intervalf v; // empty by default

    v.extendBy (1.0f);
    v.extendBy (2.0f);

    assert (v.intersects (0.5f));

    return 0;
}

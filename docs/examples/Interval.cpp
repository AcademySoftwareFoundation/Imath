#include <ImathInterval.h>

void
interval_example()
{
    Imath::Intervalf v; // empty by default

    v.extendBy (1.0f);
    v.extendBy (2.0f);

    assert (v.intersects (1.5f));
}

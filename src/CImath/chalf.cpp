#include <../Imath/half.h>

#include "chalf.h"

static_assert (sizeof (IMATH_NAMESPACE::half) == sizeof (imath_half));

extern "C"
{
    imath_half imath_half_create (float f)
    {
        IMATH_NAMESPACE::half h (f);
        imath_half* c_h = reinterpret_cast<imath_half*> (&h);

        return *c_h;
    }

    imath_half imath_half_copy (const imath_half* h)
    {
        imath_half h_copy;
        h_copy._h = h->_h;

        return h_copy;
    }

    float imath_half_toFloat (const imath_half* h)
    {
        const IMATH_NAMESPACE::half* cpp_h = reinterpret_cast<const IMATH_NAMESPACE::half*> (h);
        float f (*cpp_h);
        return f;
    }
}

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "../CImath/chalf.h"

#define TEST(x)                                                                                    \
    if (argc < 2 || !strcmp (argv[1], #x))                                                         \
    {                                                                                              \
        int result = x();                                                                          \
                                                                                                   \
        if (result != 0)                                                                           \
        {                                                                                          \
            return result;                                                                         \
        }                                                                                          \
    }

const float test_values[] = { 1.0, 0.0, -1.0, 123.0, 1234.0, 0.25 };

int
testHalfToFloat()
{
    printf ("Create float from half\n");

    for (int i = 0; i < (sizeof (test_values) / sizeof (test_values[0])); i++)
    {
        float f_in   = test_values[i];
        imath_half h = imath_half_create (f_in);
        float f_out  = imath_half_toFloat (&h);

        if (fabs (f_in - f_out) > FLT_EPSILON)
        {
            printf ("%f != %f\n", f_in, f_out);
            return 1;
        }
    }

    return 0;
}

int
main (int argc, char* argv[])
{
    TEST (testHalfToFloat);

    return 0;
}

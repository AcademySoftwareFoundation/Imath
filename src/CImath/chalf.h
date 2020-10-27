#ifndef _CHALF_H_
#define _CHALF_H_

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        unsigned short _h;
    } imath_half;

    // Constructors
    imath_half imath_half_create (float f);
    imath_half imath_half_copy (const imath_half* h);

    // Conversion to float
    float imath_half_toFloat (const imath_half* h);

#ifdef __cplusplus
}
#endif
#endif

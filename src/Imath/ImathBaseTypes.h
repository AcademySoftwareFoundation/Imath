//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

/// @file

#ifndef INCLUDED_IMATHBASETYPES_H
#define INCLUDED_IMATHBASETYPES_H 1

#pragma once

#include <math.h>
#include <stdint.h>

// These should be compatible with C as well, just make sure
#ifdef __cplusplus
extern "C" {
#endif

/// @defgroup BaseTypes Simple type definitions for use in public API
///
/// This file contains structures which define stable definitions for
/// data storage layout. These types are provided here such that they
/// can be used in any public API, and are not subject to namespace
/// changes or other potential symbol name changes, providing a stable
/// ABI. This then frees up the Imath types to have custom adapters or
/// layouts in the future to better optimize for local hardware, or
/// whatever it may choose to do, but the ABI can remain stable.
///
/// @{
///

#define IMATH_V2_TYPE(T, suffix)                                               \
    typedef struct                                                             \
    {                                                                          \
        union                                                                  \
        {                                                                      \
            struct                                                             \
            {                                                                  \
                T x, y;                                                        \
            };                                                                 \
            T arr[2];                                                          \
        };                                                                     \
    } imath_v2##suffix##_t

/// imath_v2b_t
IMATH_V2_TYPE (uint8_t, b);
/// imath_v2s_t
IMATH_V2_TYPE (int16_t, s);
/// imath_v2i_t
IMATH_V2_TYPE (int32_t, i);
/// imath_v2i64_t
IMATH_V2_TYPE (int64_t, i64);
/// imath_v2f_t
IMATH_V2_TYPE (float, f);
/// imath_v2d_t
IMATH_V2_TYPE (double, d);

#undef IMATH_V2_TYPE

////////////////////////////////////////

#define IMATH_V3_TYPE(T, suffix)                                               \
    typedef struct                                                             \
    {                                                                          \
        union                                                                  \
        {                                                                      \
            struct                                                             \
            {                                                                  \
                T x, y, z;                                                     \
            };                                                                 \
            T arr[3];                                                          \
        };                                                                     \
    } imath_v3##suffix##_t

/// imath_v3b_t
IMATH_V3_TYPE (uint8_t, b);
/// imath_v3s_t
IMATH_V3_TYPE (int16_t, s);
/// imath_v3i_t
IMATH_V3_TYPE (int32_t, i);
/// imath_v3i64_t
IMATH_V3_TYPE (int64_t, i64);
/// imath_v3f_t
IMATH_V3_TYPE (float, f);
/// imath_v3d_t
IMATH_V3_TYPE (double, d);

#undef IMATH_V3_TYPE

////////////////////////////////////////

#define IMATH_V4_TYPE(T, suffix)                                               \
    typedef struct                                                             \
    {                                                                          \
        union                                                                  \
        {                                                                      \
            struct                                                             \
            {                                                                  \
                T x, y, z, w;                                                  \
            };                                                                 \
            T arr[4];                                                          \
        };                                                                     \
    } imath_v4##suffix##_t

/// imath_v4b_t
IMATH_V4_TYPE (uint8_t, b);
/// imath_v4s_t
IMATH_V4_TYPE (int16_t, s);
/// imath_v4i_t
IMATH_V4_TYPE (int32_t, i);
/// imath_v4i64_t
IMATH_V4_TYPE (int64_t, i64);
/// imath_v4f_t
IMATH_V4_TYPE (float, f);
/// imath_v4d_t
IMATH_V4_TYPE (double, d);

#undef IMATH_V4_TYPE

////////////////////////////////////////////////////////////////////////////////

#define IMATH_BOX_DIM_TYPE(D, T)                                               \
    typedef struct                                                             \
    {                                                                          \
        imath_v##D##T##_t min;                                                 \
        imath_v##D##T##_t max;                                                 \
    } imath_box##D##T##_t

/// imath_box2b_t
IMATH_BOX_DIM_TYPE (2, b);
/// imath_box2s_t
IMATH_BOX_DIM_TYPE (2, s);
/// imath_box2i_t
IMATH_BOX_DIM_TYPE (2, i);
/// imath_box2i64_t
IMATH_BOX_DIM_TYPE (2, i64);
/// imath_box2f_t
IMATH_BOX_DIM_TYPE (2, f);
/// imath_box2d_t
IMATH_BOX_DIM_TYPE (2, d);

/// imath_box3b_t
IMATH_BOX_DIM_TYPE (3, b);
/// imath_box3s_t
IMATH_BOX_DIM_TYPE (3, s);
/// imath_box3i_t
IMATH_BOX_DIM_TYPE (3, i);
/// imath_box3i64_t
IMATH_BOX_DIM_TYPE (3, i64);
/// imath_box3f_t
IMATH_BOX_DIM_TYPE (3, f);
/// imath_box3d_t
IMATH_BOX_DIM_TYPE (3, d);

#undef IMATH_BOX_DIM_TYPE

////////////////////////////////////////

#define IMATH_MATRIX_TYPE(D, T, suffix)                                        \
    typedef struct                                                             \
    {                                                                          \
        T x[D][D];                                                             \
    } imath_m##D##D##suffix##_t

/// imath_m22f_t
IMATH_MATRIX_TYPE (2, float, f);
/// imath_m33f_t
IMATH_MATRIX_TYPE (3, float, f);
/// imath_m44f_t
IMATH_MATRIX_TYPE (4, float, f);

/// imath_m22d_t
IMATH_MATRIX_TYPE (2, double, d);
/// imath_m33d_t
IMATH_MATRIX_TYPE (3, double, d);
/// imath_m44d_t
IMATH_MATRIX_TYPE (4, double, d);

#undef IMATH_MATRIX_TYPE

/// @}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // INCLUDED_IMATHBASETYPES_H

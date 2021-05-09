//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//
//#define IMATH_HALF_NO_TABLES_AT_ALL
//#define IMATH_HALF_EXCEPTIONS_ENABLED
//
#define IMATH_USE_ORIGINAL_HALF_IMPLEMENTATION
#include <half.h>

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <memory>

using namespace IMATH_NAMESPACE;

void
perf_test_half_to_float (float *floats, const uint16_t *halfs, int numentries)
{
    struct timespec startn, endn;
    const half *halfvals = reinterpret_cast<const half *>( halfs );

    clock_gettime (CLOCK_MONOTONIC, &startn);
    for (int i = 0; i < numentries; ++i)
        floats[i] = imath_half_to_float (halfs[i]);
    clock_gettime (CLOCK_MONOTONIC, &endn);

    struct timespec starto, endo;
    clock_gettime (CLOCK_MONOTONIC, &starto);
    for (int i = 0; i < numentries; ++i)
        floats[i] = static_cast<float>( halfvals[i] );
    clock_gettime (CLOCK_MONOTONIC, &endo);

    uint64_t nnanos, onanos;
    if (endn.tv_nsec < startn.tv_nsec)
    {
        endn.tv_nsec += 1000000000;
        endn.tv_sec -= 1;
    }
    if (endo.tv_nsec < starto.tv_nsec)
    {
        endo.tv_nsec += 1000000000;
        endo.tv_sec -= 1;
    }

    onanos = (endo.tv_nsec - starto.tv_nsec) + 1000000000 * (endo.tv_sec - starto.tv_sec);
    nnanos = (endn.tv_nsec - startn.tv_nsec) + 1000000000 * (endn.tv_sec - startn.tv_sec);
    fprintf (stderr,
             "half -> float Old: %10lu (%g ns) New: %10lu (%g ns) (%10ld)\n",
             onanos,
             (double) onanos / ((double) numentries),
             nnanos,
             (double) nnanos / ((double) numentries),
             (int64_t) onanos - (int64_t) nnanos);
}

void
perf_test_float_to_half (uint16_t *halfs, const float *floats, int numentries)
{
    struct timespec startn, endn;
    half *halfvals = reinterpret_cast<half *>( halfs );

    clock_gettime (CLOCK_MONOTONIC, &startn);
    for (int i = 0; i < numentries; ++i)
        halfs[i] = imath_float_to_half (floats[i]);
    clock_gettime (CLOCK_MONOTONIC, &endn);

    struct timespec starto, endo;
    clock_gettime (CLOCK_MONOTONIC, &starto);
    for (int i = 0; i < numentries; ++i)
        halfvals[i] = half (floats[i]);
    clock_gettime (CLOCK_MONOTONIC, &endo);

    uint64_t nnanos, onanos;
    if (endn.tv_nsec < startn.tv_nsec)
    {
        endn.tv_nsec += 1000000000;
        endn.tv_sec -= 1;
    }
    if (endo.tv_nsec < starto.tv_nsec)
    {
        endo.tv_nsec += 1000000000;
        endo.tv_sec -= 1;
    }

    onanos = (endo.tv_nsec - starto.tv_nsec) + 1000000000 * (endo.tv_sec - starto.tv_sec);
    nnanos = (endn.tv_nsec - startn.tv_nsec) + 1000000000 * (endn.tv_sec - startn.tv_sec);
    fprintf (stderr,
             "float -> half Old: %10lu (%g ns) New: %10lu (%g ns) (%10ld)\n",
             onanos,
             (double) onanos / ((double) numentries),
             nnanos,
             (double) nnanos / ((double) numentries),
             (int64_t) onanos - (int64_t) nnanos);
}

int
main (int argc, char* argv[])
{
    int ret = 0;
    int numentries = 1920*1080*3;
    if (argc > 1)
    {
        numentries = atoi (argv[1]);

        if (numentries <= 0)
        {
            fprintf (stderr, "Bad entry count '%s'\n", argv[1]);
            ret = 1;
        }
    }

    if ( numentries > 0 )
    {
        uint16_t* halfs = new uint16_t[numentries];
        float* floats   = new float[numentries];

        if (halfs && floats)
        {
            srand (numentries);
            for (int i = 0; i < numentries; ++i)
            {
                halfs[i] = (uint16_t) (rand());
                floats[i] = imath_half_to_float (halfs[i]);
            }
            perf_test_half_to_float( floats, halfs, numentries );

            // test float -> half with real-world values
            for (int i = 0; i < numentries; ++i)
                floats[i] = 65504.0 * (drand48() * 2.0 - 1.0);
            perf_test_float_to_half( halfs, floats, numentries );
        }

        delete [] halfs;
        delete [] floats;
    }
    
    return ret;
}

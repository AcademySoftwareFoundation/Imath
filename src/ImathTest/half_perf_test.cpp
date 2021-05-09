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
#ifdef _MSC_VER
#    include <windows.h>
#else
#    include <time.h>
#endif

#include <memory>

using namespace IMATH_NAMESPACE;

uint64_t get_ticks( void )
{
#ifdef _MSC_VER
    static uint64_t scale = 0;
    if ( scale == 0 )
    {
        LARGE_INTEGER freq;
        QueryPerformanceFrequency ( &freq );
        scale = (1000000000 / freq.QuadPart);
    }

    LARGE_INTEGER ticks;
    QueryPerformanceCounter ( &ticks );
    return ticks.QuadPart * scale;
#else
    struct timespec t;
    uint64_t nsecs;

    static uint64_t start = 0;
    if ( start == 0 )
    {
        clock_gettime (CLOCK_MONOTONIC, &t);
        start = t.tv_sec;
    }

    clock_gettime (CLOCK_MONOTONIC, &t);
    nsecs = (t.tv_sec - start) * 1000000000;
    nsecs += t.tv_nsec;
    return nsecs;
#endif
}

void
perf_test_half_to_float (float *floats, const uint16_t *halfs, int numentries)
{
    const half *halfvals = reinterpret_cast<const half *>( halfs );

    uint64_t st = get_ticks();
    for (int i = 0; i < numentries; ++i)
        floats[i] = imath_half_to_float (halfs[i]);
    uint64_t et = get_ticks();

    uint64_t ost = get_ticks();
    for (int i = 0; i < numentries; ++i)
        floats[i] = static_cast<float>( halfvals[i] );
    uint64_t oet = get_ticks();

    uint64_t onanos = (oet - ost);
    uint64_t nnanos = (et - st);
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
    half *halfvals = reinterpret_cast<half *>( halfs );

    uint64_t st = get_ticks();
    for (int i = 0; i < numentries; ++i)
        halfs[i] = imath_float_to_half (floats[i]);
    uint64_t et = get_ticks();

    uint64_t ost = get_ticks();
    for (int i = 0; i < numentries; ++i)
        halfvals[i] = half (floats[i]);
    uint64_t oet = get_ticks();

    uint64_t onanos = (oet - ost);
    uint64_t nnanos = (et - st);
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

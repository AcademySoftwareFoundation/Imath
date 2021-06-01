//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifdef NDEBUG
#    undef NDEBUG
#endif

#include <ImathInterval.h>
#include <ImathRandom.h>
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <vector>

// Include ImathForward *after* other headers to validate forward declarations
#include <ImathForward.h>

using namespace std;
using namespace IMATH_INTERNAL_NAMESPACE;

namespace
{

template <class T>
static void
testConstructors (const char* type)
{
    cout << "    constructors for type " << type << endl;

    //
    // Empty
    //
    {
        IMATH_INTERNAL_NAMESPACE::Interval<T> b;
        assert (b.min == T (std::numeric_limits<T>::max()) && b.max == T (std::numeric_limits<T>::lowest()));
    }

    //
    // Single point
    //
    {
        T p (42);
        IMATH_INTERNAL_NAMESPACE::Interval<T> b (p);
        assert (b.min == p && b.max == p);
    }

    //
    // Min and max
    //
    {
        T p0 (42);
        T p1 (666);
        IMATH_INTERNAL_NAMESPACE::Interval<T> b (p0, p1);
        assert (b.min == p0 && b.max == p1);
    }

    {
        T p0 (666);
        T p1 (42);
        IMATH_INTERNAL_NAMESPACE::Interval<T> b (p0, p1);
        assert (b.min == p0 && b.max == p1);
    }
}

template <class T>
void
testMakeEmpty (const char* type)
{
    cout << "    makeEmpty() for type " << type << endl;

    //
    // Empty interval
    //
    {
        IMATH_INTERNAL_NAMESPACE::Interval<T> b;
        b.makeEmpty();
        assert (b.min == T (std::numeric_limits<T>::max()) && b.max == T (std::numeric_limits<T>::lowest()));
    }

    //
    // Non-empty, has volume
    //
    {
        IMATH_INTERNAL_NAMESPACE::Interval<T> b (T (-1), T (1));
        b.makeEmpty();
        assert (b.min == T (std::numeric_limits<T>::max()) && b.max == T (std::numeric_limits<T>::lowest()));
    }

    //
    // Non-empty, no volume
    //
    {
        T min (0);
        T max (10);
        IMATH_INTERNAL_NAMESPACE::Interval<T> b (min, max);
        b.makeEmpty();
        assert (b.min == T (std::numeric_limits<T>::max()) && b.max == T (std::numeric_limits<T>::lowest()));
    }
}

template <class T>
void
testMakeInfinite (const char* type)
{
    cout << "    makeInfinite() for type " << type << endl;

    //
    // Infinite interval
    //
    {
        IMATH_INTERNAL_NAMESPACE::Interval<T> b;
        b.makeInfinite();
        assert (b.min == T (std::numeric_limits<T>::lowest()) && b.max == T (std::numeric_limits<T>::max()));
    }

    //
    // Non-empty, has volume
    //
    {
        IMATH_INTERNAL_NAMESPACE::Interval<T> b (T (-1), T (1));
        b.makeInfinite();
        assert (b.min == T (std::numeric_limits<T>::lowest()) && b.max == T (std::numeric_limits<T>::max()));
    }

    //
    // Non-empty, no volume
    //
    {
        T min (0);
        T max (1);

        IMATH_INTERNAL_NAMESPACE::Interval<T> b (min, max);
        b.makeInfinite();
        assert (b.min == T (std::numeric_limits<T>::lowest()) && b.max == T (std::numeric_limits<T>::max()));
    }
}

template <class T>
void
testExtendByPoint (const char* type)
{
    cout << "    extendBy() point for type " << type << endl;

    IMATH_INTERNAL_NAMESPACE::Rand32 rand (0);

    const unsigned int iters = 10;

    //
    // Extend empty interval with a single point.
    //
    for (unsigned int i = 0; i < iters; i++)
    {
        T p (rand.nextf (-12345, 12345));
        IMATH_INTERNAL_NAMESPACE::Interval<T> b;
        b.extendBy (p);
        assert (b.min == p && b.max == p);
    }

    //
    // Extend empty interval with a number of random points. Note that
    // this also covers extending a non-empty interval.
    //
    for (unsigned int i = 0; i < iters; i++)
    {
        IMATH_INTERNAL_NAMESPACE::Interval<T> b;

        T min;
        T max;

        for (unsigned int j = 0; j < i; j++)
        {
            T p (rand.nextf (-12345, 12345));

            if (j == 0)
            {
                min = p;
                max = p;
            }

            min = std::min (min, p);
            max = std::max (max, p);

            b.extendBy (p);

            assert (b.min == min && b.max == max);
        }
    }
}

template <class T>
void
testExtendByInterval (const char* type)
{
    cout << "    extendBy() interval for type " << type << endl;

    //
    // Extend empty interval with an empty interval
    //
    {
        IMATH_INTERNAL_NAMESPACE::Interval<T> b;
        b.extendBy (IMATH_INTERNAL_NAMESPACE::Interval<T>());
        assert (b.min == T (std::numeric_limits<T>::max()) && b.max == T (std::numeric_limits<T>::lowest()));
    }

    //
    // Extend empty interval with a non-empty interval and vice versa.
    //
    {
        T p0 (-1);
        T p1 (1);

        IMATH_INTERNAL_NAMESPACE::Interval<T> b0;
        b0.extendBy (IMATH_INTERNAL_NAMESPACE::Interval<T> (p0, p1));
        assert (b0.min == p0 && b0.max == p1);

        IMATH_INTERNAL_NAMESPACE::Interval<T> b1 (p0, p1);
        b1.extendBy (IMATH_INTERNAL_NAMESPACE::Interval<T>());
        assert (b1.min == p0 && b1.max == p1);
    }

    //
    // Extend non-empty interval with non-empty interval. Starts with empty, then builds.
    //
    IMATH_INTERNAL_NAMESPACE::Rand32 rand (0);
    const unsigned int iters = 10;
    {
        IMATH_INTERNAL_NAMESPACE::Interval<T> b;

        T min, max;

        for (unsigned int i = 1; i < iters; i++)
        {
            T p0 (rand.nextf (0, 999));
            T p1 (rand.nextf (1000, 1999));

            min = b.min;
            max = b.max;
            min = std::min (min, p0);
            max = std::max (max, p1);

            b.extendBy (IMATH_INTERNAL_NAMESPACE::Interval<T> (p0, p1));

            assert (b.min == min && b.max == max);
        }
    }
}

template <class T>
void
testComparators (const char* type)
{
    cout << "    comparators for type " << type << endl;

    IMATH_INTERNAL_NAMESPACE::Rand32 rand (0);

    //
    // Compare empty.
    //
    {
        IMATH_INTERNAL_NAMESPACE::Interval<T> b0;
        IMATH_INTERNAL_NAMESPACE::Interval<T> b1;

        assert (b0 == b1);
        assert (!(b0 != b1));
    }

    //
    // Compare empty to non-empty.
    //
    {
        T p0 (-1);
        T p1 (1);

        IMATH_INTERNAL_NAMESPACE::Interval<T> b0;
        IMATH_INTERNAL_NAMESPACE::Interval<T> b1 (p0, p1);
        assert (!(b0 == b1));
        assert (b0 != b1);
    }

    //
    // Compare two non-empty
    //
    {
        T p0 (-1);
        T p1 (1);

        T p2 (-2);
        T p3 (2);

        IMATH_INTERNAL_NAMESPACE::Interval<T> b0 (p0, p1);
        IMATH_INTERNAL_NAMESPACE::Interval<T> b1 (p2, p3);
        IMATH_INTERNAL_NAMESPACE::Interval<T> b2 (p0, p1);

        assert (b0 != b1);
        assert (!(b0 == b1));

        assert (b0 == b2);
        assert (!(b0 != b2));
    }
}

template <class T>
void
testIntersects (const char* type)
{
    cout << "    intersects() for type " << type << endl;

    IMATH_INTERNAL_NAMESPACE::Rand32 rand (0);

    //
    // Intersect point with empty interval.
    //
    {
        IMATH_INTERNAL_NAMESPACE::Interval<T> b;
        T p (1);

        assert (!b.intersects (p));
    }

    //
    // Intersect point with non-empty, has-volume interval.
    //
    {
        IMATH_INTERNAL_NAMESPACE::Interval<T> b (T (-1), T (1));
        T p0 (0);
        T p1 (5);
        T p2 (-5);

        assert (b.intersects (p0));
        assert (!b.intersects (p1));
        assert (!b.intersects (p2));
    }

    //
    // Intersect point with non-empty, no-volume interval.
    //
    {
        T min (0);
        T max (1);

        T p0 (0);
        T p1 (5);
        IMATH_INTERNAL_NAMESPACE::Interval<T> b (min, max);

        assert (b.intersects (p0));
        assert (!b.intersects (p1));
    }

    //
    // Intersect empty interval with empty interval.
    //
    {
        IMATH_INTERNAL_NAMESPACE::Interval<T> b0;
        IMATH_INTERNAL_NAMESPACE::Interval<T> b1;

        assert (!b0.intersects (b1));
        assert (!b1.intersects (b0));
    }

    //
    // Intersect empty interval with non-empty has-volume intervales.
    //
    {
        IMATH_INTERNAL_NAMESPACE::Interval<T> b0;
        IMATH_INTERNAL_NAMESPACE::Interval<T> b1 (T (-1), T (1));
        IMATH_INTERNAL_NAMESPACE::Interval<T> b2 (T (1), T (2));

        assert (!b0.intersects (b1));
        assert (!b0.intersects (b2));

        assert (!b1.intersects (b0));
        assert (!b2.intersects (b0));
    }

    //
    // Intersect empty interval with non-empty no-volume interval.
    //
    {
        T min (0);
        T max                    = min;
        max[T::dimensions() - 1] = 1;

        IMATH_INTERNAL_NAMESPACE::Interval<T> b0;
        IMATH_INTERNAL_NAMESPACE::Interval<T> b1 (min, max);

        assert (!b0.intersects (b1));
        assert (!b1.intersects (b0));
    }

    //
    // Intersect non-empty has-volume interval with non-empty has-volume interval.
    //
    {
        IMATH_INTERNAL_NAMESPACE::Interval<T> b1 (T (-1), T (1));
        IMATH_INTERNAL_NAMESPACE::Interval<T> b2 (T (-1), T (1));
        IMATH_INTERNAL_NAMESPACE::Interval<T> b3 (T (1), T (2));
        IMATH_INTERNAL_NAMESPACE::Interval<T> b4 (T (2), T (3));

        assert (b1.intersects (b1));
        assert (b1.intersects (b3));
        assert (!b1.intersects (b4));

        assert (b3.intersects (b1));
        assert (!b4.intersects (b1));
    }

    //
    // Intersect non-empty has-volume interval with non-empty no-volume interval.
    //
    {
        IMATH_INTERNAL_NAMESPACE::Interval<T> b0 (T (-1), T (1));

        T min (0);
        T max (1);

        IMATH_INTERNAL_NAMESPACE::Interval<T> b1 (min, max);
        IMATH_INTERNAL_NAMESPACE::Interval<T> b2 (min + T (2), max + T (2));

        assert (b0.intersects (b1));
        assert (b1.intersects (b0));

        assert (!b0.intersects (b2));
        assert (!b2.intersects (b1));
    }

    //
    // Intersect non-empty no-volume interval with non-empty no-volume interval.
    //
    {
        T min (0);
        T max (1);

        IMATH_INTERNAL_NAMESPACE::Interval<T> b0 (min, max);
        IMATH_INTERNAL_NAMESPACE::Interval<T> b1 (min, max + T (2));
        IMATH_INTERNAL_NAMESPACE::Interval<T> b2 (min + T (2), max + T (2));

        assert (b0.intersects (b1));
        assert (b1.intersects (b0));

        assert (!b0.intersects (b2));
        assert (!b2.intersects (b0));
    }
}

template <class T>
void
testSize (const char* type)
{
    cout << "    size() for type " << type << endl;

    //
    // Size of empty interval.
    //
    {
        IMATH_INTERNAL_NAMESPACE::Interval<T> b;
        assert (b.size() == T (0));
    }

    //
    // Size of non-empty, has-volume interval.
    //
    {
        IMATH_INTERNAL_NAMESPACE::Interval<T> b0 (T (-1), T (1));
        assert (b0.size() == T (2));
 
        T p (42);

        IMATH_INTERNAL_NAMESPACE::Interval<T> b1 (-p, p);
        assert (b1.size() == p * T (2));
    }

    //
    // Size of non-empty, no-volume interval.
    //
    {
        T min (0);
        T max (1);

        IMATH_INTERNAL_NAMESPACE::Interval<T> b (min, max);

        assert (b.size() == max);
    }
}

template <class T>
void
testCenter (const char* type)
{
    cout << "    center() for type " << type << endl;

    //
    // Center of empty interval.
    //
    {
        IMATH_INTERNAL_NAMESPACE::Interval<T> b;
        assert (b.center() == T (0));
    }

    //
    // Center of non-empty, has-volume interval.
    //
    {
        IMATH_INTERNAL_NAMESPACE::Interval<T> b0 (T (-1), T (1));
        assert (b0.center() == T (0));

        T p0 (1);
        T p1 (2);

        IMATH_INTERNAL_NAMESPACE::Interval<T> b1 (p0, p1);
        assert (b1.center() == (p1 + p0) / 2);
    }

    //
    // Center of non-empty, no-volume interval.
    //
    {
        T min (0);
        T max (2);

        IMATH_INTERNAL_NAMESPACE::Interval<T> b (min, max);

        assert (b.center() == max / 2);
    }
}

template <class T>
void
testIsEmpty (const char* type)
{
    cout << "    isEmpty() for type " << type << endl;

    //
    // Empty interval.
    //
    {
        IMATH_INTERNAL_NAMESPACE::Interval<T> b;
        assert (b.isEmpty());
    }

    //
    // Non-empty, has-volume interval.
    //
    {
        IMATH_INTERNAL_NAMESPACE::Interval<T> b0 (T (-1), T (1));
        assert (!b0.isEmpty());

        T p0 (2);
        T p1 (4);
        IMATH_INTERNAL_NAMESPACE::Interval<T> b1 (p0, p1);
        assert (!b1.isEmpty());
    }

    //
    // Non-empty, no-volume interval.
    //
    {
        T min (0);
        T max (2);

        IMATH_INTERNAL_NAMESPACE::Interval<T> b (min, max);

        assert (!b.isEmpty());
    }
}

template <class T>
void
testIsInfinite (const char* type)
{
    cout << "    isInfinite() for type " << type << endl;

    //
    // Infinite interval.
    //
    {
        IMATH_INTERNAL_NAMESPACE::Interval<T> b;
        b.makeInfinite();
        assert (b.isInfinite());
    }

    //
    // Non-empty, has-volume interval.
    //
    {
        IMATH_INTERNAL_NAMESPACE::Interval<T> b0 (T (-1), T (1));
        assert (!b0.isInfinite());

        T p0 (2);
        T p1 (4);

        IMATH_INTERNAL_NAMESPACE::Interval<T> b1 (p0, p1);
        assert (!b1.isInfinite());
    }

    //
    // Non-empty, no-volume interval.
    //
    {
        T min (0);
        T max (2);

        IMATH_INTERNAL_NAMESPACE::Interval<T> b (min, max);

        assert (!b.isInfinite());
    }
}

template <class T>
void
testHasVolume (const char* type)
{
    cout << "    hasVolume() for type " << type << endl;

    //
    // Empty interval.
    //
    {
        IMATH_INTERNAL_NAMESPACE::Interval<T> b;
        assert (!b.hasVolume());
    }

    //
    // Infinite interval.
    //
    {
        IMATH_INTERNAL_NAMESPACE::Interval<T> b;
        b.makeInfinite();
        assert (b.hasVolume());
    }

    //
    // Non-empty, has-volume interval.
    //
    {
        IMATH_INTERNAL_NAMESPACE::Interval<T> b0 (T (-1), T (1));
        assert (b0.hasVolume());

        T p0 (2);
        T p1 (4);

        IMATH_INTERNAL_NAMESPACE::Interval<T> b1 (p0, p1);
        assert (b1.hasVolume());
    }

    //
    // Non-empty, no-volume interval.
    //
    {
        T min (0);
        T max (2);

        IMATH_INTERNAL_NAMESPACE::Interval<T> b (min, max);
        b.makeEmpty();
        assert (!b.hasVolume());
    }
}

template <class T>
void
testStream (const char* type)
{
    cout << "    hasVolume() for type " << type << endl;

    T min (0);
    T max (1);
    
    IMATH_INTERNAL_NAMESPACE::Interval<T> b (min, max);
    std::stringstream s1;
    s1 << '(' << min << ' ' << max << ')';

    std::stringstream s2;
    s2 << b;

    assert (s1.str() == s2.str());
}

} // anonymous namespace

void
testInterval()
{
    cout << "Testing interval methods" << endl;

    //
    // Constructors
    //
    testConstructors<short> ("short");
    testConstructors<int> ("int");
    testConstructors<float> ("float");
    testConstructors<double> ("double");

    //
    // makeEmpty()
    //
    testMakeEmpty<short> ("short");
    testMakeEmpty<int> ("int");
    testMakeEmpty<float> ("float");
    testMakeEmpty<double> ("double");

    //
    // makeInfinite()
    //
    testMakeInfinite<short> ("short");
    testMakeInfinite<int> ("int");
    testMakeInfinite<float> ("float");
    testMakeInfinite<double> ("double");
    
    //
    // extendBy() (point)
    //
    testExtendByPoint<short> ("short");
    testExtendByPoint<int> ("int");
    testExtendByPoint<float> ("float");
    testExtendByPoint<double> ("double");

    //
    // extendBy() interval
    //
    testExtendByInterval<short> ("short");
    testExtendByInterval<int> ("int");
    testExtendByInterval<float> ("float");
    testExtendByInterval<double> ("double");

    //
    // == and !==
    //
    testComparators<short> ("short");
    testComparators<int> ("int");
    testComparators<float> ("float");
    testComparators<double> ("double");

    //
    // size()
    //
    testSize<short> ("short");
    testSize<int> ("int");
    testSize<float> ("float");
    testSize<double> ("double");

    //
    // center()
    //
    testCenter<short> ("short");
    testCenter<int> ("int");
    testCenter<float> ("float");
    testCenter<double> ("double");

    //
    // isEmpty()
    //
    testIsEmpty<short> ("short");
    testIsEmpty<int> ("int");
    testIsEmpty<float> ("float");
    testIsEmpty<double> ("double");

    //
    // isInfinite()
    //
    testIsInfinite<short> ("short");
    testIsInfinite<int> ("int");
    testIsInfinite<float> ("float");
    testIsInfinite<double> ("double");
    
    //
    // hasVolume()
    //
    testHasVolume<short> ("short");
    testHasVolume<int> ("int");
    testHasVolume<float> ("float");
    testHasVolume<double> ("double");

    //
    // stream
    //
    testStream<short> ("short");
    testStream<int> ("int");
    testStream<float> ("float");
    testStream<double> ("double");

    cout << "ok\n" << endl;
}

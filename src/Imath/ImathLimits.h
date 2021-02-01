//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

//
// Limitations of the basic C++ numerical data types
//

#ifndef INCLUDED_IMATHLIMITS_H
#define INCLUDED_IMATHLIMITS_H

#include "ImathNamespace.h"
#include <float.h>
#include <limits.h>

//------------------------------------------
// In Windows, min and max are macros.  Yay.
//------------------------------------------

#if defined _WIN32 || defined _WIN64
#    ifdef min
#        undef min
#    endif
#    ifdef max
#        undef max
#    endif
#endif

IMATH_INTERNAL_NAMESPACE_HEADER_ENTER

//
// Template class limits<T> returns information about the limits
// of numerical data type T:
//
//	min()		largest possible negative value of type T
//
//	max()		largest possible positive value of type T
//
//	smallest()	smallest possible positive value of type T
//			(for float and double: smallest normalized
//			positive value)
//
//	epsilon()	smallest possible e of type T, for which
//			1 + e != 1
//
//	isIntegral()	returns true if T is an integral type
//
//	isSigned()	returns true if T is signed
//
// Class limits<T> is useful to implement template classes or
// functions which depend on the limits of a numerical type
// which is not known in advance; for example:
//
//	template <class T> max (T x[], int n)
//	{
//	    T m = limits<T>::min();
//
//	    for (int i = 0; i < n; i++)
//		if (m < x[i])
//		    m = x[i];
//
//	    return m;
//	}
//
// Class limits<T> has been implemented for the following types:
//
//	char, signed char, unsigned char
//	short, unsigned short
//	int, unsigned int
//	long, unsigned long
//	float
//	double
//	long double
//
// Class limits<T> has only static member functions, all of which
// are implemented as inlines.  No objects of type limits<T> are
// ever created.
//

///
/// Template class limits<T> returns information about the limits
/// of numerical data type T.
///

template <class T> struct limits
{
    /// Largest possible negative value
    IMATH_HOSTDEVICE static constexpr T min() noexcept;

    /// Largest possible positive value
    IMATH_HOSTDEVICE static constexpr T max() noexcept;

    /// Smallest possible positive value
    IMATH_HOSTDEVICE static constexpr T smallest() noexcept;

    /// Smallest possible e for which 1+e != 1
    IMATH_HOSTDEVICE static constexpr T epsilon() noexcept;

    /// Return true if the type is integral.
    IMATH_HOSTDEVICE static constexpr bool isIntegral() noexcept;

    /// Return true if the type is signed.
    IMATH_HOSTDEVICE static constexpr bool isSigned() noexcept;
};

/// @cond Doxygen_Suppress

template <> struct limits<char>
{
    IMATH_HOSTDEVICE static constexpr char min() noexcept { return CHAR_MIN; }
    IMATH_HOSTDEVICE static constexpr char max() noexcept { return CHAR_MAX; }
    IMATH_HOSTDEVICE static constexpr char smallest() noexcept { return 1; }
    IMATH_HOSTDEVICE static constexpr char epsilon() noexcept { return 1; }
    IMATH_HOSTDEVICE static constexpr bool isIntegral() noexcept { return true; }
    IMATH_HOSTDEVICE static constexpr bool isSigned() noexcept { return (char) ~0 < 0; }
};

template <> struct limits<signed char>
{
    IMATH_HOSTDEVICE static constexpr signed char min() noexcept { return SCHAR_MIN; }
    IMATH_HOSTDEVICE static constexpr signed char max() noexcept { return SCHAR_MAX; }
    IMATH_HOSTDEVICE static constexpr signed char smallest() noexcept { return 1; }
    IMATH_HOSTDEVICE static constexpr signed char epsilon() noexcept { return 1; }
    IMATH_HOSTDEVICE static constexpr bool isIntegral() noexcept { return true; }
    IMATH_HOSTDEVICE static constexpr bool isSigned() noexcept { return true; }
};

template <> struct limits<unsigned char>
{
    IMATH_HOSTDEVICE static constexpr unsigned char min() noexcept { return 0; }
    IMATH_HOSTDEVICE static constexpr unsigned char max() noexcept { return UCHAR_MAX; }
    IMATH_HOSTDEVICE static constexpr unsigned char smallest() noexcept { return 1; }
    IMATH_HOSTDEVICE static constexpr unsigned char epsilon() noexcept { return 1; }
    IMATH_HOSTDEVICE static constexpr bool isIntegral() noexcept { return true; }
    IMATH_HOSTDEVICE static constexpr bool isSigned() noexcept { return false; }
};

template <> struct limits<short>
{
    IMATH_HOSTDEVICE static constexpr short min() noexcept { return SHRT_MIN; }
    IMATH_HOSTDEVICE static constexpr short max() noexcept { return SHRT_MAX; }
    IMATH_HOSTDEVICE static constexpr short smallest() noexcept { return 1; }
    IMATH_HOSTDEVICE static constexpr short epsilon() noexcept { return 1; }
    IMATH_HOSTDEVICE static constexpr bool isIntegral() noexcept { return true; }
    IMATH_HOSTDEVICE static constexpr bool isSigned() noexcept { return true; }
};

template <> struct limits<unsigned short>
{
    IMATH_HOSTDEVICE static constexpr unsigned short min() noexcept { return 0; }
    IMATH_HOSTDEVICE static constexpr unsigned short max() noexcept { return USHRT_MAX; }
    IMATH_HOSTDEVICE static constexpr unsigned short smallest() noexcept { return 1; }
    IMATH_HOSTDEVICE static constexpr unsigned short epsilon() noexcept { return 1; }
    IMATH_HOSTDEVICE static constexpr bool isIntegral() noexcept { return true; }
    IMATH_HOSTDEVICE static constexpr bool isSigned() noexcept { return false; }
};

template <> struct limits<int>
{
    IMATH_HOSTDEVICE static constexpr int min() noexcept { return INT_MIN; }
    IMATH_HOSTDEVICE static constexpr int max() noexcept { return INT_MAX; }
    IMATH_HOSTDEVICE static constexpr int smallest() noexcept { return 1; }
    IMATH_HOSTDEVICE static constexpr int epsilon() noexcept { return 1; }
    IMATH_HOSTDEVICE static constexpr bool isIntegral() noexcept { return true; }
    IMATH_HOSTDEVICE static constexpr bool isSigned() noexcept { return true; }
};

template <> struct limits<unsigned int>
{
    IMATH_HOSTDEVICE static constexpr unsigned int min() noexcept { return 0; }
    IMATH_HOSTDEVICE static constexpr unsigned int max() noexcept { return UINT_MAX; }
    IMATH_HOSTDEVICE static constexpr unsigned int smallest() noexcept { return 1; }
    IMATH_HOSTDEVICE static constexpr unsigned int epsilon() noexcept { return 1; }
    IMATH_HOSTDEVICE static constexpr bool isIntegral() noexcept { return true; }
    IMATH_HOSTDEVICE static constexpr bool isSigned() noexcept { return false; }
};

template <> struct limits<long>
{
    IMATH_HOSTDEVICE static constexpr long min() noexcept { return LONG_MIN; }
    IMATH_HOSTDEVICE static constexpr long max() noexcept { return LONG_MAX; }
    IMATH_HOSTDEVICE static constexpr long smallest() noexcept { return 1; }
    IMATH_HOSTDEVICE static constexpr long epsilon() noexcept { return 1; }
    IMATH_HOSTDEVICE static constexpr bool isIntegral() noexcept { return true; }
    IMATH_HOSTDEVICE static constexpr bool isSigned() noexcept { return true; }
};

template <> struct limits<unsigned long>
{
    IMATH_HOSTDEVICE static constexpr unsigned long min() noexcept { return 0; }
    IMATH_HOSTDEVICE static constexpr unsigned long max() noexcept { return ULONG_MAX; }
    IMATH_HOSTDEVICE static constexpr unsigned long smallest() noexcept { return 1; }
    IMATH_HOSTDEVICE static constexpr unsigned long epsilon() noexcept { return 1; }
    IMATH_HOSTDEVICE static constexpr bool isIntegral() noexcept { return true; }
    IMATH_HOSTDEVICE static constexpr bool isSigned() noexcept { return false; }
};

template <> struct limits<float>
{
    IMATH_HOSTDEVICE static constexpr float min() noexcept { return -FLT_MAX; }
    IMATH_HOSTDEVICE static constexpr float max() noexcept { return FLT_MAX; }
    IMATH_HOSTDEVICE static constexpr float smallest() noexcept { return FLT_MIN; }
    IMATH_HOSTDEVICE static constexpr float epsilon() noexcept { return FLT_EPSILON; }
    IMATH_HOSTDEVICE static constexpr bool isIntegral() noexcept { return false; }
    IMATH_HOSTDEVICE static constexpr bool isSigned() noexcept { return true; }
};

template <> struct limits<double>
{
    IMATH_HOSTDEVICE static constexpr double min() noexcept { return -DBL_MAX; }
    IMATH_HOSTDEVICE static constexpr double max() noexcept { return DBL_MAX; }
    IMATH_HOSTDEVICE static constexpr double smallest() noexcept { return DBL_MIN; }
    IMATH_HOSTDEVICE static constexpr double epsilon() noexcept { return DBL_EPSILON; }
    IMATH_HOSTDEVICE static constexpr bool isIntegral() noexcept { return false; }
    IMATH_HOSTDEVICE static constexpr bool isSigned() noexcept { return true; }
};

template <> struct limits<long double>
{
    IMATH_HOSTDEVICE static constexpr long double min() noexcept { return -LDBL_MAX; }
    IMATH_HOSTDEVICE static constexpr long double max() noexcept { return LDBL_MAX; }
    IMATH_HOSTDEVICE static constexpr long double smallest() noexcept { return LDBL_MIN; }
    IMATH_HOSTDEVICE static constexpr long double epsilon() noexcept { return LDBL_EPSILON; }
    IMATH_HOSTDEVICE static constexpr bool isIntegral() noexcept { return false; }
    IMATH_HOSTDEVICE static constexpr bool isSigned() noexcept { return true; }
};

/// @endcond

IMATH_INTERNAL_NAMESPACE_HEADER_EXIT

#endif // INCLUDED_IMATHLIMITS_H

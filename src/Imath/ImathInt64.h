//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifndef INCLUDED_IMATH_INT64_H
#define INCLUDED_IMATH_INT64_H

//----------------------------------------------------------------------------
//
//	Int64 -- unsigned 64-bit integers
//
//----------------------------------------------------------------------------

#include "ImathNamespace.h"
#include <limits.h>

IMATH_INTERNAL_NAMESPACE_HEADER_ENTER

#if (defined _WIN32 || defined _WIN64) && _MSC_VER >= 1300
typedef unsigned __int64 Int64;
typedef __int64 SInt64;
#elif ULONG_MAX == 18446744073709551615LU
typedef long unsigned int Int64;
typedef long int SInt64;
#else
typedef long long unsigned int Int64;
typedef long long int SInt64;
#endif

IMATH_INTERNAL_NAMESPACE_HEADER_EXIT

#endif // INCLUDED_IMATH_INT64_H

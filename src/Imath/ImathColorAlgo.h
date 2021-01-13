//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

///
/// @file  ImathColorAlgo.h
///
/// @brief Color conversion functions and general color algorithms
///
/// hsv2rgb(), rgb2hsv(), rgb2packed(), packed2rgb()
/// See each funtion definition for details.
///

#ifndef INCLUDED_IMATHCOLORALGO_H
#define INCLUDED_IMATHCOLORALGO_H

#include "ImathColor.h"
#include "ImathExport.h"
#include "ImathLimits.h"
#include "ImathMath.h"
#include "ImathNamespace.h"

IMATH_INTERNAL_NAMESPACE_HEADER_ENTER

//
// Non-templated helper routines for color conversion.
// These routines eliminate type warnings under g++.
//

IMATH_EXPORT Vec3<double> hsv2rgb_d (const Vec3<double>& hsv) noexcept;

IMATH_EXPORT Color4<double> hsv2rgb_d (const Color4<double>& hsv) noexcept;

IMATH_EXPORT Vec3<double> rgb2hsv_d (const Vec3<double>& rgb) noexcept;

IMATH_EXPORT Color4<double> rgb2hsv_d (const Color4<double>& rgb) noexcept;

///
/// Convert 3-channel hsv to rgb.
///

template <class T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Vec3<T>
hsv2rgb (const Vec3<T>& hsv) noexcept
{
    if (limits<T>::isIntegral())
    {
        Vec3<double> v = Vec3<double> (hsv.x / double (limits<T>::max()),
                                       hsv.y / double (limits<T>::max()),
                                       hsv.z / double (limits<T>::max()));
        Vec3<double> c = hsv2rgb_d (v);
        return Vec3<T> ((T) (c.x * limits<T>::max()),
                        (T) (c.y * limits<T>::max()),
                        (T) (c.z * limits<T>::max()));
    }
    else
    {
        Vec3<double> v = Vec3<double> (hsv.x, hsv.y, hsv.z);
        Vec3<double> c = hsv2rgb_d (v);
        return Vec3<T> ((T) c.x, (T) c.y, (T) c.z);
    }
}

///
/// Convert 4-channel hsv to rgb (with alpha).
///

template <class T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Color4<T>
hsv2rgb (const Color4<T>& hsv) noexcept
{
    if (limits<T>::isIntegral())
    {
        Color4<double> v = Color4<double> (hsv.r / float (limits<T>::max()),
                                           hsv.g / float (limits<T>::max()),
                                           hsv.b / float (limits<T>::max()),
                                           hsv.a / float (limits<T>::max()));
        Color4<double> c = hsv2rgb_d (v);
        return Color4<T> ((T) (c.r * limits<T>::max()),
                          (T) (c.g * limits<T>::max()),
                          (T) (c.b * limits<T>::max()),
                          (T) (c.a * limits<T>::max()));
    }
    else
    {
        Color4<double> v = Color4<double> (hsv.r, hsv.g, hsv.b, hsv.a);
        Color4<double> c = hsv2rgb_d (v);
        return Color4<T> ((T) c.r, (T) c.g, (T) c.b, (T) c.a);
    }
}

///
/// Convert 3-channel rgb to hsv.
///

template <class T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Vec3<T>
rgb2hsv (const Vec3<T>& rgb) noexcept
{
    if (limits<T>::isIntegral())
    {
        Vec3<double> v = Vec3<double> (rgb.x / double (limits<T>::max()),
                                       rgb.y / double (limits<T>::max()),
                                       rgb.z / double (limits<T>::max()));
        Vec3<double> c = rgb2hsv_d (v);
        return Vec3<T> ((T) (c.x * limits<T>::max()),
                        (T) (c.y * limits<T>::max()),
                        (T) (c.z * limits<T>::max()));
    }
    else
    {
        Vec3<double> v = Vec3<double> (rgb.x, rgb.y, rgb.z);
        Vec3<double> c = rgb2hsv_d (v);
        return Vec3<T> ((T) c.x, (T) c.y, (T) c.z);
    }
}

///
/// Convert 4-channel rgb to hsv (with alpha).
///

template <class T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Color4<T>
rgb2hsv (const Color4<T>& rgb) noexcept
{
    if (limits<T>::isIntegral())
    {
        Color4<double> v = Color4<double> (rgb.r / float (limits<T>::max()),
                                           rgb.g / float (limits<T>::max()),
                                           rgb.b / float (limits<T>::max()),
                                           rgb.a / float (limits<T>::max()));
        Color4<double> c = rgb2hsv_d (v);
        return Color4<T> ((T) (c.r * limits<T>::max()),
                          (T) (c.g * limits<T>::max()),
                          (T) (c.b * limits<T>::max()),
                          (T) (c.a * limits<T>::max()));
    }
    else
    {
        Color4<double> v = Color4<double> (rgb.r, rgb.g, rgb.b, rgb.a);
        Color4<double> c = rgb2hsv_d (v);
        return Color4<T> ((T) c.r, (T) c.g, (T) c.b, (T) c.a);
    }
}

///
/// Convert 3-channel rgb to PackedColor
///

template <class T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 PackedColor
rgb2packed (const Vec3<T>& c) noexcept
{
    if (limits<T>::isIntegral())
    {
        float x = c.x / float (limits<T>::max());
        float y = c.y / float (limits<T>::max());
        float z = c.z / float (limits<T>::max());
        return rgb2packed (V3f (x, y, z));
    }
    else
    {
        // clang-format off
	return (  (PackedColor) (c.x * 255)		|
		(((PackedColor) (c.y * 255)) << 8)	|
		(((PackedColor) (c.z * 255)) << 16)	| 0xFF000000 );
        // clang-format on
    }
}

///
/// Convert 4-channel rgb to PackedColor (with alpha)
///

template <class T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 PackedColor
rgb2packed (const Color4<T>& c) noexcept
{
    if (limits<T>::isIntegral())
    {
        float r = c.r / float (limits<T>::max());
        float g = c.g / float (limits<T>::max());
        float b = c.b / float (limits<T>::max());
        float a = c.a / float (limits<T>::max());
        return rgb2packed (C4f (r, g, b, a));
    }
    else
    {
        // clang-format off
	return (  (PackedColor) (c.r * 255)		|
		(((PackedColor) (c.g * 255)) << 8)	|
		(((PackedColor) (c.b * 255)) << 16)	|
		(((PackedColor) (c.a * 255)) << 24));
        // clang-format on
    }
}

///
/// Convert PackedColor to 3-channel rgb. Return the result in the
/// `out` parameter.
///

template <class T>
IMATH_HOSTDEVICE void
packed2rgb (PackedColor packed, Vec3<T>& out) noexcept
{
    if (limits<T>::isIntegral())
    {
        T f   = limits<T>::max() / ((PackedColor) 0xFF);
        out.x = (packed & 0xFF) * f;
        out.y = ((packed & 0xFF00) >> 8) * f;
        out.z = ((packed & 0xFF0000) >> 16) * f;
    }
    else
    {
        T f   = T (1) / T (255);
        out.x = (packed & 0xFF) * f;
        out.y = ((packed & 0xFF00) >> 8) * f;
        out.z = ((packed & 0xFF0000) >> 16) * f;
    }
}

///
/// Convert PackedColor to 4-channel rgb (with alpha). Return the
/// result in the `out` parameter.
///

template <class T>
IMATH_HOSTDEVICE void
packed2rgb (PackedColor packed, Color4<T>& out) noexcept
{
    if (limits<T>::isIntegral())
    {
        T f   = limits<T>::max() / ((PackedColor) 0xFF);
        out.r = (packed & 0xFF) * f;
        out.g = ((packed & 0xFF00) >> 8) * f;
        out.b = ((packed & 0xFF0000) >> 16) * f;
        out.a = ((packed & 0xFF000000) >> 24) * f;
    }
    else
    {
        T f   = T (1) / T (255);
        out.r = (packed & 0xFF) * f;
        out.g = ((packed & 0xFF00) >> 8) * f;
        out.b = ((packed & 0xFF0000) >> 16) * f;
        out.a = ((packed & 0xFF000000) >> 24) * f;
    }
}

IMATH_INTERNAL_NAMESPACE_HEADER_EXIT

#endif // INCLUDED_IMATHCOLORALGO_H

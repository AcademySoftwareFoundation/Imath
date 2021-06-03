//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef _PYIMATH_FIXEDARRAY_TRAITS_H
#define _PYIMATH_FIXEDARRAY_TRAITS_H

#include <Python.h>
#include <ImathVec.h>
#include <half.h>
#include <string>


namespace PyImath {

//  See https://docs.python.org/2/library/struct.html
//
//  Section 7.3.2.2
//
static  char       PyFmtStr_int[2] = {'i', '\0'};
static  char     PyFmtStr_float[2] = {'f', '\0'};
static  char    PyFmtStr_double[2] = {'d', '\0'};
static  char      PyFmtStr_bool[2] = {'?', '\0'};
static  char      PyFmtStr_char[2] = {'b', '\0'};
static  char     PyFmtStr_short[2] = {'h', '\0'};
static  char      PyFmtStr_long[2] = {'l', '\0'};
static  char  PyFmtStr_longlong[2] = {'q', '\0'};
static  char     PyFmtStr_uchar[2] = {'B', '\0'};
static  char    PyFmtStr_ushort[2] = {'H', '\0'};
static  char      PyFmtStr_uint[2] = {'I', '\0'};
static  char     PyFmtStr_ulong[2] = {'L', '\0'};
static  char PyFmtStr_ulonglong[2] = {'Q', '\0'};
static  char      PyFmtStr_half[2] = {'e', '\0'};


template <typename T> constexpr  char* PyFormat();

template <> constexpr  char*                PyFormat<int>()  { return PyFmtStr_int;       } 
template <> constexpr  char*              PyFormat<float>()  { return PyFmtStr_float;     }
template <> constexpr  char*             PyFormat<double>()  { return PyFmtStr_double;    }
template <> constexpr  char*               PyFormat<bool>()  { return PyFmtStr_bool;      }
template <> constexpr  char*               PyFormat<char>()  { return PyFmtStr_char;      }
template <> constexpr  char*              PyFormat<short>()  { return PyFmtStr_short;     }
template <> constexpr  char*               PyFormat<long>()  { return PyFmtStr_long;      }
template <> constexpr  char*          PyFormat<long long>()  { return PyFmtStr_longlong;  }
template <> constexpr  char*      PyFormat<unsigned char>()  { return PyFmtStr_uchar;     }
template <> constexpr  char*     PyFormat<unsigned short>()  { return PyFmtStr_ushort;    }
template <> constexpr  char*       PyFormat<unsigned int>()  { return PyFmtStr_uint;      }
template <> constexpr  char*      PyFormat<unsigned long>()  { return PyFmtStr_ulong;     }
template <> constexpr  char* PyFormat<unsigned long long>()  { return PyFmtStr_ulonglong; }
template <> constexpr  char*               PyFormat<half>()  { return PyFmtStr_half;      }

template <> constexpr  char*   PyFormat<IMATH_NAMESPACE::Vec2<short> >()    { return PyFmtStr_short;  } 
template <> constexpr  char*   PyFormat<IMATH_NAMESPACE::Vec2<int> >()      { return PyFmtStr_int;    } 
template <> constexpr  char*   PyFormat<IMATH_NAMESPACE::Vec2<int64_t> >()  { return PyFmtStr_long;   } 
template <> constexpr  char*   PyFormat<IMATH_NAMESPACE::Vec2<float> >()    { return PyFmtStr_float;  } 
template <> constexpr  char*   PyFormat<IMATH_NAMESPACE::Vec2<double> >()   { return PyFmtStr_double; } 
template <> constexpr  char*   PyFormat<IMATH_NAMESPACE::Vec3<short> >()    { return PyFmtStr_short;  } 
template <> constexpr  char*   PyFormat<IMATH_NAMESPACE::Vec3<int> >()      { return PyFmtStr_int;    } 
template <> constexpr  char*   PyFormat<IMATH_NAMESPACE::Vec3<int64_t> >()  { return PyFmtStr_long;   } 
template <> constexpr  char*   PyFormat<IMATH_NAMESPACE::Vec3<float> >()    { return PyFmtStr_float;  } 
template <> constexpr  char*   PyFormat<IMATH_NAMESPACE::Vec3<double> >()   { return PyFmtStr_double; } 
template <> constexpr  char*   PyFormat<IMATH_NAMESPACE::Vec4<short> >()    { return PyFmtStr_short;  } 
template <> constexpr  char*   PyFormat<IMATH_NAMESPACE::Vec4<int> >()      { return PyFmtStr_int;    } 
template <> constexpr  char*   PyFormat<IMATH_NAMESPACE::Vec4<int64_t> >()  { return PyFmtStr_long;   } 
template <> constexpr  char*   PyFormat<IMATH_NAMESPACE::Vec4<float> >()    { return PyFmtStr_float;  } 
template <> constexpr  char*   PyFormat<IMATH_NAMESPACE::Vec4<double> >()   { return PyFmtStr_double; } 


template <typename T> struct FixedArrayWidth { static const Py_ssize_t value; };

template <> struct FixedArrayWidth<short>                            { static const Py_ssize_t value = 1; };
template <> struct FixedArrayWidth<int>                              { static const Py_ssize_t value = 1; };
template <> struct FixedArrayWidth<int64_t>                          { static const Py_ssize_t value = 1; };
template <> struct FixedArrayWidth<float>                            { static const Py_ssize_t value = 1; };
template <> struct FixedArrayWidth<double>                           { static const Py_ssize_t value = 1; };
template <> struct FixedArrayWidth<unsigned char>                    { static const Py_ssize_t value = 1; };
template <> struct FixedArrayWidth<IMATH_NAMESPACE::Vec2<short> >    { static const Py_ssize_t value = 2; };
template <> struct FixedArrayWidth<IMATH_NAMESPACE::Vec2<int> >      { static const Py_ssize_t value = 2; };
template <> struct FixedArrayWidth<IMATH_NAMESPACE::Vec2<int64_t> >  { static const Py_ssize_t value = 2; };
template <> struct FixedArrayWidth<IMATH_NAMESPACE::Vec2<float> >    { static const Py_ssize_t value = 2; };
template <> struct FixedArrayWidth<IMATH_NAMESPACE::Vec2<double> >   { static const Py_ssize_t value = 2; };
template <> struct FixedArrayWidth<IMATH_NAMESPACE::Vec3<short> >    { static const Py_ssize_t value = 3; };
template <> struct FixedArrayWidth<IMATH_NAMESPACE::Vec3<int> >      { static const Py_ssize_t value = 3; };
template <> struct FixedArrayWidth<IMATH_NAMESPACE::Vec3<int64_t> >  { static const Py_ssize_t value = 3; };
template <> struct FixedArrayWidth<IMATH_NAMESPACE::Vec3<float> >    { static const Py_ssize_t value = 3; };
template <> struct FixedArrayWidth<IMATH_NAMESPACE::Vec3<double> >   { static const Py_ssize_t value = 3; };
template <> struct FixedArrayWidth<IMATH_NAMESPACE::Vec4<short> >    { static const Py_ssize_t value = 4; };
template <> struct FixedArrayWidth<IMATH_NAMESPACE::Vec4<int> >      { static const Py_ssize_t value = 4; };
template <> struct FixedArrayWidth<IMATH_NAMESPACE::Vec4<int64_t> >  { static const Py_ssize_t value = 4; };
template <> struct FixedArrayWidth<IMATH_NAMESPACE::Vec4<float> >    { static const Py_ssize_t value = 4; };
template <> struct FixedArrayWidth<IMATH_NAMESPACE::Vec4<double> >   { static const Py_ssize_t value = 4; };


template <typename T> struct FixedArrayDimension { static const Py_ssize_t value; };

template <> struct FixedArrayDimension<short>                            { static const Py_ssize_t value = 1; };
template <> struct FixedArrayDimension<int>                              { static const Py_ssize_t value = 1; };
template <> struct FixedArrayDimension<int64_t>                          { static const Py_ssize_t value = 1; };
template <> struct FixedArrayDimension<float>                            { static const Py_ssize_t value = 1; };
template <> struct FixedArrayDimension<double>                           { static const Py_ssize_t value = 1; };
template <> struct FixedArrayDimension<unsigned char>                    { static const Py_ssize_t value = 1; };
template <> struct FixedArrayDimension<IMATH_NAMESPACE::Vec2<short> >    { static const Py_ssize_t value = 2; };
template <> struct FixedArrayDimension<IMATH_NAMESPACE::Vec2<int> >      { static const Py_ssize_t value = 2; };
template <> struct FixedArrayDimension<IMATH_NAMESPACE::Vec2<int64_t> >  { static const Py_ssize_t value = 2; };
template <> struct FixedArrayDimension<IMATH_NAMESPACE::Vec2<float> >    { static const Py_ssize_t value = 2; };
template <> struct FixedArrayDimension<IMATH_NAMESPACE::Vec2<double> >   { static const Py_ssize_t value = 2; };
template <> struct FixedArrayDimension<IMATH_NAMESPACE::Vec3<short> >    { static const Py_ssize_t value = 2; };
template <> struct FixedArrayDimension<IMATH_NAMESPACE::Vec3<int> >      { static const Py_ssize_t value = 2; };
template <> struct FixedArrayDimension<IMATH_NAMESPACE::Vec3<int64_t> >  { static const Py_ssize_t value = 2; };
template <> struct FixedArrayDimension<IMATH_NAMESPACE::Vec3<float> >    { static const Py_ssize_t value = 2; };
template <> struct FixedArrayDimension<IMATH_NAMESPACE::Vec3<double> >   { static const Py_ssize_t value = 2; };
template <> struct FixedArrayDimension<IMATH_NAMESPACE::Vec4<short> >    { static const Py_ssize_t value = 2; };
template <> struct FixedArrayDimension<IMATH_NAMESPACE::Vec4<int> >      { static const Py_ssize_t value = 2; };
template <> struct FixedArrayDimension<IMATH_NAMESPACE::Vec4<int64_t> >  { static const Py_ssize_t value = 2; };
template <> struct FixedArrayDimension<IMATH_NAMESPACE::Vec4<float> >    { static const Py_ssize_t value = 2; };
template <> struct FixedArrayDimension<IMATH_NAMESPACE::Vec4<double> >   { static const Py_ssize_t value = 2; };


template <typename T> struct FixedArrayAtomicSize { static const Py_ssize_t value; };

template <> struct FixedArrayAtomicSize<short>                            { static const Py_ssize_t value = sizeof(short); };
template <> struct FixedArrayAtomicSize<int>                              { static const Py_ssize_t value = sizeof(int); };
template <> struct FixedArrayAtomicSize<int64_t>                          { static const Py_ssize_t value = sizeof(int); };
template <> struct FixedArrayAtomicSize<float>                            { static const Py_ssize_t value = sizeof(float); };
template <> struct FixedArrayAtomicSize<double>                           { static const Py_ssize_t value = sizeof(double); };
template <> struct FixedArrayAtomicSize<unsigned char>                    { static const Py_ssize_t value = sizeof(unsigned char); };
template <> struct FixedArrayAtomicSize<IMATH_NAMESPACE::Vec2<short> >    { static const Py_ssize_t value = sizeof(short); };
template <> struct FixedArrayAtomicSize<IMATH_NAMESPACE::Vec2<int> >      { static const Py_ssize_t value = sizeof(int); };
template <> struct FixedArrayAtomicSize<IMATH_NAMESPACE::Vec2<int64_t> >  { static const Py_ssize_t value = sizeof(int64_t); };
template <> struct FixedArrayAtomicSize<IMATH_NAMESPACE::Vec2<float> >    { static const Py_ssize_t value = sizeof(float); };
template <> struct FixedArrayAtomicSize<IMATH_NAMESPACE::Vec2<double> >   { static const Py_ssize_t value = sizeof(double); };
template <> struct FixedArrayAtomicSize<IMATH_NAMESPACE::Vec3<short> >    { static const Py_ssize_t value = sizeof(short); };
template <> struct FixedArrayAtomicSize<IMATH_NAMESPACE::Vec3<int> >      { static const Py_ssize_t value = sizeof(int); };
template <> struct FixedArrayAtomicSize<IMATH_NAMESPACE::Vec3<int64_t> >  { static const Py_ssize_t value = sizeof(int64_t); };
template <> struct FixedArrayAtomicSize<IMATH_NAMESPACE::Vec3<float> >    { static const Py_ssize_t value = sizeof(float); };
template <> struct FixedArrayAtomicSize<IMATH_NAMESPACE::Vec3<double> >   { static const Py_ssize_t value = sizeof(double); };
template <> struct FixedArrayAtomicSize<IMATH_NAMESPACE::Vec4<short> >    { static const Py_ssize_t value = sizeof(short); };
template <> struct FixedArrayAtomicSize<IMATH_NAMESPACE::Vec4<int> >      { static const Py_ssize_t value = sizeof(int); };
template <> struct FixedArrayAtomicSize<IMATH_NAMESPACE::Vec4<int64_t> >  { static const Py_ssize_t value = sizeof(int64_t); };
template <> struct FixedArrayAtomicSize<IMATH_NAMESPACE::Vec4<float> >    { static const Py_ssize_t value = sizeof(float); };
template <> struct FixedArrayAtomicSize<IMATH_NAMESPACE::Vec4<double> >   { static const Py_ssize_t value = sizeof(double); };

} // namespace

#endif

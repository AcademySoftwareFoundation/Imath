//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#ifndef _PyImathBufferProtocol_h_
#define _PyImathBufferProtocol_h_

#include <boost/python.hpp>

namespace PyImath {

//  For more information on working with the protocol see:
//
//    https://docs.python.org/2.7/c-api/buffer.html
//    https://docs.python.org/3.7.10/c-api/buffer.html

template <class T>
void add_buffer_protocol (boost::python::class_<T> &classObj);

template <class ArrayT>
ArrayT* fixedArrayFromBuffer (PyObject *obj);

}

#endif

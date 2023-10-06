# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

pyimath_define_module(imath
  LIBNAME PyBindImath
  PRIV_EXPORT PYIMATH_BUILD
  CURDIR ${CMAKE_CURRENT_SOURCE_DIR}
  LIBSOURCE
    PyImathVec.cpp
  MODSOURCE
    imathmodule.cpp
    PyImathVec.cpp
  HEADERS
    PyImathExport.h
  DEPENDENCIES
    Imath
  )

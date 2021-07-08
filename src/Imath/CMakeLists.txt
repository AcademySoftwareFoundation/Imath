# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

imath_define_library(Imath
  PRIV_EXPORT IMATH_EXPORTS
  CURDIR ${CMAKE_CURRENT_SOURCE_DIR}
  SOURCES
    ImathRandom.cpp
    ImathColorAlgo.cpp
    ImathFun.cpp
    ImathMatrixAlgo.cpp
    toFloat.h
    half.cpp
  HEADERS
    ImathBoxAlgo.h
    ImathBox.h
    ImathColorAlgo.h
    ImathColor.h
    ImathEuler.h
    ImathExport.h
    ImathForward.h
    ImathFrame.h
    ImathFrustum.h
    ImathFrustumTest.h
    ImathFun.h
    ImathGL.h
    ImathGLU.h
    ImathInt64.h
    ImathInterval.h
    ImathLineAlgo.h
    ImathLine.h
    ImathMath.h
    ImathMatrixAlgo.h
    ImathMatrix.h
    ImathNamespace.h
    ImathPlane.h
    ImathPlatform.h
    ImathQuat.h
    ImathRandom.h
    ImathRoots.h
    ImathShear.h
    ImathSphere.h
    ImathTypeTraits.h
    ImathVecAlgo.h
    ImathVec.h
    half.h
    halfFunction.h
    halfLimits.h
  )

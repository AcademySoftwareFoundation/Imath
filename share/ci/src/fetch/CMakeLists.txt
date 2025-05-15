# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

#
# A simple test program that uses Imath built via CMake's FetchContent
#

cmake_minimum_required(VERSION 3.14)
project(ImathFetchTest)
find_package(Imath)

set(IMATH_REPO "https://github.com/AcademySoftwareFoundation/Imath" CACHE STRING "Imath repository URL")
set(IMATH_TAG "main" CACHE STRING "Imath tag to checkout")
  
if(NOT TARGET Imath::Imath AND NOT Imath_FOUND)
  
  message(STATUS "Fetching Imath from ${IMATH_REPO} ${IMATH_TAG}")

  include(FetchContent)
  FetchContent_Declare(Imath
    GIT_REPOSITORY "${IMATH_REPO}"
    GIT_TAG "${IMATH_TAG}"
    GIT_SHALLOW ON
  )
    
  FetchContent_GetProperties(Imath)
  if(NOT Imath_POPULATED)
    FetchContent_MakeAvailable(Imath)
  endif()
endif()

add_executable(ImathFetchTest fetch.cpp)
target_link_libraries(ImathFetchTest Imath::Imath)

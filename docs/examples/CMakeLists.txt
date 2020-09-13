# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

cmake_minimum_required(VERSION 3.10)

project(imath-examples)

find_package(Imath)

add_executable(imath-examples
  main.cpp
  Color3.cpp
  Color4.cpp
  Euler.cpp
  Frustum.cpp
  Interval.cpp
  Line3.cpp
  Matrix22.cpp
  Matrix33.cpp
  Matrix44.cpp
  Plane3.cpp
  Quat.cpp
  Shear6.cpp
  Sphere3.cpp
  Vec2.cpp
  Vec3.cpp
  Vec4.cpp
  half.cpp
)

target_link_libraries(imath-examples Imath::Imath)

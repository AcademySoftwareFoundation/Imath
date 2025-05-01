
cmake_minimum_required(VERSION 3.14)

project(imath_program)

find_package(Imath)

add_executable(imath_program main.cpp)

target_link_libraries(imath_program Imath::Imath)

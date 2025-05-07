
cmake_minimum_required(VERSION 3.14)

project(pyimath_program)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
find_package(Boost REQUIRED COMPONENTS python)

find_package(Imath COMPONENTS PyImath)

add_executable(pyimath_program main.cpp)

target_link_libraries(pyimath_program 
    Imath::Imath
    Imath::PyImath
    Boost::boost
    Boost::python
    Python3::Python
)

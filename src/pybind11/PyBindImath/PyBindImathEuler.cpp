//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include <ImathEuler.h>
#include <ImathVec.h>
#include <pybind11/pybind11.h>

namespace {
    
// Function to register the Euler class methods
    template <class T>
    void register_euler_methods(pybind11::class_<IMATH_NAMESPACE::Euler<T>, IMATH_NAMESPACE::Vec3<T>>& c) {
        c.def(pybind11::init<>())
                .def(pybind11::init<const IMATH_NAMESPACE::Vec3<T>&, typename IMATH_NAMESPACE::Euler<T>::Order>(), pybind11::arg("v"), pybind11::arg("order") = IMATH_NAMESPACE::Euler<T>::XYZ)
                .def(pybind11::init<T, T, T, typename IMATH_NAMESPACE::Euler<T>::Order>(), pybind11::arg("x"), pybind11::arg("y"), pybind11::arg("z"), pybind11::arg("order") = IMATH_NAMESPACE::Euler<T>::XYZ)
                .def(pybind11::self == pybind11::self)
                .def(pybind11::self != pybind11::self)
                .def("toMatrix33", &IMATH_NAMESPACE::Euler<T>::toMatrix33)
                .def("toMatrix44", &IMATH_NAMESPACE::Euler<T>::toMatrix44)
                .def("toQuat", &IMATH_NAMESPACE::Euler<T>::toQuat)
                .def("order", &IMATH_NAMESPACE::Euler<T>::order)
                .def("setOrder", &IMATH_NAMESPACE::Euler<T>::setOrder)
                .def("makeNear", &IMATH_NAMESPACE::Euler<T>::makeNear)
                .def("extract", pybind11::overload_cast<const IMATH_NAMESPACE::Matrix33<T>&>(&IMATH_NAMESPACE::Euler<T>::extract))
                .def("extract", pybind11::overload_cast<const IMATH_NAMESPACE::Matrix44<T>&>(&IMATH_NAMESPACE::Euler<T>::extract))
                .def("extract", pybind11::overload_cast<const IMATH_NAMESPACE::Quat<T>&>(&IMATH_NAMESPACE::Euler<T>::extract))
                .def("toXYZVector", &IMATH_NAMESPACE::Euler<T>::toXYZVector)
                .def("__str__", [](const IMATH_NAMESPACE::Euler<T>& e) {
                    std::stringstream stream;
                    stream << "Euler(" << e.x << ", " << e.y << ", " << e.z << ", " << e.order() << ")";
                    return stream.str();
                })
                .def("__repr__", [](const IMATH_NAMESPACE::Euler<T>& e) {
                    std::stringstream stream;
                    stream << "Euler(" << e.x << ", " << e.y << ", " << e.z << ", " << e.order() << ")";
                    return stream.str();
                });
    }

// Function to register the Euler class in the module
    template <class T>
    void register_euler(pybind11::module& m, const char* name) {
        pybind11::class_<IMATH_NAMESPACE::Euler<T>, IMATH_NAMESPACE::Vec3<T>> c(m, name);
        register_euler_methods<T>(c);
    }
} // namespace

namespace PyBindImath {

// Function to register the Euler types for float and double
void
register_imath_euler(pybind11::module& m)
{
    register_euler<float>(m, "Eulerf");
    register_euler<double>(m, "Eulerd");
    
    // Enums for Euler Orders
    pybind11::enum_<Imath::Euler<float>::Order>(m, "Order")
        .value("XYZ", IMATH_NAMESPACE::Euler<float>::XYZ)
        .value("XZY", IMATH_NAMESPACE::Euler<float>::XZY)
        .value("YZX", IMATH_NAMESPACE::Euler<float>::YZX)
        .value("YXZ", IMATH_NAMESPACE::Euler<float>::YXZ)
        .value("ZXY", IMATH_NAMESPACE::Euler<float>::ZXY)
        .value("ZYX", IMATH_NAMESPACE::Euler<float>::ZYX)
        .value("XZX", IMATH_NAMESPACE::Euler<float>::XZX)
        .value("XYX", IMATH_NAMESPACE::Euler<float>::XYX)
        .value("YXY", IMATH_NAMESPACE::Euler<float>::YXY)
        .value("YZY", IMATH_NAMESPACE::Euler<float>::YZY)
        .value("ZYZ", IMATH_NAMESPACE::Euler<float>::ZYZ)
        .value("ZXZ", IMATH_NAMESPACE::Euler<float>::ZXZ)
        .export_values();

    // Enums for Axis
    pybind11::enum_<Imath::Euler<float>::Axis>(m, "Axis")
        .value("X", IMATH_NAMESPACE::Euler<float>::X)
        .value("Y", IMATH_NAMESPACE::Euler<float>::Y)
        .value("Z", IMATH_NAMESPACE::Euler<float>::Z)
        .export_values();

    // Enums for InputLayout
    pybind11::enum_<Imath::Euler<float>::InputLayout>(m, "InputLayout")
        .value("XYZLayout", IMATH_NAMESPACE::Euler<float>::XYZLayout)
        .value("IJKLayout", IMATH_NAMESPACE::Euler<float>::IJKLayout)
        .export_values();
}

} // namespace PyBindImath
 



//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include <ImathEuler.h>
#include <ImathVec.h>
#include <pybind11/pybind11.h>

namespace PyBindImath {

// Function to register the Euler class methods
    template <class T>
    void register_euler_methods(pybind11::class_<Imath::Euler<T>, Imath::Vec3<T>>& c) {
        c.def(pybind11::init<>())
                .def(pybind11::init<const Imath::Vec3<T>&, typename Imath::Euler<T>::Order>(), pybind11::arg("v"), pybind11::arg("order") = Imath::Euler<T>::XYZ)
                .def(pybind11::init<T, T, T, typename Imath::Euler<T>::Order>(), pybind11::arg("x"), pybind11::arg("y"), pybind11::arg("z"), pybind11::arg("order") = Imath::Euler<T>::XYZ)
                .def(pybind11::self == pybind11::self)
                .def(pybind11::self != pybind11::self)
                .def("toMatrix33", &Imath::Euler<T>::toMatrix33)
                .def("toMatrix44", &Imath::Euler<T>::toMatrix44)
                .def("toQuat", &Imath::Euler<T>::toQuat)
                .def("order", &Imath::Euler<T>::order)
                .def("setOrder", &Imath::Euler<T>::setOrder)
                .def("makeNear", &Imath::Euler<T>::makeNear)
                .def("extract", pybind11::overload_cast<const Imath::Matrix33<T>&>(&Imath::Euler<T>::extract))
                .def("extract", pybind11::overload_cast<const Imath::Matrix44<T>&>(&Imath::Euler<T>::extract))
                .def("extract", pybind11::overload_cast<const Imath::Quat<T>&>(&Imath::Euler<T>::extract))
                .def("toXYZVector", &Imath::Euler<T>::toXYZVector)
                .def("__str__", [](const Imath::Euler<T>& e) {
                    std::stringstream stream;
                    stream << "Euler(" << e.x << ", " << e.y << ", " << e.z << ", " << e.order() << ")";
                    return stream.str();
                })
                .def("__repr__", [](const Imath::Euler<T>& e) {
                    std::stringstream stream;
                    stream << "Euler(" << e.x << ", " << e.y << ", " << e.z << ", " << e.order() << ")";
                    return stream.str();
                });
    }

// Function to register the Euler class in the module
    template <class T>
    void register_euler(pybind11::module& m, const char* name) {
        pybind11::class_<Imath::Euler<T>, Imath::Vec3<T>> c(m, name);
        register_euler_methods<T>(c);
    }

// Function to register the Euler types for float and double
    void register_imath_euler(pybind11::module& m) {
        register_euler<float>(m, "Eulerf");
        register_euler<double>(m, "Eulerd");

        // Enums for Euler Orders
        pybind11::enum_<Imath::Euler<float>::Order>(m, "Order")
                .value("XYZ", Imath::Euler<float>::XYZ)
                .value("XZY", Imath::Euler<float>::XZY)
                .value("YZX", Imath::Euler<float>::YZX)
                .value("YXZ", Imath::Euler<float>::YXZ)
                .value("ZXY", Imath::Euler<float>::ZXY)
                .value("ZYX", Imath::Euler<float>::ZYX)
                .value("XZX", Imath::Euler<float>::XZX)
                .value("XYX", Imath::Euler<float>::XYX)
                .value("YXY", Imath::Euler<float>::YXY)
                .value("YZY", Imath::Euler<float>::YZY)
                .value("ZYZ", Imath::Euler<float>::ZYZ)
                .value("ZXZ", Imath::Euler<float>::ZXZ)
                .export_values();

        // Enums for Axis
        pybind11::enum_<Imath::Euler<float>::Axis>(m, "Axis")
                .value("X", Imath::Euler<float>::X)
                .value("Y", Imath::Euler<float>::Y)
                .value("Z", Imath::Euler<float>::Z)
                .export_values();

        // Enums for InputLayout
        pybind11::enum_<Imath::Euler<float>::InputLayout>(m, "InputLayout")
                .value("XYZLayout", Imath::Euler<float>::XYZLayout)
                .value("IJKLayout", Imath::Euler<float>::IJKLayout)
                .export_values();
    }

}  // namespace PyBindImath

//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include <ImathShear.h>
#include <pybind11/pybind11.h>
#include <sstream>

namespace PyBindImath {

    template <class T>
    void register_shear_methods(pybind11::class_<IMATH_NAMESPACE::Shear6<T>> &c)
    {
        c.def(pybind11::init<>(), "Default constructor (all components set to 0)")
                .def(pybind11::init<T, T, T>(),
                     "Construct from three values (XY, XZ, YZ); other components set to 0",
                     pybind11::arg("XY"), pybind11::arg("XZ"), pybind11::arg("YZ"))
                .def(pybind11::init<T, T, T, T, T, T>(),
                     "Construct from six values (XY, XZ, YZ, YX, ZX, ZY)",
                     pybind11::arg("XY"), pybind11::arg("XZ"), pybind11::arg("YZ"),
                     pybind11::arg("YX"), pybind11::arg("ZX"), pybind11::arg("ZY"))
                .def("setValue",
                     (void (IMATH_NAMESPACE::Shear6<T>::*)(T, T, T, T, T, T)) &IMATH_NAMESPACE::Shear6<T>::setValue,
                     "Set shear values")
                .def("setValue",
                     (void (IMATH_NAMESPACE::Shear6<T>::*)(const IMATH_NAMESPACE::Shear6<T>&)) &IMATH_NAMESPACE::Shear6<T>::setValue,
                     "Copy shear values from another shear")
                .def("getValue", &IMATH_NAMESPACE::Shear6<T>::getValue,
                     "Get shear values as a tuple")
                .def("negate", &IMATH_NAMESPACE::Shear6<T>::negate,
                     pybind11::return_value_policy::reference_internal,
                     "Negate the shear transformation")
                .def(pybind11::self == pybind11::self)
                .def(pybind11::self != pybind11::self)
                .def("__str__", [](const IMATH_NAMESPACE::Shear6<T>& s) {
                    std::stringstream ss;
                    ss << "Shear6(" << s[0] << ", " << s[1] << ", " << s[2] << ", "
                       << s[3] << ", " << s[4] << ", " << s[5] << ")";
                    return ss.str();
                })
                .def("__repr__", [](const IMATH_NAMESPACE::Shear6<T>& s) {
                    std::stringstream ss;
                    ss << "Shear6(" << s[0] << ", " << s[1] << ", " << s[2] << ", "
                       << s[3] << ", " << s[4] << ", " << s[5] << ")";
                    return ss.str();
                });
    }

    template <class T>
    void register_shear(pybind11::module &m, const char* name)
    {
        pybind11::class_<IMATH_NAMESPACE::Shear6<T>> c(m, name);
        register_shear_methods<T>(c);
    }

    void register_imath_shear(pybind11::module &m)
    {
        register_shear<float>(m, "Shear6f");
        register_shear<double>(m, "Shear6d");
    }

} // namespace PyBindImath
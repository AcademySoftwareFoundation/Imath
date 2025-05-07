//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include <ImathFrustum.h>
#include <ImathVec.h>
#include <ImathMatrix.h>

namespace {

template <class T, class V, class S>
void register_frustum(pybind11::module& m, const char *name)
{
    pybind11::class_<T> c(m, name);
    c.def(pybind11::init<>(), "Uninitialized by default")
    .def(pybind11::init<T>(), pybind11::arg("frustum"), "Copy constructor")
    .def(pybind11::init<S, S, S, S>(), pybind11::arg("nearPlane"), pybind11::arg("farPlane"), pybind11::arg("fovx"), pybind11::arg("aspect"), "Initialize with basic frustum properties")
    
    .def_readwrite("nearPlane", &T::nearPlane, "The near clipping plane")
    .def_readwrite("farPlane", &T::farPlane, "The far clipping plane")
    .def_readwrite("fovx", &T::fovx, "The field of view in x direction")
    .def_readwrite("aspect", &T::aspect, "The aspect ratio")

    .def("set", pybind11::overload_cast<S, S, S, S>(&T::set), pybind11::arg("nearPlane"), pybind11::arg("farPlane"), pybind11::arg("fovx"), pybind11::arg("aspect"), "Set frustum properties")
    .def("projectionMatrix", &T::projectionMatrix, "Returns the projection matrix of the frustum")
    .def("transform", &T::transform, pybind11::arg("matrix"), "Applies a transformation matrix to the frustum")
    .def("intersects", [](T& self, const V& point) {
                bool result = self.intersects(point);
                return result;
            }, pybind11::arg("point"), "Determines if the point is inside the frustum")
    
    .def("screenToWorld", [](T& self, const V& screenPoint) {
                V worldPoint;
                self.screenToWorld(screenPoint, worldPoint);
                return worldPoint;
            }, pybind11::arg("screenPoint"), "Convert a screen space point to world space")
    
    .def("worldToScreen", [](T& self, const V& worldPoint) {
                V screenPoint;
                self.worldToScreen(worldPoint, screenPoint);
                return screenPoint;
            }, pybind11::arg("worldPoint"), "Convert a world space point to screen space")
    
    .def("__str__", [](const T &obj) {
            std::stringstream ss;
            ss << obj;
            return ss.str();
        })
        ;
}

} // namespace
 
namespace PyBindImath {

void
register_imath_frustum(pybind11::module &m) 
{
    register_frustum<IMATH_NAMESPACE::Frustumf, IMATH_NAMESPACE::V3f, IMATH_NAMESPACE::M44f>(m, "Frustumf");
    register_frustum<IMATH_NAMESPACE::Frustumd, IMATH_NAMESPACE::V3d, IMATH_NAMESPACE::M44d>(m, "Frustumd");
}

} // namespace PyBindImath


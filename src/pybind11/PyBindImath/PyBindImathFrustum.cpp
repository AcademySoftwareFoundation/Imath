//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include <ImathFrustum.h>
namespace PyBindImath {
namespace py = pybind11;
namespace {

template<typename T>
struct GetClassName {};

template <>
struct GetClassName<IMATH_NAMESPACE::Frustumf> {
    static constexpr char const* value = "Frustumf";
};

template <>
struct GetClassName<IMATH_NAMESPACE::Frustumd> {
    static constexpr char const* value = "Frustumd";
};

template <typename F, typename T = typename F::value_type>
void register_Frustum(py::module& m, char const* name)
{
    py::class_<F>(m, name)
    .def(py::init<>(), "Frustum() default construction")
    .def(py::init<F>(), py::arg("frustum"), "Copy constructor")
    .def(py::init<T, T, T, T, T, T, bool>(), "Frustum(nearPlane,farPlane,left,right,top,bottom,ortho) construction")
    .def(py::init<T, T, T, T, T>(), "Frustum(nearPlane,farPlane,fovx,fovy,aspect) construction")

    .def("set", py::overload_cast<T, T, T, T, T, T, bool>(&F::set),
        "F.set(nearPlane, farPlane, left, right, top, bottom, "
        "[ortho])\n"
        "F.set(nearPlane, farPlane, fovx, fovy, aspect)       "
        "         -- sets the entire state of "
        "frustum F as specified.  Only one of "
        "fovx or fovy may be non-zero.")             
    .def("set", py::overload_cast<T, T, T, T, T>(&F::set))

    .def("modifyNearAndFar", &F::modifyNearAndFar,
        "F.modifyNearAndFar(nearPlane, farPlane) -- modifies "
	    "the already-valid frustum F as specified")

    .def("setOrthographic", &F::setOrthographic,
        "F.setOrthographic(b) -- modifies the "
        "already-valid frustum F to be orthographic "
        "or not")

    .def("nearPlane", &F::nearPlane,
        "F.nearPlane() -- returns the coordinate of the "
        "near clipping plane of frustum F")
            
    .def("farPlane", &F::farPlane, 
        "F.farPlane() -- returns the coordinate of the "
        "far clipping plane of frustum F")

    // The following two functions provide backwards compatibility
    // with the previous API for this class.

    .def("near", &F::nearPlane,
        "F.near() -- returns the coordinate of the "
        "near clipping plane of frustum F")
    
    .def("far", &F::farPlane, 
        "F.far() -- returns the coordinate of the "
        "far clipping plane of frustum F")

    .def("left", &F::left,
        "F.left() -- returns the left coordinate of "
        "the near clipping window of frustum F")
            
    .def("right", &F::right,
        "F.right() -- returns the right coordinate of "
        "the near clipping window of frustum F")
            
    .def("top", &F::top,
        "F.top() -- returns the top coordinate of "
        "the near clipping window of frustum F")
            
    .def("bottom", &F::bottom,
        "F.bottom() -- returns the bottom coordinate "
        "of the near clipping window of frustum F")
            
    .def("orthographic", &F::orthographic,
        "F.orthographic() -- returns whether frustum "
        "F is orthographic or not")

    .def("planes", [](F const& self, IMATH_NAMESPACE::Plane3<T>* planes) -> void
    {
        self.planes(planes);
    })
    .def("planes", [](F const& self, IMATH_NAMESPACE::Plane3<T> *p, IMATH_NAMESPACE::Matrix44<T> const& m) -> void
    {
        self.planes(p, m);
    })
    .def("planes", [](F const& self, IMATH_NAMESPACE::Matrix44<T> const& m)
    {
        IMATH_NAMESPACE::Plane3<T> p[6];
        self.planes(p, m);
        return py::make_tuple(p[0], p[1], p[2], p[3], p[4], p[5]);    
    })
    .def("planes", [](F const& self)
    {
        IMATH_NAMESPACE::Plane3<T> p[6];
        self.planes(p);
    
        return py::make_tuple(p[0],p[1],p[2],p[3],p[4],p[5]);    
    })

    .def("fovx", &F::fovx,
        "F.fovx() -- derives and returns the "
        "x field of view (in radians) for frustum F")
            
    .def("fovy", &F::fovy,
        "F.fovy() -- derives and returns the "
        "y field of view (in radians) for frustum F")
            
    .def("aspect", &F::aspect,
        "F.aspect() -- derives and returns the "
        "aspect ratio for frustum F")
            
    .def("projectionMatrix", &F::projectionMatrix,
        "F.projectionMatrix() -- derives and returns "
        "the projection matrix for frustum F")
            
    .def("window", &F::window,
        "F.window(l,r,b,t) -- takes a rectangle in "
        "the screen space (i.e., -1 <= l <= r <= 1, "
        "-1 <= b <= t <= 1) of F and returns a new "
        "Frustum whose near clipping-plane window "
        "is that rectangle in local space")

    .def("projectScreenToRay", &F::projectScreenToRay, 
        "F.projectScreenToRay(V) -- returns a Line3 "
        "through V, a V2 point in screen space")
            
    .def("projectScreenToRay", [](F const& self, py::sequence const& seq) -> IMATH_NAMESPACE::Line3<T> {
        if(seq.size() != 2) {
            throw std::invalid_argument ( "projectScreenToRay expects a sequence of length 2");
        }

        IMATH_NAMESPACE::Vec2<T> const point{py::cast<T>(seq[0]), py::cast<T>(seq[1])};
        return self.projectScreenToRay(point);
    })
            
    .def("projectPointToScreen", &F::projectPointToScreen, 
        "F.projectPointToScreen(V) -- returns the "
        "projection of V3 V into screen space")
            
    .def("projectPointToScreen", [](F const& self, py::sequence const& seq) -> IMATH_NAMESPACE::Vec2<T> {
        if(seq.size() != 3) {
            throw std::invalid_argument ( "projectPointToScreen expects a sequence of length 3");
        }

        IMATH_NAMESPACE::Vec3<T> const point{py::cast<T>(seq[0]), py::cast<T>(seq[1]), py::cast<T>(seq[2])};
        return self.projectPointToScreen(point);
    })
    
    .def("ZToDepth", &F::ZToDepth,
        "F.ZToDepth(z, zMin, zMax) -- returns the "
        "depth (Z in the local space of the "
        "frustum F) corresponding to z (a result of "
        "transformation by F's projection matrix) "
        "after normalizing z to be between zMin "
        "and zMax")
             
    .def("normalizedZToDepth", &F::normalizedZToDepth,
        "F.normalizedZToDepth(z) -- returns the "
        "depth (Z in the local space of the "
        "frustum F) corresponding to z (a result of "
        "transformation by F's projection matrix), "
        "which is assumed to have been normalized "
        "to [-1, 1]")
            
    .def("DepthToZ", &F::DepthToZ,
        "F.DepthToZ(depth, zMin, zMax) -- converts "
        "depth (Z in the local space of the frustum "
        "F) to z (a result of  transformation by F's "
        "projection matrix) which is normalized to "
        "[zMin, zMax]")
            
    .def("worldRadius", &F::worldRadius,
        "F.worldRadius(V, r) -- returns the radius "
        "in F's local space corresponding to the "
        "point V and radius r in screen space")
            
    .def("worldRadius", [](F const& self, py::sequence const& seq, T radius) -> T {
        if(seq.size() != 3) {
            throw std::invalid_argument ( "worldRadius expects a sequence of length 3");
        }

        IMATH_NAMESPACE::Vec3<T> const point{py::cast<T>(seq[0]), py::cast<T>(seq[1]), py::cast<T>(seq[2])};
        return self.worldRadius(point, radius);
    })
            
    .def("screenRadius", &F::screenRadius,
        "F.screenRadius(V, r) -- returns the radius "
        "in screen space corresponding to "
        "the point V and radius r in F's local "
        "space")
            
    .def("screenRadius", [](F const& self, py::sequence const& seq, T radius) -> T{
        if(seq.size() != 3) {
            throw std::invalid_argument ("screenRadius expects a sequence of length 3");
        }

        IMATH_NAMESPACE::Vec3<T> const point{py::cast<T>(seq[0]), py::cast<T>(seq[1]), py::cast<T>(seq[2])};
        return self.screenRadius(point, radius);
    })
    ;
}

} // namespace

namespace PyBindImath {
        
void register_imath_frustum(py::module &m)
{
    register_Frustum<IMATH_NAMESPACE::Frustumf>(m, "Frustumf");
    register_Frustum<IMATH_NAMESPACE::Frustumd>(m, "Frustumd");
}

} // PyBindImath

//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include <ImathFrustum.h>
#include <ImathFrustumTest.h>
#include <ImathBox.h>

namespace py = pybind11;
using namespace IMATH_NAMESPACE;

namespace {

template <class Frustum>
void
register_frustum(py::module& m, char const* name)
{
    typedef typename Frustum::value_type T;

    py::class_<Frustum> c(m, name);
    c.attr("__module__") = "";
    c.def(py::init<>(), "Frustum() default construction")
        .def(py::init<Frustum>(), py::arg("frustum"), "Copy constructor")
        .def(py::init<T, T, T, T, T, T, bool>(), "Frustum(nearPlane,farPlane,left,right,top,bottom,ortho) construction")
        .def(py::init<T, T, T, T, T>(), "Frustum(nearPlane,farPlane,fovx,fovy,aspect) construction")

        .def(py::self == py::self)
        .def(py::self != py::self)

        .def("set", py::overload_cast<T, T, T, T, T, T, bool>(&Frustum::set),
             "F.set(nearPlane, farPlane, left, right, top, bottom, "
             "[ortho])\n"
             "F.set(nearPlane, farPlane, fovx, fovy, aspect)       "
             "         -- sets the entire state of "
             "frustum F as specified.  Only one of "
             "fovx or fovy may be non-zero.")             
        .def("set", py::overload_cast<T, T, T, T, T>(&Frustum::set))

        .def("modifyNearAndFar", &Frustum::modifyNearAndFar,
             "F.modifyNearAndFar(nearPlane, farPlane) -- modifies "
             "the already-valid frustum F as specified")

        .def("setOrthographic", &Frustum::setOrthographic,
             "F.setOrthographic(b) -- modifies the "
             "already-valid frustum F to be orthographic "
             "or not")

        .def("nearPlane", &Frustum::nearPlane,
             "F.nearPlane() -- returns the coordinate of the "
             "near clipping plane of frustum F")
            
        .def("farPlane", &Frustum::farPlane, 
             "F.farPlane() -- returns the coordinate of the "
             "far clipping plane of frustum F")

        // The following two functions provide backwards compatibility
        // with the previous API for this class.

        .def("near", &Frustum::nearPlane,
             "F.near() -- returns the coordinate of the "
             "near clipping plane of frustum F")
    
        .def("far", &Frustum::farPlane, 
             "F.far() -- returns the coordinate of the "
             "far clipping plane of frustum F")

        .def("left", &Frustum::left,
             "F.left() -- returns the left coordinate of "
             "the near clipping window of frustum F")
            
        .def("right", &Frustum::right,
             "F.right() -- returns the right coordinate of "
             "the near clipping window of frustum F")
            
        .def("top", &Frustum::top,
             "F.top() -- returns the top coordinate of "
             "the near clipping window of frustum F")
            
        .def("bottom", &Frustum::bottom,
             "F.bottom() -- returns the bottom coordinate "
             "of the near clipping window of frustum F")
            
        .def("orthographic", &Frustum::orthographic,
             "F.orthographic() -- returns whether frustum "
             "F is orthographic or not")

        .def("planes", [](const Frustum& self, Plane3<T>* planes) -> void
            {
                self.planes(planes);
            })
        .def("planes", [](const Frustum& self, Plane3<T> *p, Matrix44<T> const& m) -> void
            {
                self.planes(p, m);
            })
        .def("planes", [](const Frustum& self, Matrix44<T> const& m)
            {
                Plane3<T> p[6];
                self.planes(p, m);
                return py::make_tuple(p[0], p[1], p[2], p[3], p[4], p[5]);    
            })
        .def("planes", [](const Frustum& self)
            {
                Plane3<T> p[6];
                self.planes(p);
    
                return py::make_tuple(p[0],p[1],p[2],p[3],p[4],p[5]);    
            })

        .def("fovx", &Frustum::fovx,
             "F.fovx() -- derives and returns the "
             "x field of view (in radians) for frustum F")
            
        .def("fovy", &Frustum::fovy,
             "F.fovy() -- derives and returns the "
             "y field of view (in radians) for frustum F")
            
        .def("aspect", &Frustum::aspect,
             "F.aspect() -- derives and returns the "
             "aspect ratio for frustum F")
            
        .def("projectionMatrix", &Frustum::projectionMatrix,
             "F.projectionMatrix() -- derives and returns "
             "the projection matrix for frustum F")
            
        .def("window", &Frustum::window,
             "F.window(l,r,b,t) -- takes a rectangle in "
             "the screen space (i.e., -1 <= l <= r <= 1, "
             "-1 <= b <= t <= 1) of F and returns a new "
             "Frustum whose near clipping-plane window "
             "is that rectangle in local space")

        .def("projectScreenToRay", &Frustum::projectScreenToRay, 
             "F.projectScreenToRay(V) -- returns a Line3 "
             "through V, a V2 point in screen space")
            
        .def("projectScreenToRay", [](const Frustum& self, py::sequence const& seq) -> Line3<T> {
                if(seq.size() != 2) {
                    throw std::invalid_argument ( "projectScreenToRay expects a sequence of length 2");
                }

                Vec2<T> const point{py::cast<T>(seq[0]), py::cast<T>(seq[1])};
                return self.projectScreenToRay(point);
            })
            
        .def("projectPointToScreen", &Frustum::projectPointToScreen, 
             "F.projectPointToScreen(V) -- returns the "
             "projection of V3 V into screen space")
            
        .def("projectPointToScreen", [](const Frustum& self, py::sequence const& seq) -> Vec2<T> {
                if(seq.size() != 3) {
                    throw std::invalid_argument ( "projectPointToScreen expects a sequence of length 3");
                }

                Vec3<T> const point{py::cast<T>(seq[0]), py::cast<T>(seq[1]), py::cast<T>(seq[2])};
                return self.projectPointToScreen(point);
            })
    
        .def("ZToDepth", &Frustum::ZToDepth,
             "F.ZToDepth(z, zMin, zMax) -- returns the "
             "depth (Z in the local space of the "
             "frustum F) corresponding to z (a result of "
             "transformation by F's projection matrix) "
             "after normalizing z to be between zMin "
             "and zMax")
             
        .def("normalizedZToDepth", &Frustum::normalizedZToDepth,
             "F.normalizedZToDepth(z) -- returns the "
             "depth (Z in the local space of the "
             "frustum F) corresponding to z (a result of "
             "transformation by F's projection matrix), "
             "which is assumed to have been normalized "
             "to [-1, 1]")
            
        .def("DepthToZ", &Frustum::DepthToZ,
             "F.DepthToZ(depth, zMin, zMax) -- converts "
             "depth (Z in the local space of the frustum "
             "F) to z (a result of  transformation by F's "
             "projection matrix) which is normalized to "
             "[zMin, zMax]")
            
        .def("worldRadius", &Frustum::worldRadius,
             "F.worldRadius(V, r) -- returns the radius "
             "in F's local space corresponding to the "
             "point V and radius r in screen space")
            
        .def("worldRadius", [](const Frustum& self, py::sequence const& seq, T radius) -> T {
            if(seq.size() != 3) {
                throw std::invalid_argument ( "worldRadius expects a sequence of length 3");
            }

            Vec3<T> const point{py::cast<T>(seq[0]), py::cast<T>(seq[1]), py::cast<T>(seq[2])};
            return self.worldRadius(point, radius);
        })
            
        .def("screenRadius", &Frustum::screenRadius,
             "F.screenRadius(V, r) -- returns the radius "
             "in screen space corresponding to "
             "the point V and radius r in F's local "
             "space")
            
        .def("screenRadius", [](const Frustum& self, py::sequence const& seq, T radius) -> T{
            if(seq.size() != 3) {
                throw std::invalid_argument ("screenRadius expects a sequence of length 3");
            }

            Vec3<T> const point{py::cast<T>(seq[0]), py::cast<T>(seq[1]), py::cast<T>(seq[2])};
            return self.screenRadius(point, radius);
        })
        .def("__repr__", [name](const Frustum& f) {
            std::stringstream stream;
            if (std::is_same<T, float>::value) {
                stream.precision(9);
            } else if (std::is_same<T, double>::value) {
                stream.precision(17);
            }
            stream << name << "(" << f.nearPlane() << ", " << f.farPlane() << ", "
                   << f.left() << ", " << f.right() << ", " << f.top() << ", "
                   << f.bottom() << ", " << f.orthographic() << ")";    
            return stream.str();
        })
        ;
}

template <class FrustumTest>
void
register_frustumtest(py::module& m, char const* name)
{
    typedef typename FrustumTest::value_type T;
    typedef Vec3<T> Vec;
    typedef Box<Vec> Box;
    
    py::class_<FrustumTest> frustumTest(m, name);
    frustumTest.attr("__module__") = "";
    frustumTest.def(py::init<>(), "FrustumTest() default construction")
        .def(py::init<FrustumTest>(), "Copy constructor")
        .def(py::init<Frustum<T>,Matrix44<T>>(),"construct from frustum and camera transform")

        .def("isVisible", [](const FrustumTest& self, const Vec& v) {
            return self.isVisible(v);
        })
        .def("isVisible", [](const FrustumTest& self, const Box& box) {
            return self.isVisible(box);
        })
        .def("completelyContains", [](const FrustumTest& self, const Box& box) {
            return self.completelyContains(box);
        })
        .def("cameraMat", &FrustumTest::cameraMat)
        .def("currentFrustum", &FrustumTest::currentFrustum)
        ;
}

} // namespace

namespace PyBindImath {
        
void register_imath_frustum(py::module &m)
{
    register_frustum<Frustumf>(m, "Frustumf");
    register_frustum<Frustumd>(m, "Frustumd");

    register_frustumtest<FrustumTestf>(m, "FrustumTestf");
    register_frustumtest<FrustumTestd>(m, "FrustumTestd");
}

} // PyBindImath

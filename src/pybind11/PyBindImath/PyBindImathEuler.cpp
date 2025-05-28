//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include "PyBindImathVec.h"
#include <ImathEuler.h>
#include <ImathVec.h>
//#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace IMATH_NAMESPACE;

namespace {
    
// needed to convert Eulerf::Order to Euler<T>::Order
template <class T>
static typename Euler<T>::Order
interpretOrder(typename Eulerf::Order order)
{
    typename Euler<T>::Order o = Euler<T>::XYZ;
    switch(order)
    {
      case Eulerf::XYZ:
          o = Euler<T>::XYZ;
          break;
      case Eulerf::XZY:
          o = Euler<T>::XZY;
          break;
      case Eulerf::YZX:
          o = Euler<T>::YZX;
          break;
      case Eulerf::YXZ:
          o = Euler<T>::YXZ;
          break;
      case Eulerf::ZXY:
          o = Euler<T>::ZXY;
          break;
      case Eulerf::ZYX:
          o = Euler<T>::ZYX;
          break;
      case Eulerf::XZX:
          o = Euler<T>::XZX;
          break;
      case Eulerf::XYX:
          o = Euler<T>::XYX;
          break;
      case Eulerf::YXY:
          o = Euler<T>::YXY;
          break;
      case Eulerf::YZY:
          o = Euler<T>::YZY;
          break;
      case Eulerf::ZYZ:
          o = Euler<T>::ZYZ;
          break;
      case Eulerf::ZXZ:
          o = Euler<T>::ZXZ;
          break;
      case Eulerf::XYZr:
          o = Euler<T>::XYZr;
          break;
      case Eulerf::XZYr:
          o = Euler<T>::XZYr;
          break;
      case Eulerf::YZXr:
          o = Euler<T>::YZXr;
          break;
      case Eulerf::YXZr:
          o = Euler<T>::YXZr;
          break;
      case Eulerf::ZXYr:
          o = Euler<T>::ZXYr;
          break;
      case Eulerf::ZYXr:
          o = Euler<T>::ZYXr;
          break;
      case Eulerf::XZXr:
          o = Euler<T>::XZXr;
          break;
      case Eulerf::XYXr:
          o = Euler<T>::XYXr;
          break;
      case Eulerf::YXYr:
          o = Euler<T>::YXYr;
          break;
      case Eulerf::YZYr:
          o = Euler<T>::YZYr;
          break;
      case Eulerf::ZYZr:
          o = Euler<T>::ZYZr;
          break;
      case Eulerf::ZXZr:
          o = Euler<T>::ZXZr;
          break;            
      default:
          break;
    }
    
    return o;
}

// needed to convert Eulerf::InputLayout to Euler<T>::InputLayout
template <class T>
static typename Euler<T>::InputLayout
interpretInputLayout(typename Eulerf::InputLayout layout)
{
    if (layout == Eulerf::XYZLayout)
        return Euler<T>::XYZLayout;
    return Euler<T>::IJKLayout;
}

// needed to convert Eulerf::Axis to Euler<T>::Axis
template <class T>
static typename Euler<T>::Axis
interpretAxis(typename Eulerf::Axis axis)
{
    if (axis == Eulerf::X)
        return Euler<T>::X;
    else if (axis == Eulerf::Y)
        return Euler<T>::Y;
    else
        return Euler<T>::Z;
}

#if XXX
template <class T>
static Euler<T> *
eulerConstructor1(const Vec3<T> &v,
                  typename Eulerf::Order order,
                  typename Eulerf::InputLayout layout = Eulerf::IJKLayout)
{
    typename Euler<T>::Order o = interpretOrder<T>(order);
    typename Euler<T>::InputLayout l = interpretInputLayout<T>(layout);
    return new Euler<T>(v, o, l);
}

template <class T>
static Euler<T> *
eulerConstructor1a(const Vec3<T> &v)
{
    return eulerConstructor1 (v, Eulerf::Default);
}

template <class T>
static Euler<T> *
eulerConstructor1b(const Vec3<T> &v, int iorder)
{
    typename Euler<T>::Order o = typename Euler<T>::Order (iorder);
    return new Euler<T>(v, o);
}

//

template <class T>
static Euler<T> *
eulerConstructor1d(const Euler<T>& e, int iorder)
{
    typename Euler<T>::Order o = typename Euler<T>::Order (iorder);
    return new Euler<T>(e, o);
}

template <class T>
static Euler<T> *
eulerConstructor1e(const Euler<T>& e, int iorder, int layout)
{
    typename Euler<T>::Order o = typename Euler<T>::Order (iorder);
    typename Euler<T>::InputLayout l = typename Euler<T>::InputLayout (layout);
    return new Euler<T>(e, o, l);
}
#endif

// Function to register the Euler class methods
template <class Euler>
void
register_euler(py::module& m, const char* name)
{
    typedef typename Euler::BaseType T;
    typedef typename Euler::Order Order;
    typedef typename Euler::Axis Axis;
    typedef typename Euler::InputLayout InputLayout;
    typedef Vec3<T> Vec;
    
    py::class_<Euler, Vec> euler(m, name);
    euler.attr("__module__") = "";
    euler.def(py::init<>(), "imath Euler default construction")
        .def(py::init([](Eulerf::Order order) {
            Order o = interpretOrder<T>(order);
            return Euler(o);
        }))
        .def(py::init([](const Eulerf& other) {
            return Euler(other);
        }))
        .def(py::init([](const Eulerd& other) {
            return Euler(other);
        }))
        .def(py::init([](const Euler& e, int iorder) {
            return Euler(e, Order(iorder));
        }))
        .def(py::init([](const Euler& e, int iorder, int layout) {
            return Euler(e, Order(iorder), InputLayout(layout));
        }))
        .def(py::init([](const Vec& v, Eulerf::Order order, Eulerf::InputLayout layout = Eulerf::IJKLayout) {
            Order o = interpretOrder<T>(order);
            InputLayout l = interpretInputLayout<T>(layout);
            return Euler(v, o, l);
        }))
        .def(py::init([](const Vec& v) {
            return Euler(v, Euler::Default, Euler::IJKLayout);
        }))
        .def(py::init([](const Vec& v, int iorder) {
            return Euler(v, Order(iorder));
        }))
        .def(py::init([](T i, T j, T k) {
            return Euler(i, j, k, Euler::Default, Euler::IJKLayout);
        }))
        .def(py::init([](T i, T j, T k, int iorder) {
            return Euler(i, j, k, Order(iorder), Euler::IJKLayout);
        }))
        .def(py::init([](T i, T j, T k, int iorder, int layout) {
            return Euler(i, j, k, Order(iorder), InputLayout(layout));
        }))
        .def(py::init([](const Matrix33<T>& m) {
            return Euler(m, Euler::Default);
        }))
        .def(py::init([](const Matrix33<T>& m, int iorder) {
            return Euler(m, Order(iorder));
        }))
        .def(py::init([](const Matrix44<T>& m) {
            return Euler(m, Euler::Default);
        }))
        .def(py::init([](const Matrix44<T>& m, int iorder) {
            return Euler(m, Order(iorder));
        }))
        .def(py::init([](const Quat<T>& q, int iorder) {
            typename Euler::Order o = typename Euler::Order (iorder);
            Euler e(o);
            e.extract(q);
            return e;
        }))
                         
#if XXX
        .def("__init__", make_constructor(eulerConstructor1<T>))
        .def("__init__", make_constructor(eulerConstructor1a<T>))
        .def("__init__", make_constructor(eulerConstructor1b<T>))
        .def("__init__", make_constructor(eulerConstructor1d<T>))
        .def("__init__", make_constructor(eulerConstructor1e<T>))

        .def(py::init<const Vec3<T>&, typename Euler::Order>(), py::arg("v"), py::arg("order") = Euler::XYZ)
        .def(py::init<T, T, T, typename Euler::Order>(), py::arg("x"), py::arg("y"), py::arg("z"), py::arg("order") = Euler::XYZ)
#endif
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("toMatrix33", &Euler::toMatrix33)
        .def("toMatrix44", &Euler::toMatrix44)
        .def("toQuat", &Euler::toQuat)
        .def("order", &Euler::order)
        .def("angleOrder", [](const Euler& self) {
            int i, j, k;
            self.angleOrder(i, j, k);
            return V3i (i, j, k);
        })        
        .def("frameStatic", &Euler::frameStatic, 
             "e.frameStatic() -- returns true if the angles of e\n"
             "are measured relative to a set of fixed axes,\n"
             "or false if the angles of e are measured relative to\n"
             "each other\n")
            
        .def("initialAxis", &Euler::initialAxis, 
             "e.initialAxis() -- returns the initial rotation\n"
             "axis of e (EULER_X_AXIS, EULER_Y_AXIS, EULER_Z_AXIS)")
        
        .def("initialRepeated", &Euler::initialRepeated,
             "e.initialRepeated() -- returns 1 if the initial\n"
             "rotation axis of e is repeated (for example,\n"
             "e.order() == EULER_XYX); returns 0 if the initial\n"
             "rotation axis is not repeated.\n")
        .def("parityEven", &Euler::parityEven, 
             "e.parityEven() -- returns the parity of the\n"
             "axis permutation of e\n")
        .def("set", [](Euler& self, Eulerf::Axis axis, int relative, int parityEven, int firstRepeats) {
            auto a = interpretAxis<T>(axis);
            self.set (a, relative, parityEven, firstRepeats);
        },
            "e.set(i,r,p,f) -- sets the rotation order in e\n"
            "according to the following flags:\n"
            "\n"
            "   i   initial axis (EULER_X_AXIS,\n"
            "       EULER_Y_AXIS or EULER_Z_AXIS)\n"
            "\n"
            "   r   rotation angles are measured relative\n"
            "       to each other (r == 1), or relative to a\n"
            "       set of fixed axes (r == 0)\n"
            "\n"
            "   p   parity of axis permutation is even (r == 1)\n"
            "       or odd (r == 0)\n"
            "\n"
            "   f   first rotation axis is repeated (f == 1)\n"
            "	or not repeated (f == 0)\n")
        .def("setOrder", [](Euler& self, Eulerf::Order order) {
            Order o = interpretOrder<T>(order);
            self.setOrder(o);
        })
        .def("setXYZVector", [](Euler& self, const Vec& v) {
            self.setXYZVector(v);
        })
        .def("setXYZVector", [](Euler& self, const py::tuple& t) {
            self.setXYZVector(vecFromTuple<Vec>(t));
        })
        .def("makeNear", &Euler::makeNear)
        .def("extract", py::overload_cast<const Matrix33<T>&>(&Euler::extract))
        .def("extract", py::overload_cast<const Matrix44<T>&>(&Euler::extract))
        .def("extract", py::overload_cast<const Quat<T>&>(&Euler::extract))
        .def("toXYZVector", &Euler::toXYZVector)
        .def("toMatrix33", &Euler::toMatrix33, "e.toMatrix33() -- converts e into a 3x3 matrix\n")
        .def("toMatrix44", &Euler::toMatrix44, "e.toMatrix44() -- converts e into a 4x4 matrix\n")
        .def("toQuat", &Euler::toQuat, "e.toQuat() -- converts e into a quaternion\n")
 
        .def("__repr__", [name](const Euler& e) {
            std::stringstream stream;
            if (std::is_same<T, float>::value) {
                stream.precision(9);
            } else if (std::is_same<T, double>::value) {
                stream.precision(17);
            }
            stream << name << "(" << e.x << ", " << e.y << ", " << e.z << ", " << e.order() << ")";
            return stream.str();
        })
        ;

    py::enum_<Order>(euler, "Order")
        .value("XYZ", Euler::XYZ)
        .value("XZY", Euler::XZY)
        .value("YZX", Euler::YZX)
        .value("YXZ", Euler::YXZ)
        .value("ZXY", Euler::ZXY)
        .value("ZYX", Euler::ZYX)
        .value("XZX", Euler::XZX)
        .value("XYX", Euler::XYX)
        .value("YXY", Euler::YXY)
        .value("YZY", Euler::YZY)
        .value("ZYZ", Euler::ZYZ)
        .value("ZXZ", Euler::ZXZ)

        .value("XYZr", Euler::XYZr)
        .value("XZYr", Euler::XZYr)
        .value("YZXr", Euler::YZXr)
        .value("YXZr", Euler::YXZr)
        .value("ZXYr", Euler::ZXYr)
        .value("ZYXr", Euler::ZYXr)
        .value("XZXr", Euler::XZXr)
        .value("XYXr", Euler::XYXr)
        .value("YXYr", Euler::YXYr)
        .value("YZYr", Euler::YZYr)
        .value("ZYZr", Euler::ZYZr)
        .value("ZXZr", Euler::ZXZr)
        .export_values();

    euler.attr("Default") = Euler::XYZ;
    
    // Enums for Axis
    py::enum_<Axis>(euler, "Axis")
        .value("X", Euler::X)
        .value("Y", Euler::Y)
        .value("Z", Euler::Z)
        .export_values();

    // Enums for InputLayout
    py::enum_<InputLayout>(euler, "InputLayout")
        .value("XYZLayout", Euler::XYZLayout)
        .value("IJKLayout", Euler::IJKLayout)
        .export_values();
}

} // namespace

namespace PyBindImath {

// Function to register the Euler types for float and double
void
register_imath_euler(py::module& m)
{
    register_euler<Eulerf>(m, "Eulerf");
    register_euler<Eulerd>(m, "Eulerd");

    m.attr("EULER_XYZ")  = Eulerf::XYZ;
    m.attr("EULER_XZY")  = Eulerf::XZY;
    m.attr("EULER_YZX")  = Eulerf::YZX;
    m.attr("EULER_YXZ")  = Eulerf::YXZ;
    m.attr("EULER_ZXY")  = Eulerf::ZXY;
    m.attr("EULER_ZYX")  = Eulerf::ZYX;
    m.attr("EULER_XZX")  = Eulerf::XZX;
    m.attr("EULER_XYX")  = Eulerf::XYX;
    m.attr("EULER_YXY")  = Eulerf::YXY;
    m.attr("EULER_YZY")  = Eulerf::YZY;
    m.attr("EULER_ZYZ")  = Eulerf::ZYZ;
    m.attr("EULER_ZXZ")  = Eulerf::ZXZ;
    m.attr("EULER_XYZr") = Eulerf::XYZr;
    m.attr("EULER_XZYr") = Eulerf::XZYr;
    m.attr("EULER_YZXr") = Eulerf::YZXr;
    m.attr("EULER_YXZr") = Eulerf::YXZr;
    m.attr("EULER_ZXYr") = Eulerf::ZXYr;
    m.attr("EULER_ZYXr") = Eulerf::ZYXr;
    m.attr("EULER_XZXr") = Eulerf::XZXr;
    m.attr("EULER_XYXr") = Eulerf::XYXr;
    m.attr("EULER_YXYr") = Eulerf::YXYr;
    m.attr("EULER_YZYr") = Eulerf::YZYr;
    m.attr("EULER_ZYZr") = Eulerf::ZYZr;
    m.attr("EULER_ZXZr") = Eulerf::ZXZr;
    m.attr("EULER_X_AXIS") = Eulerf::X;
    m.attr("EULER_Y_AXIS") = Eulerf::Y;
    m.attr("EULER_Z_AXIS") = Eulerf::Z;
    m.attr("EULER_IJKLayout") = Eulerf::IJKLayout;
    m.attr("EULER_XYZLayout") = Eulerf::XYZLayout;
}

} // namespace PyBindImath
 



//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include "PyBindImathVec.h"

namespace py = pybind11;
using namespace IMATH_NAMESPACE;

namespace {

template <typename T>
bool
lessThanShear(const Shear6<T>& a, const Shear6<T>& b)
{
    return (a[0] <= b[0] && a[1] <= b[1] && a[2] <= b[2] 
            && a[3] <= b[3] && a[4] <= b[4] && a[5] <= b[5]) && a != b;
}

template <class Shear>
void
throwIfZero(const Shear& s)
{
    if (s.xy == 0 ||
        s.xz == 0 || 
        s.yz == 0 || 
        s.yx == 0 || 
        s.zx == 0 || 
        s.zy == 0)
        throw std::domain_error ("Division by Zero");
}
    
template <typename T>
void
throwIfZeroT(T s)
{
    if (s == 0)
        throw std::domain_error ("Division by Zero");
}

template <class Shear>
Shear
shearFromTuple(const py::tuple& t)
{
    typedef typename Shear::BaseType T;

    if (t.size() == 6)
        return Shear(t[0].cast<T>(), t[1].cast<T>(), t[2].cast<T>(), t[3].cast<T>(), t[4].cast<T>(), t[5].cast<T>());
    if (t.size() == 3)
        return Shear(t[0].cast<T>(), t[1].cast<T>(), t[2].cast<T>(), 0, 0, 0);
    throw std::invalid_argument ("Shear6 expects tuple of length 3 or 6");
}
    

template <class Shear>
void
register_shear(py::module& m, const char *name)
{
    typedef typename Shear::BaseType T;

    py::class_<Shear> c(m, name);    
    c.attr("__module__") = "";
    c.def(py::init<>(), "default construction: (0 0 0 0 0 0)")
        .def(py::init<T,T,T>(), "Shear(XY,XZ,YZ) construction: (XY XZ YZ 0 0 0)")
        .def(py::init<V3i>(), "Shear(v) construction: (v.x v.y v.z 0 0 0)")
        .def(py::init<V3f>(), "Shear(v) construction: (v.x v.y v.z 0 0 0)")
        .def(py::init<V3d>(), "Shear(v) construction: (v.x v.y v.z 0 0 0)")
        .def(py::init<T,T,T,T,T,T>(), "Shear(XY, XZ, YZ, YX, ZX, ZY) construction")
        .def(py::init([](T a) { return Shear(a,a,a,a,a,a); }))
        .def(py::init([](const Shear6f& s) { return Shear(s); }))
        .def(py::init([](const Shear6d& s) { return Shear(s); }))
        .def(py::init([](py::tuple& t) {
            return shearFromTuple<Shear>(t);
        }))
        .def("__add__",      [](const Shear& self, const Shear6f& o) { return Shear(self) + Shear(o); })
        .def("__add__",      [](const Shear& self, const Shear6d& o) { return Shear(self) + Shear(o); })
        .def("__add__",      [](const Shear& self, float t) { return Shear(self) + Shear(t,t,t,t,t,t); })
        .def("__add__",      [](const Shear& self, int t) { return Shear(self) + Shear(t,t,t,t,t,t); })
        .def("__add__",      [](const Shear& self, const py::tuple& t) { return Shear(self) + shearFromTuple<Shear>(t); })

        .def("__radd__",     [](const Shear& self, const Shear6f& o) { return Shear(o) + Shear(self); })
        .def("__radd__",     [](const Shear& self, const Shear6d& o) { return Shear(o) + Shear(self); })
        .def("__radd__",     [](const Shear& self, float t) { return Shear(t,t,t,t,t,t) + Shear(self); })
        .def("__radd__",     [](const Shear& self, int t) { return  Shear(t,t,t,t,t,t) + Shear(self); })
        .def("__radd__",     [](const Shear& self, const py::tuple& t) { return shearFromTuple<Shear>(t) + Shear(self); })

        .def("__iadd__",     [](const Shear& self, const Shear6f& o) { return Shear(self) + Shear(o); })
        .def("__iadd__",     [](const Shear& self, const Shear6d& o) { return Shear(self) + Shear(o); })
        .def("__iadd__",     [](const Shear& self, float t) { return Shear(self) + Shear(t,t,t,t,t,t); })
        .def("__iadd__",     [](const Shear& self, int t) { return Shear(self) + Shear(t,t,t,t,t,t); })
        .def("__iadd__",     [](const Shear& self, const py::tuple& t) { return Shear(self) + shearFromTuple<Shear>(t); })

        .def("__sub__",      [](const Shear& self, const Shear6f& o) { return Shear(self) - Shear(o); })
        .def("__sub__",      [](const Shear& self, const Shear6d& o) { return Shear(self) - Shear(o); })
        .def("__sub__",      [](const Shear& self, float t) { return Shear(self) - Shear(t,t,t,t,t,t); })
        .def("__sub__",      [](const Shear& self, int t) { return Shear(self) - Shear(t,t,t,t,t,t); })
        .def("__sub__",      [](const Shear& self, const py::tuple& t) { return Shear(self) - shearFromTuple<Shear>(t); })

        .def("__rsub__",     [](const Shear& self, const Shear6f& o) { return Shear(o) - Shear(self); })
        .def("__rsub__",     [](const Shear& self, const Shear6d& o) { return Shear(o) - Shear(self); })
        .def("__rsub__",     [](const Shear& self, float t) { return Shear(t,t,t,t,t,t) - Shear(self); })
        .def("__rsub__",     [](const Shear& self, int t) { return  Shear(t,t,t,t,t,t) - Shear(self); })
        .def("__rsub__",     [](const Shear& self, const py::tuple& t) { return shearFromTuple<Shear>(t) - Shear(self); })

        .def("__isub__",     [](const Shear& self, const Shear6f& o) { return Shear(self) - Shear(o); })
        .def("__isub__",     [](const Shear& self, const Shear6d& o) { return Shear(self) - Shear(o); })
        .def("__isub__",     [](const Shear& self, float t) { return Shear(self) - Shear(t,t,t,t,t,t); })
        .def("__isub__",     [](const Shear& self, int t) { return Shear(self) - Shear(t,t,t,t,t,t); })
        .def("__isub__",     [](const Shear& self, const py::tuple& t) { return Shear(self) - shearFromTuple<Shear>(t); })

        .def("__mul__",      [](const Shear& self, const Shear6f& o) { return Shear(self) * Shear(o); })
        .def("__mul__",      [](const Shear& self, const Shear6d& o) { return Shear(self) * Shear(o); })
        .def("__mul__",      [](const Shear& self, float t) { return Shear(self) * Shear(t,t,t,t,t,t); })
        .def("__mul__",      [](const Shear& self, int t) { return Shear(self) * Shear(t,t,t,t,t,t); })
        .def("__mul__",      [](const Shear& self, const py::tuple& t) { return Shear(self) * shearFromTuple<Shear>(t); })

        .def("__rmul__",     [](const Shear& self, const Shear6f& o) { return Shear(o) * Shear(self); })
        .def("__rmul__",     [](const Shear& self, const Shear6d& o) { return Shear(o) * Shear(self); })
        .def("__rmul__",     [](const Shear& self, float t) { return Shear(t,t,t,t,t,t) * Shear(self); })
        .def("__rmul__",     [](const Shear& self, int t) { return  Shear(t,t,t,t,t,t) * Shear(self); })
        .def("__rmul__",     [](const Shear& self, const py::tuple& t) { return shearFromTuple<Shear>(t) * Shear(self); })

        .def("__imul__",     [](const Shear& self, const Shear6f& o) { return Shear(self) * Shear(o); })
        .def("__imul__",     [](const Shear& self, const Shear6d& o) { return Shear(self) * Shear(o); })
        .def("__imul__",     [](const Shear& self, float t) { return Shear(self) * Shear(t,t,t,t,t,t); })
        .def("__imul__",     [](const Shear& self, int t) { return Shear(self) * Shear(t,t,t,t,t,t); })
        .def("__imul__",     [](const Shear& self, const py::tuple& t) { return Shear(self) * shearFromTuple<Shear>(t); })

        .def("__div__",      [](const Shear& self, const Shear6f& o) { throwIfZero(o); return Shear(self) / Shear(o); })
        .def("__div__",      [](const Shear& self, const Shear6d& o) { throwIfZero(o); return Shear(self) / Shear(o); })
        .def("__div__",      [](const Shear& self, float t) { throwIfZeroT(t); return Shear(self) / Shear(t,t,t,t,t,t); })
        .def("__div__",      [](const Shear& self, int t) { throwIfZeroT(t); return Shear(self) / Shear(t,t,t,t,t,t); })
        .def("__div__",      [](const Shear& self, const py::tuple& t) { Shear s = shearFromTuple<Shear>(t); throwIfZero(s); return Shear(self) / s; })

        .def("__rdiv__",     [](const Shear& self, const Shear6f& o) { throwIfZero(self); return Shear(o) / Shear(self); })
        .def("__rdiv__",     [](const Shear& self, const Shear6d& o) { throwIfZero(self); return Shear(o) / Shear(self); })
        .def("__rdiv__",     [](const Shear& self, float t) { throwIfZero(self); return Shear(t,t,t,t,t,t) / Shear(self); })
        .def("__rdiv__",     [](const Shear& self, int t) { throwIfZero(self); return  Shear(t,t,t,t,t,t) / Shear(self); })
        .def("__rdiv__",     [](const Shear& self, const py::tuple& t) { throwIfZero(self); return shearFromTuple<Shear>(t) / Shear(self); })

        .def("__idiv__",     [](const Shear& self, const Shear6f& o) { throwIfZero(o); return Shear(self) / Shear(o); })
        .def("__idiv__",     [](const Shear& self, const Shear6d& o) { throwIfZero(o); return Shear(self) / Shear(o); })
        .def("__idiv__",     [](const Shear& self, float t) { throwIfZeroT(t); return Shear(self) / Shear(t,t,t,t,t,t); })
        .def("__idiv__",     [](const Shear& self, int t) { throwIfZeroT(t); return Shear(self) / Shear(t,t,t,t,t,t); })
        .def("__idiv__",     [](const Shear& self, const py::tuple& t) { Shear s = shearFromTuple<Shear>(t); throwIfZero(s); return Shear(self) / s; })

        .def("__truediv__",      [](const Shear& self, const Shear6f& o) { throwIfZero(o); return Shear(self) / Shear(o); })
        .def("__truediv__",      [](const Shear& self, const Shear6d& o) { throwIfZero(o); return Shear(self) / Shear(o); })
        .def("__truediv__",      [](const Shear& self, float t) { throwIfZeroT(t); return Shear(self) / Shear(t,t,t,t,t,t); })
        .def("__truediv__",      [](const Shear& self, int t) { throwIfZeroT(t); return Shear(self) / Shear(t,t,t,t,t,t); })
        .def("__truediv__",      [](const Shear& self, const py::tuple& t) { Shear s = shearFromTuple<Shear>(t); throwIfZero(s); return Shear(self) / s; })

        .def("__rtruediv__",     [](const Shear& self, const Shear6f& o) { throwIfZero(self); return Shear(o) / Shear(self); })
        .def("__rtruediv__",     [](const Shear& self, const Shear6d& o) { throwIfZero(self); return Shear(o) / Shear(self); })
        .def("__rtruediv__",     [](const Shear& self, float t) { throwIfZero(self); return Shear(t,t,t,t,t,t) / Shear(self); })
        .def("__rtruediv__",     [](const Shear& self, int t) { throwIfZero(self); return  Shear(t,t,t,t,t,t) / Shear(self); })
        .def("__rtruediv__",     [](const Shear& self, const py::tuple& t) { throwIfZero(self); return shearFromTuple<Shear>(t) / Shear(self); })

        .def("__itruediv__",     [](const Shear& self, const Shear6f& o) { throwIfZero(o); return Shear(self) / Shear(o); })
        .def("__itruediv__",     [](const Shear& self, const Shear6d& o) { throwIfZero(o); return Shear(self) / Shear(o); })
        .def("__itruediv__",     [](const Shear& self, float t) { throwIfZeroT(t); return Shear(self) / Shear(t,t,t,t,t,t); })
        .def("__itruediv__",     [](const Shear& self, int t) { throwIfZeroT(t); return Shear(self) / Shear(t,t,t,t,t,t); })
        .def("__itruediv__",     [](const Shear& self, const py::tuple& t) { Shear s = shearFromTuple<Shear>(t); throwIfZero(s); return Shear(self) / s; })

        .def("__neg__", [](const Shear& self) { return -self; })
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("setValue", [](Shear& self, T xy, T xz, T yz, T yx, T zx, T zy) 
        {
            self.setValue(xy, xz, yz, yx, zx, zy);
        })
        .def("setValue", [](Shear& self, const Shear& other) 
        {
            self.setValue(other);
        })
        .def("getValue", [](Shear& self, T& xy, T& xz, T& yz, T& yx, T& zx, T& zy) 
        {
            self.setValue(xy, xy, yz, yz, zx, zy);
        })
        .def("negate", &Shear::negate)
        .def_static("baseTypeLowest", &Shear::baseTypeLowest)
        .def_static("baseTypeMax", &Shear::baseTypeMax)
        .def_static("baseTypeSmallest", &Shear::baseTypeSmallest)
        .def_static("baseTypeEpsilon", &Shear::baseTypeEpsilon)
        .def("equalWithAbsError", &Shear::equalWithAbsError)
        .def("equalWithRelError", &Shear::equalWithRelError)
        .def("__lt__", [](const Shear& a, const Shear& b) { return lessThanShear(a, b); })
        .def("__le__", [](const Shear& a, const Shear& b) { return !lessThanShear(b, a); })
        .def("__gt__", [](const Shear& a, const Shear& b) { return lessThanShear(b, a); })
        .def("__ge__", [](const Shear& a, const Shear& b) { return !lessThanShear(a, b); })
        .def("__getitem__", [](Shear& self, int i) { return self[i]; })
        .def("__setitem__", [](Shear& self, int i, T a)
        {
            if(i < 0 || i > 5)
                throw std::domain_error ("Index out of range");
            self[i] = a;
        })
        .def("__len__", [](const Shear& self) { return 6; })
        .def("__repr__", [name](const Shear &self) {
            std::stringstream stream;
            if (std::is_same<T, float>::value) {
                stream.precision(9);
            } else if (std::is_same<T, double>::value) {
                stream.precision(17);
            }
            stream << name 
                   << "(" << self.xy
                   << ", " << self.xz
                   << ", " << self.yz
                   << ", " << self.yx
                   << ", " << self.zx
                   << ", " << self.zy
                   << ")";
            return stream.str();
        })
        ;
}

} // namespace

namespace PyBindImath {

void
register_imath_shear(py::module& m)
{
    register_shear<Shear6f>(m, "Shear6f");
    register_shear<Shear6d>(m, "Shear6d");
}
    
} //namespace PyBinIMath

//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include <ImathVec.h>
#include <ImathVecAlgo.h>

//
// Wrappings for:
//
//  V2s, V2i, V2f, V2d
//  V3s, V3i, V3f, V3d
//  V4s, V4i, V4f, V4d
//
// The wrappings support inter-type operability, i.e:
//
//   v = V2i(1,2) + V2f(3,4)
//
// The wrappings also support constructors and operations between
// vectors and tuples and lists, i.e:
//
//   v = V2i((1,2))
//   v = V2i([1,2])
//   v = V2i(1,2) + (3,4)
//   v = V2i(1,2) + [3,4]
//

namespace py = pybind11;
using namespace IMATH_NAMESPACE;

namespace {

//
// Convert the given tuple to a Vec, to support:
//
//   v = V2i(1,2) + (3,4)
//

template <class Vec>
Vec
vecFromTuple(const py::tuple& t)
{
    typedef typename Vec::BaseType T;

    if (t.size() != Vec::dimensions())
    {
        std::stringstream s;
        s << "tuple must have exactly " << Vec::dimensions() << " elements";
        throw py::value_error(s.str());
    }

    Vec v;
    switch (Vec::dimensions())
    {
    case 4:
        v[3] = t[3].cast<T>();
        // fall through for 2,1,0...
    case 3:
        v[2] = t[2].cast<T>();
        // fall through for 1,0...
    case 2:
    default:
        v[1] = t[1].cast<T>();
        v[0] = t[0].cast<T>();
        break;
    }
    return v;
}

//
// Convert the given list to a Vec, to support:
//
//   v = V2i(1,2) + [3,4]
//

template <class Vec>
Vec
vecFromList(const py::list& l)
{
    typedef typename Vec::BaseType T;

    if (l.size() != Vec::dimensions())
    {
        std::stringstream s;
        s << "list must have exactly " << Vec::dimensions() << " elements";
        throw py::value_error(s.str());
    }

    Vec v;
    switch (Vec::dimensions())
    {
    case 4:
        v[3] = l[3].cast<T>();
        // fall through for 2,1,0...
    case 3:
        v[2] = l[2].cast<T>();
    case 2:
    default:
        // fall through for 1,0...
        v[1] = l[1].cast<T>();
        v[0] = l[0].cast<T>();
        break;
    }
    return v;
}

template <typename Vec>
bool
lessThan(const Vec& a, const Vec& b)
{
    if constexpr (std::is_same_v<Vec, Vec2<typename Vec::BaseType>>) {
        return (a.x < b.x) || (a.x == b.x && a.y < b.y);
    } else if constexpr (std::is_same_v<Vec, Vec3<typename Vec::BaseType>>) {
        return (a.x < b.x) || (a.x == b.x && ((a.y < b.y) || (a.y == b.y && a.z < b.z)));
    } else if constexpr (std::is_same_v<Vec, Vec4<typename Vec::BaseType>>) {
        return (a.x < b.x) || (a.x == b.x && ((a.y < b.y) || (a.y == b.y && ((a.z < b.z) || (a.z == b.z && a.w < b.w)))));
    }
    return false;
}

template <class Vec>
std::string
repr(const char* name, const Vec& v)
{
    typedef typename Vec::BaseType T;

    std::stringstream s;

    if constexpr (std::is_same_v<T, float>) {
        s.precision(9);
    } else if constexpr (std::is_same_v<T, double>) {
        s.precision(17);
    }
    s << std::fixed;
    s << name << "(" << v[0] << ", " << v[1];
    if (Vec::dimensions() > 2)
        s << ", " << v[2];
    if (Vec::dimensions() > 3)
        s << ", " << v[3];
    s << ")";
    return s.str();
}

template <class Vec>
py::class_<Vec>
register_vec(py::class_<Vec>& c)
{
    typedef typename Vec::BaseType T;

    return c.def(py::self == py::self)
        .def("__len__", [](const Vec& self) {
            return self.dimensions();
        })
        .def("__getitem__", [](const Vec &v, size_t i) {
            if (i < 0 || i >= v.dimensions())
                throw std::domain_error ("invalid index");
            return v[static_cast<int>(i)];
        })
        .def("__setitem__", [](Vec &v, size_t i, T value) {
            if (i < 0 || i >= v.dimensions())
                throw std::domain_error ("invalid index");
            v[static_cast<int>(i)] = value;
        })
        .def(py::self != py::self)
        .def(py::self ^ py::self)

        .def("__add__", [](const Vec& self, const py::tuple& t) { return self + vecFromTuple<Vec>(t); })
        .def("__add__", [](const Vec& self, const py::list& t) { return self + vecFromList<Vec>(t); })
        .def("__add__", [](const Vec& self, T t) { return self + Vec(t); })
        .def("__radd__", [](const Vec& self, const py::tuple& t) { return self + vecFromTuple<Vec>(t); })
        .def("__radd__", [](const Vec& self, const py::list& t) { return self + vecFromList<Vec>(t); })
        .def("__radd__", [](const Vec& self, T t) { return self + Vec(t); })
        .def("__iadd__", [](Vec& self, const py::tuple& t) { return self += vecFromTuple<Vec>(t); })
        .def("__iadd__", [](Vec& self, const py::list& t) { return self += vecFromList<Vec>(t); })
        .def("__iadd__", [](Vec& self, T t) { return self += Vec(t); })

        .def("__sub__", [](const Vec& self, const py::tuple& t) { return self - vecFromTuple<Vec>(t); })
        .def("__sub__", [](const Vec& self, const py::list& t) { return self - vecFromList<Vec>(t); })
        .def("__sub__", [](const Vec& self, T t) { return self - Vec(t); })
        .def("__rsub__", [](const Vec& self, const py::tuple& t) {  return vecFromTuple<Vec>(t) - self; })
        .def("__rsub__", [](const Vec& self, const py::list& t) {  return vecFromList<Vec>(t) - self; })
        .def("__rsub__", [](const Vec& self, T t) { return Vec(t) - self; })
        .def("__isub__", [](Vec& self, const py::tuple& t) { return self -= vecFromTuple<Vec>(t); })
        .def("__isub__", [](Vec& self, const py::list& t) { return self -= vecFromList<Vec>(t); })
        .def("__isub__", [](Vec& self, T t) { return self -= Vec(t); })
        .def(-py::self)

        .def("__mul__", [](const Vec& self, const py::tuple& t) { return self * vecFromTuple<Vec>(t); })
        .def("__mul__", [](const Vec& self, const py::list& t) { return self * vecFromList<Vec>(t); })
        .def("__mul__", [](const Vec& self, T t) { return self * Vec(t); })
        .def("__rmul__", [](const Vec& self, const py::tuple& t) {  return vecFromTuple<Vec>(t) * self; })
        .def("__rmul__", [](const Vec& self, const py::list& t) {  return vecFromList<Vec>(t) * self; })
        .def("__rmul__", [](const Vec& self, T t) { return Vec(t) * self; })
        .def("__imul__", [](Vec& self, const py::tuple& t) { return self *= vecFromTuple<Vec>(t); })
        .def("__imul__", [](Vec& self, const py::list& t) { return self *= vecFromList<Vec>(t); })
        .def("__imul__", [](Vec& self, T t) { return self *= Vec(t); })

        .def("__div__", [](const Vec& self, const py::tuple& t) { return self / vecFromTuple<Vec>(t); })
        .def("__div__", [](const Vec& self, const py::list& t) { return self / vecFromList<Vec>(t); })
        .def("__div__", [](const Vec& self, T t) { return self / Vec(t); })
        .def("__truediv__", [](const Vec& self, const py::tuple& t) { return self / vecFromTuple<Vec>(t); })
        .def("__truediv__", [](const Vec& self, const py::list& t) { return self / vecFromList<Vec>(t); })
        .def("__truediv__", [](const Vec& self, T t) { return self / Vec(t); })
        .def("__rdiv__", [](const Vec& self, const py::tuple& t) {  return vecFromTuple<Vec>(t) / self; })
        .def("__rdiv__", [](const Vec& self, const py::list& t) {  return vecFromList<Vec>(t) / self; })
        .def("__rdiv__", [](const Vec& self, T t) { return Vec(t) / self; })
        .def("__idiv__", [](Vec& self, const py::tuple& t) { return self /= vecFromTuple<Vec>(t); })
        .def("__idiv__", [](Vec& self, const py::list& t) { return self /= vecFromList<Vec>(t); })
        .def("__idiv__", [](Vec& self, T t) { return self /= Vec(t); })
        .def("__itruediv__", [](Vec& self, const py::tuple& t) { return self /= vecFromTuple<Vec>(t); })
        .def("__itruediv__", [](Vec& self, const py::list& t) { return self /= vecFromList<Vec>(t); })
        .def("__itruediv__", [](Vec& self, T t) { return self /= Vec(t); })

        .def("__lt__", &lessThan<Vec>)
        .def("__le__", [](const Vec& a, const Vec& b) { return !lessThan(b, a); })
        .def("__gt__", [](const Vec& a, const Vec& b) { return lessThan(b, a); })
        .def("__ge__", [](const Vec& a, const Vec& b) { return !lessThan(a, b); })

        .def_static("baseTypeEpsilon", &Vec::baseTypeEpsilon, "epsilon value of the base type of the vector")
        .def_static("baseTypeMax", &Vec::baseTypeMax, "max value of the base type of the vector")
        .def_static("baseTypeLowest", &Vec::baseTypeLowest, "largest negative value of the base type of the vector")
        .def_static("baseTypeSmallest", &Vec::baseTypeSmallest, "smallest value of the base type of the vector")

        // allow int or float as the 3rd arg (epsilon)
        .def("equalWithAbsError", [](Vec& self, const Vec& other, int e) {
            return self.equalWithAbsError(other, T(e));
        })
        .def("equalWithAbsError", [](Vec& self, const Vec& other, float e) {
            return self.equalWithAbsError(other, T(e));
        })
        .def("equalWithRelError", [](Vec& self, const Vec& other, int e) {
            return self.equalWithRelError(other, T(e));
        })
        .def("equalWithRelError", [](Vec& self, const Vec& other, float e) {
            return self.equalWithRelError(other, T(e));
        })

        .def("dimensions", &Vec::dimensions, "return the number of dimensions in the vector")
        .def("dot", &Vec::dot, "return inner product of the two vectors")
        .def("negate", &Vec::negate, "negate the vector in place")
        .def("length2", &Vec::length2,"return the squared magnitude of the vector")
        .def("closestVertex", [](Vec& self, const Vec& v0, const Vec& v1, const Vec& v2) {
            // In C++/Imath it's a global function; in python it's a member:
            //   self.closestVertex(v0,v1,v2)
            // vs:
            //   closestVertex(v0,v1,v2,self)
            return IMATH_NAMESPACE::closestVertex(v0, v1, v2, self);
        }, "return the vertex of the triangle closes to the given vector")
        ;
}

//
// operations *between* vector classes, i.e. V3f + V3i.
//

template <class Vec, class Other>
void
register_vec_arithmetic(py::class_<Vec>& c)
{
    c.def(py::init([](const Other& o) {return Vec(o);}))
        .def("__add__",      [](const Vec& self, const Other& o) { return Vec(self) + Vec(o); })
        .def("__iadd__",     [](const Vec& self, const Other& o) { return Vec(self) + Vec(o); })
        .def("__sub__",      [](const Vec& self, const Other& o) { return Vec(self) - Vec(o); })
        .def("__isub__",     [](const Vec& self, const Other& o) { return Vec(self) - Vec(o); })
        .def("__mul__",      [](const Vec& self, const Other& o) { return Vec(self) * Vec(o); })
        .def("__imul__",     [](const Vec& self, const Other& o) { return Vec(self) * Vec(o); })
        .def("__div__",      [](const Vec& self, const Other& o) { return Vec(self) / Vec(o); })
        .def("__truediv__",  [](const Vec& self, const Other& o) { return Vec(self) / Vec(o); })
        .def("__idiv__",     [](const Vec& self, const Other& o) { return Vec(self) / Vec(o); })
        .def("__itruediv__", [](const Vec& self, const Other& o) { return Vec(self) / Vec(o); });
}

//
// operations that apply only apply to float and double, not short and int
//

template <class Vec>
py::class_<Vec>
register_vec_fp(py::class_<Vec> c)
{
    return c.def("length", &Vec::length, "return the magnitude of the vector")
        .def("normalize", &Vec::normalize, "destructively normalizes v and returns a reference to it")
        .def("normalizeExc", &Vec::normalizeExc, "destructively normalizes V and returns a reference to it, throwing an exception if length() == 0")
        .def("normalizeNonNull",  &Vec::normalizeNonNull, "destructively normalizes V and returns a reference to it, faster if length() != 0")
        .def("normalized", &Vec::normalized, "return a normalized copy of v")
        .def("normalizedExc", &Vec::normalizedExc, "returnsa normalized copy of v, throwing an exception if length() == 0")
        .def("normalizedNonNull", &Vec::normalizedNonNull, "return a normalized copy of v, faster if lngth() != 0")
        .def("orthogonal", orthogonal<Vec>, "return the vector that is perpendicular to this vector")
        .def("project", [](const Vec& self, const Vec& p) {
            // In C++/Imath it's a global function; in python it's a member:
            //   self.project(p)
            // vs:
            //   project(p, self)
            return IMATH_NAMESPACE::project(p, self);
        }, "return the projection of the given vector onto this vector")
        .def("reflect", [](const Vec& self, const Vec& n) {
            // In C++/Imath it's a global function; in python it's a member:
            //   self.reflect(n)
            // vs:
            //   reflect(self, n)
            return IMATH_NAMESPACE::reflect(self, n);
        }, "return the direction of a ray after reflection off a plane with the given normal")
        ;
}

//
// Vec2<T>
//

template <class Vec>
py::class_<Vec>
register_vec2(py::module& m, const char * name)
{
    typedef typename Vec::BaseType T;

    py::class_<Vec> c(m, name);
    c.def("__repr__", [name](const Vec& v) { return repr(name, v); })
        .def(py::init([](){return Vec(0);}))
        .def(py::init<short>())
        .def(py::init<short,short>())
        .def(py::init<int>())
        .def(py::init<int,int>())
        .def(py::init<float>())
        .def(py::init<double>())
        .def(py::init<float,float>())
        .def(py::init<double,double>())
        .def(py::init([](const py::tuple& t) { return vecFromTuple<Vec>(t); }))
        .def(py::init([](const py::list& l) { return vecFromList<Vec>(l); }))
        .def(py::self % py::self)
        .def_readwrite("x", &Vec::x)
        .def_readwrite("y", &Vec::y)
        .def("setValue", [](Vec& self, T x, T y) { self.setValue(x, y); }, "set to the given x,y values")
        .def("cross", &Vec::cross, "return the right-handed cross product with the given vector")
        ;

    register_vec<Vec>(c);

    return py::cast<py::class_<Vec>>(c);
}

//
// Vec3<T>
//

template <class Vec>
py::class_<Vec>
register_vec3(py::module& m, const char * name)
{
    typedef typename Vec::BaseType T;

    py::class_<Vec> c(m, name);
    c.def("__repr__", [name](const Vec& v) { return repr(name, v); })
        .def(py::init([](){return Vec(0);}))
        .def(py::init<short>())
        .def(py::init<short,short,short>())
        .def(py::init<int>())
        .def(py::init<int,int,int>())
        .def(py::init<float>())
        .def(py::init<float,float,float>())
        .def(py::init<double>())
        .def(py::init<double,double,double>())
        .def(py::init([](const py::tuple& o) { Vec v; return vecFromTuple<Vec>(o); }))
        .def(py::init([](const py::list& o) { Vec v; return vecFromList<Vec>(o); }))
        .def(py::self % py::self)
        .def(py::self %= py::self)
        .def_readwrite("x", &Vec::x)
        .def_readwrite("y", &Vec::y)
        .def_readwrite("z", &Vec::z)
        .def("setValue", [](Vec& self, T x, T y, T z) { self.setValue(x, y, z); }, "set to the given x,y,z values")
        .def("cross", &Vec::cross, "return the right-handed cross product with the given vector")
        ;

    register_vec<Vec>(c);

    return py::cast<py::class_<Vec>>(c);
}

//
// Vec4<T>
//

template <class Vec>
py::class_<Vec>
register_vec4(py::module& m, const char * name)
{
    typedef typename Vec::BaseType T;

    py::class_<Vec> c(m, name);
    c.def("__repr__", [name](const Vec& v) { return repr(name, v); })
        .def(py::init([](){return Vec(0);}))
        .def(py::init<short>())
        .def(py::init<short,short,short,short>())
        .def(py::init<int>())
        .def(py::init<int,int,int,int>())
        .def(py::init<float>())
        .def(py::init<float,float,float,float>())
        .def(py::init<double>())
        .def(py::init<double,double,double,double>())
        .def(py::init([](const py::tuple& o) { Vec v; return vecFromTuple<Vec>(o); }))
        .def(py::init([](const py::list& o) { Vec v; return vecFromList<Vec>(o); }))
        .def_readwrite("x", &Vec::x)
        .def_readwrite("y", &Vec::y)
        .def_readwrite("z", &Vec::z)
        .def_readwrite("w", &Vec::z)
        .def("setValue", [](Vec& self, T x, T y, T z, T w) { self.setValue(x, y, z, w); }, "set to the given x,y,z,w values")
        ;

    register_vec<Vec>(c);

    return py::cast<py::class_<Vec>>(c);
}

} // namespace

namespace PyBindImath {

void
register_imath_vec(py::module& m)
{
    // register all combinations of types, to support interoperability

    auto v2s = register_vec2<V2s>(m, "V2s");
    auto v2i = register_vec2<V2i>(m, "V2i");
    auto v2f = register_vec_fp(register_vec2<V2f>(m, "V2f"));
    auto v2d = register_vec_fp(register_vec2<V2d>(m, "V2d"));

    register_vec_arithmetic<V2s,V2s>(v2s);
    register_vec_arithmetic<V2s,V2i>(v2s);
    register_vec_arithmetic<V2s,V2f>(v2s);
    register_vec_arithmetic<V2s,V2d>(v2s);

    register_vec_arithmetic<V2i,V2s>(v2i);
    register_vec_arithmetic<V2i,V2i>(v2i);
    register_vec_arithmetic<V2i,V2f>(v2i);
    register_vec_arithmetic<V2i,V2d>(v2i);

    register_vec_arithmetic<V2f,V2s>(v2f);
    register_vec_arithmetic<V2f,V2i>(v2f);
    register_vec_arithmetic<V2f,V2f>(v2f);
    register_vec_arithmetic<V2f,V2d>(v2f);

    register_vec_arithmetic<V2d,V2s>(v2d);
    register_vec_arithmetic<V2d,V2i>(v2d);
    register_vec_arithmetic<V2d,V2f>(v2d);
    register_vec_arithmetic<V2d,V2d>(v2d);

    auto v3s = register_vec3<V3s>(m, "V3s");
    auto v3i = register_vec3<V3i>(m, "V3i");
    auto v3f = register_vec_fp(register_vec3<V3f>(m, "V3f"));
    auto v3d = register_vec_fp(register_vec3<V3d>(m, "V3d"));

    register_vec_arithmetic<V3s,V3s>(v3s);
    register_vec_arithmetic<V3s,V3i>(v3s);
    register_vec_arithmetic<V3s,V3f>(v3s);
    register_vec_arithmetic<V3s,V3d>(v3s);

    register_vec_arithmetic<V3i,V3s>(v3i);
    register_vec_arithmetic<V3i,V3i>(v3i);
    register_vec_arithmetic<V3i,V3f>(v3i);
    register_vec_arithmetic<V3i,V3d>(v3i);

    register_vec_arithmetic<V3f,V3s>(v3f);
    register_vec_arithmetic<V3f,V3i>(v3f);
    register_vec_arithmetic<V3f,V3f>(v3f);
    register_vec_arithmetic<V3f,V3d>(v3f);

    register_vec_arithmetic<V3d,V3s>(v3d);
    register_vec_arithmetic<V3d,V3i>(v3d);
    register_vec_arithmetic<V3d,V3f>(v3d);
    register_vec_arithmetic<V3d,V3d>(v3d);

    auto v4s = register_vec4<V4s>(m, "V4s");
    auto v4i = register_vec4<V4i>(m, "V4i");
    auto v4f = register_vec_fp(register_vec4<V4f>(m, "V4f"));
    auto v4d = register_vec_fp(register_vec4<V4d>(m, "V4d"));

    register_vec_arithmetic<V4s,V4s>(v4s);
    register_vec_arithmetic<V4s,V4i>(v4s);
    register_vec_arithmetic<V4s,V4f>(v4s);
    register_vec_arithmetic<V4s,V4d>(v4s);

    register_vec_arithmetic<V4i,V4s>(v4i);
    register_vec_arithmetic<V4i,V4i>(v4i);
    register_vec_arithmetic<V4i,V4f>(v4i);
    register_vec_arithmetic<V4i,V4d>(v4i);

    register_vec_arithmetic<V4f,V4s>(v4f);
    register_vec_arithmetic<V4f,V4i>(v4f);
    register_vec_arithmetic<V4f,V4f>(v4f);
    register_vec_arithmetic<V4f,V4d>(v4f);

    register_vec_arithmetic<V4d,V4s>(v4d);
    register_vec_arithmetic<V4d,V4i>(v4d);
    register_vec_arithmetic<V4d,V4f>(v4d);
    register_vec_arithmetic<V4d,V4d>(v4d);
}

} // namespace PyBindImath



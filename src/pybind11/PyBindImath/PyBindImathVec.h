
//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include <ImathNamespace.h>
#include <ImathVec.h>
#include <ImathVecAlgo.h>
#include <ImathMatrix.h>
#include <ImathColor.h>

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

template <class Vec>
Vec
vecFromObjectT(const py::object& o)
{
    typedef typename Vec::BaseType T;
    
    auto vecType = py::type::of<Vec>();
    if (py::isinstance(o, vecType))
        return o.cast<Vec>();
    if (py::isinstance<py::list>(o)) 
        return vecFromList<Vec>(o.cast<py::list>());
    if (py::isinstance<py::tuple>(o)) 
        return vecFromList<Vec>(o.cast<py::tuple>());
    if (py::isinstance<py::int_>(o)) 
        return Vec(T(o.cast<int>()));
    if (py::isinstance<py::float_>(o)) 
        return Vec(T(o.cast<float>()));

    std::string typeName = std::string(py::str(vecType.attr("__name__")));

    std::string oTypeName = std::string(py::str(o.get_type().attr("__name__")));

    std::stringstream s;
    s << "invalid vector: " << py::str(o) << " of type " << oTypeName << ", expected " << typeName;
    throw py::value_error(s.str());
}

template <class Vec>
inline Vec
vecFromObject(const py::object& o)
{
    return vecFromObjectT<Vec>(o);
}

template <>
inline V3i
vecFromObject<V3i>(const py::object& o)
{
    if (py::isinstance(o, py::type::of<V3i>()))
        return V3i(o.cast<V3i>());
    if (py::isinstance(o, py::type::of<V3f>()))
        return V3i(o.cast<V3f>());
    if (py::isinstance(o, py::type::of<V3d>()))
        return V3i(o.cast<V3d>());
    return vecFromObjectT<V3i>(o);
}

template <>
inline V3f
vecFromObject<V3f>(const py::object& o)
{
    if (py::isinstance(o, py::type::of<V3i>()))
        return V3f(o.cast<V3i>());
    if (py::isinstance(o, py::type::of<V3f>()))
        return V3f(o.cast<V3f>());
    if (py::isinstance(o, py::type::of<V3d>()))
        return V3f(o.cast<V3d>());
    return vecFromObjectT<V3f>(o);
}

template <>
inline V3d
vecFromObject<V3d>(const py::object& o)
{
    if (py::isinstance(o, py::type::of<V3i>()))
        return V3d(o.cast<V3i>());
    if (py::isinstance(o, py::type::of<V3f>()))
        return V3d(o.cast<V3f>());
    if (py::isinstance(o, py::type::of<V3d>()))
        return V3d(o.cast<V3d>());
    return vecFromObjectT<V3d>(o);
}


template <class Vec>
std::string
repr(const char* name, const Vec& v)
{
    typedef typename Vec::BaseType T;

    std::stringstream s;

    s << name << "(";
    if (std::is_same<T, unsigned char>::value)
    {
        s << static_cast<int>(v[0]) << ", " << static_cast<int>(v[1]);
        if (Vec::dimensions() > 2)
            s << ", " << static_cast<int>(v[2]);
        if (Vec::dimensions() > 3)
            s << ", " << static_cast<int>(v[3]);
    }
    else
    {
        if (std::is_same<T, float>::value) {
            s.precision(9);
        } else if (std::is_same<T, double>::value) {
            s.precision(17);
        }
        s << std::fixed;
        s << v[0] << ", " << v[1];
        if (Vec::dimensions() > 2)
            s << ", " << v[2];
        if (Vec::dimensions() > 3)
            s << ", " << v[3];
    }
    s << ")";
    return s.str();
}

template <typename T>
bool lessThanVec(const Vec2<T>& a, const Vec2<T>& b)
{
    return (a.x < b.x) || (a.x == b.x && a.y < b.y);
}

template <typename T>
bool lessThanVec(const Vec3<T>& a, const Vec3<T>& b)
{
    return (a.x < b.x) ||
        (a.x == b.x && ((a.y < b.y) ||
                        (a.y == b.y && a.z < b.z)));
         
}

template <typename T>
bool lessThanVec(const Vec4<T>& a, const Vec4<T>& b)
{
    return (a.x < b.x) ||
        (a.x == b.x && ((a.y < b.y) ||
                        (a.y == b.y && ((a.z < b.z) ||
                                        (a.z == b.z && a.w < b.w)))));
}

template <typename T>
bool lessThanVec(const Color4<T>& a, const Color4<T>& b)
{
    return (a.r < b.r) ||
        (a.r == b.r && ((a.g < b.g) ||
                        (a.g == b.g && ((a.b < b.b) ||
                                        (a.b == b.b && a.a < b.a)))));
}

template <class Vec>
py::class_<Vec>
register_vec(py::class_<Vec>& c)
{
    typedef typename Vec::BaseType T;

    return c.def(py::self == py::self)
        .def("__eq__", [](const Vec& self, const py::tuple& other) {
            return self == vecFromTuple<Vec>(other);
        })
        .def("__ne__", [](const Vec& self, const py::tuple& other) {
            return self != vecFromTuple<Vec>(other);
        })
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

        .def("__add__", [](const Vec& self, const py::tuple& t) { return self + vecFromTuple<Vec>(t); })
        .def("__add__", [](const Vec& self, const py::list& t) { return self + vecFromList<Vec>(t); })
        .def("__add__", [](const Vec& self, T t) { return self + Vec(t); })
        .def("__radd__", [](const Vec& self, const py::tuple& t) { return self + vecFromTuple<Vec>(t); })
        .def("__radd__", [](const Vec& self, const py::list& t) { return vecFromList<Vec>(t) + self; })
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
        .def("__lt__", [](const Vec& a, const Vec& b) { return lessThanVec(a, b); })
        .def("__le__", [](const Vec& a, const Vec& b) { return !lessThanVec(b, a); })
        .def("__gt__", [](const Vec& a, const Vec& b) { return lessThanVec(b, a); })
        .def("__ge__", [](const Vec& a, const Vec& b) { return !lessThanVec(a, b); })

        .def("negate", &Vec::negate, "negate the vector in place")

        .def_static("baseTypeEpsilon", &Vec::baseTypeEpsilon, "epsilon value of the base type of the vector")
        .def_static("baseTypeMax", &Vec::baseTypeMax, "max value of the base type of the vector")
        .def_static("baseTypeLowest", &Vec::baseTypeLowest, "largest negative value of the base type of the vector")
        .def_static("baseTypeSmallest", &Vec::baseTypeSmallest, "smallest value of the base type of the vector")
        ;

    return c;
}

template <class Vec>
py::class_<Vec>
register_vec_geom(py::class_<Vec>& c)
{
    typedef typename Vec::BaseType T;

        // allow int or float as the 3rd arg (epsilon)
    c.def("equalWithAbsError", [](Vec& self, const py::object& o, int e) {
        return self.equalWithAbsError(vecFromObject<Vec>(o), T(e));
    })
        .def("equalWithAbsError", [](Vec& self, const py::object& o, float e) {
            return self.equalWithAbsError(vecFromObject<Vec>(o), T(e));
        })
        .def("equalWithRelError", [](Vec& self, const py::object& o, int e) {
            return self.equalWithRelError(vecFromObject<Vec>(o), T(e));
        })
        .def("equalWithRelError", [](Vec& self, const py::object& o, float e) {
            return self.equalWithRelError(vecFromObject<Vec>(o), T(e));
        })
        
        .def("dimensions", &Vec::dimensions, "return the number of dimensions in the vector")
        .def("dot", &Vec::dot, "return inner product of the two vectors")
        .def("length2", &Vec::length2,"return the squared magnitude of the vector")
        .def("closestVertex", [](Vec& self, const Vec& v0, const Vec& v1, const Vec& v2) {
            // In C++/Imath it's a global function; in python it's a member:
            //   self.closestVertex(v0,v1,v2)
            // vs:
            //   closestVertex(v0,v1,v2,self)
            return IMATH_NAMESPACE::closestVertex(v0, v1, v2, self);
        }, "return the vertex of the triangle closes to the given vector")
        .def(py::self ^ py::self)
        ;

    return register_vec<Vec>(c);
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

template <class Vec, class Mat>
py::class_<Vec>
register_vec_mat(py::class_<Vec> c)
{
    c.def("__mul__", [](const Vec& self, const Mat& m) { return self * m; })
        .def("__imul__", [](Vec& self, const Mat& m) { return self *= m; })
        ;
    
    return register_vec_fp(c);
}


} // namespace

//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include <ImathNamespace.h>

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
vecFromObject(const py::object& o)
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

    std::stringstream s;
    s << "invalid vector: " << py::str(o);
    throw py::value_error(s.str());
}

} // namespace

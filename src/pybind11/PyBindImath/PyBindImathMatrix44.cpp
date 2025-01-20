//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include <ImathMatrix.h>

#include <ostream>

namespace PyBindImath {
namespace py = pybind11;

namespace {

template<typename T>
struct GetClassName {};

template <>
struct GetClassName<IMATH_NAMESPACE::M44f> {
    static constexpr const char* value = "Matrix44f";
};

template <>
struct GetClassName<IMATH_NAMESPACE::M44d> {
    static constexpr const char* value = "Matrix44d";
};

template <class T, std::size_t SIZE>
struct MatrixRow {
    explicit MatrixRow(T *data) : _data(data) {}
    T& operator [] (std::size_t i) { return _data[i]; }
    T*_data;

    static void register_class(py::module& m, std::string const& rowName)
    {
        // no default constructor
        py::class_<MatrixRow>(m, rowName.c_str())
            .def("__len__", []{return SIZE;})
            .def("__getitem__", [](MatrixRow const& self, std::size_t index) -> T 
            {
                // TODO: Check data boundaries 
                return self._data[index];
            })
            .def("__setitem__", [](MatrixRow& self, std::size_t index, T const& value)
            {
                // TODO: Check data boundaries 
                self._data[index] = value; 
            })
            ;
    }
};

template<typename M, typename T = typename M::value_type>
std::string Matrix44_repr(M const& v) {
    std::ostringstream oss;

    oss.precision(9);
    oss << std::fixed;

    oss << GetClassName<M>::value << "(("
        << v[0][0] << ", " << v[0][1] << ", " << v[0][2] << ", " << v[0][3] << "), ("
        << v[1][0] << ", " << v[1][1] << ", " << v[1][2] << ", " << v[1][3] << "), ("
        << v[2][0] << ", " << v[2][1] << ", " << v[2][2] << ", " << v[2][3] << "), ("
        << v[3][0] << ", " << v[3][1] << ", " << v[3][2] << ", " << v[3][3] << "))";

    return oss.str();
}

template <typename M, std::size_t SIZE, typename T = typename M::value_type, typename R = MatrixRow<T, SIZE>>
void register_Matrix44(py::module& m)
{
    // TODO: Finish implementation of Matrix class

    std::string const matrixName = GetClassName<M>::value;

    // Register MatrixRow
    constexpr char const* rowSuffix = "Row";
    R::register_class(m, matrixName + rowSuffix);

    // Register Matrix type
    py::class_<M>(m, matrixName.c_str())
    .def(py::init<>())
    .def("__getitem__", [](M& self, std::size_t index) -> R {
            T* data = self[index];
            return R{data};
        }, py::arg("i"),
        "Access element at the given index.")
    // set item is not required since MatrixRow is returned for set and get operations
    .def("__repr__", &Matrix44_repr<M>)
    ;
}

} // namespace

void register_imath_matrix(py::module& m)
{
    // TODO: M22 and M33

    register_Matrix44<IMATH_NAMESPACE::M44f, 4>(m);
    register_Matrix44<IMATH_NAMESPACE::M44d, 4>(m);
}

} // PyBindImath
//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include "PyBindImathVec.h"
#include <ImathMatrix.h>
#include <ImathMatrixAlgo.h>

namespace py = pybind11;
using namespace IMATH_NAMESPACE;

namespace {

template <class Matrix>
std::string
reprMatrix(const char* name, const Matrix& m)
{
    typedef typename Matrix::BaseType T;

    std::stringstream s;

    if (std::is_same<T, float>::value) {
        s.precision(9);
    } else if (std::is_same<T, double>::value) {
        s.precision(17);
    }
    s << std::fixed;
    s << name << "(";
    const int n = static_cast<int>(m.dimensions());
    int k = n * n;
    for (int x=0; x<n; x++)
        for (int y=0; y<n; y++)
        {
            s << m[x][y];
            if (--k > 0)
                s << ", ";
        }
    s << ")";
    return s.str();
}

template <class Matrix>
static bool
lessThanMatrix(const Matrix& mat1, const Matrix& mat2)
{
    const int n = static_cast<int>(mat1.dimensions());
    for (int x=0; x<n; x++)
        for (int y=0; y<n; y++)
            if(mat1[x][y] > mat2[x][y])
                return false;
    
    return mat1 != mat2;
}

template <class M>
class RowProxy {
  public:
    using T = typename M::BaseType;
    
    RowProxy(T* r) : row(r) {}

    T& operator[](size_t col)
    {
        if (col >= M::dimensions())
            throw py::index_error();
        return row[col];
    }

  private:

    T* row;
};

template <template <class> class M, class T>
py::class_<M<T>>
register_matrix(py::class_<M<T>>& m, const char* name)
{
    using Matrix = M<T>;

    auto ri = py::return_value_policy::reference_internal;

    return m.def("__repr__", [name](const Matrix& self) { return reprMatrix(name, self); })
        .def(py::init([](){return Matrix();}))
        .def(py::init<T>())
        .def("__getitem__", [](Matrix& self, size_t i) {
            if (i < 0 || i >= Matrix::dimensions())
                throw py::index_error();
            return RowProxy<Matrix>(self[i]);
        })
        .def("__len__", [](const Matrix& self) {
            return self.dimensions();
        })

        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("__lt__", &lessThanMatrix<Matrix>)
        .def("__le__", [](const Matrix& a, const Matrix& b) { return !lessThanMatrix(b, a); })
        .def("__gt__", [](const Matrix& a, const Matrix& b) { return lessThanMatrix(b, a); })
        .def("__ge__", [](const Matrix& a, const Matrix& b) { return !lessThanMatrix(a, b); })
        .def("equalWithAbsError", [](Matrix& self, const Matrix& other, double e) {
            return self.equalWithAbsError(other, typename Matrix::BaseType(e));
        })
        .def("equalWithRelError", [](Matrix& self, const Matrix& other, double e) {
            return self.equalWithRelError(other, typename Matrix::BaseType(e));
        })

        .def("__iadd__", [](Matrix &self, const M<float>& other) -> const Matrix& { return self += Matrix(other); }, ri)
        .def("__iadd__", [](Matrix &self, const M<double>& other) -> const Matrix& { return self += Matrix(other); }, ri)
        .def("__iadd__", [](Matrix &self, T v) -> const Matrix& { return self += v; }, ri)
        .def("__add__", [](const Matrix &self, const Matrix &other) { return self + other; })
        .def("__add__", [](const Matrix &self, T v) { return self + v; })
        .def("__radd__", [](const Matrix &self, T v) { return Matrix(v) + self; })

        .def("__isub__", [](Matrix &self, const M<float>& other) -> const Matrix& { return self -= Matrix(other); }, ri)
        .def("__isub__", [](Matrix &self, const M<double>& other) -> const Matrix& { return self -= Matrix(other); }, ri)
        .def("__isub__", [](Matrix &self, T v) -> const Matrix& { return self -= v; }, ri)
        .def("__sub__", [](const Matrix &self, const Matrix &other) { return self - other; })
        .def("__sub__", [](const Matrix &self, T v) { return self - v; })
        .def("__rsub__", [](const Matrix &self, T v) { return Matrix(v) - self; })

        .def("negate", [](Matrix &self) -> const Matrix& { return self = -self; }, ri)
        .def("__neg__", [](Matrix &self) -> const Matrix& { return self = -self; }, ri)

        .def("__imul__", [](Matrix &self, const M<float>& other) -> const Matrix& { return self *= Matrix(other); }, ri)
        .def("__imul__", [](Matrix &self, const M<double>& other) -> const Matrix& { return self *= Matrix(other); }, ri)
        .def("__imul__", [](Matrix &self, T v) -> const Matrix& { return self *= v; }, ri)
        .def("__mul__", [](const Matrix &self, const M<float> &other) { return self * Matrix(other); })
        .def("__mul__", [](const Matrix &self, const M<double> &other) { return self * Matrix(other); })
        .def("__mul__", [](const Matrix &self, T v) { return self * v; })
        .def("__rmul__", [](const Matrix &self, const M<float> &other) { return Matrix(other) * self; })
        .def("__rmul__", [](const Matrix &self, const M<double> &other) { return Matrix(other) * self; })
        .def("__rmul__", [](const Matrix &self, T v) { return self * v; })

        .def("__idiv__", [](Matrix &self, T v) -> const Matrix& { return self /= v; }, ri)
        .def("__div__", [](const Matrix &self, T v) { return self / v; })
        .def("__itruediv__", [](Matrix &self, T v) -> const Matrix& { return self /= v; }, ri)
        .def("__truediv__", [](const Matrix &self, T v) { return self / v; })

        .def("makeIdentity", &Matrix::makeIdentity)

        .def("setValue", [](Matrix& self, const Matrix& o) { return self.setValue(o); }, ri, "setValue")

        .def("transpose", &Matrix::transpose,ri,"transpose() transpose this matrix")
        .def("transposed", &Matrix::transposed,"transposed() return a transposed copy of this matrix")
        .def("invert", [](Matrix& self) { return self.invert(); }, ri) 
        .def("inverse", [](Matrix& self) { return self.inverse(); }, ri) 
        .def("determinant", &Matrix::determinant,"determinant() return the determinant of this matrix")

        .def_static("baseTypeEpsilon", &Matrix::baseTypeEpsilon,"baseTypeEpsilon() epsilon value of the base type of the matrix")
        .def_static("baseTypeMax", &Matrix::baseTypeMax,"baseTypeMax() max value of the base type of the matrix")
        .def_static("baseTypeLowest", &Matrix::baseTypeLowest,"baseTypeLowest() largest negative value of the base type of the matrix")
        .def_static("baseTypeSmallest", &Matrix::baseTypeSmallest,"baseTypeSmallest() smallest value of the base type of the matrix")
        ;
    
}
    
template <class T>
py::class_<Matrix22<T>>
register_matrix22(py::module& module, const char * name)
{
    using Matrix = Matrix22<T>;
    auto ri = py::return_value_policy::reference_internal;

    py::class_<Matrix> m(module, name);
    m.attr("__module__") = "";
    m.def(py::init<T,T,T,T>())
        .def(py::init([](std::tuple<T, T> row0, std::tuple<T, T> row1) {
            return Matrix(std::get<0>(row0), std::get<1>(row0),
                          std::get<0>(row1), std::get<1>(row1));
        }))
        .def(py::init([](const Matrix22<float>& other) {
            return Matrix(other);
        }))
        .def(py::init([](const Matrix22<double>& other) {
            return Matrix(other);
        }))
        .def("multDirMatrix", [](const Matrix& self, const py::object& src, Vec2<T>& dst) {
            self.multDirMatrix(vecFromObject<Vec2<T>>(src), dst); }, "mult matrix")
        .def("multDirMatrix", [](const Matrix& self, const py::object& src) {
            Vec2<T> dst; self.multDirMatrix(vecFromObject<Vec2<T>>(src), dst); return dst; }, "mult matrix")
        .def("extractEuler", [](const Matrix& self, Vec2<T>& dst) { T rot; extractEuler(self, rot); dst[0] = rot; return dst; }, ri)

        .def("rotate", [](Matrix& self, T r) { return self.rotate(r); }, ri, "rotate matrix")
        .def("setRotation", [](Matrix& self, T r) { return self.setRotation(r); }, ri, "setRotation(s)")

        .def("scale", [](Matrix& self, const py::object& s) { return self.scale(vecFromObject<Vec2<T>>(s)); }, ri, "rotate matrix")
        .def("setScale", [](Matrix& self, T s) { return self.setScale(s); }, ri, "setScale(s)")
        .def("setScale", [](Matrix& self, const py::object& s) { return self.setScale(vecFromObject<Vec2<T>>(s)); }, ri, "setScale(s)")
        ;
    
    register_matrix<Matrix22, T>(m, name);
    return py::cast<py::class_<Matrix>>(m);
}

template <class Matrix>
py::tuple
jacobiEigensolve(const Matrix& m)
{
    typedef typename Matrix::BaseType T;
    typedef typename Matrix::BaseVecType Vec;

    // For the C++ version, we just assume that the passed-in matrix is
    // symmetric, but we assume that many of our script users are less
    // sophisticated and might get tripped up by this.  Also, the cost
    // of doing this check is likely miniscule compared to the Pythonic
    // overhead.

    // Give a fairly generous tolerance to account for possible epsilon drift:
    const int d = Matrix::dimensions();
    const T tol = std::sqrt(std::numeric_limits<T>::epsilon());
    for (int i = 0; i < d; ++i)
    {
        for (int j = i+1; j < d; ++j)
        {
            const T Aij = m[i][j],
                    Aji = m[j][i];
            if (std::abs(Aij - Aji) >= tol){
              throw std::invalid_argument
              ("Symmetric eigensolve requires a symmetric matrix (matrix[i][j] == matrix[j][i]).");
            }
        }
    }

    Matrix tmp = m;
    Matrix Q;
    Vec S;
    jacobiEigenSolver (tmp, S, Q);
    return py::make_tuple (Q, S);
}

M44d
procrustes(const std::vector<V3d>& from_input,
           const std::vector<V3d>& to_input,
           const py::object& weights_input = py::none(),
           bool doScale = false)
{
    // Verify the lengths
    const size_t n = from_input.size();
    if (n != to_input.size())
    {
        throw py::value_error("'from' and 'to' should have the same length");
    }
    
    bool useWeights = !weights_input.is_none();
    std::vector<double> weights;
    
    if (useWeights)
    {
        // Convert py::object to vector<double>
        try {
            weights = weights_input.cast<std::vector<double>>();
        } catch (const py::cast_error&) {
            throw py::type_error("weights must be a sequence of numbers or None");
        }
        
        if (n != weights.size())
        {
            throw py::value_error("'fromPts', 'toPts', and 'weights' should all have the same length");
        }
    }
    
    if (n == 0)
    {
        throw py::value_error("Input sequences cannot be empty");
    }
    
    // Convert to raw pointers for the C++ function
    if (useWeights)
        return procrustesRotationAndTranslation(from_input.data(), to_input.data(), weights.data(), n, doScale);
    else
        return procrustesRotationAndTranslation(from_input.data(), to_input.data(), n, doScale);
}

template <class T>
py::class_<Matrix33<T>>
register_matrix33(py::module& module, const char * name)
{
    using Matrix = Matrix33<T>;
    auto ri = py::return_value_policy::reference_internal;

    py::class_<Matrix> m(module, name);
    m.attr("__module__") = "";
    m.def(py::init<T,T,T,T,T,T,T,T,T>())
        .def(py::init([](std::tuple<T, T, T> row0, std::tuple<T, T, T> row1, std::tuple<T, T, T> row2) {
            return Matrix(std::get<0>(row0), std::get<1>(row0), std::get<2>(row0),
                               std::get<0>(row1), std::get<1>(row1), std::get<2>(row1),
                               std::get<0>(row2), std::get<1>(row2), std::get<2>(row2));
        }))
        .def(py::init([](const Matrix33<float>& other) {
            return Matrix(other);
        }))
        .def(py::init([](const Matrix33<double>& other) {
            return Matrix(other);
        }))
        .def("multDirMatrix", [](const Matrix& self, const py::object& src, Vec2<T>& dst) {
            self.multDirMatrix(vecFromObject<Vec2<T>>(src), dst); }, "mult matrix")
        .def("multDirMatrix", [](const Matrix& self, const py::object& src) {
            Vec2<T> dst; self.multDirMatrix(vecFromObject<Vec2<T>>(src), dst); return dst; }, "mult matrix")
        .def("multVecMatrix", [](const Matrix& self, const py::object& src, Vec2<T>& dst) {
            self.multVecMatrix(vecFromObject<Vec2<T>>(src), dst); }, "mult matrix")
        .def("multVecMatrix", [](const Matrix& self, const py::object& src) {
            Vec2<T> dst; self.multVecMatrix(vecFromObject<Vec2<T>>(src), dst); return dst; }, "mult matrix")

        .def("rotate", [](Matrix& self, T r) { return self.rotate(r); }, ri, "rotate matrix")
        .def("setRotation", [](Matrix& self, T r) { return self.setRotation(r); }, ri, "setRotation(s)")

        .def("setTranslation", [](Matrix& self, const py::object& t) { return self.setTranslation(vecFromObject<Vec2<T>>(t)); }, ri, "setTranslation(s)")
        .def("translate", [](Matrix& self, const py::object& t) { return self.translate(vecFromObject<Vec2<T>>(t)); }, ri, "translate matrix")
        .def("translation", &Matrix::translation, "translation()")

        .def("scale", [](Matrix& self, const py::object& s) { return self.scale(vecFromObject<Vec2<T>>(s)); }, ri, "rotate matrix")
        .def("setScale", [](Matrix& self, T s) { return self.setScale(s); }, ri, "setScale(s)")
        .def("setScale", [](Matrix& self, const py::object& s) { return self.setScale(vecFromObject<Vec2<T>>(s)); }, ri, "setScale(s)")

        .def("shear", [](Matrix& self, T s) { return self.shear(s); }, ri, "setShear")
        .def("shear", [](Matrix& self, const Vec2<T>& s) { return self.shear(s); }, ri, "setShear")
        .def("shear", [](Matrix& self, const py::list& s) { return self.shear(vecFromObject<Vec2<T>>(s)); }, ri, "setShear")
        .def("shear", [](Matrix& self, const py::tuple& s) {
            if (s.size() == 2)
                return self.shear(Vec2<T>(T(s[0].cast<T>()),T(s[1].cast<T>())));
            throw std::domain_error ("m.shear needs tuple of length 2");
        }, ri, "shear")
        .def("setShear", [](Matrix& self, T s) { return self.setShear(s); }, ri, "setShear")
        .def("setShear", [](Matrix& self, const Vec2<T>& s) { return self.setShear(s); }, ri, "setShear")
        .def("setShear", [](Matrix& self, const py::list& s) { return self.setShear(vecFromObject<Vec2<T>>(s)); }, ri, "setShear")
        .def("setShear", [](Matrix& self, const py::tuple& s) {
            if (s.size() == 2)
                return self.setShear(Vec2<T>(T(s[0].cast<T>()),T(s[1].cast<T>())));
            throw std::domain_error ("m.setShear needs tuple of length 2");
        }, ri, "setShear")
        .def("gjInvert", [](Matrix& self) { return self.gjInvert(); }, ri) 
        .def("gjInverse", [](Matrix& self) { return self.gjInverse(); }, ri) 
        .def("minorOf", &Matrix::minorOf,"minorOf() return the matrix minor of the (row,col) element of this matrix")
        .def("fastMinor", &Matrix::fastMinor,"fastMinor() return the matrix minor using the specified rows and columns of this matrix")

        .def("removeScaling", [](Matrix& self, int exc) { return removeScaling(self, exc); }, py::arg("exc")=1, ri)
        .def("removeScalingAndShear", [](Matrix& self, int exc) { return removeScalingAndShear(self, exc); }, py::arg("exc")=1, ri)

        .def("sansScaling", [](const Matrix& self, int exc) { return sansScaling(self, exc); }, py::arg("exc")=1, ri)
        .def("sansScalingAndShear", [](const Matrix& self, int exc) { return sansScalingAndShear(self, exc); }, py::arg("exc")=1, ri)

        .def("extractAndRemoveScalingAndShear", [](Matrix& self, Vec2<T>& dstScl, Vec2<T>& dstShr, int exc) 
        {
            T dstShrTmp;
            extractAndRemoveScalingAndShear(self, dstScl, dstShrTmp, exc);
            dstShr.setValue(dstShrTmp, T(0));
        }, py::arg("dstScl"), py::arg("dstShr"), py::arg("exc") = 1)
        .def("extractEuler", [](const Matrix& self, Vec2<T>& dstObj)
        {
            T dst;
            extractEuler(self, dst);
            dstObj.setValue(dst, T (0));
        })
        .def("extractSHRT", [](const Matrix& self, Vec2<T> &s, Vec2<T> &h, Vec2<T> &r, Vec2<T> &t, int exc)
            {
                T hTmp, rTmp;
                int b = extractSHRT(self, s, hTmp, rTmp, t, exc);
                h.setValue(hTmp, T (0));
                r.setValue(rTmp, T (0));
                return b;
            }, py::arg("s"), py::arg("h"), py::arg("r"), py::arg("t"), py::arg("exc") = 1)
        .def("extractScaling", [](const Matrix& self, Vec2<T> &dst, int exc) { extractScaling(self, dst, exc); }, py::arg("dst"), py::arg("exc") = 1)

        .def("outerProduct", [](Matrix& self, Vec3<T>& a, Vec3<T>& b) { self = outerProduct(a, b); })
        .def("extractScalingAndShear", [](Matrix& self, Vec2<T> &dstScl, Vec2<T> &dstShr, int exc)
            {
                T dstShrTmp;
                extractScalingAndShear(self, dstScl, dstShrTmp, exc);
                dstShr.setValue(dstShrTmp, T (0));
            }, py::arg("dstScl"), py::arg("dstShr"), py::arg("exc") = 1)
        .def("singularValueDecomposition", [](const Matrix& self, bool forcePositiveDeterminant = false)
            {
                Matrix U, V;
                Vec3<T> S;
                jacobiSVD (self, U, S, V, std::numeric_limits<T>::epsilon(), forcePositiveDeterminant);
                return py::make_tuple (U, S, V);
            })
        .def("symmetricEigensolve", &jacobiEigensolve<Matrix>) 
        ;
    
    register_matrix<Matrix33, T>(m, name);
    return py::cast<py::class_<Matrix>>(m);
}

template <class T>
py::class_<Matrix44<T>>
register_matrix44(py::module& module, const char * name)
{
    using Matrix = Matrix44<T>;
    using Vec = Vec3<T>;
    
    auto ri = py::return_value_policy::reference_internal;

    py::class_<Matrix> m(module, name);
    m.attr("__module__") = "";
    m.def(py::init<T,T,T,T, T,T,T,T, T,T,T,T, T,T,T,T>())
        .def(py::init([](std::tuple<T, T, T, T> row0,
                         std::tuple<T, T, T, T> row1,
                         std::tuple<T, T, T, T> row2,
                         std::tuple<T, T, T, T> row3) {
            return Matrix(std::get<0>(row0), std::get<1>(row0), std::get<2>(row0), std::get<3>(row0),
                          std::get<0>(row1), std::get<1>(row1), std::get<2>(row1), std::get<3>(row1),
                          std::get<0>(row2), std::get<1>(row2), std::get<2>(row2), std::get<3>(row2),
                          std::get<0>(row3), std::get<1>(row3), std::get<2>(row3), std::get<3>(row3));
        }))
        .def(py::init([](const Matrix44<float>& other) {
            return Matrix(other);
        }))
        .def(py::init([](const Matrix44<double>& other) {
            return Matrix(other);
        }))
        .def("multDirMatrix", [](const Matrix& self, const py::object& src, Vec& dst) {
            self.multDirMatrix(vecFromObject<Vec>(src), dst); }, "mult matrix")
        .def("multDirMatrix", [](const Matrix& self, const py::object& src) {
            Vec dst; self.multDirMatrix(vecFromObject<Vec>(src), dst); return dst; }, "mult matrix")
        .def("multVecMatrix", [](const Matrix& self, const py::object& src, Vec& dst) {
            self.multVecMatrix(vecFromObject<Vec>(src), dst); }, "mult matrix")
        .def("multVecMatrix", [](const Matrix& self, const py::object& src) {
            Vec dst; self.multVecMatrix(vecFromObject<Vec>(src), dst); return dst; }, "mult matrix")

        .def("rotate", [](Matrix& self, const Vec& r) { return self.rotate(r); }, ri, "rotate matrix")
        .def("rotationMatrix", [](Matrix& self, py::object from, py::object to) {
            Matrix rot = rotationMatrix(vecFromObject<Vec>(from), vecFromObject<Vec>(to));
            return self.setValue(rot);
        })
        .def("rotationMatrixWithUpDir", [](Matrix& self, py::object from, py::object to, py::object up) {
            Matrix rot = rotationMatrixWithUpDir(vecFromObject<Vec>(from), vecFromObject<Vec>(to), vecFromObject<Vec>(up));
            return self.setValue(rot);
        })
        .def("setEulerAngles", [](Matrix& self, const py::object& v) { self.setEulerAngles(vecFromObject<Vec>(v)); })
        .def("setAxisAngle", [](Matrix& self, const py::object& v, T angle) { self.setAxisAngle(vecFromObject<Vec>(v), angle); })

        .def("setTranslation", [](Matrix& self, const py::object& t) { return self.setTranslation(vecFromObject<Vec>(t)); }, ri, "setTranslation(s)")
        .def("translate", [](Matrix& self, const py::object& t) { return self.translate(vecFromObject<Vec>(t)); }, ri, "translate matrix")
        .def("translation", &Matrix::translation, "translation()")

        .def("scale", [](Matrix& self, const py::object& s) { return self.scale(vecFromObject<Vec>(s)); }, ri, "rotate matrix")
        .def("setScale", [](Matrix& self, T s) { return self.setScale(s); }, ri, "setScale(s)")
        .def("setScale", [](Matrix& self, const py::object& s) { return self.setScale(vecFromObject<Vec>(s)); }, ri, "setScale(s)")

        .def("shear", [](Matrix& self, const Vec& s) { return self.shear(s); }, ri, "setShear")
        .def("shear", [](Matrix& self, const py::list& s) { return self.shear(vecFromObject<Vec>(s)); }, ri, "setShear")
        .def("shear", [](Matrix& self, const py::tuple& s) {
            if (s.size() == 3)
                return self.shear(Vec(T(s[0].cast<T>()),T(s[1].cast<T>()),T(s[2].cast<T>())));
            if (s.size() == 6)
                return self.shear(Shear6<T>(T(s[0].cast<T>()),T(s[1].cast<T>()),T(s[2].cast<T>()),
                                               T(s[3].cast<T>()),T(s[4].cast<T>()),T(s[5].cast<T>())));
            throw std::domain_error ("m.shear needs tuple of length 3 or 6");
        })
        .def("setShear", [](Matrix& self, const Vec& s) { return self.setShear(s); }, ri, "setShear")
        .def("setShear", [](Matrix& self, const py::list& s) { return self.setShear(vecFromObject<Vec>(s)); }, ri, "setShear")
        .def("setShear", [](Matrix& self, const py::tuple& s) {
            if (s.size() == 3)
                return self.setShear(Vec(T(s[0].cast<T>()),T(s[1].cast<T>()),T(s[2].cast<T>())));
            if (s.size() == 6)
                return self.setShear(Shear6<T>(T(s[0].cast<T>()),T(s[1].cast<T>()),T(s[2].cast<T>()),
                                               T(s[3].cast<T>()),T(s[4].cast<T>()),T(s[5].cast<T>())));
            throw std::domain_error ("m.setShear needs tuple of length 3 or 6");
        })
        .def("gjInvert", [](Matrix& self) { return self.gjInvert(); }, ri) 
        .def("gjInverse", [](Matrix& self) { return self.gjInverse(); }, ri) 
        .def("minorOf", &Matrix::minorOf,"minorOf() return the matrix minor of the (row,col) element of this matrix")
        .def("fastMinor", &Matrix::fastMinor,"fastMinor() return the matrix minor using the specified rows and columns of this matrix")

        .def("removeScaling", [](Matrix& self, int exc) { return removeScaling(self, exc); }, py::arg("exc")=1, ri)
        .def("removeScalingAndShear", [](Matrix& self, int exc) { return removeScalingAndShear(self, exc); }, py::arg("exc")=1, ri)

        .def("sansScaling", [](const Matrix& self, int exc) { return sansScaling(self, exc); }, py::arg("exc")=1, ri)
        .def("sansScalingAndShear", [](const Matrix& self, int exc) { return sansScalingAndShear(self, exc); }, py::arg("exc")=1, ri)

        .def("extractEulerXYZ", [](const Matrix& self, Vec3<T>& dst)
        {
            extractEulerXYZ(self, dst);
        })
        .def("extractEulerZYX", [](const Matrix& self, Vec3<T>& dst)
        {
            extractEulerZYX(self, dst);
        })

        .def("extractAndRemoveScalingAndShear", [](Matrix& self, Vec& dstScl, Vec& dstShr, int exc) 
             {
                 extractAndRemoveScalingAndShear(self, dstScl, dstShr, exc);
             }, py::arg("dstScl"), py::arg("dstShr"), py::arg("exc") = 1)
        .def("extractSHRT", [](const Matrix& self, Vec &s, Vec &h, Vec &r, Vec &t, int exc)
            {
                return extractSHRT(self, s, h, r, t, exc);
            }, py::arg("s"), py::arg("h"), py::arg("r"), py::arg("t"), py::arg("exc") = 1)
        .def("extractScaling", [](const Matrix& self, Vec &dst, int exc) { extractScaling(self, dst, exc); }, py::arg("dst"), py::arg("exc") = 1)

        .def("outerProduct", [](Matrix& self, Vec4<T>& a, Vec4<T>& b) { self = outerProduct(a, b); })
        .def("extractScalingAndShear", [](Matrix& self, Vec &dstScl, Vec &dstShr, int exc)
            {
                extractScalingAndShear(self, dstScl, dstShr, exc);
            }, py::arg("dstScl"), py::arg("dstShr"), py::arg("exc") = 1)
        .def("singularValueDecomposition", [](const Matrix& self, bool forcePositiveDeterminant = false)
            {
                Matrix U, V;
                Vec4<T> S;
                jacobiSVD (self, U, S, V, std::numeric_limits<T>::epsilon(), forcePositiveDeterminant);
                return py::make_tuple (U, S, V);
            })
        .def("symmetricEigensolve", &jacobiEigensolve<Matrix>) 
        ;
    
    module.def("procrustesRotationAndTranslation", 
               &procrustes,
               py::arg("fromPts"), 
               py::arg("toPts"), 
               py::arg("weights") = py::none(),
               py::arg("doScale") = false,
               "Computes the orthogonal transform (consisting only of rotation and translation) mapping the "
               "'fromPts' points as close as possible to the 'toPts' points in the least squares norm. The 'fromPts' and "
               "'toPts' lists must be the same length or the function will error out. If weights "
               "are provided, then the points are weighted (that is, some points are considered more important "
               "than others while computing the transform). If the 'doScale' parameter is True, then "
               "the resulting matrix is also allowed to have a uniform scale.");
    
    register_matrix<Matrix44, T>(m, name);
    return py::cast<py::class_<Matrix>>(m);
}

template <class M>
void
register_rowproxy(py::module& module, const char* name)
{
    using T = typename M::BaseType;

    py::class_<RowProxy<M>> r(module, name, py::module_local());
    r.attr("__module__") = "";
    r.def("__getitem__", [](RowProxy<M>& r, size_t i) -> T&
        {
            if (i < 0 || i >= M::dimensions())
                throw py::index_error();
            return r[i];
        })
        .def("__setitem__", [](RowProxy<M>& r, size_t i, T val)
        {
            if (i < 0 || i >= M::dimensions())
                throw py::index_error();
            r[i] = val;
        })
        ;
}
    
} // namespace

namespace PyBindImath {

void
register_imath_matrix(py::module& module)
{
    register_rowproxy<M22f>(module, "RowProxy22f");
    register_rowproxy<M22d>(module, "RowProxy22d");
    auto m22f = register_matrix22<float>(module, "M22f");
    auto m22d = register_matrix22<double>(module, "M22d");

    register_rowproxy<M33f>(module, "RowProxy33f");
    register_rowproxy<M33d>(module, "RowProxy33d");
    auto m33f = register_matrix33<float>(module, "M33f");
    auto m33d = register_matrix33<double>(module, "M33d");

    register_rowproxy<M44f>(module, "RowProxy44f");
    register_rowproxy<M44d>(module, "RowProxy44d");
    auto m44f = register_matrix44<float>(module, "M44f");
    auto m44d = register_matrix44<double>(module, "M44d");
}

} // namespace PyBindImath


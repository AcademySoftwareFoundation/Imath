//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include <Python.h>
#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>
#include <boost/format.hpp>
#include <ImathVec.h>
#include <ImathMatrixAlgo.h>
#include <ImathEuler.h>
#include "PyImathQuat.h"
#include "PyImathExport.h"
#include "PyImathDecorators.h"
#include "PyImath.h"
#include "PyImathMathExc.h"
#include "PyImathOperators.h"
#include "PyImathQuatOperators.h"

// XXX incomplete array wrapping, docstrings missing

namespace PyImath {
template <> const char *PyImath::QuatfArray::name() { return "QuatfArray"; }
template <> const char *PyImath::QuatdArray::name() { return "QuatdArray"; }
}

namespace PyImath {
using namespace boost::python;
using namespace IMATH_NAMESPACE;

template <class T> struct QuatName { static const char *value; };
template<> const char *QuatName<float>::value  = "Quatf";
template<> const char *QuatName<double>::value = "Quatd";

template <class T>
static std::string Quat_str(const Quat<T> &v)
{
    std::stringstream stream;
    stream << QuatName<T>::value << "(" << v[0] << ", " << v[1] << ", " 
           << v[2] << ", " << v[3] << ")";
    return stream.str();
}

// Non-specialized repr is same as str
template <class T>
static std::string Quat_repr(const Quat<T> &v)
{
    return Quat_str(v);
}

// Specialization for float to full precision
template <>
std::string Quat_repr(const Quat<float> &v)
{
    return (boost::format("%s(%.9g, %.9g, %.9g, %.9g)")
                        % QuatName<float>::value
                        % v[0] % v[1] % v[2] % v[3]).str();
}

// Specialization for double to full precision
template <>
std::string Quat_repr(const Quat<double> &v)
{
    return (boost::format("%s(%.17g, %.17g, %.17g, %.17g)")
                        % QuatName<double>::value
                        % v[0] % v[1] % v[2] % v[3]).str();
}


template <class T>
static Quat<T> &
invert(Quat<T> &quat)
{
    MATH_EXC_ON;
    return quat.invert();
}

template <class T>
static Quat<T> 
inverse(Quat<T> &quat)
{
    MATH_EXC_ON;
    return quat.inverse();
}

template <class T>
static Quat<T> &
normalize(Quat<T> &quat)
{
    MATH_EXC_ON;
    return quat.normalize();
}

template <class T>
static Quat<T> 
normalized(Quat<T> &quat)
{
    MATH_EXC_ON;
    return quat.normalized();
}

template <class T>
static T
length (Quat<T> &quat)
{
    MATH_EXC_ON;
    return quat.length();
}

template <class T>
static Vec3<T>
rotateVector(const Quat<T> &quat, const Vec3<T> &original)
{
    MATH_EXC_ON;
    return quat.rotateVector(original);
}

template <class T>
static Quat<T> &
setAxisAngle(Quat<T> &quat, const Vec3<T> &axis, T radians)
{
    MATH_EXC_ON;
    return quat.setAxisAngle(axis, radians);
}

template <class T>
static Quat<T> &
setRotation(Quat<T> &quat, const Vec3<T> &from, const Vec3<T> &to)
{
    MATH_EXC_ON;
    return quat.setRotation(from, to);
}

template <class T>
static T
angle (Quat<T> &quat)
{
    MATH_EXC_ON;
    return quat.angle();
}

template <class T>
static Vec3<T>
axis (Quat<T> &quat)
{
    MATH_EXC_ON;
    return quat.axis();
}

template <class T>
static Matrix33<T>
toMatrix33 (Quat<T> &quat)
{
    MATH_EXC_ON;
    return quat.toMatrix33();
}

template <class T>
static Matrix44<T>
toMatrix44 (Quat<T> &quat)
{
    MATH_EXC_ON;
    return quat.toMatrix44();
}

template <class T>
static Quat<T> 
log(Quat<T> &quat)
{
    MATH_EXC_ON;
    return quat.log();
}

template <class T>
static Quat<T> 
exp(Quat<T> &quat)
{
    MATH_EXC_ON;
    return quat.exp();
}

template <class T>
static void
setR(Quat<T> &quat, const double &r)
{
    quat.r = r;
}

template <class T>
static void
setV(Quat<T> &quat, const Vec3<T> &v)
{
    quat.v = v;
}

template <class T>
static void
extract(Quat<T> &quat, const Matrix44<T> &mat)
{
    MATH_EXC_ON;
    Quat<T> q = IMATH_NAMESPACE::extractQuat(mat);
    quat.r = q.r;
    quat.v = q.v;
}

template <class T>
static T scalar(Quat<T> &quat)
{
    return quat.r;
}

template <class T>
static Vec3<T> vector(Quat<T> &quat)
{
    return quat.v;
}

template <class T>
static Quat<T>
slerp(const Quat<T> &quat, const Quat<T> &other, T t)
{
    MATH_EXC_ON;
    return IMATH_NAMESPACE::slerp (quat, other, t);
}

template <class T>
static Quat<T>
slerpShortestArc(const Quat<T>& quat, const Quat<T>& other, T t)
{
    MATH_EXC_ON;
    return IMATH_NAMESPACE::slerpShortestArc (quat, other, t);
}

template <class T>
static const Quat<T> &
imul (Quat<T> &quat, const Quat<T> &other)
{
    MATH_EXC_ON;
    return quat *= other;
}

template <class T>
static const Quat<T> &
imulT (Quat<T> &quat, T t)
{
    MATH_EXC_ON;
    return quat *= t;
}

template <class T>
static const Quat<T> &
idiv (Quat<T> &quat, const Quat<T> &other)
{
    MATH_EXC_ON;
    return quat /= other;
}

template <class T>
static const Quat<T> &
idivT (Quat<T> &quat, T t)
{
    MATH_EXC_ON;
    return quat /= t;
}

template <class T>
static const Quat<T> &
iadd (Quat<T> &quat, const Quat<T> &other)
{
    MATH_EXC_ON;
    return quat += other;
}

template <class T>
static const Quat<T> &
isub (Quat<T> &quat, const Quat<T> &other)
{
    MATH_EXC_ON;
    return quat -= other;
}

template <class T>
static Matrix33<T>
rmulM33(Quat<T> &quat, Matrix33<T> &m)
{
    MATH_EXC_ON;
    return m * quat;
}

template <class T>
static Matrix33<T>
mulM33(Quat<T> &quat, Matrix33<T> &m)
{
    MATH_EXC_ON;
    return quat * m;
}

template <class T>
static Quat<T>
mul(Quat<T> &quat, Quat<T> &other)
{
    MATH_EXC_ON;
    return quat * other;
}

template <class T>
static Quat<T>
div(Quat<T> &quat, Quat<T> &other)
{
    MATH_EXC_ON;
    return quat / other;
}

template <class T>
static Quat<T>
divT(Quat<T> &quat, T t)
{
    MATH_EXC_ON;
    return quat / t;
}

template <class T>
static Quat<T>
mulT(Quat<T> &quat, T t)
{
    MATH_EXC_ON;
    return quat * t;
}

template <class T>
static Quat<T>
add(Quat<T> &quat, Quat<T> &other)
{
    MATH_EXC_ON;
    return quat + other;
}

template <class T>
static Quat<T>
sub(Quat<T> &quat, Quat<T> &other)
{
    MATH_EXC_ON;
    return quat - other;
}

template <class T>
static Quat<T>
neg(Quat<T> &quat)
{
    MATH_EXC_ON;
    return -quat;
}

template <class T>
static Quat<T>
conj(Quat<T> &quat)
{
    MATH_EXC_ON;
    return ~quat;
}

template <class T>
static T
dot(Quat<T> &quat, Quat<T> &other)
{
    MATH_EXC_ON;
    return quat ^ other;
}

template <class T>
static Vec3<T>
rmulVec3(Quat<T> &quat, const Vec3<T> &v)
{
    MATH_EXC_ON;
    return v * quat.toMatrix44();
}

template <class T>
static FixedArray< Vec3<T> >
rmulVec3Array(Quat<T> &quat, const FixedArray< Vec3<T> > &a)
{
    MATH_EXC_ON;
    Matrix44<T> m = quat.toMatrix44();
    size_t len = a.len();
    FixedArray< Vec3<T> > r(len);
    for (size_t i = 0; i < len; i++)
        r[i] = a[i] * m;
    return r;
}

template <class T>
static Quat<T> *
quatConstructor1(const Euler<T> &euler)
{
    MATH_EXC_ON;
    return new Quat<T>(euler.toQuat());
}

template <class T>
static Quat<T> *
quatConstructor2(const Matrix33<T> &mat)
{
    MATH_EXC_ON;
    return new Quat<T>(Euler<T>(mat).toQuat());
}

template <class T>
static Quat<T> *
quatConstructor3(const Matrix44<T> &mat)
{
    MATH_EXC_ON;
    return new Quat<T>(Euler<T>(mat).toQuat());
}

template <class T>
class_<Quat<T> >
register_Quat()
{
    class_<Quat<T> > quat_class(QuatName<T>::value, QuatName<T>::value,init<Quat<T> >("copy construction"));
    quat_class
        .def(init<>("imath Quat initialization") )
        .def(init<Quat<float> >("imath Quat copy initialization") )
        .def(init<Quat<double> >("imath Quat copy initialization") )
        .def(init<T,T,T,T>("make Quat from components") )
        .def(init<T, Vec3<T> >("make Quat from components") )
        .def("__init__", make_constructor(quatConstructor1<T>))
        .def("__init__", make_constructor(quatConstructor2<T>))
        .def("__init__", make_constructor(quatConstructor3<T>))
        .def("identity",&Quat<T>::identity)
        .def("invert",&invert<T>,return_internal_reference<>(),
        	 "q.invert() -- inverts quaternion q\n"
			 "(modifying q); returns q")
             
        .def("inverse",&inverse<T>,
        	 "q.inverse() -- returns the inverse of\n"
			 "quaternion q; q is not modified\n")
             
        .def("normalize",&normalize<T>,return_internal_reference<>(),
        	 "q.normalize() -- normalizes quaternion q\n"
			 "(modifying q); returns q")
            
        .def("normalized",&normalized<T>,
        	 "q.normalized() -- returns a normalized version\n"
			 "of quaternion q; q is not modified\n")
             
        .def("length",&length<T>)

        .def("rotateVector",&rotateVector<T>,
			"q.rotateVector(orig) -- Given a vector orig,\n"
                        "   calculate orig' = q x orig x q*\n\n"
                        "   Assumes unit quaternions")
             
        .def("setAxisAngle",&setAxisAngle<T>,return_internal_reference<>(),
			"q.setAxisAngle(x,r) -- sets the value of\n"
			"quaternion q so that q represents a rotation\n"
			"of r radians around axis x")
             
        .def("setRotation",&setRotation<T>,return_internal_reference<>(),
        	 "q.setRotation(v,w) -- sets the value of\n"
			 "quaternion q so that rotating vector v by\n"
			 "q produces vector w")
             
        .def("angle",&angle<T>,
        	 "q.angle() -- returns the rotation angle\n"
			 "(in radians) represented by quaternion q")
             
        .def("axis",&axis<T>,
        	 "q.axis() -- returns the rotation axis\n"
			 "represented by quaternion q")
             
        .def("toMatrix33",&toMatrix33<T>,
             "q.toMatrix33() -- returns a 3x3 matrix that\n"
			 "represents the same rotation as quaternion q")
             
        .def("toMatrix44",&toMatrix44<T>,
        	 "q.toMatrix44() -- returns a 4x4 matrix that\n"
			 "represents the same rotation as quaternion q")
             
        .def("log",&log<T>)
        .def("exp",&exp<T>)
        .def_readwrite("v",&Quat<T>::v)                       
        .def_readwrite("r",&Quat<T>::r)
        .def("v", &vector<T>,
			  "q.v() -- returns the v (vector) component\n"
			  "of quaternion q")
              
        .def("r", &scalar<T>,
        	 "q.r() -- returns the r (scalar) component\n"
			 "of quaternion q")
                       
        .def("setR", &setR<T>,
        	 "q.setR(s) -- sets the r (scalar) component\n"
			 "of quaternion q to s")
             
        .def("setV", &setV<T>,
        	 "q.setV(w) -- sets the v (vector) component\n"
			 "of quaternion q to w")
             
        .def("extract", &extract<T>,
        	 "q.extract(m) -- extracts the rotation component\n"
			 "from 4x4 matrix m and stores the result in q")
             
        .def("slerp", &slerp<T>,
        	 "q.slerp(p,t) -- performs sperical linear\n"
			 "interpolation between quaternions q and p:\n"
			 "q.slerp(p,0) returns q; q.slerp(p,1) returns p.\n"
			 "q and p must be normalized\n")

        .def("slerpShortestArc", &slerpShortestArc<T>,
        	 "q.slerpShortestArc(p,t) -- performs spherical linear\n"
                         "interpolation along the shortest arc between\n"
                         "quaternions q and either p or -p, whichever is\n"
                         "closer. q and p must be normalized\n")
             
        .def("__str__",Quat_str<T>)
        .def("__repr__",Quat_repr<T>)
        .def ("__imul__", &imul<T>, return_internal_reference<>())
        .def ("__imul__", &imulT<T>, return_internal_reference<>())
        .def ("__idiv__", idiv<T>, return_internal_reference<>())
        .def ("__idiv__", &idivT<T>, return_internal_reference<>())
        .def ("__itruediv__", idiv<T>, return_internal_reference<>())
        .def ("__itruediv__", &idivT<T>, return_internal_reference<>())
        .def ("__iadd__", &iadd<T>, return_internal_reference<>())
        .def ("__isub__", &isub<T>, return_internal_reference<>())
        .def(self == self) // NOSONAR - suppress SonarCloud bug report.
        .def(self != self) // NOSONAR - suppress SonarCloud bug report.
        .def ("__rmul__", &rmulM33<T>)
        .def ("__mul__", &mulM33<T>)
        .def ("__mul__", &mul<T>)
        .def ("__div__", &div<T>)
        .def ("__div__", &divT<T>)
        .def ("__truediv__", &div<T>)
        .def ("__truediv__", &divT<T>)
        .def ("__mul__", &mulT<T>)
        .def ("__rmul__", &mulT<T>)
        .def ("__add__", &add<T>)
        .def ("__sub__", &sub<T>)
        .def ("__neg__", &neg<T>)
        .def ("__invert__", &conj<T>)
        .def ("__xor__", &dot<T>)
        .def ("__rmul__", &rmulVec3<T>)
        .def ("__rmul__", &rmulVec3Array<T>)
        ;

    decoratecopy(quat_class);

    return quat_class;
}

// XXX fixme - template this
// really this should get generated automatically...

template <class T,int index>
static FixedArray<T>
QuatArray_get(FixedArray<IMATH_NAMESPACE::Quat<T> > &qa)
{
    return FixedArray<T>(&(qa.unchecked_index(0).r) + index,
                         qa.len(), 4*qa.stride(), qa.handle(), qa.writable());
}

template <class T>
struct QuatArray_SetRotationTask : public Task
{
    const FixedArray<IMATH_NAMESPACE::Vec3<T> > &from;
    const FixedArray<IMATH_NAMESPACE::Vec3<T> > &to;
    FixedArray<IMATH_NAMESPACE::Quat<T> >       &result;

    QuatArray_SetRotationTask (const FixedArray<IMATH_NAMESPACE::Vec3<T> > &fromIn,
                               const FixedArray<IMATH_NAMESPACE::Vec3<T> > &toIn,
                               FixedArray<IMATH_NAMESPACE::Quat<T> >       &resultIn)
        : from (fromIn), to (toIn), result (resultIn) {}

    void execute (size_t start, size_t end)
    {
        for (size_t i = start; i < end; ++i)
            result[i].setRotation (from[i], to[i]);
    }
};

template <class T> static void
QuatArray_setRotation (FixedArray<IMATH_NAMESPACE::Quat<T> > &va,
                       const FixedArray<IMATH_NAMESPACE::Vec3<T> > &from,
                       const FixedArray<IMATH_NAMESPACE::Vec3<T> > &to)
{
    MATH_EXC_ON;
    size_t len = va.match_dimension(from); 
    va.match_dimension(to); 

    // Validate that 'va' is writable before entering the thread-task.
    if (!va.writable())
        throw std::invalid_argument ("Input fixed array is read-only.");

    QuatArray_SetRotationTask<T> task (from, to, va);
    dispatchTask (task, len);
}

template <class T>
struct QuatArray_OrientToVectors : public Task
{
    const FixedArray<IMATH_NAMESPACE::Vec3<T> > &forward;
    const FixedArray<IMATH_NAMESPACE::Vec3<T> > &up;
    FixedArray<IMATH_NAMESPACE::Quat<T> >       &result;
    bool alignForward;

    QuatArray_OrientToVectors (const FixedArray<IMATH_NAMESPACE::Vec3<T> > &forwardIn,
                               const FixedArray<IMATH_NAMESPACE::Vec3<T> > &upIn,
                               FixedArray<IMATH_NAMESPACE::Quat<T> >       &resultIn,
                               bool alignForwardIn)
        : forward (forwardIn), up (upIn), result (resultIn),
          alignForward (alignForwardIn) {}

    void execute (size_t start, size_t end)
    {
        Vec3<T> f(0), u(0);
        Euler<T> eu(0,0,0);
        const Vec3<T> fRef(1,0,0);

        for (size_t i = start; i < end; ++i)
        {
            if (alignForward)
            {
                f = forward[i].normalized();
                u = up[i] - f.dot(up[i])*f;
                u.normalize();
            }
            else
            {
                u = up[i].normalized();
                f = forward[i] - u.dot(forward[i])*u;
                f.normalize();
            }

            extractEulerXYZ (rotationMatrixWithUpDir (fRef, f, u), eu);
            result[i] = eu.toQuat();
        }
    }
};

template <class T> static void
QuatArray_orientToVectors (FixedArray<IMATH_NAMESPACE::Quat<T> >       &va,
                           const FixedArray<IMATH_NAMESPACE::Vec3<T> > &forward,
                           const FixedArray<IMATH_NAMESPACE::Vec3<T> > &up,
                           bool alignForward)
{
    MATH_EXC_ON;
    size_t len = va.match_dimension(forward);
    va.match_dimension(up);

    // Validate that 'va' is writable before entering the thread-task.
    if (!va.writable())
        throw std::invalid_argument ("Input fixed array is read-only.");

    QuatArray_OrientToVectors<T> task (forward, up, va, alignForward);
    dispatchTask (task, len);
}

template <class T>
struct QuatArray_Axis : public Task
{
    const FixedArray<IMATH_NAMESPACE::Quat<T> > &va;
    FixedArray<IMATH_NAMESPACE::Vec3<T> >       &result;

    QuatArray_Axis (const FixedArray<IMATH_NAMESPACE::Quat<T> > &vaIn,
                    FixedArray<IMATH_NAMESPACE::Vec3<T> >       &resultIn)
        : va (vaIn), result (resultIn) {}

    void execute (size_t start, size_t end)
    {
        for (size_t i = start; i < end; ++i)
            result[i] = va[i].axis(); 
    } 
};

template <class T> static FixedArray<IMATH_NAMESPACE::Vec3<T> >
QuatArray_axis(const FixedArray<IMATH_NAMESPACE::Quat<T> > &va)
{
    MATH_EXC_ON;
    size_t len = va.len(); 
    FixedArray<IMATH_NAMESPACE::Vec3<T> > retval (Py_ssize_t(len), UNINITIALIZED);

    QuatArray_Axis<T> task (va, retval);
    dispatchTask (task, len);
    return retval;
}

template <class T>
struct QuatArray_Angle : public Task
{
    const FixedArray<IMATH_NAMESPACE::Quat<T> > &va;
    FixedArray<T>                               &result;

    QuatArray_Angle (const FixedArray<IMATH_NAMESPACE::Quat<T> > &vaIn,
                     FixedArray<T>                               &resultIn)
        : va (vaIn), result (resultIn) {}

    void execute (size_t start, size_t end)
    {
        for (size_t i = start; i < end; ++i)
            result[i] = va[i].angle(); 
    } 
};


template <class T> static FixedArray<T>
QuatArray_angle(const FixedArray<IMATH_NAMESPACE::Quat<T> > &va)
{
    MATH_EXC_ON;
    size_t len = va.len(); 
    FixedArray<T> retval (Py_ssize_t(len), UNINITIALIZED);

    QuatArray_Angle<T> task (va, retval);
    dispatchTask (task, len);
    return retval;
}

template <class T>
struct QuatArray_RmulVec3 : public Task
{
    const FixedArray<IMATH_NAMESPACE::Quat<T> > &a;
    const Vec3<T>                               &v;
    FixedArray<Vec3<T> >                        &r;

    QuatArray_RmulVec3 (const FixedArray<IMATH_NAMESPACE::Quat<T> > &aIn,
                        const Vec3<T> &vIn, FixedArray<Vec3<T> > &rIn)
        : a (aIn), v (vIn), r (rIn) {}

    void execute(size_t start, size_t end)
    {
        for (size_t i = start; i < end; ++i)
        {
            Matrix44<T> m = a[i].toMatrix44();
            r[i] = v * m;
        }
    }
};

template <class T>
static FixedArray< Vec3<T> >
QuatArray_rmulVec3 (const FixedArray< IMATH_NAMESPACE::Quat<T> > &a, const Vec3<T> &v)
{
    MATH_EXC_ON;
    size_t len = a.len();
    FixedArray< Vec3<T> > r (Py_ssize_t(len), UNINITIALIZED);

    QuatArray_RmulVec3<T> task (a, v, r);
    dispatchTask (task, len);
    return r;
}

template <class T>
struct QuatArray_RmulVec3Array : public Task
{
    const FixedArray<IMATH_NAMESPACE::Quat<T> > &a;
    const FixedArray<Vec3<T> >                  &b;
    FixedArray<Vec3<T> >                        &r;

    QuatArray_RmulVec3Array (const FixedArray<IMATH_NAMESPACE::Quat<T> > &aIn,
                             const FixedArray<Vec3<T> > &bIn,
                             FixedArray<Vec3<T> > &rIn)
        : a (aIn), b (bIn), r (rIn) {}

    void execute(size_t start, size_t end)
    {
        for (size_t i = start; i < end; ++i)
        {
            Matrix44<T> m = a[i].toMatrix44();
            r[i] = b[i] * m;
        }
    }
};

template <class T>
static FixedArray< Vec3<T> >
QuatArray_rmulVec3Array (const FixedArray< IMATH_NAMESPACE::Quat<T> > &a,
                         const FixedArray< Vec3<T> > &b)
{
    MATH_EXC_ON;
    size_t len = a.match_dimension(b);
    FixedArray< Vec3<T> > r (Py_ssize_t(len), UNINITIALIZED);

    QuatArray_RmulVec3Array<T> task (a, b, r);
    dispatchTask (task, len);
    return r;
}

template <class T>
struct QuatArray_SetAxisAngle : public Task
{
    const FixedArray<IMATH_NAMESPACE::Vec3<T> > &axis;
    const FixedArray<T>                         &angles;
    FixedArray<IMATH_NAMESPACE::Quat<T> >       &quats;

    QuatArray_SetAxisAngle (const FixedArray<IMATH_NAMESPACE::Vec3<T> > &axisIn,
                            const FixedArray<T>                         &anglesIn,
                            FixedArray<IMATH_NAMESPACE::Quat<T> >       &quatsIn)
        : axis (axisIn), angles (anglesIn), quats (quatsIn) {}

    void execute(size_t start, size_t end)
    {
        for (size_t i = start; i < end; ++i)
        {
            quats[i].setAxisAngle (axis[i], angles[i]);
        }
    }
};

template <class T>
static FixedArray< IMATH_NAMESPACE::Quat<T> > &
QuatArray_setAxisAngle (FixedArray< IMATH_NAMESPACE::Quat<T> > &quats,
                        const FixedArray< IMATH_NAMESPACE::Vec3<T> > &axis,
                        const FixedArray<T> &angles)
{
    MATH_EXC_ON;
    size_t len = quats.match_dimension(axis);
    quats.match_dimension(angles);

    // Validate that 'va' is writable before entering the thread-task.
    if (!quats.writable())
        throw std::invalid_argument ("Input fixed array is read-only.");

    QuatArray_SetAxisAngle<T> task (axis, angles, quats);
    dispatchTask (task, len);
    return quats;
}

template <class T>
struct QuatArray_RotateVector : public Task
{
    FixedArray<IMATH_NAMESPACE::Vec3<T> >       &result;
    const FixedArray<IMATH_NAMESPACE::Vec3<T> > &vectors;
    const FixedArray<IMATH_NAMESPACE::Quat<T> > &quats;

    QuatArray_RotateVector (FixedArray<IMATH_NAMESPACE::Vec3<T> >       &resultIn,
                            const FixedArray<IMATH_NAMESPACE::Vec3<T> > &vectorsIn,
                            const FixedArray<IMATH_NAMESPACE::Quat<T> > &quatsIn)
        : result (resultIn), vectors (vectorsIn), quats (quatsIn) {}

    void execute(size_t start, size_t end)
    {
        for (size_t i = start; i < end; ++i)
        {
            result[i] = quats[i].rotateVector (vectors[i]);
        }
    }
};

template <class T>
static FixedArray<IMATH_NAMESPACE::Vec3<T> >
QuatArray_rotateVector (const FixedArray< IMATH_NAMESPACE::Quat<T> > &quats,
                        const FixedArray< IMATH_NAMESPACE::Vec3<T> > &vectors)
{
    MATH_EXC_ON;
    size_t len = quats.match_dimension(vectors);
    FixedArray< IMATH_NAMESPACE::Vec3<T> > result (len);

    QuatArray_RotateVector<T> task (result, vectors, quats);
    dispatchTask (task, len);
    return result;
}

template <class T>
struct QuatArray_Inverse : public Task
{
    const FixedArray<IMATH_NAMESPACE::Quat<T> > &quats;
    FixedArray<IMATH_NAMESPACE::Quat<T> >       &result;

    QuatArray_Inverse (FixedArray<IMATH_NAMESPACE::Quat<T> >       &resultIn,
                       const FixedArray<IMATH_NAMESPACE::Quat<T> > &quatsIn)
        : quats (quatsIn), result (resultIn) {}

    void execute (size_t start, size_t end)
    {
        for (size_t i = start; i < end; ++i)
            result[i] = quats[i].inverse();
    } 
};

template <class T>
static FixedArray<IMATH_NAMESPACE::Quat<T> >
QuatArray_inverse(const FixedArray<IMATH_NAMESPACE::Quat<T> > &quats)
{
    MATH_EXC_ON;
    size_t len = quats.len();
    FixedArray<IMATH_NAMESPACE::Quat<T> > result (len);

    QuatArray_Inverse<T> task (result, quats);
    dispatchTask (task, len);

    return result;
}

template <class T>
struct QuatArray_SetEulerXYZ : public Task
{
    const FixedArray<IMATH_NAMESPACE::Vec3<T> > &rot;
    FixedArray<IMATH_NAMESPACE::Quat<T> >       &quats;

    QuatArray_SetEulerXYZ (const FixedArray<IMATH_NAMESPACE::Vec3<T> > &rotIn,
                           FixedArray<IMATH_NAMESPACE::Quat<T> >       &quatsIn)
        : rot (rotIn), quats (quatsIn) {}

    void execute (size_t start, size_t end)
    {
        for (size_t i = start; i < end; ++i)
        {
            Eulerf e(rot[i]);
            quats[i] = e.toQuat();
        }
    }
};

template <class T>
static void
QuatArray_setEulerXYZ (FixedArray< IMATH_NAMESPACE::Quat<T> > &quats,
                       const FixedArray< IMATH_NAMESPACE::Vec3<T> > &rot)
{
    MATH_EXC_ON;
    size_t len = quats.match_dimension(rot);

    // Validate that 'va' is writable before entering the thread-task.
    if (!quats.writable())
        throw std::invalid_argument ("Input fixed array is read-only.");

    QuatArray_SetEulerXYZ<T> task (rot, quats);
    dispatchTask (task, len);
}

template <class T>
struct QuatArray_Mul : public Task
{
    const FixedArray<IMATH_NAMESPACE::Quat<T> > &q1;
    const FixedArray<IMATH_NAMESPACE::Quat<T> > &q2;
    FixedArray<IMATH_NAMESPACE::Quat<T> >       &result;

    QuatArray_Mul (const FixedArray<IMATH_NAMESPACE::Quat<T> > &q1In,
                   const FixedArray<IMATH_NAMESPACE::Quat<T> > &q2In,
                   FixedArray<IMATH_NAMESPACE::Quat<T> >       &resultIn)
        : q1 (q1In), q2 (q2In), result (resultIn) {}

    void execute (size_t start, size_t end)
    {
        for (size_t i = start; i < end; ++i)
        {
            result[i] = q1[i] * q2[i];
        }
    }
};

template <class T>
static FixedArray< IMATH_NAMESPACE::Quat<T> >
QuatArray_mul(const FixedArray< IMATH_NAMESPACE::Quat<T> > &q1,
              const FixedArray< IMATH_NAMESPACE::Quat<T> > &q2)
{
    MATH_EXC_ON;
    size_t len = q1.match_dimension(q2);
    FixedArray< IMATH_NAMESPACE::Quat<T> > result (Py_ssize_t(len), UNINITIALIZED);

    QuatArray_Mul<T> task (q1, q2, result);
    dispatchTask (task, len);
    return result;
}

template <class T>
struct QuatArray_QuatConstructor1 : public Task
{
    const FixedArray<IMATH_NAMESPACE::Euler<T> > &euler;
    FixedArray<IMATH_NAMESPACE::Quat<T> >        &result;

    QuatArray_QuatConstructor1 (const FixedArray<IMATH_NAMESPACE::Euler<T> > &eulerIn,
                                FixedArray<IMATH_NAMESPACE::Quat<T> >        &resultIn)
        : euler (eulerIn), result (resultIn) {}

    void execute (size_t start, size_t end)
    {
        for (size_t i = start; i < end; ++i)
        {
            result[i] = euler[i].toQuat();
        }
    }
};

template <class T>
static FixedArray<IMATH_NAMESPACE::Quat<T> > *
QuatArray_quatConstructor1(const FixedArray<IMATH_NAMESPACE::Euler<T> > &e)
{
    MATH_EXC_ON;
    size_t len = e.len();
    FixedArray<IMATH_NAMESPACE::Quat<T> >* result =
        new FixedArray<IMATH_NAMESPACE::Quat<T> > (Py_ssize_t(len), UNINITIALIZED);

    QuatArray_QuatConstructor1<T> task (e, *result);
    dispatchTask (task, len);
    return result;
}

template <class T>
struct QuatArray_ExtractTask : public Task
{
    const FixedArray<Matrix44<double> >    &m;
    FixedArray<IMATH_NAMESPACE::Quat<T> >  &result;

    QuatArray_ExtractTask (const FixedArray<Matrix44<double> >    &mIn,
                           FixedArray<IMATH_NAMESPACE::Quat<T> >  &resultIn)
        : m (mIn), result (resultIn) {}

    void execute (size_t start, size_t end)
    {
        for (size_t i = start; i < end; ++i)
        {
            result[i] = IMATH_NAMESPACE::extractQuat (m[i]);
        }
    }
};

template <class T>
static void
QuatArray_extract(FixedArray<IMATH_NAMESPACE::Quat<T> > &q,
                  const FixedArray<Matrix44<double> >   &m)
{
    MATH_EXC_ON;
    const size_t len = q.match_dimension(m);

    QuatArray_ExtractTask<T> task (m, q);
    dispatchTask (task, len);
}

template <class T>
class_<FixedArray<IMATH_NAMESPACE::Quat<T> > >
register_QuatArray()
{
    using boost::mpl::true_;
    using boost::mpl::false_;

    typedef IMATH_NAMESPACE::Quat<T>   QuatT;

    class_<FixedArray<QuatT> > quatArray_class = FixedArray<QuatT>::register_("Fixed length array of IMATH_NAMESPACE::Quat");
    quatArray_class
        .add_property("r",&QuatArray_get<T,0>)
        .add_property("x",&QuatArray_get<T,1>)
        .add_property("y",&QuatArray_get<T,2>)
        .add_property("z",&QuatArray_get<T,3>)
        .def("setRotation", &QuatArray_setRotation<T>,
             "set rotation angles for each quat",
             (args("from", "to")))
        .def("orientToVectors", &QuatArray_orientToVectors<T>,
             "Sets the orientations to match the given forward and up vectors, "
             "matching the forward vector exactly if 'alignForward' is True, matching "
             "the up vector exactly if 'alignForward' is False.  If the vectors are "
             "already orthogonal, both vectors will be matched exactly.",
             (args("forward", "up", "alignForward")))
        .def("extract", &QuatArray_extract<T>,
             "Extract the rotation component of an M44d and return it as a quaternion.",
             (args("lxform")))
        .def("axis", &QuatArray_axis<T>,
             "get rotation axis for each quat")
        .def("angle", &QuatArray_angle<T>,
             "get rotation angle about the axis returned by axis() for each quat")
        .def("setAxisAngle", &QuatArray_setAxisAngle<T>,
             "set the quaternion arrays from a given axis and angle",
             (args("axis", "angle")), return_value_policy<copy_non_const_reference>())
        .def("setEulerXYZ", &QuatArray_setEulerXYZ<T>,
             "set the quaternion arrays from a given euler XYZ angle vector",
             (args("euler")))
        .def("rotateVector", &QuatArray_rotateVector<T>,
             "Rotate the supplied vectors by the quaternions.  Assumes quaternions are normalized.",
             (args("vector")))
        .def("inverse", &QuatArray_inverse<T>,
             "Return 1/Q for each quaternion.",
             (args("QuatArray")))
        .def("__rmul__", &QuatArray_rmulVec3<T>)
        .def("__rmul__", &QuatArray_rmulVec3Array<T>)
        .def("__init__", make_constructor(QuatArray_quatConstructor1<T>))
        ;

    generate_member_bindings<op_quatDot<QuatT>, true_>
        (quatArray_class, "dot",
         "Return the element-by-element Euclidean inner product",
         args("qB"));
    generate_member_bindings<op_quatDot<QuatT>, true_>
        (quatArray_class, "euclideanInnerProduct",
         "Return the element-by-element Euclidean inner product",
         args("qB"));
    generate_member_bindings<op_quatNormalize<QuatT> >
        (quatArray_class, "normalize",
         "Normalize each quaternion in the array");
    generate_member_bindings<op_quatNormalized<QuatT> >
        (quatArray_class, "normalized",
         "Return a new quaternion array with unit quaternions.");

    generate_member_bindings<op_neg<QuatT> >
        (quatArray_class, "__neg__" , "-self");
    generate_member_bindings<op_mul<QuatT,QuatT>, true_ >
        (quatArray_class, "__mul__",  "self * qB", args("qB"));
    generate_member_bindings<op_mul<QuatT,T>, false_ >
        (quatArray_class, "__mul__",  "self * x",  args("x"));
    generate_member_bindings<op_mul<QuatT,T>, false_ >
        (quatArray_class, "__rmul__", "self * x",  args("x"));
    generate_member_bindings<op_quatDot<QuatT>, true_ >
        (quatArray_class, "__xor__",  "self.dot(qB)",  args("qB"));

    generate_member_bindings<op_quatSlerp<QuatT>, true_, false_ >
        (quatArray_class,
         "slerp",
         "Return the element-by-element shortest arc spherical linear interpolation between self and B.",
         args("qB", "t"));

    add_comparison_functions(quatArray_class);
    decoratecopy(quatArray_class);

    return quatArray_class;
}

template PYIMATH_EXPORT class_<IMATH_NAMESPACE::Quat<float> > register_Quat<float>();
template PYIMATH_EXPORT class_<IMATH_NAMESPACE::Quat<double> > register_Quat<double>();
		 
template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Quat<float> > > register_QuatArray<float>();
template PYIMATH_EXPORT class_<FixedArray<IMATH_NAMESPACE::Quat<double> > > register_QuatArray<double>();
template<> PYIMATH_EXPORT IMATH_NAMESPACE::Quat<float> FixedArrayDefaultValue<IMATH_NAMESPACE::Quat<float> >::value() { return IMATH_NAMESPACE::Quat<float>(); }
template<> PYIMATH_EXPORT IMATH_NAMESPACE::Quat<double> FixedArrayDefaultValue<IMATH_NAMESPACE::Quat<double> >::value() { return IMATH_NAMESPACE::Quat<double>(); }
}

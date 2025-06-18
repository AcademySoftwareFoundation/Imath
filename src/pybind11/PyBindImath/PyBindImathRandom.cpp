//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include "PyBindImath.h"
#include "ImathRandom.h"
#include "ImathVec.h"

namespace py = pybind11;
using namespace IMATH_NAMESPACE;

namespace {

template <class Rand, class T>
T
nextf2 (Rand &rand, T min, T max)
{
    return rand.nextf(min, max);
}

template <class Rand>
float
nextGauss (Rand &rand)
{
    return gaussRand(rand);
}

template <class T, class Rand>
Vec3<T>
nextGaussSphere(Rand &rand, const Vec3<T> &v)
{
    return gaussSphereRand<Vec3<T>,Rand>(rand);
}

template <class T, class Rand>
Vec2<T>
nextGaussSphere(Rand &rand, const Vec2<T> &v)
{
    return gaussSphereRand<Vec2<T>,Rand>(rand);
}

template <class T, class Rand>
Vec3<T>
nextHollowSphere(Rand &rand, const Vec3<T> &v)
{
    return hollowSphereRand<Vec3<T>,Rand>(rand);
}

template <class T, class Rand>
Vec2<T>
nextHollowSphere(Rand &rand, const Vec2<T> &v)
{
    return hollowSphereRand<Vec2<T>,Rand>(rand);
}

template <class T, class Rand>
Vec3<T>
nextSolidSphere(Rand &rand, const Vec3<T> &v)
{
    return solidSphereRand<Vec3<T>,Rand>(rand);
}

template <class T, class Rand>
Vec2<T>
nextSolidSphere(Rand &rand, const Vec2<T> &v)
{
    return solidSphereRand<Vec2<T>,Rand>(rand);
}

} // namespace

namespace PyBindImath {

void
register_imath_random(py::module& m)
{
    py::class_<Rand32> rand32_class(m, "Rand32", "32-bit random number generator");
    rand32_class.attr("__module__") = "";
    rand32_class
        .def(py::init<>(), "default construction")
        .def(py::init<unsigned long int>(), "r = Rand32(seed) -- initialize with integer seed", py::arg("seed"))
        .def(py::init<const Rand32&>(), "copy constructor")
        
        .def("init", &Rand32::init,
             "r.init(i) -- initialize with integer seed i", py::arg("seed"))
             
        .def("nexti", &Rand32::nexti,
             "r.nexti() -- return the next integer value in the uniformly-distributed sequence")
             
        .def("nextf", 
             static_cast<float (Rand32::*)()>(&Rand32::nextf),
             "r.nextf() -- return the next floating-point value in the uniformly-distributed sequence")
        .def("nextf", 
             [](Rand32& self, float min, float max) { return nextf2(self, min, max); },
             "r.nextf(min, max) -- return the next floating-point value in the uniformly-distributed sequence between min and max",
             py::arg("min"), py::arg("max"))
             
        .def("nextb", &Rand32::nextb,
             "r.nextb() -- return the next boolean value in the uniformly-distributed sequence")

        .def("nextGauss", 
             [](Rand32& self) { return nextGauss(self); },
             "r.nextGauss() -- returns the next floating-point value in the normally (Gaussian) distributed sequence")
             
        .def("nextGaussSphere", 
             [](Rand32& self, const Vec3<float>& v) { 
                 return nextGaussSphere<float>(self, v); 
             },
             "r.nextGaussSphere(v) -- returns the next point whose distance from the origin has a normal (Gaussian) distribution with mean 0 and variance 1. The vector argument, v, specifies the dimension and number type.",
             py::arg("v"))
        .def("nextGaussSphere", 
             [](Rand32& self, const Vec3<double>& v) { 
                 return nextGaussSphere<double>(self, v); 
             })
        .def("nextGaussSphere", 
             [](Rand32& self, const Vec2<float>& v) { 
                 return nextGaussSphere<float>(self, v); 
             })
        .def("nextGaussSphere", 
             [](Rand32& self, const Vec2<double>& v) { 
                 return nextGaussSphere<double>(self, v); 
             })
        
        .def("nextHollowSphere", 
             [](Rand32& self, const Vec3<float>& v) { 
                 return nextHollowSphere<float>(self, v); 
             },
             "r.nextHollowSphere(v) -- return the next point uniformly distributed on the surface of a sphere of radius 1 centered at the origin. The vector argument, v, specifies the dimension and number type.",
             py::arg("v"))
        .def("nextHollowSphere", 
             [](Rand32& self, const Vec3<double>& v) { 
                 return nextHollowSphere<double>(self, v); 
             })
        .def("nextHollowSphere", 
             [](Rand32& self, const Vec2<float>& v) { 
                 return nextHollowSphere<float>(self, v); 
             })
        .def("nextHollowSphere", 
             [](Rand32& self, const Vec2<double>& v) { 
                 return nextHollowSphere<double>(self, v); 
             })

        .def("nextSolidSphere", 
             [](Rand32& self, const Vec3<float>& v) { 
                 return nextSolidSphere<float>(self, v); 
             },
             "r.nextSolidSphere(v) -- return the next point uniformly distributed in a sphere of radius 1 centered at the origin. The vector argument, v, specifies the dimension and number type.",
             py::arg("v"))
        .def("nextSolidSphere", 
             [](Rand32& self, const Vec3<double>& v) { 
                 return nextSolidSphere<double>(self, v); 
             })
        .def("nextSolidSphere", 
             [](Rand32& self, const Vec2<float>& v) { 
                 return nextSolidSphere<float>(self, v); 
             })
        .def("nextSolidSphere", 
             [](Rand32& self, const Vec2<double>& v) { 
                 return nextSolidSphere<double>(self, v); 
             });

    py::class_<Rand48> rand48_class(m, "Rand48", "48-bit random number generator");
    rand48_class.attr("__module__") = "";
    rand48_class
        .def(py::init<>(), "default construction")
        .def(py::init<unsigned long int>(), "r = Rand48(seed) -- initialize with integer seed", py::arg("seed"))
        .def(py::init<const Rand48&>(), "copy constructor")
        
        .def("init", &Rand48::init,
             "r.init(i) -- initialize with integer seed i", py::arg("seed"))
             
        .def("nexti", &Rand48::nexti,
             "r.nexti() -- return the next integer value in the uniformly-distributed sequence")
             
        .def("nextf", 
             static_cast<double (Rand48::*)()>(&Rand48::nextf),
             "r.nextf() -- return the next double value in the uniformly-distributed sequence")
        .def("nextf", 
             [](Rand48& self, double min, double max) { return nextf2(self, min, max); },
             "r.nextf(min, max) -- return the next double value in the uniformly-distributed sequence between min and max",
             py::arg("min"), py::arg("max"))
             
        .def("nextb", &Rand48::nextb,
             "r.nextb() -- return the next boolean value in the uniformly-distributed sequence")
 
        .def("nextGauss", 
             [](Rand48& self) { return nextGauss(self); },
             "r.nextGauss() -- returns the next floating-point value in the normally (Gaussian) distributed sequence")
             
        .def("nextGaussSphere", 
             [](Rand48& self, const Vec3<float>& v) { 
                 return nextGaussSphere<float>(self, v); 
             },
             "r.nextGaussSphere(v) -- returns the next point whose distance from the origin has a normal (Gaussian) distribution with mean 0 and variance 1. The vector argument, v, specifies the dimension and number type.",
             py::arg("v"))
        .def("nextGaussSphere", 
             [](Rand48& self, const Vec3<double>& v) { 
                 return nextGaussSphere<double>(self, v); 
             })
        .def("nextGaussSphere", 
             [](Rand48& self, const Vec2<float>& v) { 
                 return nextGaussSphere<float>(self, v); 
             })
        .def("nextGaussSphere", 
             [](Rand48& self, const Vec2<double>& v) { 
                 return nextGaussSphere<double>(self, v); 
             })
        
        .def("nextHollowSphere", 
             [](Rand48& self, const Vec3<float>& v) { 
                 return nextHollowSphere<float>(self, v); 
             },
             "r.nextHollowSphere(v) -- return the next point uniformly distributed on the surface of a sphere of radius 1 centered at the origin. The vector argument, v, specifies the dimension and number type.",
             py::arg("v"))
        .def("nextHollowSphere", 
             [](Rand48& self, const Vec3<double>& v) { 
                 return nextHollowSphere<double>(self, v); 
             })
        .def("nextHollowSphere", 
             [](Rand48& self, const Vec2<float>& v) { 
                 return nextHollowSphere<float>(self, v); 
             })
        .def("nextHollowSphere", 
             [](Rand48& self, const Vec2<double>& v) { 
                 return nextHollowSphere<double>(self, v); 
             })

        .def("nextSolidSphere", 
             [](Rand48& self, const Vec3<float>& v) { 
                 return nextSolidSphere<float>(self, v); 
             },
             "r.nextSolidSphere(v) -- return the next point uniformly distributed in a sphere of radius 1 centered at the origin. The vector argument, v, specifies the dimension and number type.",
             py::arg("v"))
        .def("nextSolidSphere", 
             [](Rand48& self, const Vec3<double>& v) { 
                 return nextSolidSphere<double>(self, v); 
             })
        .def("nextSolidSphere", 
             [](Rand48& self, const Vec2<float>& v) { 
                 return nextSolidSphere<float>(self, v); 
             })
        .def("nextSolidSphere", 
             [](Rand48& self, const Vec2<double>& v) { 
                 return nextSolidSphere<double>(self, v); 
             });

    // Apply copy decorations if needed
}

} // namespace PyImath

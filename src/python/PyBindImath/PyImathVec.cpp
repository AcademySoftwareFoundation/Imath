#include "PyImath.h"
#include <ImathVec.h>

namespace PyImath {

void register_imath_vec(pybind11::module& m)
{
    // TODO fill in
    pybind11::class_<Imath::V3f>(m, "V3f");
}

}
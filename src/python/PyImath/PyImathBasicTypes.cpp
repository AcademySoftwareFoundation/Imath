//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

// clang-format off

#include <Python.h>
#include <boost/python.hpp>
#include "PyImath.h"
#include "PyImathExport.h"
#include "PyImathBasicTypes.h"
#include "PyImathFixedArray.h"
#include "PyImathFixedVArray.h"
#include "PyImathBufferProtocol.h"

using namespace boost::python;

namespace PyImath {

void
register_basicTypes()
{
    class_<BoolArray> bclass = BoolArray::register_("Fixed length array of bool");
    add_comparison_functions(bclass);

    class_<SignedCharArray> scclass = SignedCharArray::register_("Fixed length array of signed chars");
    add_arithmetic_math_functions(scclass);
    add_mod_math_functions(scclass);
    add_comparison_functions(scclass);
    add_ordered_comparison_functions(scclass);

    class_<UnsignedCharArray> ucclass = UnsignedCharArray::register_("Fixed length array of unsigned chars");
    add_arithmetic_math_functions(ucclass);
    add_mod_math_functions(ucclass);
    add_comparison_functions(ucclass);
    add_ordered_comparison_functions(ucclass);
    add_buffer_protocol<UnsignedCharArray>(ucclass);

    class_<ShortArray> sclass = ShortArray::register_("Fixed length array of shorts");
    add_arithmetic_math_functions(sclass);
    add_mod_math_functions(sclass);
    add_comparison_functions(sclass);
    add_ordered_comparison_functions(sclass);

    class_<UnsignedShortArray> usclass = UnsignedShortArray::register_("Fixed length array of unsigned shorts");
    add_arithmetic_math_functions(usclass);
    add_mod_math_functions(usclass);
    add_comparison_functions(usclass);
    add_ordered_comparison_functions(usclass);

    class_<IntArray> iclass = IntArray::register_("Fixed length array of ints");
    add_arithmetic_math_functions(iclass);
    add_mod_math_functions(iclass);
    add_comparison_functions(iclass);
    add_ordered_comparison_functions(iclass);
    add_explicit_construction_from_type<float>(iclass);
    add_explicit_construction_from_type<double>(iclass);
    add_buffer_protocol<IntArray>(iclass);

    class_<UnsignedIntArray> uiclass = UnsignedIntArray::register_("Fixed length array of unsigned ints");
    add_arithmetic_math_functions(uiclass);
    add_mod_math_functions(uiclass);
    add_comparison_functions(uiclass);
    add_ordered_comparison_functions(uiclass);
    add_explicit_construction_from_type<float>(uiclass);
    add_explicit_construction_from_type<double>(uiclass);

    class_<FloatArray> fclass = FloatArray::register_("Fixed length array of floats");
    add_arithmetic_math_functions(fclass);
    add_pow_math_functions(fclass);
    add_comparison_functions(fclass);
    add_ordered_comparison_functions(fclass);
    add_explicit_construction_from_type<int>(fclass);
    add_explicit_construction_from_type<double>(fclass);
    add_buffer_protocol<FloatArray>(fclass);

    class_<DoubleArray> dclass = DoubleArray::register_("Fixed length array of doubles");
    add_arithmetic_math_functions(dclass);
    add_pow_math_functions(dclass);
    add_comparison_functions(dclass);
    add_ordered_comparison_functions(dclass);
    add_explicit_construction_from_type<int>(dclass);
    add_explicit_construction_from_type<float>(dclass);
    add_buffer_protocol<DoubleArray>(dclass);

    class_<VIntArray>   ivclass = VIntArray::register_("Variable fixed length array of ints");
    class_<VFloatArray> fvclass = VFloatArray::register_("Variable fixed length array of floats");
    class_<VV2iArray> v2ivclass = VV2iArray::register_("Variable fixed length array of V2i");
    class_<VV2fArray> v2fvclass = VV2fArray::register_("Variable fixed length array of V2f");
    // Don't add other functionality until its defined better.
}

} // namespace PyImath

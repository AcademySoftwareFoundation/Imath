# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

# this is set by python/config/ModuleDefine.cmake
get_target_property(TEST_PYIMATH_OUTDIR imath_python${Python_VERSION_MAJOR} LIBRARY_OUTPUT_DIRECTORY)
add_test(NAME PyImath_Python_TEST COMMAND Python${Python_VERSION_MAJOR}::Interpreter ${PROJECT_SOURCE_DIR}/src/python/PyImathPythonTest/RunTests.py ${TEST_PYIMATH_OUTDIR})

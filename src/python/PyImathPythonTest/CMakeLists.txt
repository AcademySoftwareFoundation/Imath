# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

# TODO we need to find out what the preinstall equivalent to ${Python_SITEARCH} is
add_test(NAME PyImath_Python_TEST COMMAND Python${Python_VERSION_MAJOR}::Interpreter ${PROJECT_SOURCE_DIR}/src/python/PyImathPythonTest/RunTests.py ${Python_SITEARCH})

# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

get_target_property(TEST_PYIMATH_OUTDIR Imath::Imath LIBRARY_OUTPUT_DIRECTORY)

set(PYIMATH_MODULE_NAME imath) 
set(PYIMATH_MODULE ${PYIMATH_MODULE_NAME}_module)
set(PYIMATH_LIBRARY PyImath)

add_test(NAME PyImath.RunTests.py
  COMMAND ${CMAKE_COMMAND} -E env
          "PYTHONPATH=$<TARGET_FILE_DIR:${PYIMATH_MODULE}>"
          "PATH=$<TARGET_FILE_DIR:${PYIMATH_LIBRARY}>:$ENV{PATH}"
          ${Python3_EXECUTABLE}
          ${CMAKE_CURRENT_SOURCE_DIR}/RunTests.py
          ${TEST_PYIMATH_OUTDIR}
)


# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

if(TARGET Python2::Interpreter)
  add_test(PyImath.PyImathTest_Python2
    ${Python2_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/pyImathTest.in
  )
  set_tests_properties(PyImath.PyImathTest_Python2 PROPERTIES
    ENVIRONMENT "PYTHONPATH=${CMAKE_BINARY_DIR}/python${Python2_VERSION_MAJOR}_${Python2_VERSION_MINOR}"
  )
endif()

if(TARGET Python3::Interpreter)
  add_test(PyImath.PyImathTest_Python3
    ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/pyImathTest.in
  )
  set_tests_properties(PyImath.PyImathTest_Python3 PROPERTIES
    ENVIRONMENT "PYTHONPATH=${CMAKE_BINARY_DIR}/python${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}"
  )

  add_executable(PyImathTestC main.cpp testStringTable.cpp)

  target_link_libraries(PyImathTestC 
        Imath::Config
        Imath::PyImath_Python${Python_VERSION_MAJOR}_${Python_VERSION_MINOR}
        Python3::Python
        Boost::${PYIMATH_BOOST_PY_COMPONENT})
  set_target_properties(PyImathTestC PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

  add_test(NAME Imath.PyImathTestC COMMAND $<TARGET_FILE:PyImathTestC>)
  set(PYTHONPATH ${CMAKE_BINARY_DIR}/python${Python_VERSION_MAJOR}_${Python_VERSION_MINOR})
  set_tests_properties(Imath.PyImathTestC PROPERTIES ENVIRONMENT PYTHONPATH=${PYTHONPATH})
  
endif()
  

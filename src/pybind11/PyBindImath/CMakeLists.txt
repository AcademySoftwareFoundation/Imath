# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
find_package(pybind11 REQUIRED)

#
# Source/headers
#

set(PYBINDIMATH_SOURCES
    PyBindImathFun.cpp
    PyBindImathBox.cpp
    PyBindImathVec.cpp
    PyBindImathPlane.cpp
    PyBindImathLine.cpp
    
    # PyBindImathEuler build Error pybind test
    # PyBindImathFrustum build failing
)

set(PYBINDIMATH_HEADERS
    PyBindImathExport.h
    PyBindImath.h
)

#
# shared library, e.g. libPyBindImath_Python3_11-3_2.so.30.3.2.0
#

set(PYBINDIMATH_LIBRARY PyBindImath)

add_library(${PYBINDIMATH_LIBRARY} SHARED ${PYBINDIMATH_SOURCES})

target_link_libraries(${PYBINDIMATH_LIBRARY} PRIVATE Imath::Imath pybind11::module)

# Set include directories
target_include_directories(${PYBINDIMATH_LIBRARY} PRIVATE ${Python3_INCLUDE_DIRS} ${Imath_INCLUDE_DIRS})

if(NOT "${PYBINDIMATH_LIB_PYTHONVER_ROOT}" STREQUAL "")
  set(pythonver_root "${PYBINDIMATH_LIB_PYTHONVER_ROOT}${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}")
  message("pythonver_root ${PYBINDIMATH_LIB_PYTHONVER_ROOT}${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}")
endif()

if(BUILD_SHARED_LIBS)
  # This creates the so-versioned library symlinks
  set_target_properties(${PYBINDIMATH_LIBRARY} PROPERTIES
    SOVERSION ${IMATH_LIB_SOVERSION}
    VERSION ${IMATH_LIB_VERSION}
    OUTPUT_NAME "${PYBINDIMATH_CURLIB_OUTROOT}${PYBINDIMATH_LIBRARY}${pythonver_root}${PYBINDIMATH_LIB_SUFFIX}"
  )
endif()

#
# python module, e.g. pybindimath.cpython-311-x86_64-linux-gnu.so
#

set(PYBINDIMATH_MODULE pybindimath)

pybind11_add_module(pybindimath MODULE pybindimathmodule.cpp $<TARGET_OBJECTS:${PYBINDIMATH_LIBRARY}>)

target_link_libraries(${PYBINDIMATH_MODULE} PRIVATE Imath::Imath pybind11::module)

if(SKBUILD)
  set(PYTHON_INSTALL_DIR ${SKBUILD_PLATLIB_DIR})
else()
  set(PYTHON_INSTALL_DIR "lib/python${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}/site-packages")
endif()

if (IMATH_INSTALL)

  # module
  
  install(TARGETS ${PYBINDIMATH_MODULE} DESTINATION ${PYTHON_INSTALL_DIR} COMPONENT python)

  # shared library

  install(TARGETS ${PYBINDIMATH_LIBRARY} 
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
      FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
      INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

  if(BUILD_SHARED_LIBS AND (NOT "${IMATH_LIB_SUFFIX}" STREQUAL "") AND IMATH_INSTALL_SYM_LINK)

    # create symlinks for the shared object so versions

    string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
    set(verlibname ${CMAKE_SHARED_LIBRARY_PREFIX}${PYBINDIMATH_LIBRARY}${pythonver_root}${IMATH_LIB_SUFFIX}${CMAKE_${uppercase_CMAKE_BUILD_TYPE}_POSTFIX}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(baselibname ${CMAKE_SHARED_LIBRARY_PREFIX}${PYBINDIMATH_LIBRARY}${pythonver_root}${CMAKE_${uppercase_CMAKE_BUILD_TYPE}_POSTFIX}${CMAKE_SHARED_LIBRARY_SUFFIX})
    file(CREATE_LINK ${verlibname} ${CMAKE_CURRENT_BINARY_DIR}/${baselibname} SYMBOLIC)
    if(WIN32)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${baselibname} DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})
      install(CODE "message(STATUS \"Creating symlink ${CMAKE_INSTALL_FULL_BINDIR}/${baselibname} -> ${verlibname}\")")
    else()
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${baselibname} DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})
      install(CODE "message(STATUS \"Creating symlink ${CMAKE_INSTALL_FULL_LIBDIR}/${baselibname} -> ${verlibname}\")")
    endif()
  endif()

  # pkgconfig

  set(pcinfile PyBindImath.pc.in)
  set(prefix ${CMAKE_INSTALL_PREFIX})
  set(exec_prefix "\${prefix}")
  set(libdir "\${exec_prefix}/${CMAKE_INSTALL_LIBDIR}")
  set(includedir "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}")
  string(REPLACE ".in" "" pcout ${pcinfile})
  configure_file(${pcinfile} ${CMAKE_CURRENT_BINARY_DIR}/${pcout} @ONLY)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${pcout} DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

endif()

if(IMATH_BUILD_APPLE_FRAMEWORKS)
    set_target_properties(${PYBINDIMATH_LIBRARY} PROPERTIES
        FRAMEWORK TRUE
        FRAMEWORK_VERSION PYBINDIMATH_LIB_SUFFIX
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER github.com/AcademySoftwareFoundation/Imath.PyBind
        # OUTPUT_NAME "pybind"
        MACOSX_FRAMEWORK_IDENTIFIER github.com/AcademySoftwareFoundation/Imath.PyBind
        MACOSX_FRAMEWORK_BUNDLE_VERSION PYBINDIMATH_LIB_SUFFIX
        MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${Imath_VERSION})
endif()

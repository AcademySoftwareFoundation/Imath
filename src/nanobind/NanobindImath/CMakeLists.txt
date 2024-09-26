# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

find_package(Python REQUIRED COMPONENTS Interpreter Development)
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
find_package(nanobind REQUIRED)

#
# Source/headers
#

set(NANOBINDIMATH_SOURCES
    NanobindImathBox.cpp
    NanobindImathVec.cpp
)

set(NANOBINDIMATH_HEADERS
    NanobindImathExport.h
    NanobindImath.h
)

#
# shared library, e.g. libNanobindImath_Python3_11-3_2.so.30.3.2.0
#

set(NANOBINDIMATH_LIBRARY NanobindImath)

add_library(${NANOBINDIMATH_LIBRARY} SHARED ${NANOBINDIMATH_SOURCES})

nanobind_build_library(nanobind-static)

target_link_libraries(${NANOBINDIMATH_LIBRARY} PRIVATE Imath::Imath nanobind-static)

# Set include directories
target_include_directories(${NANOBINDIMATH_LIBRARY} PRIVATE ${Python3_INCLUDE_DIRS} ${Imath_INCLUDE_DIRS})

if(NOT "${NANOBINDIMATH_LIB_PYTHONVER_ROOT}" STREQUAL "")
  set(pythonver_root "${NANOBINDIMATH_LIB_PYTHONVER_ROOT}${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}")
  message("pythonver_root ${NANOBINDIMATH_LIB_PYTHONVER_ROOT}${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}")
endif()

if(BUILD_SHARED_LIBS)
  # This creates the so-versioned library symlinks
  set_target_properties(${NANOBINDIMATH_LIBRARY} PROPERTIES
    SOVERSION ${IMATH_LIB_SOVERSION}
    VERSION ${IMATH_LIB_VERSION}
    OUTPUT_NAME "${NANOBINDIMATH_CURLIB_OUTROOT}${NANOBINDIMATH_LIBRARY}${pythonver_root}${NANOBINDIMATH_LIB_SUFFIX}"
  )
endif()

#
# python module, e.g. nanobindimath.cpython-311-x86_64-linux-gnu.so
#

set(NANOBINDIMATH_MODULE nanobindimath)

nanobind_add_module(nanobindimath MODULE nanobindimathmodule.cpp $<TARGET_OBJECTS:${NANOBINDIMATH_LIBRARY}>)

target_link_libraries(${NANOBINDIMATH_MODULE} PRIVATE Imath::Imath)

if(SKBUILD)
  set(PYTHON_INSTALL_DIR ${SKBUILD_PLATLIB_DIR})
else()
  set(PYTHON_INSTALL_DIR "lib/python${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}/site-packages")
endif()

if (IMATH_INSTALL)

  # module

  install(TARGETS ${NANOBINDIMATH_MODULE} DESTINATION ${PYTHON_INSTALL_DIR} COMPONENT python)

  # shared library

  install(TARGETS ${NANOBINDIMATH_LIBRARY}
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
      INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

  if(BUILD_SHARED_LIBS AND (NOT "${IMATH_LIB_SUFFIX}" STREQUAL "") AND IMATH_INSTALL_SYM_LINK)

    # create symlinks for the shared object so versions

    string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
    set(verlibname ${CMAKE_SHARED_LIBRARY_PREFIX}${NANOBINDIMATH_LIBRARY}${pythonver_root}${IMATH_LIB_SUFFIX}${CMAKE_${uppercase_CMAKE_BUILD_TYPE}_POSTFIX}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(baselibname ${CMAKE_SHARED_LIBRARY_PREFIX}${NANOBINDIMATH_LIBRARY}${pythonver_root}${CMAKE_${uppercase_CMAKE_BUILD_TYPE}_POSTFIX}${CMAKE_SHARED_LIBRARY_SUFFIX})
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

  set(pcinfile NanobindImath.pc.in)
  set(prefix ${CMAKE_INSTALL_PREFIX})
  set(exec_prefix "\${prefix}")
  set(libdir "\${exec_prefix}/${CMAKE_INSTALL_LIBDIR}")
  set(includedir "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}")
  string(REPLACE ".in" "" pcout ${pcinfile})
  configure_file(${pcinfile} ${CMAKE_CURRENT_BINARY_DIR}/${pcout} @ONLY)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${pcout} DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

endif()

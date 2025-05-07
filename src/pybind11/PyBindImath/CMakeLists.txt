# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

#
# Declare the libPyImath library, identified by PYIMATH_LIBRARY
#
# The name has a "_Python<major>_<minor>-<release>" suffix,
# i.e. libPyBindImath_Python3_10-3_2.so, subject to
# PYIMATH_LIB_PYTHONVER_ROOT and PYIMATH_LIB_SUFFIX
#

find_package(pybind11 CONFIG REQUIRED)

set(PYBINDIMATH_LIBRARY PyBindImath)

message(STATUS "Configuring pybindimath module and ${PYBINDIMATH_LIBRARY} library")

set(PYBINDIMATH_SOURCES
    PyBindImathFun.cpp
    PyBindImathBox.cpp
    PyBindImathVec.cpp
    PyBindImathPlane.cpp
    PyBindImathLine.cpp
#    PyBindImathEuler.cpp
#    PyBindImathFrustum.cpp
)

set(PYBINDIMATH_HEADERS
    PyBindImathExport.h
    PyBindImath.h
)

#
# libPyBindImath library
#

if(BUILD_SHARED_LIBS)
    add_library(${PYBINDIMATH_LIBRARY} SHARED ${PYBINDIMATH_HEADERS} ${PYBINDIMATH_SOURCES})
    if(WIN32)
        # IMATH_DLL controls the export declarations for Windows
        target_compile_definitions(${PYBINDIMATH_LIBRARY} PUBLIC IMATH_DLL)

        # PYBINDIMATH_BUILD is used by PyBindImathExport.h to set
        # PYBINDIMATH_EXPORT to either dllexport or dllimport
        target_compile_definitions(${PYBINDIMATH_LIBRARY} PRIVATE PYBINDIMATH_BUILD)
    endif()
else()
    add_library(${PYBINDIMATH_LIBRARY} STATIC ${PYBINDIMATH_HEADERS} ${PYBINDIMATH_SOURCES})
endif()

add_library(Imath::PyBindImath ALIAS ${PYBINDIMATH_LIBRARY})

if (IMATH_BUILD_APPLE_FRAMEWORKS)
  set_target_properties(PyBindImath PROPERTIES 
    FRAMEWORK TRUE
    FRAMEWORK_VERSION "${IMATH_LIB_VERSION}"
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "org.aswf.PyBindImath"
    MACOSX_FRAMEWORK_IDENTIFIER "org.aswf.PyBindImath"
    MACOSX_FRAMEWORK_BUNDLE_VERSION "${IMATH_LIB_VERSION}"
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING "${Imath_VERSION}"
    MACOSX_RPATH TRUE)
else()
  set_target_properties(${PYBINDIMATH_LIBRARY} PROPERTIES
    OUTPUT_NAME "${PYBINDIMATH_LIBRARY}${PYBINDIMATH_LIB_SUFFIX}"
    SOVERSION ${IMATH_LIB_SOVERSION}
    VERSION ${IMATH_LIB_VERSION}
    POSITION_INDEPENDENT_CODE ON
  )
endif()

target_include_directories(${PYBINDIMATH_LIBRARY}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
      $<INSTALL_INTERFACE:include/$PYBINDIMATH_LIBRARY>
)

target_compile_definitions(${PYBINDIMATH_LIBRARY} PRIVATE IMATH_EXPORTS)

target_link_libraries(${PYBINDIMATH_LIBRARY} PUBLIC Imath)
target_link_libraries(${PYBINDIMATH_LIBRARY} PRIVATE pybind11::module)

set_property(TARGET ${PYBINDIMATH_LIBRARY} PROPERTY PUBLIC_HEADER ${PYBINDIMATH_HEADERS})

#
# Python module, identified by PYBINDIMATH_MODULE e.g. pybindimath.cpython-311-x86_64-linux-gnu.so
#

# NOTE: the target needs the "_module" suffix to distinquish it from
# the PyBindImath library target. The OUTPUT_NAME gets set below to name
# it properly

set(PYBINDIMATH_MODULE_NAME pybindimath)
set(PYBINDIMATH_MODULE ${PYBINDIMATH_MODULE_NAME}_module)

set(PYBINDIMATH_MODULE_SOURCES pybindimathmodule.cpp)

pybind11_add_module(${PYBINDIMATH_MODULE} ${PYBINDIMATH_MODULE_SOURCES})

set_target_properties(${PYBINDIMATH_MODULE} PROPERTIES
    OUTPUT_NAME ${PYBINDIMATH_MODULE_NAME} # override the _d suffix for Debug builds
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/python${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}/"
    PREFIX ""
    DEBUG_POSTFIX ""
)

target_link_libraries(${PYBINDIMATH_MODULE}
    PRIVATE
        ${PYBINDIMATH_LIBRARY}
        Imath::Imath
)

#
# Installation
#

if (IMATH_INSTALL)

  if (NOT DEFINED PYTHON_INSTALL_DIR)

    #
    # Set the PYTHON_INSTALL_DIR but only if it's not already set,
    # which allows an externally-set value to take effect.
    #
  
    set(PYTHON_INSTALL_DIR "lib/python${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}/site-packages")
    message(STATUS "installing ${PYBINDIMATH_MODULE} to ${PYTHON_INSTALL_DIR}")

  else()

    message(STATUS "installing ${PYBINDIMATH_MODULE} to ${PYTHON_INSTALL_DIR} (set externally)")
  
  endif()
  
  #
  # Install python module 
  #
  
  install(TARGETS ${PYBINDIMATH_MODULE} LIBRARY DESTINATION ${PYTHON_INSTALL_DIR})

  #
  # Install the library
  # 

  install(TARGETS ${PYBINDIMATH_LIBRARY}
    EXPORT "${PYBINDIMATH_LIBRARY}Targets"
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT runtime
      OPTIONAL
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PYBINDIMATH_OUTPUT_SUBDIR}
  )

  # Install export set for downstream CMake

  if (NOT IMATH_BUILD_APPLE_FRAMEWORKS)
    install(EXPORT ${PYBINDIMATH_LIBRARY}Targets
      DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Imath
      FILE ${PYBINDIMATH_LIBRARY}Targets.cmake
      NAMESPACE ${PROJECT_NAME}::
    )
  endif()
  
  export(EXPORT Imath
    FILE "${CMAKE_BINARY_DIR}/config/PyBindImathTargets.cmake"
    NAMESPACE Imath::
  )

  if(BUILD_SHARED_LIBS
     AND (NOT "${PYBINDIMATH_LIB_SUFFIX}" STREQUAL "")
     AND IMATH_INSTALL_SYM_LINK
     AND NOT IMATH_BUILD_APPLE_FRAMEWORKS)

    # create symlink from libPyBindImath_Python3_11-3_2.so to libPyBindImath.so
    string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
    set(postfix ${CMAKE_${uppercase_CMAKE_BUILD_TYPE}_POSTFIX})
    set(VERSIONED_LIB_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PYBINDIMATH_LIBRARY}${PYBINDIMATH_LIB_SUFFIX}${postfix}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(BASE_LIB_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PYBINDIMATH_LIBRARY}${postfix}${CMAKE_SHARED_LIBRARY_SUFFIX})

    file(CREATE_LINK ${VERSIONED_LIB_FILENAME} ${CMAKE_CURRENT_BINARY_DIR}/${BASE_LIB_FILENAME} SYMBOLIC)
    if(WIN32)
      set(INSTALL_DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})
    else()
      set(INSTALL_DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})
    endif()
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${BASE_LIB_FILENAME} DESTINATION ${INSTALL_DESTINATION})
    install(CODE "message(STATUS \"Creating symlink ${INSTALL_DESTINATION}/${BASE_LIB_FILENAME} -> ${VERSIONED_LIB_FILENAME}\")")

  endif()

  # pkgconfig

  if(IMATH_INSTALL_PKG_CONFIG AND NOT IMATH_BUILD_APPLE_FRAMEWORKS)
    set(pcinfile PyBindImath.pc.in)
    set(prefix ${CMAKE_INSTALL_PREFIX})
    set(exec_prefix "\${prefix}")
    set(libdir "\${exec_prefix}/${CMAKE_INSTALL_LIBDIR}")
    set(includedir "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}")
    string(REPLACE ".in" "" pcout ${pcinfile})
    configure_file(${pcinfile} ${CMAKE_CURRENT_BINARY_DIR}/${pcout} @ONLY)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${pcout} DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
    message(STATUS "generating ${pcout}") 
  else()
    message(STATUS "pkg-config generation disabled for ${PYBINDIMATH_LIBRARY}")
  endif()

endif()

if (MSVC)
    target_compile_options(${PYBINDIMATH_LIBRARY} PRIVATE /bigobj)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  set_source_files_properties(
    ${PYBINDIMATH_MODULE_SOURCES} ${PYBINDIMATH_SOURCES} ${PYBINDIMATH_HEADERS}
    PROPERTIES COMPILE_FLAGS "-Wno-self-assign-overloaded"
  )
endif()

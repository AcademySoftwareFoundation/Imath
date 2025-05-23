# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

#
# The PyImath directory declares both the imath module and the PyImath
# library, which is used by downstream projects (e.g. Alembic) that
# provide python wrappings based on Imath.
#

#
# Declare the libPyImath library, identified by PYIMATH_LIBRARY.
# Note that PYIMATH_LIB_SUFFIX adds the "_Python3_9-3_4" to the library filename.
#

set(PYIMATH_LIBRARY PyImath)

message(STATUS "Configuring imath module and ${PYIMATH_LIBRARY} library")

set(PYIMATH_SOURCES
    PyImath.cpp
    PyImathAutovectorize.cpp
    PyImathBox2Array.cpp
    PyImathBox3Array.cpp
    PyImathBox.cpp
    PyImathBufferProtocol.cpp
    PyImathColor3.cpp
    PyImathColor4.cpp
    PyImathEuler.cpp
    PyImathFixedArray.cpp
    PyImathFrustum.cpp
    PyImathLine.cpp
    PyImathMatrix22.cpp
    PyImathMatrix33.cpp
    PyImathMatrix44.cpp
    PyImathPlane.cpp
    PyImathQuat.cpp
    PyImathRandom.cpp
    PyImathShear.cpp
    PyImathStringArray.cpp
    PyImathStringTable.cpp
    PyImathTask.cpp
    PyImathUtil.cpp
    PyImathFixedVArray.cpp
    PyImathVec2fd.cpp
    PyImathVec2si.cpp
    PyImathVec3fd.cpp
    PyImathVec3siArray.cpp
    PyImathVec3si.cpp
    PyImathVec4fd.cpp
    PyImathVec4siArray.cpp
    PyImathVec4si.cpp
)

set(PYIMATH_HEADERS
    PyImath.h
    PyImathAPI.h
    PyImathAutovectorize.h
    PyImathBasicTypes.h
    PyImathBox.h
    PyImathBoxArrayImpl.h
    PyImathBufferProtocol.h
    PyImathColor.h
    PyImathColor3ArrayImpl.h
    PyImathColor4Array2DImpl.h
    PyImathColor4ArrayImpl.h
    PyImathDecorators.h
    PyImathEuler.h
    PyImathExport.h
    PyImathFixedArray.h
    PyImathFixedArray2D.h
    PyImathFixedArrayTraits.h
    PyImathFixedMatrix.h
    PyImathFixedVArray.h
    PyImathFrustum.h
    PyImathFun.h
    PyImathLine.h
    PyImathMathExc.h
    PyImathMatrix.h
    PyImathOperators.h
    PyImathPlane.h
    PyImathQuat.h
    PyImathQuatOperators.h
    PyImathRandom.h
    PyImathShear.h
    PyImathStringArray.h
    PyImathStringArrayRegister.h
    PyImathStringTable.h
    PyImathTask.h
    PyImathUtil.h
    PyImathVec.h
    PyImathVec2Impl.h
    PyImathVec3ArrayImpl.h
    PyImathVec3Impl.h
    PyImathVec4ArrayImpl.h
    PyImathVec4Impl.h
    PyImathVecOperators.h
)

if(BUILD_SHARED_LIBS)
    add_library(${PYIMATH_LIBRARY} SHARED ${PYIMATH_HEADERS} ${PYIMATH_SOURCES})
    if(WIN32)
        # IMATH_DLL controls the export declarations for Windows
        target_compile_definitions(${PYIMATH_LIBRARY} PUBLIC IMATH_DLL)

        # PYIMATH_BUILD is used by PyImathExport.h to set
        # PYIMATH_EXPORT to either dllexport or dllimport
        target_compile_definitions(${PYIMATH_LIBRARY} PRIVATE PYIMATH_BUILD)
    endif()
else()
   add_library(${PYIMATH_LIBRARY} STATIC ${PYIMATH_HEADERS} ${PYIMATH_SOURCES})
endif()

add_library(Imath::PyImath ALIAS ${PYIMATH_LIBRARY})

if (IMATH_BUILD_APPLE_FRAMEWORKS)
  set_target_properties(PyBindImath PROPERTIES 
    FRAMEWORK TRUE
    FRAMEWORK_VERSION "${IMATH_LIB_VERSION}"
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "org.aswf.PyImath"
    MACOSX_FRAMEWORK_IDENTIFIER "org.aswf.PyImath"
    MACOSX_FRAMEWORK_BUNDLE_VERSION "${IMATH_LIB_VERSION}"
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING "${Imath_VERSION}"
    MACOSX_RPATH TRUE)
else()
  set_target_properties(${PYIMATH_LIBRARY} PROPERTIES
    OUTPUT_NAME "${PYIMATH_LIBRARY}${PYIMATH_LIB_SUFFIX}"
    SOVERSION ${IMATH_LIB_SOVERSION}
    VERSION ${IMATH_LIB_VERSION}
    POSITION_INDEPENDENT_CODE ON
  )
endif()

target_include_directories(${PYIMATH_LIBRARY}
     PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/${PYIMATH_LIBRARY}>
        ${Python3_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
)

target_compile_definitions(${PYIMATH_LIBRARY} PRIVATE IMATH_EXPORTS)

target_link_libraries(${PYIMATH_LIBRARY} PUBLIC Imath)
target_link_libraries(${PYIMATH_LIBRARY} PRIVATE Boost::boost Boost::python Python3::Module)

set_property(TARGET ${PYIMATH_LIBRARY} PROPERTY PUBLIC_HEADER ${PYIMATH_HEADERS})

#
# Python module, identified by PYIMATH_MODULE (e.g. "imath).
# Note that CMake on Windows apparently does not respect case in
# target names, and since the target for the main Imath library is
# "Imath", the target here is "imath_module". The 
#

set(PYIMATH_MODULE_NAME imath) 
set(PYIMATH_MODULE ${PYIMATH_MODULE_NAME}_module)

set(PYIMATH_MODULE_SOURCES
    imathmodule.cpp
    PyImathFun.cpp
    PyImathBasicTypes.cpp
)

# Build "imath", not "imath_d" in Debug.
# Note this must appear before add_library()
#set(CMAKE_DEBUG_POSTFIX "")

add_library(${PYIMATH_MODULE} MODULE ${PYIMATH_MODULE_SOURCES})

if(WIN32)
    # Ensure correct module extension on Windows
    set_target_properties(${PYIMATH_MODULE} PROPERTIES SUFFIX ".pyd")
endif()

set_target_properties(${PYIMATH_MODULE} PROPERTIES
    OUTPUT_NAME ${PYIMATH_MODULE_NAME} # override the _d suffix for Debug builds
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/python${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}/"
    PREFIX ""
    DEBUG_POSTFIX ""
)

target_include_directories(${PYIMATH_MODULE} PRIVATE ${Python3_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

target_link_libraries(${PYIMATH_MODULE} 
    PRIVATE
        ${PYIMATH_LIBRARY}
        Imath::Imath
        Boost::boost
        Boost::python
        Python3::Module
)

#
# PyImathConfig
#

add_library(PyImathConfig INTERFACE)
target_include_directories(PyImathConfig INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
install(TARGETS PyImathConfig EXPORT ${PROJECT_NAME})
add_library(PyImath::Config ALIAS PyImathConfig)


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
    message(STATUS "installing ${PYIMATH_MODULE} module to ${PYTHON_INSTALL_DIR}")

  else()

    message(STATUS "installing ${PYIMATH_MODULE} module to ${PYTHON_INSTALL_DIR} (set externally)")
  
  endif()
  
  #
  # Install the python module 
  #

  install(TARGETS ${PYIMATH_MODULE} LIBRARY DESTINATION ${PYTHON_INSTALL_DIR})

  #
  # Install the library
  #
  
  install(TARGETS ${PYIMATH_LIBRARY}
    EXPORT "${PYIMATH_LIBRARY}Targets"
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT runtime
      OPTIONAL
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PYIMATH_OUTPUT_SUBDIR}
  )

  # Install export set for downstream CMake

  if (NOT IMATH_BUILD_APPLE_FRAMEWORKS)
    install(EXPORT ${PYIMATH_LIBRARY}Targets
      DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Imath
      FILE ${PYIMATH_LIBRARY}Targets.cmake
      NAMESPACE ${PROJECT_NAME}::
    )
  endif()
  
  export(EXPORT Imath
    FILE "${CMAKE_BINARY_DIR}/config/PyImathTargets.cmake"
    NAMESPACE Imath::
  )

  if(BUILD_SHARED_LIBS
     AND (NOT "${PYIMATH_LIB_SUFFIX}" STREQUAL "")
     AND IMATH_INSTALL_SYM_LINK
     AND NOT IMATH_BUILD_APPLE_FRAMEWORKS)

    # create symlink from libPyImath_Python3_11-3_2.so to libPyImath.so
    string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
    set(postfix ${CMAKE_${uppercase_CMAKE_BUILD_TYPE}_POSTFIX})
    set(VERSIONED_LIB_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PYIMATH_LIBRARY}${PYIMATH_LIB_SUFFIX}${postfix}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(BASE_LIB_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PYIMATH_LIBRARY}${postfix}${CMAKE_SHARED_LIBRARY_SUFFIX})

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
    set(pcinfile PyImath.pc.in)
    set(prefix ${CMAKE_INSTALL_PREFIX})
    set(exec_prefix "\${prefix}")
    set(libdir "\${exec_prefix}/${CMAKE_INSTALL_LIBDIR}")
    set(includedir "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}")
    string(REPLACE ".in" "" pcout ${pcinfile})
    configure_file(${pcinfile} ${CMAKE_CURRENT_BINARY_DIR}/${pcout} @ONLY)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${pcout} DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
    message(STATUS "generating ${pcout}") 
  else()
    message(STATUS "pkg-config generation disabled for PyImath")
  endif()

endif()

#
# disable the self-assign warnings:
#
#   warning: explicitly assigning value of variable of type 'boost::python::self_ns::self_t' to itself [-Wself-assign-overloaded]
#        .def(self ^= self)
#

if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  set_source_files_properties(
    ${PYIMATH_MODULE_SOURCES} ${PYIMATH_SOURCES} ${PYIMATH_HEADERS}
    PROPERTIES COMPILE_FLAGS "-Wno-self-assign-overloaded"
  )
elseif(NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set_source_files_properties(
    ${PYIMATH_MODULE_SOURCES} ${PYIMATH_SOURCES} ${PYIMATH_HEADERS}
    PROPERTIES COMPILE_FLAGS "-Wdeprecated-declarations"
  )
endif()

if (MSVC)
    target_compile_options(${PYIMATH_LIBRARY} PRIVATE /bigobj)
endif()

#
# Suppress boost's pragma messages about deprecated headers:
#    include/boost/detail/iterator.hpp:13:1: note: ‘#pragma message: This header is deprecated. Use <iterator> instead.’
#    BOOST_HEADER_DEPRECATED("<iterator>")
#

target_compile_definitions(${PYIMATH_LIBRARY} PRIVATE BOOST_ALLOW_DEPRECATED_HEADERS)
target_compile_definitions(${PYIMATH_MODULE} PRIVATE BOOST_ALLOW_DEPRECATED_HEADERS)


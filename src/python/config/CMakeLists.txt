# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

### The autoconf setup for this folder generates a PyImathConfig.h file
### but no source actually uses that, so let's elide that for now

add_library(PyImathConfig INTERFACE)
target_include_directories(PyImathConfig INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
install(TARGETS PyImathConfig EXPORT ${PROJECT_NAME})
add_library(PyImath::Config ALIAS PyImathConfig)

if(IMATH_INSTALL_PKG_CONFIG)
  # use a helper function to avoid variable pollution, but pretty simple
  function(pyimath_pkg_config_help pcinfile)
    set(prefix ${CMAKE_INSTALL_PREFIX})
    set(exec_prefix "\${prefix}")
    set(libdir "\${exec_prefix}/${CMAKE_INSTALL_LIBDIR}")
    set(includedir "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}")
    string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
    set(LIB_SUFFIX_DASH ${IMATH_LIB_SUFFIX}${CMAKE_${uppercase_CMAKE_BUILD_TYPE}_POSTFIX})
    string(REPLACE ".in" "" pcout ${pcinfile})
    configure_file(${pcinfile} ${CMAKE_CURRENT_BINARY_DIR}/${pcout} @ONLY)
    install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/${pcout}
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
    )
  endfunction()
  pyimath_pkg_config_help(../PyImath.pc.in)
else()
  message(STATUS "pkg-config generation disabled")
endif()

# The main export of the configuration - This is the
# moral equivalent of a pkg-config file for cmake
# and replaces the Find*.cmake of the "old" cmake
include(CMakePackageConfigHelpers)

write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${IMATH_VERSION}
  COMPATIBILITY SameMajorVersion
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
install(EXPORT ${PROJECT_NAME}
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  FILE ${PROJECT_NAME}Config.cmake
  NAMESPACE ${PROJECT_NAME}::
  EXPORT_LINK_INTERFACE_LIBRARIES
)

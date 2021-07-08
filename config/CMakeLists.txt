# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

if (IMATH_ENABLE_LARGE_STACK)
  set(IMATH_HAVE_LARGE_STACK ON)
endif()
if (IMATH_USE_DEFAULT_VISIBILITY)
  set(IMATH_ENABLE_API_VISIBILITY OFF)
else()
  set(IMATH_ENABLE_API_VISIBILITY ON)
endif()

configure_file(ImathConfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/ImathConfig.h)

add_library(ImathConfig INTERFACE)

target_include_directories(ImathConfig INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${IMATH_OUTPUT_SUBDIR}>)

install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ImathConfig.h
  DESTINATION
    ${CMAKE_INSTALL_INCLUDEDIR}/${IMATH_OUTPUT_SUBDIR}
)

install(TARGETS ImathConfig EXPORT ${PROJECT_NAME})

add_library(Imath::Config ALIAS ImathConfig)

if(IMATH_INSTALL_PKG_CONFIG)
  # use a helper function to avoid variable pollution, but pretty simple
  function(imath_pkg_config_help pcinfile)
    set(prefix ${CMAKE_INSTALL_PREFIX})
    set(exec_prefix "\${prefix}")
    set(libdir ${CMAKE_INSTALL_FULL_LIBDIR})
    set(includedir ${CMAKE_INSTALL_FULL_INCLUDEDIR})
    string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
    set(LIB_SUFFIX_DASH ${IMATH_LIB_SUFFIX}${CMAKE_${uppercase_CMAKE_BUILD_TYPE}_POSTFIX})
    string(REPLACE ".in" "" pcout ${pcinfile})
    configure_file(${pcinfile} ${CMAKE_CURRENT_BINARY_DIR}/${pcout} @ONLY)
    install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/${pcout}
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
    )
  endfunction()
  imath_pkg_config_help(Imath.pc.in)
else()
  message(STATUS "pkg-config generation disabled")
endif()

#
# The main export of the configuration: this is the cmake equivalent
# of a pkg-config file and replaces the Find*.cmake of the "old" cmake
#

include(CMakePackageConfigHelpers)

configure_package_config_file(ImathConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${IMATH_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(EXPORT ${PROJECT_NAME}
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  EXPORT_LINK_INTERFACE_LIBRARIES
)

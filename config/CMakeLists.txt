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
set(IMATH_CONFIG_DIR "${CMAKE_CURRENT_BINARY_DIR}" CACHE INTERNAL "")

add_library(ImathConfig INTERFACE)
add_library(Imath::Config ALIAS ImathConfig)

target_include_directories(ImathConfig INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${IMATH_OUTPUT_SUBDIR}>)

if (IMATH_INSTALL)
  if (NOT IMATH_BUILD_APPLE_FRAMEWORKS)
    install(
      FILES ${CMAKE_CURRENT_BINARY_DIR}/ImathConfig.h
      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${IMATH_OUTPUT_SUBDIR}
    )
  endif()
  install(TARGETS ImathConfig EXPORT Imath)
endif()

if(IMATH_INSTALL_PKG_CONFIG AND NOT IMATH_BUILD_APPLE_FRAMEWORKS)

  # Prepend prefix for includedir only if CMAKE_INSTALL_INCLUDEDIR is relative
  if(IS_ABSOLUTE "${CMAKE_INSTALL_INCLUDEDIR}")
      set(PKG_CONFIG_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}")
  else()
      set(PKG_CONFIG_INSTALL_INCLUDEDIR "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}")
  endif()

  # Prepend exec_prefix for libdir only if CMAKE_INSTALL_LIBDIR is relative
  if(IS_ABSOLUTE "${CMAKE_INSTALL_LIBDIR}")
      set(PKG_CONFIG_INSTALL_LIBDIR "${CMAKE_INSTALL_LIBDIR}")
  else()
      set(PKG_CONFIG_INSTALL_LIBDIR "\${exec_prefix}/${CMAKE_INSTALL_LIBDIR}")
  endif()

  # use a helper function to avoid variable pollution, but pretty simple
  function(imath_pkg_config_help pcinfile)
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
  message(STATUS "Imath pkg-config generation enabled")
else()
  message(STATUS "Imath pkg-config generation disabled")
endif()

#
# ImathConfig.cmake
#

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/ImathConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/ImathConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Imath
)

write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/ImathConfigVersion.cmake"
  VERSION ${IMATH_VERSION}
  COMPATIBILITY SameMajorVersion
)

if (IMATH_INSTALL)
  if (NOT IMATH_BUILD_APPLE_FRAMEWORKS)
    install(
      FILES ${CMAKE_CURRENT_BINARY_DIR}/ImathConfig.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/ImathConfigVersion.cmake
      DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Imath
    )

    install(EXPORT Imath
      DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Imath
      FILE ImathTargets.cmake
      NAMESPACE Imath::
      EXPORT_LINK_INTERFACE_LIBRARIES
    )
  endif()

  export(EXPORT Imath
    FILE "${CMAKE_CURRENT_BINARY_DIR}/ImathTargets.cmake"
    NAMESPACE Imath::
  )
endif()

# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the Imath Project.

include(CheckIncludeFiles)
include(CheckSymbolExists)
include(CheckLibraryExists)
include(CheckStructHasMember)

check_include_files(ucontext.h IMATH_HAVE_UCONTEXT_H)
if(IMATH_HAVE_UCONTEXT_H)
  # TODO: remove this once we cleanly have IMATH_ prefix on all #defines
  set(HAVE_UCONTEXT_H ON)
  check_struct_has_member("struct _libc_fpstate" mxcsr ucontext.h IMATH_HAVE_CONTROL_REGISTER_SUPPORT)
  if(NOT IMATH_HAVE_CONTROL_REGISTER_SUPPORT)
    check_include_files(asm/sigcontext.h IMATH_HAVE_SIGCONTEXT_H)
    if(IMATH_HAVE_SIGCONTEXT_H)
      check_struct_has_member("struct _fpstate" mxcsr asm/sigcontext.h IMATH_HAVE_SIGCONTEXT_CONTROL_REGISTER_SUPPORT)
    endif()
  endif()
endif()

if (IMATH_ENABLE_LARGE_STACK)
  set(IMATH_HAVE_LARGE_STACK ON)
endif()

configure_file(ImathConfig.h.in_cmake ${CMAKE_CURRENT_BINARY_DIR}/ImathConfig.h)
configure_file(ImathConfigInternal.h.in_cmake ${CMAKE_CURRENT_BINARY_DIR}/ImathConfigInternal.h)

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
    set(libdir "\${exec_prefix}/${CMAKE_INSTALL_LIBDIR}")
    set(includedir "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}")
    set(LIB_SUFFIX_DASH ${IMATH_LIB_SUFFIX})
    if(TARGET Threads::Threads)
      # hrm, can't use properties as they end up as generator expressions
      # which don't seem to evaluate
      if(THREADS_HAVE_PTHREAD_ARG)
        set(PTHREAD_CFLAGS "-pthread")
      endif()
      set(PTHREAD_LIBS ${CMAKE_THREAD_LIBS_INIT})
    endif()
    string(REPLACE ".in" "" pcout ${pcinfile})
    configure_file(${pcinfile} ${CMAKE_CURRENT_BINARY_DIR}/${pcout} @ONLY)
    install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/${pcout}
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
    )
  endfunction()
  imath_pkg_config_help(../Imath.pc.in)
else()
  message(NOTICE "-- WARNING pkg-config generation disabled")
endif()

# The main export of the configuration - This is the
# moral equivalent of a pkg-config file for cmake
# and replaces the Find*.cmake of the "old" cmake
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

# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

function(PYIMATH_ADD_LIBRARY_PRIV libname)
  set(options)
  set(oneValueArgs PRIV_EXPORT CURDIR CURBINDIR OUTROOT)
  set(multiValueArgs SOURCE HEADERS DEPENDENCIES PRIVATE_DEPS)
  cmake_parse_arguments(PYIMATH_CURLIB "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})


  # Currently, the python bindings REQUIRE a shared library for
  # the Iex stuff to be initialized correctly. As such, force that
  # here
  # TODO Change this back when these bindings are refactored
  add_library(${libname} SHARED ${PYIMATH_CURLIB_SOURCE})
  #if(BUILD_SHARED_LIBS)
  set_target_properties(${libname} PROPERTIES
  SOVERSION ${IMATH_LIB_SOVERSION}
  VERSION ${IMATH_LIB_VERSION}
  )
  #endif()
  set_target_properties(${libname} PROPERTIES
    OUTPUT_NAME "${PYIMATH_CURLIB_OUTROOT}${libname}${PYIMATH_LIB_SUFFIX}"
  )
  target_compile_features(${libname} PUBLIC cxx_std_${IMATH_CXX_STANDARD})
  # we are always building shared, so don't check for that
  if(PYIMATH_CURLIB_PRIV_EXPORT)
    target_compile_definitions(${libname} PRIVATE ${PYIMATH_CURLIB_PRIV_EXPORT})
  endif()
  if(WIN32)
    target_compile_definitions(${libname} PUBLIC IMATH_DLL)
  endif()
  if(PYIMATH_CURLIB_CURDIR)
    target_include_directories(${libname} PUBLIC $<BUILD_INTERFACE:${PYIMATH_CURLIB_CURDIR}>)
  endif()
  if(PYIMATH_CURLIB_CURBINDIR)
    target_include_directories(${libname} PRIVATE $<BUILD_INTERFACE:${PYIMATH_CURLIB_CURBINDIR}>)
  endif()
  if(Boost_INCLUDE_DIR)
    target_include_directories(${libname} PUBLIC ${Boost_INCLUDE_DIR})
  endif()
  target_link_libraries(${libname} PUBLIC ${PYIMATH_CURLIB_DEPENDENCIES})
  if(PYIMATH_CURLIB_PRIVATE_DEPS)
    target_link_libraries(${libname} PRIVATE ${PYIMATH_CURLIB_PRIVATE_DEPS})
  endif()
  set_target_properties(${libname} PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    POSITION_INDEPENDENT_CODE ON
  )

  add_library(${PROJECT_NAME}::${libname} ALIAS ${libname})

  install(TARGETS ${libname}
    EXPORT ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )
endfunction()

# NB: This function has a number if specific names / variables
# not to mention behavior, so be careful copying...
function(PYIMATH_DEFINE_MODULE modname)
  set(options NEEDED_BY_OTHER_MODULES)
  set(oneValueArgs PRIV_EXPORT CURDIR CURBINDIR LIBNAME)
  set(multiValueArgs LIBSOURCE MODSOURCE HEADERS DEPENDENCIES PRIVATE_DEPS MODULE_DEPS)
  cmake_parse_arguments(PYIMATH_CURMOD "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(PYIMATH_CURMOD_HEADERS)
    install(
      FILES
        ${PYIMATH_CURMOD_HEADERS}
      DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}/${IMATH_OUTPUT_SUBDIR}
    )
  endif()

  if(NOT PYIMATH_CURMOD_LIBNAME)
    message(FATAL_ERROR "NYI usage of pyimath_define_module")
    return()
  endif()

  set(libarglist SOURCE ${PYIMATH_CURMOD_LIBSOURCE})
  if(PYIMATH_CURMOD_PRIV_EXPORT)
    list(APPEND libarglist PRIV_EXPORT ${PYIMATH_CURMOD_PRIV_EXPORT})
  endif()
  if(PYIMATH_CURMOD_HEADERS)
    list(APPEND libarglist HEADERS ${PYIMATH_CURMOD_HEADERS})
  endif()
  if(PYIMATH_CURMOD_CURDIR)
    list(APPEND libarglist CURDIR ${PYIMATH_CURMOD_CURDIR})
  endif()
  if(PYIMATH_CURMOD_CURBINDIR)
    list(APPEND libarglist CURBINDIR ${PYIMATH_CURMOD_CURBINDIR})
  endif()
  if(PYIMATH_CURMOD_DEPENDENCIES)
    list(APPEND libarglist DEPENDENCIES ${PYIMATH_CURMOD_DEPENDENCIES})
  endif()
  # NB: make this one last so we can cheat and add the python and boost
  # libs as private deps at the end regardless of whether it was provided
  list(APPEND libarglist PRIVATE_DEPS ${PYIMATH_CURMOD_PRIVATE_DEPS})
  if(USE_PYTHON2)
    if(TARGET Python2::Python AND TARGET Boost::${PYIMATH_BOOST_PY_COMPONENT})
      set(libname "${PYIMATH_CURMOD_LIBNAME}${PYIMATH_LIB_PYTHONVER_ROOT}${Python2_VERSION_MAJOR}_${Python2_VERSION_MINOR}")
      set(extraDeps ${PYIMATH_CURMOD_MODULE_DEPS})
      list(TRANSFORM extraDeps APPEND ${PYIMATH_LIB_PYTHONVER_ROOT}${Python2_VERSION_MAJOR}_${Python2_VERSION_MINOR})

      pyimath_add_library_priv(${libname}
        ${libarglist}
        ${extraDeps}
        Python2::Python
        Boost::${PYIMATH_BOOST_PY_COMPONENT}
      )

      Python2_add_library(${modname}_python2 MODULE ${PYIMATH_CURMOD_MODSOURCE})
      target_link_libraries(${modname}_python2
        PRIVATE
          ${libname}
          ${extraDeps}
          ${PYIMATH_CURMOD_DEPENDENCIES}
          ${PYIMATH_CURMOD_PRIVATE_DEPS}
          Boost::${PYIMATH_BOOST_PY_COMPONENT}
        )
      set_target_properties(${modname}_python2 PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/python${Python2_VERSION_MAJOR}_${Python2_VERSION_MINOR}/"
        LIBRARY_OUTPUT_NAME "${modname}"
        DEBUG_POSTFIX ""
      )
      install(TARGETS ${modname}_python2 DESTINATION ${PyImath_Python2_SITEARCH_REL})
    endif()
  else()
    if(TARGET Python3::Python AND TARGET Boost::${PYIMATH_BOOST_PY_COMPONENT})
      set(libname "${PYIMATH_CURMOD_LIBNAME}${PYIMATH_LIB_PYTHONVER_ROOT}${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}")
      set(extraDeps ${PYIMATH_CURMOD_MODULE_DEPS})
      list(TRANSFORM extraDeps APPEND ${PYIMATH_LIB_PYTHONVER_ROOT}${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR})

      pyimath_add_library_priv(${libname}
        ${libarglist}
        ${extraDeps}
        Python3::Python
        Boost::${PYIMATH_BOOST_PY_COMPONENT}
      )
      Python3_add_library(${modname}_python3 MODULE ${PYIMATH_CURMOD_MODSOURCE})
      target_link_libraries(${modname}_python3
        PRIVATE
          ${libname} ${extraDeps}
          ${PYIMATH_CURMOD_DEPENDENCIES}
          ${PYIMATH_CURMOD_PRIVATE_DEPS}
          Boost::${PYIMATH_BOOST_PY_COMPONENT}
        )
      set_target_properties(${modname}_python3 PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/python${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}/"
        LIBRARY_OUTPUT_NAME "${modname}"
        DEBUG_POSTFIX ""
      )
      install(TARGETS ${modname}_python3 DESTINATION ${PyImath_Python3_SITEARCH_REL})
    endif()
  endif()
endfunction()

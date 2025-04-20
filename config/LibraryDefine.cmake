# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

include(CheckLibraryExists)
check_library_exists(m sin "" HAVE_LIB_M)
if (HAVE_LIB_M)
  set(IMATH_EXTRA_LIBS m)
endif()

function(IMATH_DEFINE_LIBRARY libname)
  set(options)
  set(oneValueArgs PRIV_EXPORT CURDIR CURBINDIR)
  set(multiValueArgs SOURCES HEADERS DEPENDENCIES PRIVATE_DEPS)
  cmake_parse_arguments(IMATH_CURLIB "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if (MSVC)
    set(_imath_extra_flags "$<$<COMPILE_LANGUAGE:CXX>:/EHsc>")
  endif()
  set(objlib ${libname})
  add_library(${objlib}
    ${IMATH_CURLIB_HEADERS}
    ${IMATH_CURLIB_SOURCES})

  target_compile_features(${objlib}
                          PRIVATE cxx_std_${IMATH_CXX_STANDARD}
                          INTERFACE cxx_std_11)

  if(IMATH_CURLIB_PRIV_EXPORT AND BUILD_SHARED_LIBS)
    target_compile_definitions(${objlib} PRIVATE ${IMATH_CURLIB_PRIV_EXPORT})
    if(WIN32)
      target_compile_definitions(${objlib} PUBLIC IMATH_DLL)
    endif()
  endif()
  if(IMATH_CURLIB_CURDIR)
    target_include_directories(${objlib} INTERFACE $<BUILD_INTERFACE:${IMATH_CURLIB_CURDIR}>)
  endif()
  if(IMATH_CURLIB_CURBINDIR)
    target_include_directories(${objlib} PRIVATE $<BUILD_INTERFACE:${IMATH_CURLIB_CURBINDIR}>)
  endif()
  target_link_libraries(${objlib} PUBLIC ${PROJECT_NAME}::Config ${IMATH_CURLIB_DEPENDENCIES})
  if(IMATH_CURLIB_PRIVATE_DEPS)
    target_link_libraries(${objlib} PRIVATE ${IMATH_CURLIB_PRIVATE_DEPS})
  endif()
  set_target_properties(${objlib} PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    POSITION_INDEPENDENT_CODE ON
  )
  if (NOT IMATH_USE_DEFAULT_VISIBILITY)
    set_target_properties(${objlib} PROPERTIES
      C_VISIBILITY_PRESET hidden
      CXX_VISIBILITY_PRESET hidden
      VISIBILITY_INLINES_HIDDEN ON
    )
  else()
    target_compile_definitions(${objlib} PUBLIC IMATH_USE_DEFAULT_VISIBILITY)
  endif()
  if (_imath_extra_flags)
    target_compile_options(${objlib} PRIVATE ${_imath_extra_flags})
  endif()
  set_property(TARGET ${objlib} PROPERTY PUBLIC_HEADER ${IMATH_CURLIB_HEADERS})

  if(BUILD_SHARED_LIBS)
    set_target_properties(${libname} PROPERTIES
      SOVERSION ${IMATH_LIB_SOVERSION}
      VERSION ${IMATH_LIB_VERSION}
    )
  endif()
  if(IMATH_EXTRA_LIBS)
    target_link_libraries(${libname} PUBLIC ${IMATH_EXTRA_LIBS})
  endif()

  if (NOT IMATH_BUILD_APPLE_FRAMEWORKS)
    set_target_properties(${libname} PROPERTIES
        OUTPUT_NAME "${libname}${IMATH_LIB_SUFFIX}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
  endif() 
  add_library(${PROJECT_NAME}::${libname} ALIAS ${libname})

  if (IMATH_BUILD_APPLE_FRAMEWORKS)
    # Mark files as resources
    set_source_files_properties(${IMATH_RESOURCE_FILES} PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources
    )
    set_target_properties(${libname} PROPERTIES 
      FRAMEWORK TRUE
      FRAMEWORK_VERSION "${IMATH_LIB_VERSION}"
      XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "github.com/AcademySoftwareFoundation/Imath/${libname}"
      MACOSX_FRAMEWORK_IDENTIFIER "github.com/AcademySoftwareFoundation/Imath/${libname}"
      MACOSX_FRAMEWORK_BUNDLE_VERSION "${IMATH_LIB_VERSION}"
      MACOSX_FRAMEWORK_SHORT_VERSION_STRING "${Imath_VERSION}"
      MACOSX_RPATH TRUE)

    # Manually copy resource files to the Resources directory
    set(RES_DEST_DIR "$<TARGET_FILE_DIR:${libname}>/Resources")
    add_custom_command(TARGET ${libname} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E make_directory "${RES_DEST_DIR}"
      COMMAND ${CMAKE_COMMAND} -E copy ${IMATH_RESOURCE_FILES} "${RES_DEST_DIR}/"
      COMMENT "Copying resource files to ${libname}.framework/Resources"
    )
  endif()
  
  if (IMATH_INSTALL)
    install(TARGETS ${libname}
      EXPORT ${PROJECT_NAME}
      RUNTIME 
        DESTINATION ${CMAKE_INSTALL_BINDIR}
      LIBRARY 
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
      ARCHIVE 
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
      FRAMEWORK
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT runtime
        OPTIONAL
      INCLUDES 
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
      PUBLIC_HEADER
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${IMATH_OUTPUT_SUBDIR}
    )

    if(BUILD_SHARED_LIBS AND (NOT "${IMATH_LIB_SUFFIX}" STREQUAL "") AND IMATH_INSTALL_SYM_LINK AND NOT IMATH_BUILD_APPLE_FRAMEWORKS)
      string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
      set(verlibname ${CMAKE_SHARED_LIBRARY_PREFIX}${libname}${IMATH_LIB_SUFFIX}${CMAKE_${uppercase_CMAKE_BUILD_TYPE}_POSTFIX}${CMAKE_SHARED_LIBRARY_SUFFIX})
      set(baselibname ${CMAKE_SHARED_LIBRARY_PREFIX}${libname}${CMAKE_${uppercase_CMAKE_BUILD_TYPE}_POSTFIX}${CMAKE_SHARED_LIBRARY_SUFFIX})
      file(CREATE_LINK ${verlibname} ${CMAKE_CURRENT_BINARY_DIR}/${baselibname} SYMBOLIC)
      if(WIN32)
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${baselibname} DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})
        install(CODE "message(STATUS \"Creating symlink ${CMAKE_INSTALL_FULL_BINDIR}/${baselibname} -> ${verlibname}\")")
      else()
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${baselibname} DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})
        install(CODE "message(STATUS \"Creating symlink ${CMAKE_INSTALL_FULL_LIBDIR}/${baselibname} -> ${verlibname}\")")
      endif()
      set(verlibname)
      set(baselibname)
    endif()
  endif()
endfunction()
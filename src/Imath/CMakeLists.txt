# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

#
# Imath
#

set(IMATH_LIBRARY Imath)

set(IMATH_SOURCES
    half.cpp
    ImathColorAlgo.cpp
    ImathFun.cpp
    ImathMatrixAlgo.cpp
    ImathRandom.cpp
    toFloat.h
)

set(IMATH_HEADERS
    half.h
    halfFunction.h
    halfLimits.h
    ImathBox.h
    ImathBoxAlgo.h
    ImathColor.h
    ImathColorAlgo.h
    ImathEuler.h
    ImathExport.h
    ImathForward.h
    ImathFrame.h
    ImathFrustum.h
    ImathFrustumTest.h
    ImathFun.h
    ImathGL.h
    ImathGLU.h
    ImathInt64.h
    ImathInterval.h
    ImathLine.h
    ImathLineAlgo.h
    ImathMath.h
    ImathMatrix.h
    ImathMatrixAlgo.h
    ImathNamespace.h
    ImathPlane.h
    ImathPlatform.h
    ImathQuat.h
    ImathRandom.h
    ImathRoots.h
    ImathShear.h
    ImathSphere.h
    ImathTypeTraits.h
    ImathVec.h
    ImathVecAlgo.h
    ${IMATH_CONFIG_DIR}/ImathConfig.h
)

if(BUILD_SHARED_LIBS)

    add_library(${IMATH_LIBRARY} SHARED ${IMATH_HEADERS} ${IMATH_SOURCES})

    target_compile_definitions(${IMATH_LIBRARY} PRIVATE IMATH_EXPORTS)
    if(WIN32)
      target_compile_definitions(${IMATH_LIBRARY} PUBLIC IMATH_DLL)
    endif()

    set_target_properties(${IMATH_LIBRARY} PROPERTIES
      SOVERSION ${IMATH_LIB_SOVERSION}
      VERSION ${IMATH_LIB_VERSION}
    )

else()

    add_library(${IMATH_LIBRARY} STATIC ${IMATH_HEADERS} ${IMATH_SOURCES})

endif()

add_library(Imath::Imath ALIAS ${IMATH_LIBRARY})

#
# Compile options
#

target_compile_features(${IMATH_LIBRARY}
    PRIVATE cxx_std_${IMATH_CXX_STANDARD}
    INTERFACE cxx_std_11
)

set_target_properties(${IMATH_LIBRARY} PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    POSITION_INDEPENDENT_CODE ON
)

if (NOT IMATH_USE_DEFAULT_VISIBILITY)
    set_target_properties(${IMATH_LIBRARY} PROPERTIES
      C_VISIBILITY_PRESET hidden
      CXX_VISIBILITY_PRESET hidden
      VISIBILITY_INLINES_HIDDEN ON
    )
else()
    target_compile_definitions(${IMATH_LIBRARY} PUBLIC IMATH_USE_DEFAULT_VISIBILITY)
endif()

if (MSVC)
    target_compile_options(${IMATH_LIBRARY} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:/EHsc>")
endif()

set_property(TARGET ${IMATH_LIBRARY} PROPERTY PUBLIC_HEADER ${IMATH_HEADERS})

target_include_directories(${IMATH_LIBRARY} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

#
# Link options
#

target_link_libraries(${IMATH_LIBRARY} PUBLIC ImathConfig)

include(CheckLibraryExists)
check_library_exists(m sin "" HAVE_LIB_M)
if (HAVE_LIB_M)
    target_link_libraries(${IMATH_LIBRARY} PUBLIC m)
endif()

if (IMATH_BUILD_APPLE_FRAMEWORKS)
    # Mark files as resources
    set_source_files_properties(${IMATH_RESOURCE_FILES} PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources
    )
    set_target_properties(${IMATH_LIBRARY} PROPERTIES 
      FRAMEWORK TRUE
      FRAMEWORK_VERSION "${IMATH_LIB_VERSION}"
      XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "org.aswf.Imath"
      MACOSX_FRAMEWORK_IDENTIFIER "org.aswf.Imath"
      MACOSX_FRAMEWORK_BUNDLE_VERSION "${IMATH_LIB_VERSION}"
      MACOSX_FRAMEWORK_SHORT_VERSION_STRING "${Imath_VERSION}"
      MACOSX_RPATH TRUE)

    # Copy resource files to the build directory's framework Resources/
    set(RES_DEST_DIR "$<TARGET_FILE_DIR:${IMATH_LIBRARY}>/Resources")
    add_custom_command(TARGET ${IMATH_LIBRARY} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E make_directory "${RES_DEST_DIR}"
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              ${IMATH_RESOURCE_FILES}
              "${CMAKE_BINARY_DIR}/config/ImathTargets.cmake"
              "${RES_DEST_DIR}/"
      COMMENT "Copying resource files to Imath.framework/Resources in build directory"
    )
else()
    set_target_properties(${IMATH_LIBRARY} PROPERTIES OUTPUT_NAME "Imath${IMATH_LIB_SUFFIX}" )
endif()
  
#
# Install configuration
#

if (IMATH_INSTALL)

    install(TARGETS ${IMATH_LIBRARY}
      EXPORT Imath
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
      FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT runtime
        OPTIONAL
      INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
      PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${IMATH_OUTPUT_SUBDIR}
    )

    if(BUILD_SHARED_LIBS
       AND (NOT "${IMATH_LIB_SUFFIX}" STREQUAL "")
       AND IMATH_INSTALL_SYM_LINK
       AND NOT IMATH_BUILD_APPLE_FRAMEWORKS)

      # create symlink from libImath.so to libImath-3_2.so

      string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
      set(postfix ${CMAKE_${uppercase_CMAKE_BUILD_TYPE}_POSTFIX})
      set(VERSIONED_LIB_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${IMATH_LIBRARY}${IMATH_LIB_SUFFIX}${postfix}${CMAKE_SHARED_LIBRARY_SUFFIX})
      set(BASE_LIB_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${IMATH_LIBRARY}${postfix}${CMAKE_SHARED_LIBRARY_SUFFIX})

      file(CREATE_LINK ${VERSIONED_LIB_FILENAME} ${CMAKE_CURRENT_BINARY_DIR}/${BASE_LIB_FILENAME} SYMBOLIC)
      if(WIN32)
        set(INSTALL_DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})
      else()
        set(INSTALL_DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})
      endif()
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${BASE_LIB_FILENAME} DESTINATION ${INSTALL_DESTINATION})
      install(CODE "message(STATUS \"Creating symlink ${INSTALL_DESTINATION}/${BASE_LIB_FILENAME} -> ${VERSIONED_LIB_FILENAME}\")")

    endif()

endif()


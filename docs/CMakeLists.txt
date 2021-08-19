# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

find_package(Doxygen REQUIRED)
find_package(Sphinx REQUIRED)

get_target_property(IMATH_PUBLIC_HEADER_DIR Imath INTERFACE_INCLUDE_DIRECTORIES)
file(GLOB_RECURSE IMATH_PUBLIC_HEADERS ${IMATH_PUBLIC_HEADER_DIR}/*.h)

set(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR}/src/Imath)
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs/doxygen)
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)
set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

set(CONF_PY_IN  ${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in)
set(CONF_PY_OUT ${CMAKE_CURRENT_BINARY_DIR}/conf.py)

set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/sphinx)
set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)

configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)
configure_file(${CONF_PY_IN} ${CONF_PY_OUT} @ONLY)

file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
                   DEPENDS ${IMATH_PUBLIC_HEADERS}
                   COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
                   MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
                   COMMENT "Generating docs"
                   VERBATIM)

add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})

file(GLOB_RECURSE IMATH_DOC_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/*.rst)

add_custom_command(OUTPUT ${SPHINX_INDEX_FILE}
                   COMMAND 
                     ${SPHINX_EXECUTABLE} -b html
                     # Tell Breathe where to find the Doxygen output
                     -Dbreathe_projects.Imath=${DOXYGEN_OUTPUT_DIR}/xml
                     -c ${CMAKE_CURRENT_BINARY_DIR}
                   ${SPHINX_SOURCE} ${SPHINX_BUILD}
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   DEPENDS
                   ${IMATH_DOC_SOURCE}
                   ${DOXYGEN_INDEX_FILE}
                   MAIN_DEPENDENCY ${CONF_PY_OUT}
                   COMMENT "Generating documentation with Sphinx")

add_custom_target(Sphinx ALL DEPENDS ${SPHINX_INDEX_FILE})

# Add an install target to install the docs
include(GNUInstallDirs)
install(DIRECTORY ${SPHINX_BUILD}
DESTINATION ${CMAKE_INSTALL_DOCDIR})

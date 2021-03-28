# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.


# From https://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
#
# - If the library source code has changed at all since the last
#   update, then increment revision (‘c.r.a’ becomes ‘c.r+1.a’).
#
# - If any interfaces have been added, removed, or changed since the
#   last update, increment current, and set revision to 0.
#
# - If any interfaces have been added since the last public release,
#   then increment age.
#
# - If any interfaces have been removed or changed since the last
#   public release, then set age to 0.

set(IMATH_VERSION_MAJOR 3)
set(IMATH_VERSION_MINOR 0)
set(IMATH_VERSION_PATCH 1)
set(IMATH_VERSION       ${IMATH_VERSION_MAJOR}.${IMATH_VERSION_MINOR}.${IMATH_VERSION_PATCH})
set(IMATH_VERSION_API   ${IMATH_VERSION_MAJOR}_${IMATH_VERSION_MINOR})
set(IMATH_SOCURRENT 27)
set(IMATH_SOREVISION 0)
set(IMATH_SOAGE 0)
math(EXPR IMATH_SOVERSION "${IMATH_SOCURRENT} - ${IMATH_SOAGE}")
set(IMATH_LIB_VERSION "${IMATH_SOVERSION}.${IMATH_SOAGE}.${IMATH_SOREVISION}")
message(STATUS "Configure Imath Version: ${IMATH_VERSION} Lib API: ${IMATH_LIB_VERSION}")

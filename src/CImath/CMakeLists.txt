# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

add_library(CImath SHARED
    chalf.h
    chalf.cpp
)

target_link_libraries(CImath PUBLIC Imath)

set(public_headers
    chalf.h
)

install(TARGETS CImath)
install(FILES ${public_headers}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/CImath
    COMPONENT developer
)

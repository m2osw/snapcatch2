# Copyright (c) 2006-2022  Made to Order Software Corp.  All Rights Reserved.
#
# https://snapwebsites.org/project/snapcatch2
# contact@m2osw.com
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

cmake_minimum_required(VERSION 3.10.2)

project(snapcatch2)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_package(SnapCMakeModules REQUIRED)

SnapGetVersion(SNAPCATCH2 ${CMAKE_CURRENT_SOURCE_DIR})

set(SNAPCATCH2_VERSION "${SNAPCATCH2_VERSION_MAJOR}.${SNAPCATCH2_VERSION_MINOR}.${SNAPCATCH2_VERSION_PATCH}")
set(OUTPUT_PATH ${CMAKE_BINARY_DIR}/Catch2-${SNAPCATCH2_VERSION})


add_custom_command(
    OUTPUT ${OUTPUT_PATH}/out/include/catch2/catch_all.hpp
    COMMAND ${CMAKE_SOURCE_DIR}/cmake/build-catch2.sh ${OUTPUT_PATH} ${SNAPCATCH2_VERSION}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target( run ALL
    DEPENDS ${OUTPUT_PATH}/out/include/catch2/catch_all.hpp
    COMMENT "Generating API documentation with Doxygen" VERBATIM
)

install(
    DIRECTORY
        ${OUTPUT_PATH}/out/include/

    DESTINATION
        include
)

install(
    DIRECTORY
        ${OUTPUT_PATH}/out/lib

    DESTINATION
        .

    OPTIONAL
)

install(
    DIRECTORY
        ${OUTPUT_PATH}/out/lib64

    DESTINATION
        .

    OPTIONAL
)

install(
    DIRECTORY
        ${OUTPUT_PATH}/out/share/

    DESTINATION
        share
)

install(
    FILES
        snapcatch2.hpp

    DESTINATION
        include/catch2
)

add_subdirectory(cmake)

# vim: ts=4 sw=4 et nocindent

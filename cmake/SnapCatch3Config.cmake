# - Try to find SnapCatch3
#
# Once done this will define
#
# SNAPCATCH3_FOUND        - System has SnapCatch3
# SNAPCATCH3_INCLUDE_DIRS - The SnapCatch3 include directories
#
# License:
#
# Copyright (c) 2013-2022  Made to Order Software Corp.  All Rights Reserved
#
# https://snapwebsites.org/project/snapcatch3
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

find_path(
    SNAPCATCH3_INCLUDE_DIR
        catch3/snapcatch3.hpp

    PATHS
        ENV SNAPCATCH3_INCLUDE_DIR
)

mark_as_advanced(
    SNAPCATCH3_INCLUDE_DIR
)

set(SNAPCATCH3_INCLUDE_DIRS ${SNAPCATCH3_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)

# handle the QUIETLY and REQUIRED arguments and set SnapCatch3_FOUND to
# TRUE if all listed variables are TRUE
find_package_handle_standard_args(
    SnapCatch3
    DEFAULT_MSG
    SNAPCATCH3_INCLUDE_DIR
)

# vim: ts=4 sw=4 et

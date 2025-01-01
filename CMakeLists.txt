# ------------------------------------------------------------------------------
# Lingwi - CMakeLists.txt
# ------------------------------------------------------------------------------
# Author:
#	https://github.com/itsYakub
# ------------------------------------------------------------------------------
# LICENCE (MIT):
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
# OR OTHER DEALINGS IN THE SOFTWARE.
# ------------------------------------------------------------------------------

# --------------------
# Section: CMake setup
# --------------------

cmake_minimum_required(VERSION 3.11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

# ----------------------
# Section: Fetch Content
# ----------------------

include(FetchContent)

# --------------------
# Sub-Section: libcurl
# --------------------

FetchContent_Declare(
    libcurl
    GIT_REPOSITORY https://github.com/curl/curl
)

FetchContent_MakeAvailable(libcurl)

# ----------------------
# Section: Project setup
# ----------------------

project(
	lingwi
	VERSION 1.2
	DESCRIPTION "Translation command-line utility for linux"
	HOMEPAGE_URL "https://github.com/itsYakub/lingw" 
	LANGUAGES C
)

# --------------------
# Sub-Section: Sources
# --------------------

set(
	SOURCES
	${CMAKE_SOURCE_DIR}/main.c
	# ...
)

# ---------------------
# Sub-Section: Includes
# ---------------------

set(
	INCLUDE_DIRECTORIES
	# ...
)

# ----------------------
# Sub-Section: Libraries
# ----------------------

set(
	LINK_LIBRARIES
    libcurl
	# ...
)

# ---------------------
# Section: Target setup
# ---------------------

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRECTORIES})
target_link_libraries(${PROJECT_NAME} ${LINK_LIBRARIES})

# ----------------------------------
# Section: Compiler & Linker options
# ----------------------------------

if (LINUX)

    # LINUX: Telling the linker to statically link the libgcc and libstdc++ to out project.
    # (Source: https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html)
    target_link_options(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)

endif(LINUX)

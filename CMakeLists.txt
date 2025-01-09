#
#	Rtc-REST, A bot library for Discord, written in C++, and featuring explicit multithreading through the usage of custom, asynchronous C++ CoRoutines.
#
#	Copyright 2021, 2022, 2023 Chris M. (RealTimeChris)
#
#	This library is free software; you can redistribute it and/or
#	modify it under the terms of the GNU Lesser General Public
#	License as published by the Free Software Foundation; either
#	version 2.1 of the License, or (at your option) any later version.
#
#	This library is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#	Lesser General Public License for more details.
#
#	You should have received a copy of the GNU Lesser General Public
#	License along with this library; if not, write to the Free Software
#	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
#	USA
#
# CMakeLists.txt - The CMake script for building this library.
# May 13, 2021
# https://rtc_rest.com
if (EXISTS "${_VCPKG_ROOT_DIR}")
	set(VCPKG_ROOT_DIR "${_VCPKG_ROOT_DIR}")
	set(ENV{VCPKG_INSTALLATION_ROOT} "${_VCPKG_ROOT_DIR}")
else()
	set(VCPKG_ROOT_DIR "C:/Vcpkg")
	set(ENV{VCPKG_INSTALLATION_ROOT} "${VCPKG_ROOT_DIR}")
endif() 

cmake_minimum_required(VERSION 3.20)
if (WIN32)
	if (NOT BUILD_SHARED_LIBS)
		set(OS "windows-static")
	else()
		set(OS "windows")
	endif()
elseif(APPLE)
	if (NOT BUILD_SHARED_LIBS)
		set(OS "osx-static")
	else()
		set(OS "osx")
	endif()
elseif(UNIX)
	if (NOT BUILD_SHARED_LIBS)
		set(OS "linux-static")
	else()
		set(OS "linux")
	endif()
endif()
if (EXISTS "$ENV{VCPKG_INSTALLATION_ROOT}")
	set(VCPKG_INSTALLATION_ROOT_NEW "$ENV{VCPKG_INSTALLATION_ROOT}")
	if (NOT EXISTS "${OPENSSL_ROOT_DIR}")
		set(OPENSSL_ROOT_DIR "${VCPKG_INSTALLATION_ROOT_NEW}/installed/x64-${OS}/")
	endif()
	include("${VCPKG_INSTALLATION_ROOT_NEW}/scripts/buildsystems/vcpkg.cmake")
else()
	message(WARNING "No Vcpkg root folder found, Please make sure that you properly set the library folders.")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/CMake;")

#	MIT License
#
#	Copyright (c) 2024 RealTimeChris
#
#	Permission is hereby granted, free of charge, to any person obtaining a copy of this 
#	software and associated documentation files (the "Software"), to deal in the Software 
#	without restriction, including without limitation the rights to use, copy, modify, merge, 
#	publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
#	persons to whom the Software is furnished to do so, subject to the following conditions:
#
#	The above copyright notice and this permission notice shall be included in all copies or 
#	substantial portions of the Software.
#
#	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
#	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
#	PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
#	FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
#	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
#	DEALINGS IN THE SOFTWARE.
#
# CMakeLists.txt - The CMake script for building this library.
# Dec 17, 2022
# https://github.com/RealTimeChris/rtc_rest

cmake_minimum_required(VERSION 3.18)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/CMake;")

set(PROJECT_NAME "Rtc-REST")

project(
	"${PROJECT_NAME}"
	VERSION "1.0.0"
	LANGUAGES CXX
	HOMEPAGE_URL "https://rtc_rest.com"
	DESCRIPTION "${DESCRIPTION}"
)

include (FetchContent)

FetchContent_Declare(
  jsonifier
  GIT_REPOSITORY https://github.com/realtimechris/jsonifier.git
  GIT_TAG dev
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(jsonifier)

add_library("${PROJECT_NAME}" INTERFACE)

add_library("${PROJECT_NAME}::${PROJECT_NAME}" ALIAS "${PROJECT_NAME}")

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Include/rtc_rest/*.hpp")

find_package(OpenSSL REQUIRED)

target_link_libraries(
	"${PROJECT_NAME}" INTERFACE 
	"$<$<TARGET_EXISTS:OpenSSL::Crypto>:OpenSSL::Crypto>"
	"$<$<TARGET_EXISTS:OpenSSL::SSL>:OpenSSL::SSL>"
)

set(rtc_rest-POPCNT_EXITCODE 0 CACHE INTERNAL "For cross-compiling.")
set(rtc_rest-LZCNT_EXITCODE 0 CACHE INTERNAL "For cross-compiling.")
set(rtc_rest-BMI_EXITCODE 0 CACHE INTERNAL "For cross-compiling.")
set(rtc_rest-BMI2_EXITCODE 0 CACHE INTERNAL "For cross-compiling.")
set(rtc_rest-AVX_EXITCODE 0 CACHE INTERNAL "For cross-compiling.")
set(rtc_rest-AVX2_EXITCODE 0 CACHE INTERNAL "For cross-compiling.")
set(rtc_rest-AVX512_EXITCODE 0 CACHE INTERNAL "For cross-compiling.")

set_target_properties(
     "${PROJECT_NAME}" PROPERTIES
     OUTPUT_NAME "rtc_rest"
     CXX_STANDARD_REQUIRED ON
     CXX_EXTENSIONS OFF
 )
 
 target_include_directories(
     "${PROJECT_NAME}" INTERFACE
     "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include/>"
    "$<INSTALL_INTERFACE:include/>"
 )

target_compile_features("${PROJECT_NAME}" INTERFACE "cxx_std_23")
 
target_compile_options(
	"${PROJECT_NAME}" INTERFACE
	"$<$<AND:$<STREQUAL:$<UPPER_CASE:${UBSAN}>,TRUE>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-fsanitize=undefined>"
	"$<$<CXX_COMPILER_ID:MSVC>:$<$<STREQUAL:${ASAN_ENABLED},TRUE>:/fsanitize=address>>"
	"$<$<CXX_COMPILER_ID:MSVC>:/Zc:preprocessor>"
	"$<$<CXX_COMPILER_ID:MSVC>:/permissive>"
	"$<$<CXX_COMPILER_ID:MSVC>:/Zc:lambda>"
	"$<$<CXX_COMPILER_ID:MSVC>:/bigobj>"
	"$<$<CXX_COMPILER_ID:MSVC>:/GL>"
	"$<$<CXX_COMPILER_ID:MSVC>:/Gy>"
	"${AVX_FLAG}"
)
 
target_link_options(
    "${PROJECT_NAME}" INTERFACE
    "$<$<AND:$<STREQUAL:$<UPPER_CASE:${UBSAN}>,TRUE>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-fsanitize=undefined>"
    "$<$<AND:$<CXX_COMPILER_ID:CLANG>,$<STREQUAL:${ASAN_ENABLED},TRUE>>:-fsanitize=address>"
    "$<$<AND:$<CXX_COMPILER_ID:GNU>,$<STREQUAL:${ASAN_ENABLED},TRUE>>:-fsanitize=address>"
    "$<$<CXX_COMPILER_ID:MSVC>:/INCREMENTAL:NO>"
    "$<$<CXX_COMPILER_ID:MSVC>:/OPT:ICF>"
    "$<$<CXX_COMPILER_ID:MSVC>:/OPT:REF>"
    #"$<$<CXX_COMPILER_ID:MSVC>:/LTCG>"
    "$<$<CXX_COMPILER_ID:CLANG>:-flto>"
    "$<$<CXX_COMPILER_ID:GNU>:-flto>"
    "$<$<CXX_COMPILER_ID:CLANG>:-Wl,-O2>"
    "$<$<CXX_COMPILER_ID:GNU>:-Wl,-O2>"
)

target_compile_definitions(
	"${PROJECT_NAME}" INTERFACE 
	"$<$<STREQUAL:${ASAN_ENABLED},TRUE>:ASAN_ENABLED>"
)

set(CONFIG_FILE_NAME "${PROJECT_NAME}Config.cmake")
set(EXPORTED_TARGETS_NAME "${PROJECT_NAME}Targets")
set(EXPORTED_TARGETS_FILE_NAME "${EXPORTED_TARGETS_NAME}.cmake")
set(EXPORTED_TARGETS_FILE_PATH "share/rtc_rest/${EXPORTED_TARGETS_FILE_NAME}")

include(CMakePackageConfigHelpers)
configure_package_config_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/CMake/${CONFIG_FILE_NAME}.in"
	"${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_FILE_NAME}"
	INSTALL_DESTINATION "share/rtc_rest"
	PATH_VARS
	EXPORTED_TARGETS_FILE_PATH
)

set(VERSION_FILE_NAME "${PROJECT_NAME}ConfigVersion.cmake")

write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/${VERSION_FILE_NAME}"
	VERSION "${PRODUCT_VERSION}"
	COMPATIBILITY AnyNewerVersion
)

install(
	FILES
	"${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_FILE_NAME}"
	"${CMAKE_CURRENT_BINARY_DIR}/${VERSION_FILE_NAME}"
	DESTINATION "share/rtc_rest"
)

install(
	DIRECTORY
	"${CMAKE_CURRENT_SOURCE_DIR}/Include/"
	DESTINATION "include"
)

install(
	TARGETS "${PROJECT_NAME}"
	EXPORT "${EXPORTED_TARGETS_NAME}"
)

install(
	EXPORT "${EXPORTED_TARGETS_NAME}"
	FILE "${EXPORTED_TARGETS_FILE_NAME}"
	NAMESPACE "${PROJECT_NAME}::"
	DESTINATION "share/rtc_rest"
)

if("${DEV}" STREQUAL "TRUE")
	add_subdirectory("./Tests")
endif()

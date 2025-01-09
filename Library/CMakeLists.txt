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

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/../Include/rtc_rest/*.hpp")

if (WIN32)
	add_library("${LIB_NAME}" INTERFACE "${SOURCES}" "${PRODUCT_INFO}")
elseif (APPLE)
	add_library("${LIB_NAME}" INTERFACE "${SOURCES}" "${PRODUCT_INFO}")
	find_package(Threads REQUIRED)
elseif (UNIX)
	add_library("${LIB_NAME}" INTERFACE "${SOURCES}" "${PRODUCT_INFO}")
	find_package(Threads REQUIRED)
endif()

add_library("${PROJECT_NAME}::${LIB_NAME}" ALIAS "${LIB_NAME}")

set_target_properties(
	"${LIB_NAME}" PROPERTIES 
	OUTPUT_NAME "rtc_rest"
)

if(DEV)
	include(FetchContent)
	FetchContent_Declare(
	   Jsonifier
	   GIT_REPOSITORY https://github.com/RealTimeChris/Jsonifier.git
	   GIT_TAG dev
	)
	FetchContent_MakeAvailable(Jsonifier)
else()
	find_package(Jsonifier CONFIG REQUIRED)
endif()

find_package(OpenSSL REQUIRED)

target_link_libraries(
	"${LIB_NAME}" INTERFACE 
	"$<$<TARGET_EXISTS:OpenSSL::Crypto>:OpenSSL::Crypto>"
	"$<$<TARGET_EXISTS:OpenSSL::SSL>:OpenSSL::SSL>"
)

set(RTC_REST_CPU_INSTRUCTIONS
    "$<IF:$<BOOL:${RTC_REST_CPU_FLAGS}>,${RTC_REST_CPU_FLAGS},$<IF:$<CXX_COMPILER_ID:MSVC>,/arch:AVX512,-march=native>>"
)

target_include_directories(
	"${LIB_NAME}" INTERFACE 
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../Include>"
	"$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
)

target_link_libraries(
	"${LIB_NAME}" INTERFACE 
	"$<$<TARGET_EXISTS:OpenSSL::Crypto>:OpenSSL::Crypto>"
	"$<$<TARGET_EXISTS:OpenSSL::SSL>:OpenSSL::SSL>"
)

target_compile_options(
	"${LIB_NAME}" INTERFACE 
	"$<IF:$<BOOL:${RTC_REST_CPU_INSTRUCTIONS}>,${RTC_REST_CPU_INSTRUCTIONS},$<IF:$<CXX_COMPILER_ID:MSVC>,/arch:AVX512,-march=native>>"
	"$<$<CXX_COMPILER_ID:MSVC>:$<$<STREQUAL:${ASAN_ENABLED},TRUE>:/fsanitize=address>>"
	"$<$<CXX_COMPILER_ID:CLANG>:-fcoroutines>"
	"$<$<CXX_COMPILER_ID:GNU>:-fcoroutines>"
	"$<$<CXX_COMPILER_ID:CLANG>:-Wextra>"
	"$<$<CXX_COMPILER_ID:MSVC>:/bigobj>"
	"$<$<CXX_COMPILER_ID:CLANG>:-Wall>"
	"$<$<CXX_COMPILER_ID:GNU>:-Wextra>"
	"$<$<CXX_COMPILER_ID:MSVC>:/Wall>"
	"$<$<CXX_COMPILER_ID:MSVC>:/EHsc>"
	"$<$<CXX_COMPILER_ID:GNU>:-Wall>"
	"$<$<CXX_COMPILER_ID:MSVC>:/MP1>"
	"$<$<CXX_COMPILER_ID:MSVC>:/Zi>"
	"${RTC_REST_CPU_FLAGS}"
)

target_link_options(
	"${LIB_NAME}" INTERFACE 
	"$<$<CXX_COMPILER_ID:GNU>:$<$<STREQUAL:${ASAN_ENABLED},TRUE>:-fsanitize=address>>"
	"$<$<CXX_COMPILER_ID:CLANG>:$<$<STREQUAL:${ASAN_ENABLED},TRUE>:-fsanitize=address>>"
	"$<$<CXX_COMPILER_ID:MSVC>:/OPT:REF>"
)

set(CONFIG_FILE_NAME "${PROJECT_NAME}Config.cmake")
set(EXPORTED_TARGETS_NAME "${PROJECT_NAME}Targets")
set(EXPORTED_TARGETS_FILE_NAME "${EXPORTED_TARGETS_NAME}.cmake")
set(EXPORTED_TARGETS_FILE_PATH "share/rtc_rest/${EXPORTED_TARGETS_FILE_NAME}")
set(VERSION_FILE_NAME "${PROJECT_NAME}ConfigVersion.cmake")
set(RELEASE_PDB_FILE_PATH "bin/rtc_rest.pdb")
set(DEBUG_PDB_FILE_PATH "debug/bin/rtc_rest.pdb")

include(CMakePackageConfigHelpers)
configure_package_config_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/../CMake/${CONFIG_FILE_NAME}.in"
	"${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_FILE_NAME}"
	INSTALL_DESTINATION "share/rtc_rest"
	PATH_VARS
	EXPORTED_TARGETS_FILE_PATH
	RELEASE_PDB_FILE_PATH
	DEBUG_PDB_FILE_PATH
)

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
	"${CMAKE_CURRENT_SOURCE_DIR}/../Include/"
	DESTINATION "include"
)

install(
	TARGETS "${LIB_NAME}"
	EXPORT "${EXPORTED_TARGETS_NAME}"
	RUNTIME DESTINATION "$<$<NOT:$<PLATFORM_ID:Linux>>:$<IF:$<CONFIG:Debug>,bin,bin>>"
	ARCHIVE DESTINATION "$<IF:$<CONFIG:Debug>,lib,lib>"
)

install(
	EXPORT "${EXPORTED_TARGETS_NAME}"
	FILE "${EXPORTED_TARGETS_FILE_NAME}"
	NAMESPACE "${PROJECT_NAME}::"
	DESTINATION "share/rtc_rest"
)

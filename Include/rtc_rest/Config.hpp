/*
	MIT License

	Rtc-REST, A bot library for Discord, written in C++, and featuring explicit multithreading through the usage of custom, asynchronous C++ CoRoutines.

	Copyright 2022, 2023 Chris M. (RealTimeChris)

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/
/// Config.hpp - Header for the some of the base definitions and includes.
/// Nov 8, 2021
/// https://rtc_rest.com
/// \file Base.hpp
#pragma once

#if !defined(__GNUC__)
	#pragma warning(disable : 4710)
	#pragma warning(disable : 4711)
	#pragma warning(disable : 4251)
	#pragma warning(disable : 4371)
	#pragma warning(disable : 4514)
	#pragma warning(disable : 4623)
	#pragma warning(disable : 4625)
	#pragma warning(disable : 4626)
	#pragma warning(disable : 4820)
	#pragma warning(disable : 5267)
	#pragma warning(disable : 5026)
	#pragma warning(disable : 5027)
	#pragma warning(disable : 5045)
	#pragma warning(disable : 5246)
#endif

#if defined(__clang__) || (defined(__GNUC__) && defined(__llvm__))
	#define RTC_REST_CLANG 1
#elif defined(_MSC_VER)
	#pragma warning(disable : 4820)
	#pragma warning(disable : 4371)
	#pragma warning(disable : 4324)
	#define RTC_REST_MSVC 1
#elif defined(__GNUC__) && !defined(__clang__)
	#define RTC_REST_GNUCXX 1
#endif

#if defined(RTC_REST_MSVC)
	#define RTC_REST_VISUAL_STUDIO 1
	#if defined(RTC_REST_CLANG)
		#define RTC_REST_CLANG_VISUAL_STUDIO 1
	#else
		#define RTC_REST_REGULAR_VISUAL_STUDIO 1
	#endif
#endif

#define RTC_REST_GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)

#if defined(macintosh) || defined(Macintosh) || (defined(__APPLE__) && defined(__MACH__)) || defined(TARGET_OS_MAC)
	#define RTC_REST_MAC 1
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__gnu_linux__)
	#define RTC_REST_LINUX 1
#elif defined(WIN32) || defined(_WIN32) || defined(_WIN64)
	#define RTC_REST_WIN 1
#else
	#error "Undetected platform."
#endif

#if defined(__cpp_inline_variables) && __cpp_inline_variables >= 201606L
	#define RTC_REST_HAS_INLINE_VARIABLE 1
#elif __cplusplus >= 201703L
	#define RTC_REST_HAS_INLINE_VARIABLE 1
#elif defined(RTC_REST_MSVC) && RTC_REST_MSVC >= 1912 && _MSVC_LANG >= 201703L
	#define RTC_REST_HAS_INLINE_VARIABLE 1
#else
	#define RTC_REST_HAS_INLINE_VARIABLE 0
#endif

#if RTC_REST_HAS_INLINE_VARIABLE
	#define RTC_REST_ALWAYS_INLINE_VARIABLE inline constexpr
#else
	#define RTC_REST_ALWAYS_INLINE_VARIABLE static constexpr
#endif

#if defined(NDEBUG)
	#if defined(RTC_REST_MSVC)
		#define RTC_REST_ALWAYS_INLINE [[msvc::forceinline]] inline
		#define RTC_REST_CLANG_ALWAYS_INLINE inline
		#define RTC_REST_INLINE inline
	#elif defined(RTC_REST_CLANG)
		#define RTC_REST_ALWAYS_INLINE inline __attribute__((always_inline))
		#define RTC_REST_CLANG_ALWAYS_INLINE inline __attribute__((always_inline))
		#define RTC_REST_INLINE inline
	#elif defined(RTC_REST_GNUCXX)
		#define RTC_REST_ALWAYS_INLINE inline __attribute__((always_inline))
		#define RTC_REST_CLANG_ALWAYS_INLINE inline
		#define RTC_REST_INLINE inline
	#endif
#else
	#define RTC_REST_ALWAYS_INLINE inline
	#define RTC_REST_CLANG_ALWAYS_INLINE inline
	#define RTC_REST_INLINE inline
#endif

#if defined _WIN32
	#if !defined WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#if !defined WINRT_LEAN_AND_MEAN
		#define WINRT_LEAN_AND_MEAN
	#endif
	#if !defined(NOMINMAX)
		#define NOMINMAX
	#endif
	#include <chrono>
	#include <WinSock2.h>
#else
	#if !defined 
		#define 
	#endif
	#include <arpa/inet.h>
	#include <sys/time.h>
	#include <pthread.h>
	#include <cstdint>
	#include <cstring>
	#include <time.h>
	#include <ctime>
RTC_REST_INLINE tm getTime(time_t time) {
	return *gmtime(&time);
}
#endif

#include <jsonifier/Index.hpp>

using milliseconds= std::chrono::milliseconds;
using seconds= std::chrono::seconds;
using nanoseconds = std::chrono::nanoseconds;

inline thread_local jsonifier::jsonifier_core<true> parser{};
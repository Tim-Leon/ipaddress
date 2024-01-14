#pragma once

#if defined(_WIN32)  
#include <vcruntime.h>
#endif

//COMPILE INFO
#if (_MSC_VER)
#define MSVC
#elif (__GNUC__)
#define GCC
#elif (__clang__)
#define CLANG
#elif (__EMSCRIPTEN__)
#error EMSCRIPTEN compiler not supported
#define EMSCRIPTEN
#elif (__MINGW32__)
#error MinGW compiler not supported
#elif(__MINGW32__)
#error MinGW compiler not supported
#elif(__MINGW64__)
#error MinGW compiler not supported
#endif

#ifdef _NODISCARD
#define NODISCARD [[nodiscard]]
#else
#define NODISCARD
#endif

#define NONINLINE noinline
//#define NONINLINE __noinline

#define INLINE inline
//#define INLINE __inline 


#define FORCE_INLINE __forceinline
/*
 * Return 0 or 1 depending on whether the bit is enabled
 *@param var variable
 *@param pos starts counting from 0
 */
#define CHECK_BIT(var,pos) (((var)>>(pos)) & 1)



/*https://stackoverflow.com/questions/2324658/how-to-determine-the-getVersion-of-the-c-standard-used-by-the-compiler
 * C++ pre-C++98: __cplusplus is 1.
 * C++98: __cplusplus is 199711L.
 * C++98 + TR1: This reads as C++98 and there is no way to check that I know of.
 * C++11: __cplusplus is 201103L.
 * C++14: __cplusplus is 201402L.
 * C++17: __cplusplus is 201703L.
 * C++20: Undefined but greater than 201703L
 */

//Before C++98
#if __cplusplus == 1 
#define CONSTEXPR const
#endif

//C++98
#if __cplusplus == 199711L
#define CONSTEXPR const
#endif

//C++11
#if __cplusplus == 201103L
#define CONSTEXPR constexpr
#endif

//C++14
#if __cplusplus == 201402L 
#define CONSTEXPR constexpr
#endif

//C++17
#if __cplusplus == 201703L 
#define CONSTEXPR constexpr
#endif

#ifdef _WIN32
#pragma comment(lib, "Ws2_32.lib")
#include <WS2tcpip.h>
#include <Windows.h>
#include <cstdint>
#elif __linux__
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <arpa/inet.h>
#include <netdb.h>
#endif

#include <cstring>

/*
	__linux__       Defined on Linux
	__sun           Defined on Solaris
	__FreeBSD__     Defined on FreeBSD
	__NetBSD__      Defined on NetBSD
	__OpenBSD__     Defined on OpenBSD
	__APPLE__       Defined on Mac OS X
	__hpux          Defined on HP-UX
	__osf__         Defined on Tru64 UNIX (formerly DEC OSF1)
	__sgi           Defined on Irix
	_AIX            Defined on AIX
	_WIN32          Defined on Windows
*/
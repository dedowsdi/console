#ifndef PACPLATFORM_H
#define PACPLATFORM_H 

#include "pacConfig.h"


namespace pac {
/* Initial platform/compiler-related stuff to set.
*/
#define PAC_PLATFORM_WIN32 1
#define PAC_PLATFORM_LINUX 2
#define PAC_PLATFORM_APPLE 3
#define PAC_PLATFORM_APPLE_IOS 4
#define PAC_PLATFORM_ANDROID 5
#define PAC_PLATFORM_NACL 6
#define PAC_PLATFORM_WINRT 7
#define PAC_PLATFORM_FLASHCC 8

#define PAC_COMPILER_MSVC 1
#define PAC_COMPILER_GNUC 2
#define PAC_COMPILER_BORL 3
#define PAC_COMPILER_WINSCW 4
#define PAC_COMPILER_GCCE 5
#define PAC_COMPILER_CLANG 6

#define PAC_ENDIAN_LITTLE 1
#define PAC_ENDIAN_BIG 2

#define PAC_ARCHITECTURE_32 1
#define PAC_ARCHITECTURE_64 2

/* Finds the compiler type and version.
*/
#if (defined( __WIN32__ ) || defined( _WIN32 )) && defined(__ANDROID__) // We are using NVTegra
#   define PAC_COMPILER PAC_COMPILER_GNUC
#   define PAC_COMP_VER 470
#elif defined( __GCCE__ )
#   define PAC_COMPILER PAC_COMPILER_GCCE
#   define PAC_COMP_VER _MSC_VER
//#	include <staticlibinit_gcce.h> // This is a GCCE toolchain workaround needed when compiling with GCCE 
#elif defined( __WINSCW__ )
#   define PAC_COMPILER PAC_COMPILER_WINSCW
#   define PAC_COMP_VER _MSC_VER
#elif defined( _MSC_VER )
#   define PAC_COMPILER PAC_COMPILER_MSVC
#   define PAC_COMP_VER _MSC_VER
#elif defined( __clang__ )
#   define PAC_COMPILER PAC_COMPILER_CLANG
#   define PAC_COMP_VER (((__clang_major__)*100) + \
        (__clang_minor__*10) + \
        __clang_patchlevel__)
#elif defined( __GNUC__ )
#   define PAC_COMPILER PAC_COMPILER_GNUC
#   define PAC_COMP_VER (((__GNUC__)*100) + \
        (__GNUC_MINOR__*10) + \
        __GNUC_PATCHLEVEL__)
#elif defined( __BORLANDC__ )
#   define PAC_COMPILER PAC_COMPILER_BORL
#   define PAC_COMP_VER __BCPLUSPLUS__
#   define __FUNCTION__ __FUNC__ 
#else
#   pragma error "No known compiler. Abort! Abort!"

#endif

/* define PAC_NORETURN macro */
#if PAC_COMPILER == PAC_COMPILER_MSVC
#	define PAC_NORETURN __declspec(noreturn)
#elif PAC_COMPILER == PAC_COMPILER_GNUC || PAC_COMPILER == PAC_COMPILER_CLANG
#	define PAC_NORETURN __attribute__((noreturn))
#else
#	define PAC_NORETURN
#endif

/* Finds the current platform */
#if (defined( __WIN32__ ) || defined( _WIN32 )) && !defined(__ANDROID__)
#	if defined(WINAPI_FAMILY)
#		define __PAC_HAVE_DIRECTXMATH 1
#		include <winapifamily.h>
#		if WINAPI_FAMILY == WINAPI_FAMILY_APP|| WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
#			define DESKTOP_APP 1
#			define PHONE 2
#			define PAC_PLATFORM PAC_PLATFORM_WINRT
#           ifndef _CRT_SECURE_NO_WARNINGS
#               define _CRT_SECURE_NO_WARNINGS
#           endif
#           ifndef _SCL_SECURE_NO_WARNINGS
#               define _SCL_SECURE_NO_WARNINGS
#           endif
#			if WINAPI_FAMILY == WINAPI_FAMILY_APP
#				define PAC_WINRT_TARGET_TYPE DESKTOP_APP
#			endif
#			if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
#				define PAC_WINRT_TARGET_TYPE PHONE
#			endif
#		else
#			define PAC_PLATFORM PAC_PLATFORM_WIN32
#		endif
#	else
#		define PAC_PLATFORM PAC_PLATFORM_WIN32
#	endif
#elif defined(__FLASHCC__)
#	define PAC_PLATFORM PAC_PLATFORM_FLASHCC
#elif defined( __APPLE_CC__)
    // Device                                                     Simulator
    // Both requiring OS version 6.0 or greater
#   if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 60000 || __IPHONE_OS_VERSION_MIN_REQUIRED >= 60000
#       define PAC_PLATFORM PAC_PLATFORM_APPLE_IOS
#   else
#       define PAC_PLATFORM PAC_PLATFORM_APPLE
#   endif
#elif defined(__ANDROID__)
#	define PAC_PLATFORM PAC_PLATFORM_ANDROID
#elif defined( __native_client__ ) 
#   define PAC_PLATFORM PAC_PLATFORM_NACL
#   ifndef PAC_STATIC_LIB
#       error PAC must be built as static for NaCl (PAC_STATIC=true in CMake)
#   endif
#   ifdef PAC_BUILD_RENDERSYSTEM_D3D9
#       error D3D9 is not supported on NaCl (PAC_BUILD_RENDERSYSTEM_D3D9 false in CMake)
#   endif
#   ifdef PAC_BUILD_RENDERSYSTEM_GL
#       error OpenGL is not supported on NaCl (PAC_BUILD_RENDERSYSTEM_GL=false in CMake)
#   endif
#   ifndef PAC_BUILD_RENDERSYSTEM_GLES2
#       error GLES2 render system is required for NaCl (PAC_BUILD_RENDERSYSTEM_GLES2=false in CMake)
#   endif
#else
#   define PAC_PLATFORM PAC_PLATFORM_LINUX
#endif

    /* Find the arch type */
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__) || defined(__arm64__) || defined(__aarch64__) || defined(__mips64) || defined(__mips64_)
#   define PAC_ARCH_TYPE PAC_ARCHITECTURE_64
#else
#   define PAC_ARCH_TYPE PAC_ARCHITECTURE_32
#endif

// For generating compiler warnings - should work on any compiler
// As a side note, if you start your message with 'Warning: ', the MSVC
// IDE actually does catch a warning :)
#define PAC_QUOTE_INPLACE(x) # x
#define PAC_QUOTE(x) PAC_QUOTE_INPLACE(x)
#define PAC_WARN( x )  message( __FILE__ "(" QUOTE( __LINE__ ) ") : " x "\n" )

// For marking functions as deprecated
#if PAC_COMPILER == PAC_COMPILER_MSVC
#   define PAC_DEPRECATED __declspec(deprecated)
#elif PAC_COMPILER == PAC_COMPILER_GNUC || PAC_COMPILER == PAC_COMPILER_CLANG
#   define PAC_DEPRECATED __attribute__ ((deprecated))
#else
#   pragma message("WARNING: You need to implement PAC_DEPRECATED for this compiler")
#   define PAC_DEPRECATED
#endif

//----------------------------------------------------------------------------
// Windows Settings
#if PAC_PLATFORM == PAC_PLATFORM_WIN32 || PAC_PLATFORM == PAC_PLATFORM_WINRT

// If we're not including this from a client build, specify that the stuff
// should get exported. Otherwise, import it.
#	if defined( PAC_STATIC_LIB )
		// Linux compilers don't have symbol import/export directives.
#   	define _PacExport
#   	define _PacPrivate
#   else
#   	if defined( PAC_NONCLIENT_BUILD )
#       	define _PacExport __declspec( dllexport )
#   	else
#           if defined( __MINGW32__ )
#               define _PacExport
#           else
#       	    define _PacExport __declspec( dllimport )
#           endif
#   	endif
#   	define _PacPrivate
#	endif
// Win32 compilers use _DEBUG for specifying debug builds.
// for MinGW, we set DEBUG
#   if defined(_DEBUG) || defined(DEBUG)
#       define PAC_DEBUG_MODE 1
#   else
#       define PAC_DEBUG_MODE 0
#   endif

// Disable unicode support on MingW for GCC 3, poorly supported in stdlibc++
// STLPORT fixes this though so allow if found
// MinGW C++ Toolkit supports unicode and sets the define __MINGW32_TOOLBOX_UNICODE__ in _mingw.h
// GCC 4 is also fine
#if defined(__MINGW32__)
# if PAC_COMP_VER < 400
#  if !defined(_STLPORT_VERSION)
#   include<_mingw.h>
#   if defined(__MINGW32_TOOLBOX_UNICODE__) || PAC_COMP_VER > 345
#    define PAC_UNICODE_SUPPORT 1
#   else
#    define PAC_UNICODE_SUPPORT 0
#   endif
#  else
#   define PAC_UNICODE_SUPPORT 1
#  endif
# else
#  define PAC_UNICODE_SUPPORT 1
# endif
#else
#  define PAC_UNICODE_SUPPORT 1
#endif

#endif // PAC_PLATFORM == PAC_PLATFORM_WIN32 || PAC_PLATFORM == PAC_PLATFORM_WINRT

//----------------------------------------------------------------------------
// Linux/Apple/iOS/Android/NaCl Settings
#if PAC_PLATFORM == PAC_PLATFORM_LINUX || PAC_PLATFORM == PAC_PLATFORM_APPLE || PAC_PLATFORM == PAC_PLATFORM_APPLE_IOS || \
    PAC_PLATFORM == PAC_PLATFORM_ANDROID || PAC_PLATFORM == PAC_PLATFORM_NACL || PAC_PLATFORM == PAC_PLATFORM_FLASHCC

// Enable GCC symbol visibility
#   if defined( PAC_GCC_VISIBILITY )
#       define _PacExport  __attribute__ ((visibility("default")))
#       define _PacPrivate __attribute__ ((visibility("hidden")))
#   else
#       define _PacExport
#       define _PacPrivate
#   endif

// A quick define to overcome different names for the same function
#   define stricmp strcasecmp

#   ifdef DEBUG
#       define PAC_DEBUG_MODE 1
#   else
#       define PAC_DEBUG_MODE 0
#   endif

// Always enable unicode support for the moment
// Perhaps disable in old versions of gcc if necessary
#define PAC_UNICODE_SUPPORT 1

#endif

//----------------------------------------------------------------------------
// Android Settings
#if PAC_PLATFORM == PAC_PLATFORM_ANDROID
#   ifdef PAC_UNICODE_SUPPORT
#       undef PAC_UNICODE_SUPPORT
#   endif
#	define PAC_UNICODE_SUPPORT 1
    // A quick define to overcome different names for the same function
#   define stricmp strcasecmp
#   ifdef DEBUG
#       define PAC_DEBUG_MODE 1
#   else
#       define PAC_DEBUG_MODE 0
#   endif
#   ifndef CLOCKS_PER_SEC
#	    define CLOCKS_PER_SEC  1000
#   endif
#endif
    
//----------------------------------------------------------------------------
// FlashCC Settings
#if PAC_PLATFORM == PAC_PLATFORM_FLASHCC
#   ifdef PAC_UNICODE_SUPPORT
#       undef PAC_UNICODE_SUPPORT
#   endif
#	define PAC_UNICODE_SUPPORT 0
#   ifdef DEBUG
#       define PAC_DEBUG_MODE 1
#   else
#       define PAC_DEBUG_MODE 0
#   endif
#endif

#ifndef __PAC_HAVE_DIRECTXMATH
#   define __PAC_HAVE_DIRECTXMATH 0
#endif

//----------------------------------------------------------------------------
// Endian Settings
// check for BIG_ENDIAN config flag, set PAC_ENDIAN correctly
#ifdef PAC_CONFIG_BIG_ENDIAN
#    define PAC_ENDIAN PAC_ENDIAN_BIG
#else
#    define PAC_ENDIAN PAC_ENDIAN_LITTLE
#endif

//----------------------------------------------------------------------------
// Set the default locale for strings
#if PAC_PLATFORM == PAC_PLATFORM_ANDROID
//	Locales are not supported by the C lib you have to go through JNI.
#	define PAC_DEFAULT_LOCALE ""
#else
#   define PAC_DEFAULT_LOCALE "C"
#endif

//----------------------------------------------------------------------------
// Library suffixes
// "_d" for debug builds, nothing otherwise
#if PAC_DEBUG_MODE
#   define PAC_BUILD_SUFFIX "_d"
#else
#   define PAC_BUILD_SUFFIX ""
#endif

// Integer formats of fixed bit width
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef int int32;
typedef short int16;
typedef signed char int8;
// define uint64 type
#if PAC_COMPILER == PAC_COMPILER_MSVC
	typedef unsigned __int64 uint64;
	typedef __int64 int64;
#else
	typedef unsigned long long uint64;
	typedef long long int64;
#endif

// Disable these warnings (too much noise)
#if PAC_COMPILER == PAC_COMPILER_MSVC
#ifndef _CRT_SECURE_NO_WARNINGS
#   define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef _SCL_SECURE_NO_WARNINGS
#   define _SCL_SECURE_NO_WARNINGS
#endif
// Turn off warnings generated by long std templates
// This warns about truncation to 255 characters in debug/browse info
#   pragma warning (disable : 4786)
// Turn off warnings generated by long std templates
// This warns about truncation to 255 characters in debug/browse info
#   pragma warning (disable : 4503)
// disable: "<type> needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
#   pragma warning (disable : 4251)
// disable: "non dll-interface class used as base for dll-interface class"
// Happens when deriving from Singleton because bug in compiler ignores
// template export
#   pragma warning (disable : 4275)
// disable: "C++ Exception Specification ignored"
// This is because MSVC 6 did not implement all the C++ exception
// specifications in the ANSI C++ draft.
#   pragma warning( disable : 4290 )
// disable: "no suitable definition provided for explicit template
// instantiation request" Occurs in VC7 for no justifiable reason on all
// #includes of Singleton
#   pragma warning( disable: 4661)
// disable: deprecation warnings when using CRT calls in VC8
// These show up on all C runtime lib code in VC8, disable since they clutter
// the warnings with things we may not be able to do anything about (e.g.
// generated code from nvparse etc). I doubt very much that these calls
// will ever be actually removed from VC anyway, it would break too much code.
#   pragma warning( disable: 4996)
// disable: "conditional expression constant", always occurs on 
// PAC_MUTEX_CONDITIONAL when no threading enabled
#   pragma warning (disable : 201)
// disable: "unreferenced formal parameter"
// Many versions of VC have bugs which generate this error in cases where they shouldn't
#   pragma warning (disable : 4100)
// disable: "behavior change: an object of POD type constructed with an initializer of the form () will be default-initialized"
// We have this issue in PacMemorySTLAlloc.h - so we see it over and over
#   pragma warning (disable : 4345)
#endif

}

#endif /* PACPLATFORM_H */

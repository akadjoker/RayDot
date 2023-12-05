

#pragma once

#if defined(_WIN32)


#define SYSTEM_WINDOWS
#ifndef NOMINMAX
#define NOMINMAX
#endif



#elif defined(__unix__)


#if defined(__ANDROID__)

#define SYSTEM_ANDROID

#elif defined(__linux__)

#define SYSTEM_LINUX

#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)

#define SYSTEM_FREEBSD

#elif defined(__OpenBSD__)

#define SYSTEM_OPENBSD

#elif defined(__NetBSD__)

#define SYSTEM_NETBSD

#else


#error This UNIX operating system is not supported 

#endif

#else


#error This operating system is not supported 

#endif




#if !defined(STATIC)

#if defined(SYSTEM_WINDOWS)


#define API_EXPORT __declspec(dllexport)
#define API_IMPORT __declspec(dllimport)


#ifdef _MSC_VER

#pragma warning(disable : 4251)

#endif

#else // Linux, FreeBSD, macOS

#define API_EXPORT __attribute__((__visibility__("default")))
#define API_IMPORT __attribute__((__visibility__("default")))

#endif

#else

// Static build doesn't need import/export macros
#define API_EXPORT
#define API_IMPORT

#endif
// Module:  Log4CPLUS
// File:    config-win32.h
// Created: 4/2003
// Author:  Tad E. Smith
//
//
// Copyright 2003-2009 Tad E. Smith
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/** @file */

#ifndef DCMTK_LOG4CPLUS_CONFIG_WIN32_HEADER_
#define DCMTK_LOG4CPLUS_CONFIG_WIN32_HEADER_

#ifdef _WIN32
#include <windows.h>

/* Define if you have the ftime function.  */
#define DCMTK_LOG4CPLUS_HAVE_FTIME 1

#if defined (_WIN32_WCE)
#  define DCMTK_LOG4CPLUS_DLLMAIN_HINSTANCE HANDLE
#  undef DCMTK_LOG4CPLUS_HAVE_NT_EVENT_LOG
#else
#  define DCMTK_LOG4CPLUS_DLLMAIN_HINSTANCE HINSTANCE
#  define DCMTK_LOG4CPLUS_HAVE_NT_EVENT_LOG
#  define DCMTK_LOG4CPLUS_HAVE_WIN32_CONSOLE
#endif

// log4cplus_EXPORTS is used by the CMake build system.  DLL_EXPORT is
// used by the autotools build system.
#if defined (DCMTK_log4cplus_EXPORTS) || defined (DLL_EXPORT)
#  undef DCMTK_LOG4CPLUS_BUILD_DLL
#  define DCMTK_LOG4CPLUS_BUILD_DLL
#endif

#if ! defined (DCMTK_LOG4CPLUS_BUILD_DLL)
#  undef DCMTK_LOG4CPLUS_STATIC
#  define DCMTK_LOG4CPLUS_STATIC
#endif

#if defined (DCMTK_LOG4CPLUS_STATIC) && defined (DCMTK_LOG4CPLUS_BUILD_DLL)
#  error DCMTK_LOG4CPLUS_STATIC and DCMTK_LOG4CPLUS_BUILD_DLL cannot be defined both.
#endif

#if defined (DCMTK_LOG4CPLUS_BUILD_DLL)
#  if defined (DCMTK_INSIDE_LOG4CPLUS)
#    define DCMTK_LOG4CPLUS_EXPORT __declspec(dllexport)
#  else
#    define DCMTK_LOG4CPLUS_EXPORT __declspec(dllimport)
#  endif
#else
#  define DCMTK_LOG4CPLUS_EXPORT
#endif

#ifndef DCMTK_LOG4CPLUS_SINGLE_THREADED
#  define DCMTK_LOG4CPLUS_USE_WIN32_THREADS
#endif

#if defined(_MSC_VER)
  // Warning about: identifier was truncated to '255' characters in the debug information
#  pragma warning( disable : 4786 )
  // Warning about: <type1> needs to have dll-interface to be used by clients of class <type2>
#  pragma warning( disable : 4251 )

#  if _MSC_VER >= 1400
#    define DCMTK_LOG4CPLUS_WORKING_LOCALE
#  endif

#endif


#endif // _WIN32
#endif // DCMTK_LOG4CPLUS_CONFIG_WIN32_HEADER_

//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#ifndef JSSCRIPT_CONFIG_H
#define JSSCRIPT_CONFIG_H

   #pragma warning(disable:4786)
   #ifdef JSSCRIPT_IMPLEMENTATION
      #define JSSCRIPT_IMPORT_EXPORT __declspec(dllexport)
   #else
      #define JSSCRIPT_IMPORT_EXPORT __declspec(dllimport)
      #ifdef _DEBUG
         #define _JSSCRIPTLIBNAME "jsScript.lib"
         #define _JSSCRIPTDLLNAME "jsScript.dll"
      #else
         #define _JSSCRIPTLIBNAME "jsScript.lib"
         #define _JSSCRIPTDLLNAME "jsScript.dll"
      #endif
      //#pragma message( "This component will automatically link with " _JSSCRIPTLIBNAME )
   #endif
#endif
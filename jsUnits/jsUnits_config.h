//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#ifndef JSUNITS_CONFIG_H
#define JSUNITS_CONFIG_H

   #ifdef JSUNITS_IMPLEMENTATION
      #define JSUNITS_IMPORT_EXPORT __declspec(dllexport)
   #else
      #define JSUNITS_IMPORT_EXPORT __declspec(dllimport)
      #ifdef _DEBUG
         #define _JSUNITSLIBNAME "jsUnits.lib"
         #define _JSUNITSDLLNAME "jsUnits.dll"
      #else
         #define _JSUNITSLIBNAME "jsUnits.lib"
         #define _JSUNITSDLLNAME "jsUnits.dll"
      #endif
      //#pragma message( "This component will automatically link with " _JSUNITSLIBNAME )
   #endif

#endif
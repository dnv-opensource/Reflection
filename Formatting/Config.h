#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#ifdef FORMATTING_EXPORTS
#define FORMATTING_IMPORT_EXPORT __declspec(dllexport)
#else
#define FORMATTING_IMPORT_EXPORT __declspec(dllimport)
#endif

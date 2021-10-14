#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

namespace DNVS {namespace MoFa { namespace Reflection { namespace Members {
    /*
    If you want to have identifiers or methods that are available on the global scope, register it to GlobalType.
    Example:
    The method to create a beam that allows you to write:
    'a = Beam(Point(1,2,3),Point(0,0,0));'
    The 'Beam' method is registered with the GlobalType.
    All constructors are registered in GlobalType by default. So are all enum values.
    */
    class GlobalType {};
}}}}
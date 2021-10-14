#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

namespace DNVS {namespace MoFa {namespace Reflection {namespace Types {
    struct TypeDecoration
    {
        //Examples of how the type is deduced.
        //double -> None
        //double& -> Reference
        //const double& -> Const + Reference
        //const double const* -> (Const + Pointer) * Offset + Const
        //doube** -> Pointer*Offset + Pointer
        enum TypeDecorationEnum {
            //Special enum to represent C++/CLI types
            ManagedHandle = -1,
            //Special enum to represent C++/CLI types
            ManagedReference = -2,
            //No decoration
            None = 0,
            //&
            Reference = 1,
            //const
            Const = 2,
            //volatile
            Volatile = 4,
            //*
            Pointer = 8,
            //Needed to represent nested type decorations such as ** or const double const*.
            Offset = 16
        };
    };
}}}}
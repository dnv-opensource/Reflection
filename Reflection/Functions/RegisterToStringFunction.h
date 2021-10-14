#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Formatting/FormattingService.h"
namespace DNVS {namespace MoFa { namespace Reflection { namespace Functions {
    template<typename ClassT>
    void RegisterToStringFunction(ClassT& cls)
    {
        using namespace Classes;
        typedef typename ClassT::ReflectedType ReflectedType;
        cls.Function("ToString", [](const ReflectedType& value, const DNVS::MoFa::Formatting::FormattingService& formattingService)
        {
            return ToString(value, formattingService);
        }).AddSignature(Arg("formattingService") = DNVS::MoFa::Formatting::FormattingService());

    }
}}}}
#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IFunctionDetails.h"
#include <memory>
namespace DNVS { namespace MoFa { namespace TypeUtilities {

    class LessFunctionDetails 
    {
    public:
        bool operator()(const std::shared_ptr<IFunctionDetails>& lhs, const std::shared_ptr<IFunctionDetails>& rhs) const
        {
            int value = lhs->GetDllName().compare(rhs->GetDllName());
            if(value == 0)
                value = lhs->GetClassName().compare(rhs->GetClassName());
            if(value == 0)
                value = lhs->GetFunctionName().compare(rhs->GetFunctionName());
            if(value == 0)
                value = lhs->GetSignature().compare(rhs->GetSignature());
            return value < 0;
        }
    };
}}}
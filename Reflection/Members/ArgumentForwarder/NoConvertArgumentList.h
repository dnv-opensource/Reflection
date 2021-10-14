#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Variants/Variant.h"
#include "Reflection/Variants/InternalVariantService.h"

#include <vector>

namespace DNVS {namespace MoFa { namespace Reflection { namespace Members { namespace ArgumentForwarder {

    /*
    This class is used when calling the InvokeNative method on a member.
    It does no checking on the input arguments. In stead, it expects all arguments to be of the right type.
    This makes it very fast, and also unsafe. Don't use InvokeNative unless you know that all arguments are of the correct type.
    */
    class NoConvertArgumentList 
    {
    public:
        NoConvertArgumentList(const std::vector<Variants::Variant>& args) 
            :   m_arguments(args)
        {}
        template<typename T, size_t Index>
        Types::ReturnType<T> Get()
        {
            return Variants::InternalVariantService::UnreflectUnchecked<T>(m_arguments[Index]);
        }
    private:
        const std::vector<Variants::Variant>& m_arguments;
    };

}}}}}
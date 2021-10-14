#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include "FunctionWrapper.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {
    /*
    Use this attribute if your class is a singleton, such as mmRuleMeshing.
    */
    class SingletonAttribute
    {
    public:
        template<typename CallbackT>
        SingletonAttribute(const CallbackT& singletonAccess) 
            : m_singletonAccess(FunctionWrapper<CallbackT,Variants::Variant>(singletonAccess))
        {
            static_assert(TypeUtilities::FunctionTraits<CallbackT>::Arity == 0, "The number of arguments to the singleton access must be zero");
        }
        Variants::Variant GetSingleton() const { return m_singletonAccess({}); }
    private:
        std::function<Variants::Variant(const std::vector<Objects::Object>&)> m_singletonAccess;
    };
}}}}

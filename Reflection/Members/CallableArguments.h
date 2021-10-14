#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Variants/Variant.h"

#include "Reflection/TypeConversions/TypeConversionService.h"

#include "IMember.h"

#include <vector>

namespace DNVS {namespace MoFa { namespace Reflection { namespace Members {

    class CallableArguments 
    {
    public:
        CallableArguments(const std::vector<Variants::Variant>& args, MemberPointer member) 
            :   m_arguments(args)
            ,   m_member(member) 
        {
            for(size_t i = m_arguments.size(); i<member->GetArity(); ++i)
            {
                m_arguments.push_back(m_member->GetArgumentInfo(i)->GetDefault());
            }
        }
        template<typename T, size_t Index>
        Types::ReturnType<T> Get() 
        {
            TypeConversions::TypeConversionService service(m_member->GetConversionGraph());
            return service.IntrusiveAs<T>(m_arguments[Index]);
        }
    private:
        std::vector<Variants::Variant> m_arguments;
        MemberPointer m_member;
    };

}}}}
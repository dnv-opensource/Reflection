#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ArgumentConverter.h"
#include "Reflection/Members/IMember.h"
#include <algorithm>
#include <sstream>

namespace DNVS {namespace MoFa {namespace Reflection {namespace Members { namespace Invoker {
    class FastInvoker
    {
    public:
        FastInvoker(IMember* member)
            :   m_member(member) 
            ,   m_converter(member->GetArity(), ArgumentConverter(member->GetConversionGraph())) 
            ,   m_isVararg(member->IsVarargMember())
            ,   m_arityExcludingVararg(member->GetArity()-(m_isVararg?1:0))
        {
            for(size_t i = 0; i<m_converter.size(); ++i)
                m_converter[i].SetType(m_member->GetArgumentInfo(i)->GetTypeInfo());
            for(size_t i = 0; i<m_member->GetArity(); ++i)
            {
                if(m_member->GetArgumentInfo(i)->HasDefault())
                {
                    m_defaultArguments.push_back(m_member->GetArgumentInfo(i)->GetDefault());
                    m_converter[i].Convert(m_defaultArguments.back());
                }
            }
        }
        Variants::Variant Invoke(std::vector<Variants::Variant>& args)
        {
            size_t minArgCount = (std::min)(args.size(), m_arityExcludingVararg);
            if(minArgCount < m_arityExcludingVararg)
            {
                if(minArgCount < m_member->GetMinimumArgumentCount())
                {
                    std::stringstream errorStream;
                    errorStream << "Expected " << m_member->GetMinimumArgumentCount() << " number of arguments, got only " << minArgCount;
                    throw std::runtime_error(errorStream.str());
                }
            }
            for(size_t i = 0; i<minArgCount; ++i)
                m_converter[i].Convert(args[i]);
            if(m_isVararg)
            {
                for(size_t i = m_arityExcludingVararg; i < args.size(); ++i)
                    m_converter[m_arityExcludingVararg].Convert(args[i]);
            }
            for(size_t i = args.size(); i<m_arityExcludingVararg; ++i)
            {
                size_t startIndexOfDefaults = m_arityExcludingVararg-m_defaultArguments.size();
                args.push_back(m_defaultArguments[i-startIndexOfDefaults]);
            }
            return m_member->InvokeNative(args);
        }
    private:
        bool m_isVararg;
        size_t m_arityExcludingVararg;
        std::vector<ArgumentConverter> m_converter;
        std::vector<Variants::Variant> m_defaultArguments;
        IMember* m_member;
    };
}}}}}

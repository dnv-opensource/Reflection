#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Variants/Variant.h"
#include "Reflection/TypeConversions/IConversionSequence.h"
#include "Reflection/TypeConversions/IConversionGraph.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Members { namespace Invoker {
    class ArgumentConverter
    {
    public:
        ArgumentConverter(TypeConversions::ConversionGraphPointer conversionGraph)
            : m_conversionGraph(conversionGraph)
        {}
        void SetType(const Types::DecoratedTypeInfo& type) { m_toType = type; }
        __forceinline void Convert(Variants::Variant& var)
        {
            if (var.GetDecoratedTypeInfo() == m_toType)
                return;
            ConvertImpl(var);
        }

        void ConvertImpl(Variants::Variant& var)
        {
            for (size_t i = 0; i < m_existingConversions.size(); ++i)
            {
                if (m_existingConversions[i].first == var.GetDecoratedTypeInfo())
                {
                    m_existingConversions[i].second->IntrusiveConvert(var);
                    return;
                }
            }
            TypeConversions::ConversionSequencePointer conversionSequence = m_conversionGraph->GetConversionSequence(var, m_toType);
            if (!conversionSequence->IsValid())
                throw std::runtime_error("Unable to convert from " + std::string(var.GetDecoratedTypeInfo().GetTypeInfo().name()) + " to " + std::string(m_toType.GetTypeInfo().name()));
            m_existingConversions.push_back(std::make_pair(var.GetDecoratedTypeInfo(), conversionSequence));
            conversionSequence->IntrusiveConvert(var);
        }


    private:
        Types::DecoratedTypeInfo m_toType;
        std::vector<std::pair<Types::DecoratedTypeInfo, TypeConversions::ConversionSequencePointer> > m_existingConversions;
        TypeConversions::ConversionGraphPointer m_conversionGraph;
    };
}}}}}

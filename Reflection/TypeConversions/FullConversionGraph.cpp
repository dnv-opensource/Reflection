//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <list>
#include <memory>
#include <boost\foreach.hpp>

#include "Reflection/TypeConversions/ConversionSequence.h"
#include "Reflection/TypeConversions/ComplexConversionGraph.h"
#include "Reflection/TypeConversions/FullConversionGraph.h"
#include "Reflection/Types/DynamicTypeTraits.h"

using namespace std;

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeConversions {
    ConversionSequencePointer FullConversionGraph::GetConversionSequence(const Variants::Variant& from,
                                                                         const Types::DecoratedTypeInfo& to) const
    {
        ConversionSequencePointer sequence = GetStandardConversionSequence(from, to);

        if (!sequence->IsValid() && from.IsValid())
            sequence = ComplexConversionGraph::GetConversionSequence(Types::TypeId<const Variants::Variant&>(), to);

        return sequence;
    }

    Variants::Variant FullConversionGraph::CreateLValue(const Variants::Variant& from) const
    {
        list<ConversionPointer> dynamicConversions;
        Variants::Variant dynamicValue(from);
        GetDynamicConversionSequence(dynamicConversions, dynamicValue, Types::TypeId<void>());
        if (Types::IsNonConstLValue(dynamicValue.GetDecoratedTypeInfo()))
            return dynamicValue;
        return ComplexConversionGraph::CreateLValue(dynamicValue);
    }

    void FullConversionGraph::AddConversion(const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to, ConversionType::Type conversionType, const ConversionPointer& conversion)
    {
        if (conversionType == ConversionType::ReflectionConversion)
        {
            auto sequence = std::make_shared<ConversionSequence>(conversionType);
            if (conversion)
                sequence->Add(conversion);
            m_reflectionConversions[to.GetTypeInfo()] = sequence;
        }
        else
            ComplexConversionGraph::AddConversion(from, to, conversionType, conversion);
    }

    ConversionSequencePointer FullConversionGraph::GetStandardConversionSequence(const Variants::Variant& from,
                                                                                 const Types::DecoratedTypeInfo& to) const
    {
        auto it = m_reflectionConversions.find(to.GetTypeInfo());
        if (it != m_reflectionConversions.end())
            return it->second;
        if (from.GetDecoratedTypeInfo().GetTypeInfo() == to.GetTypeInfo())
            return ComplexConversionGraph::GetConversionSequence(from, to);

        list<ConversionPointer> dynamicConversions;
        Variants::Variant dynamicValue(from);
        GetDynamicConversionSequence(dynamicConversions, dynamicValue, to);
       

        ConversionSequencePointer sequence = ComplexConversionGraph::GetConversionSequence(dynamicValue, to);
        if (!sequence->IsValid())
            return ComplexConversionGraph::GetConversionSequence(from, to);

        if (dynamicConversions.empty())
            return sequence;

        shared_ptr<ConversionSequence> preSequence(new ConversionSequence(ConversionType::DynamicTypeConversion));

        BOOST_REVERSE_FOREACH(ConversionPointer conversion, dynamicConversions)
            preSequence->Add(conversion);

        preSequence->Join(sequence);
        sequence = preSequence;

        return sequence;
    }

    void FullConversionGraph::GetDynamicConversionSequence(std::list<ConversionPointer>& dynamicConversions,
                                                           Variants::Variant& dynamicValue, const Types::DecoratedTypeInfo& to) const
    {
        auto it = m_dynamicTypeDeduction.find(dynamicValue.GetDecoratedTypeInfo().GetTypeInfo());

        if (it == m_dynamicTypeDeduction.end())
            return;

        ConversionPointer conversion = it->second.first;
        Variants::Variant converted = conversion->Convert(dynamicValue);

        if(converted.GetDecoratedTypeInfo() == dynamicValue.GetDecoratedTypeInfo())
            return;

        dynamicConversions.push_back(conversion);
        dynamicValue = converted;

        if (to == converted.GetDecoratedTypeInfo())
            return;
        GetDynamicConversionSequence(dynamicConversions, dynamicValue, to);
    }
}}}}

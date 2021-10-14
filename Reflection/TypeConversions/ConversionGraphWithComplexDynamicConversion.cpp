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
#include "Reflection/TypeConversions/ConversionGraphWithComplexDynamicConversion.h"
#include "Reflection/Types/DynamicTypeTraits.h"

using namespace std;

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeConversions {
    ConversionSequencePointer ConversionGraphWithComplexDynamicConversion::GetConversionSequence(const Variants::Variant& from,
                                                                         const Types::DecoratedTypeInfo& to) const
    {
        try {
            auto it = m_reflectionConversions.find(to.GetTypeInfo());
            if (it != m_reflectionConversions.end())
                return it->second;
            if (from.GetDecoratedTypeInfo().GetTypeInfo() == to.GetTypeInfo())
                return ComplexConversionGraph::GetConversionSequence(from, to);

            ConversionSequencePointer sequence = GetStandardConversionSequence(from, to);

            if (!sequence->IsValid())
                sequence = ComplexConversionGraph::GetConversionSequence(Types::TypeId<const Variants::Variant&>(), to);

            return sequence;
        }
        catch (...)
        {
            return nullptr;
        }
    }

    Variants::Variant ConversionGraphWithComplexDynamicConversion::CreateLValue(const Variants::Variant& from) const
    {
        list<ConversionPointer> dynamicConversions;
        Variants::Variant dynamicValue(from);
        GetDynamicConversionSequence(dynamicConversions, dynamicValue, Types::TypeId<void>());
        if (Types::IsNonConstLValue(dynamicValue.GetDecoratedTypeInfo()))
            return dynamicValue;
        return ComplexConversionGraph::CreateLValue(dynamicValue);
    }

    void ConversionGraphWithComplexDynamicConversion::AddConversion(const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to, ConversionType::Type conversionType, const ConversionPointer& conversion)
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

    ConversionSequencePointer ConversionGraphWithComplexDynamicConversion::GetStandardConversionSequence(const Variants::Variant& from,
                                                                                 const Types::DecoratedTypeInfo& to) const
    {
        list<std::pair<ConversionPointer,Variants::Variant>> dynamicConversions;
        Variants::Variant dynamicValue(from);
        while (true)
        {
            auto it = m_dynamicTypeDeduction.find(dynamicValue.GetDecoratedTypeInfo().GetTypeInfo());
            if (it == m_dynamicTypeDeduction.end())
                break;

            ConversionPointer conversion = it->second.first;

            if (!conversion)
                break;
            Variants::Variant converted = conversion->Convert(dynamicValue);

            if (converted.GetDecoratedTypeInfo() == dynamicValue.GetDecoratedTypeInfo())
                break;
            dynamicValue = converted;
            dynamicConversions.push_back(std::make_pair(conversion, dynamicValue));
            if (dynamicValue.GetDecoratedTypeInfo() == to)
                break;
        }
        ConversionSequencePointer sequence = ComplexConversionGraph::GetConversionSequence(dynamicValue, to);
        while (!dynamicConversions.empty())
        {
            if (sequence && sequence->IsValid())
                break;
            else
                dynamicConversions.pop_back();
            if (!dynamicConversions.empty())
                dynamicValue = dynamicConversions.back().second;
            else
                dynamicValue = from;
            sequence = ComplexConversionGraph::GetConversionSequence(dynamicValue, to);
        }
        if (!sequence || !sequence->IsValid())
            return sequence;

        if (dynamicConversions.empty())
            return sequence;

        shared_ptr<ConversionSequence> preSequence(new ConversionSequence(ConversionType::DynamicTypeConversion));

        BOOST_REVERSE_FOREACH(const auto& conversionPair, dynamicConversions)
            preSequence->Add(conversionPair.first);

        preSequence->Join(sequence);
        sequence = preSequence;

        return sequence;
    }

    void ConversionGraphWithComplexDynamicConversion::GetDynamicConversionSequence(std::list<ConversionPointer>& dynamicConversions,
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

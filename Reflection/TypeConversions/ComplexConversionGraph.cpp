//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeConversions/ComplexConversionGraph.h"
#include "Reflection/TypeConversions/ConversionGraph.h"
#include "Reflection/TypeConversions/ConversionSequence.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace TypeConversions {


    ComplexConversionGraph::ComplexConversionGraph()
    {
        m_downcastGraph.reset(new ConversionGraph);
        m_completeConversionGraph.reset(new ConversionGraph);
    }

    void ComplexConversionGraph::AddConversion( const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to, ConversionType::Type conversionType, const ConversionPointer& conversion )
    {
        if (conversionType == ConversionType::DowncastConversion)
            m_downcastGraph->AddConversion(from, to, conversionType, conversion);
        else if (conversionType == ConversionType::DynamicTypeConversion)
            m_dynamicTypeDeduction[from.GetTypeInfo()] = ConversionPointerTypePair(conversion, from);
        else if (conversionType == ConversionType::LValueConversion)
            m_lvalueConversions[from.GetTypeInfo()] = ConversionPointerTypePair(conversion, from);
        else if (conversionType == ConversionType::FallbackConversion)
            m_fallbackConversions.emplace(from.GetTypeInfo(), ConversionPointerTypePair(conversion, from));
        else 
        {
            m_completeConversionGraph->AddConversion(from, to, conversionType, conversion);
            //Changes in decoration should also be added to the downcast graph.
            if(from.GetTypeInfo() == to.GetTypeInfo()) 
                m_downcastGraph->AddConversion(from, to, conversionType, conversion);
        }
    }

    void ComplexConversionGraph::AddWrapper(const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to, const ConversionPointer& conversionFrom, const ConversionPointer& conversionTo)
    {
        if(conversionFrom)
            m_wrappers[from] = conversionFrom;
        if(conversionTo)
            m_unwrappers[to] = conversionTo;
        m_unwrapTypes[to] = from;
    }

    void ComplexConversionGraph::AddAlternativeConverter(const Types::DecoratedTypeInfo& from, const AlternativeConverterPointer& alternativeConverter)
    {
        m_completeConversionGraph->AddAlternativeConverter(from, alternativeConverter);
    }

    TypeConversions::ConversionSequencePointer ComplexConversionGraph::GetConversionSequence(const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to) const
    {
        return m_completeConversionGraph->GetConversionSequence(from, to);
    }

    TypeConversions::ConversionSequencePointer ComplexConversionGraph::GetConversionSequence( const Variants::Variant& from, const Types::DecoratedTypeInfo& to ) const
    {
        ConversionSequencePointer conversionSequence = m_completeConversionGraph->GetConversionSequence(from, to);
        if(conversionSequence->IsValid()) 
            return conversionSequence;
        else 
        {
            ConversionSequencePointer fallbackConversionSequence = GetFallbackConversionSequence(from, to);
            if(fallbackConversionSequence && fallbackConversionSequence->IsValid()) 
                return fallbackConversionSequence;
        }

        //Try to do a downcast conversion, and try the conversion to make sure that the dynamic_cast works.
        ConversionSequencePointer downcastConversionSequence = m_downcastGraph->GetConversionSequence(from, to);
        if(downcastConversionSequence->IsValid() && downcastConversionSequence->Convert(from).Data() != 0) 
        {
            return downcastConversionSequence;
        }
        else 
            return conversionSequence;
    }

    Variants::Variant ComplexConversionGraph::GetDynamicType( const Variants::Variant& from ) const
    {
        Variants::Variant result(from);
        while(true) 
        {
            auto it = m_dynamicTypeDeduction.find(result.GetDecoratedTypeInfo().GetTypeInfo());
            if(it != m_dynamicTypeDeduction.end()) 
            {
                ConversionPointer conversion = it->second.first;
                Variants::Variant converted = conversion->Convert(result);
                if(converted.GetDecoratedTypeInfo() == result.GetDecoratedTypeInfo()) 
                    break;
                result = converted;
            }
            else 
                break;
        }
        return result;
    }

    bool ComplexConversionGraph::HasDynamicConversion(const std::type_info& type) const
    {
        return m_dynamicTypeDeduction.count(type) > 0;
    }

    Variants::Variant ComplexConversionGraph::CreateLValue(const Variants::Variant& from) const
    {
        auto it = m_lvalueConversions.find(from.GetDecoratedTypeInfo().GetTypeInfo());
        if(it != m_lvalueConversions.end()) 
        {
            auto sequence = GetConversionSequence(from, it->second.second);
            if (!sequence || !sequence->IsValid())
                return from;
            return it->second.first->Convert(sequence->Convert(from));
        }
        else 
            throw std::runtime_error("Unable to convert " + std::string(from.GetDecoratedTypeInfo().GetTypeInfo().name()) + " into a variable");
    }

    Variants::Variant ComplexConversionGraph::TryWrapValue(const Variants::Variant& from) const
    {
        auto it = m_wrappers.find(from.GetDecoratedTypeInfo());
        if (it != m_wrappers.end())
        {
            ConversionPointer wrapper = it->second;
            return wrapper->Convert(from);
        }
        else
            return Variants::Variant();
    }

    Variants::Variant ComplexConversionGraph::TryUnwrapValue(const Variants::Variant& from) const
    {
        auto it = m_unwrappers.find(from.GetDecoratedTypeInfo());
        if (it != m_unwrappers.end())
        {
            ConversionPointer unwrapper = it->second;
            return unwrapper->Convert(from);
        }
        else
            return Variants::Variant();
    }

    Types::DecoratedTypeInfo ComplexConversionGraph::TryUnwrapType(const Types::DecoratedTypeInfo& from) const
    {
        auto it = m_unwrapTypes.find(from);
        if (it != m_unwrapTypes.end())
            return it->second;
        else
            return from;
    }

    bool ComplexConversionGraph::SupportsWrapping(const Types::DecoratedTypeInfo& from) const
    {
        return m_wrappers.find(from) != m_wrappers.end();
    }

    bool ComplexConversionGraph::SupportsUnwrapping(const Types::DecoratedTypeInfo& from) const
    {
        return m_unwrappers.find(from) != m_unwrappers.end();
    }

    ConversionSequencePointer ComplexConversionGraph::GetFallbackConversionSequence(const Variants::Variant& from, const Types::DecoratedTypeInfo& to) const
    {
        auto range = m_fallbackConversions.equal_range(from.GetDecoratedTypeInfo().GetTypeInfo());
        for(auto it = range.first; it != range.second; ++it)
        {
            ConversionSequencePointer preConvert = GetConversionSequence(from.GetDecoratedTypeInfo(), it->second.second);
            if(!preConvert || !preConvert->IsValid()) 
                return ConversionSequencePointer();
            Variants::Variant var = preConvert->Convert(from);
            ConversionPointer fallbackConversion = it->second.first;
            try {
                Variants::Variant newVar = fallbackConversion->Convert(var);
                if (newVar.GetDecoratedTypeInfo() == var.GetDecoratedTypeInfo())
                    return ConversionSequencePointer();
                ConversionSequencePointer postConvert = GetConversionSequence(newVar, to);
                if (postConvert && postConvert->IsValid())
                {
                    ConversionSequencePointer sequence(new ConversionSequence(ConversionType::NoConversion));
                    sequence->Join(preConvert);
                    std::shared_ptr<ConversionSequence> fallbackSequence(new ConversionSequence(ConversionType::FallbackConversion));
                    fallbackSequence->Add(fallbackConversion);
                    sequence->Join(fallbackSequence);
                    sequence->Join(postConvert);
                    return sequence;
                }
            }
            catch(...) {}
        }
        return ConversionSequencePointer();
    }

}}}}
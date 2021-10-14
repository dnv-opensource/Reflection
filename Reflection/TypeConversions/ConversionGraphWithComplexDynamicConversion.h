#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/TypeConversions/ComplexConversionGraph.h"
#include <list>
#include <map>

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeConversions {
    class REFLECTION_IMPORT_EXPORT ConversionGraphWithComplexDynamicConversion : public ComplexConversionGraph
    {
    public:
        using ComplexConversionGraph::GetConversionSequence;
        virtual ConversionSequencePointer GetConversionSequence(const Variants::Variant& from,
                                                                const Types::DecoratedTypeInfo& to) const;
        Variants::Variant CreateLValue(const Variants::Variant& from) const;
        void AddConversion(const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to, ConversionType::Type conversionType, const ConversionPointer& conversion);
    private:
        void GetDynamicConversionSequence(std::list<ConversionPointer>& dynamicConversions,
                                          Variants::Variant& dynamicValue, const Types::DecoratedTypeInfo& to) const;

        ConversionSequencePointer GetStandardConversionSequence(const Variants::Variant& from,
                                                                const Types::DecoratedTypeInfo& to) const;
        std::map<std::type_index, ConversionSequencePointer> m_reflectionConversions;
    };
}}}}

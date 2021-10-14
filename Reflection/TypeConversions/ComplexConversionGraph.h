#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/Config.h"

#include "Reflection/TypeConversions/IConversionGraph.h"
#include <map>

namespace DNVS {namespace MoFa { namespace Reflection { namespace TypeConversions {

    class REFLECTION_IMPORT_EXPORT ComplexConversionGraph : public IConversionGraph 
    {
    public:
        ComplexConversionGraph();
        void AddConversion(const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to, ConversionType::Type conversionType, const ConversionPointer& conversion);
        virtual void AddWrapper(const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to, const ConversionPointer& conversionFrom, const ConversionPointer& conversionTo) override;
        virtual void AddAlternativeConverter(const Types::DecoratedTypeInfo& from, const AlternativeConverterPointer& alternativeConverter) override;
        ConversionSequencePointer GetConversionSequence(const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to) const;
        ConversionSequencePointer GetConversionSequence(const Variants::Variant& from, const Types::DecoratedTypeInfo& to) const;
        Variants::Variant GetDynamicType(const Variants::Variant& from) const;
        virtual bool HasDynamicConversion(const std::type_info& type) const override;
        Variants::Variant CreateLValue(const Variants::Variant& from) const;
        Variants::Variant TryWrapValue(const Variants::Variant& from) const override;
        virtual Variants::Variant TryUnwrapValue(const Variants::Variant& from) const override;
        virtual Types::DecoratedTypeInfo TryUnwrapType(const Types::DecoratedTypeInfo& from) const override;
        virtual bool SupportsWrapping(const Types::DecoratedTypeInfo& from) const override;
        virtual bool SupportsUnwrapping(const Types::DecoratedTypeInfo& from) const override;
        ConversionSequencePointer GetFallbackConversionSequence( const Variants::Variant& from, const Types::DecoratedTypeInfo& to ) const;
    protected:
        ConversionGraphPointer m_downcastGraph;
        ConversionGraphPointer m_completeConversionGraph;
        typedef std::pair<ConversionPointer, Types::DecoratedTypeInfo> ConversionPointerTypePair;
        using TypeConversionMap = std::map<std::type_index, ConversionPointerTypePair>;
        using TypeConversionMultiMap = std::multimap<std::type_index, ConversionPointerTypePair>;
        TypeConversionMap m_dynamicTypeDeduction;
        TypeConversionMap m_lvalueConversions;
        TypeConversionMultiMap m_fallbackConversions;
        std::map<Types::DecoratedTypeInfo, ConversionPointer> m_wrappers;
        std::map<Types::DecoratedTypeInfo, ConversionPointer> m_unwrappers;
        std::map<Types::DecoratedTypeInfo, Types::DecoratedTypeInfo> m_unwrapTypes;
    };

}}}}
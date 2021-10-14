#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/TypeConversions/IConversionGraph.h"
#include "Reflection/Types/DecoratedTypeInfo.h"
#include <map>
#include <mutex>

namespace DNVS {namespace MoFa { namespace Reflection { namespace TypeConversions {
    struct ConversionGraphImpl;

    class REFLECTION_IMPORT_EXPORT ConversionGraph : public IConversionGraph 
    {
    public:
        ConversionGraph();
        ~ConversionGraph();
        void AddConversion(const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to, ConversionType::Type conversionType, const ConversionPointer& conversion);
        virtual void AddWrapper(const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to, const ConversionPointer& conversionFrom, const ConversionPointer& conversionTo) override;
        virtual void AddAlternativeConverter(const Types::DecoratedTypeInfo& from, const AlternativeConverterPointer& alternativeConverter) override;
        ConversionSequencePointer GetConversionSequence(const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to) const;
        ConversionSequencePointer GetConversionSequence(const Variants::Variant& from, const Types::DecoratedTypeInfo& to) const;
        Variants::Variant GetDynamicType(const Variants::Variant& from) const;
        virtual bool HasDynamicConversion(const std::type_info& type) const override;
        Variants::Variant CreateLValue(const Variants::Variant& from) const;
        virtual Variants::Variant TryWrapValue(const Variants::Variant& from) const;
        virtual Types::DecoratedTypeInfo TryUnwrapType(const Types::DecoratedTypeInfo& from) const override;
        virtual bool SupportsWrapping(const Types::DecoratedTypeInfo& from) const override;
        virtual bool SupportsUnwrapping(const Types::DecoratedTypeInfo& from) const override;
        virtual Variants::Variant TryUnwrapValue(const Variants::Variant& from) const override;
    private:
        typedef std::map<std::pair<Types::DecoratedTypeInfo, Types::DecoratedTypeInfo>, ConversionSequencePointer> ConversionSequences;
        mutable ConversionSequences m_conversionSequences;
        ConversionSequencePointer m_noConversionNeeded;
        ConversionSequencePointer m_invalidConversion;
        std::multimap<Types::DecoratedTypeInfo, AlternativeConverterPointer> m_alternativeConverters;
        std::unique_ptr<ConversionGraphImpl> m_impl;
        mutable std::mutex m_mutex;
        ConversionSequencePointer GetAlternativeConversionSequence(const Variants::Variant& from, const Types::DecoratedTypeInfo& to) const;
    };

}}}}
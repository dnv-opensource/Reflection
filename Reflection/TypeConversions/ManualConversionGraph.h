#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "IConversion.h"
#include "IConversionGraph.h"

#include <set>
#include <map>
#include "Details/GraphVertex.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace TypeConversions { namespace Details {
    class GraphPath;
}}}}}

namespace DNVS {namespace MoFa { namespace Reflection { namespace TypeConversions {

    class REFLECTION_IMPORT_EXPORT ManualConversionGraph : public IConversionGraph 
    {
    public:
        void AddConversion(const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to, ConversionType::Type conversionType, const ConversionPointer& conversion);
        ConversionSequencePointer GetConversionSequence(const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to) const;
        ConversionSequencePointer GetConversionSequence(const Variants::Variant& from, const Types::DecoratedTypeInfo& to) const;
        virtual void AddAlternativeConverter(const Types::DecoratedTypeInfo& from, const AlternativeConverterPointer& alternativeConverter) override;
        Variants::Variant GetDynamicType(const Variants::Variant& from) const;
        virtual bool HasDynamicConversion(const std::type_info& type) const override;
        Variants::Variant CreateLValue(const Variants::Variant& from) const;
        Details::GraphVertex* LookupVertexCreateIfMissing( const Types::DecoratedTypeInfo& from );
        const Details::GraphVertex* LookupVertex( const Types::DecoratedTypeInfo& from ) const;
        void AdjustHeadIfNecessary( Details::GraphPath& path, ConversionType::Type conversionType, const Variants::Variant& from ) const;
        Variants::Variant TryWrapValue(const Variants::Variant& from) const override;
        virtual bool SupportsWrapping(const Types::DecoratedTypeInfo& from) const override;
        virtual Variants::Variant TryUnwrapValue(const Variants::Variant& from) const override;
        virtual Types::DecoratedTypeInfo TryUnwrapType(const Types::DecoratedTypeInfo& from) const override;

        void AddWrapper(const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to, const ConversionPointer& conversionFrom, const ConversionPointer& conversionTo) override;


        bool SupportsUnwrapping(const Types::DecoratedTypeInfo& from) const override;

    private:
        template<typename FunctorT>
        ConversionSequencePointer GetConversionSequenceImpl(const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to, FunctorT& pathAdjuster) const;
        std::map<Types::DecoratedTypeInfo, Details::GraphVertex> m_vertices;
        typedef std::map<std::pair<Types::DecoratedTypeInfo, Types::DecoratedTypeInfo>, ConversionSequencePointer> ConversionSequences;
        mutable ConversionSequences m_conversionSequences;
    };
}}}}
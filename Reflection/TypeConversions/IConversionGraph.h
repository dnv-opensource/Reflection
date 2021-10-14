#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/Config.h"

#include "Reflection/Types/DecoratedTypeInfo.h"
#include "Reflection/TypeConversions/IConversionSequence.h"
#include "Reflection/TypeConversions/IConversion.h"
#include <memory>
#include "IAlternativeConverter.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace TypeConversions {
    class IConversion;

    ///Defines all legal type conversions.
    class REFLECTION_IMPORT_EXPORT IConversionGraph {
    public:
        ///Adds a type conversion between two types.
        virtual void AddConversion(const Types::DecoratedTypeInfo& from,const Types::DecoratedTypeInfo& to,ConversionType::Type conversionType,const ConversionPointer& conversion) = 0;
        virtual void AddWrapper(const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to, const ConversionPointer& conversionFrom, const ConversionPointer& conversionTo) = 0;
        virtual void AddAlternativeConverter(const Types::DecoratedTypeInfo& from, const AlternativeConverterPointer& alternativeConverter) = 0;
        virtual ConversionSequencePointer GetConversionSequence(const Types::DecoratedTypeInfo& from,const Types::DecoratedTypeInfo& to) const = 0;
        virtual ConversionSequencePointer GetConversionSequence(const Variants::Variant& from,const Types::DecoratedTypeInfo& to) const = 0;
        virtual Variants::Variant GetDynamicType(const Variants::Variant& from) const = 0;
        virtual bool HasDynamicConversion(const std::type_info& type) const = 0;
        virtual Variants::Variant CreateLValue(const Variants::Variant& from) const = 0;
        virtual Variants::Variant TryWrapValue(const Variants::Variant& from) const = 0;
        virtual Types::DecoratedTypeInfo TryUnwrapType(const Types::DecoratedTypeInfo& from) const = 0;
        virtual bool SupportsWrapping(const Types::DecoratedTypeInfo& from) const = 0;
        virtual bool SupportsUnwrapping(const Types::DecoratedTypeInfo& from) const = 0;
        virtual Variants::Variant TryUnwrapValue(const Variants::Variant& from) const = 0;
    };
    typedef std::shared_ptr<IConversionGraph> ConversionGraphPointer;
}}}}
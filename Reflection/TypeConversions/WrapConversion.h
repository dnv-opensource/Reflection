#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Types/DeduceTypeTag.h"
#include "Reflection/Variants/SmartPointers/SmartPointerHelper.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeConversions {
    template<typename From, typename To>
    class WrapTypeConversion : public IConversion
    {
    public:
        virtual Variants::Variant Convert(const Variants::Variant& input)
        {
            //No need to wrap this input. It is wrapped correctly already.
            if (Variants::IsVariantOfThisSmartPointerType<To>(input))
                return input;
            From arg = Variants::InternalVariantService::UnreflectUnchecked<From>(input);
            Variants::Variant result;
            Variants::InternalVariantService::SetVariantData<To>(result, To(arg));
            result.SetDecoratedTypeInfo(Types::TypeId<To>());
            return result;
        }

        virtual void IntrusiveConvert(Variants::Variant& variable)
        {
            variable = Convert(variable);
        }
    };

    template<typename From, typename To>
    void AddWrapConversionImpl(ConversionGraphPointer conversionGraph, Types::PointerTag, Types::SmartPointerTag)
    {
        conversionGraph->AddWrapper(Types::TypeId<From>(), Types::TypeId<To>(), std::make_shared<WrapTypeConversion<From, To>>(), nullptr);
    }

    template<typename From, typename To>
    void AddWrapConversionImpl(ConversionGraphPointer conversionGraph, ...)
    {
    }

    template<typename From, typename To>
    void AddWrapConversion(ConversionGraphPointer conversionGraph)
    {
        Types::TypeTag<From> fromTag;
        Types::TypeTag<To> toTag;
        AddWrapConversionImpl<From,To>(conversionGraph, fromTag, toTag);
    }
}}}}
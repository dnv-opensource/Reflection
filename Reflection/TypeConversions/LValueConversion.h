#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Types/DeduceTypeTag.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeConversions {
    template<typename T>
    class CreateLValue : public IConversion
    {
    public:
        virtual Variants::Variant Convert(const Variants::Variant& input)
        {
            Types::ReturnType<T> arg = Variants::InternalVariantService::UnreflectUnchecked<Types::ReturnType<T>>(input);
            Variants::Variant result;
            Variants::InternalVariantService::SetVariantData<T>(result, arg);
            result.SetDecoratedTypeInfo(Types::TypeId<T&>());
            return result;
        }

        virtual void IntrusiveConvert(Variants::Variant& variable)
        {
            variable = Convert(variable);
        }
    };

    template<typename T>
    class CreateLValueSmallPod : public IConversion
    {
    public:
        virtual Variants::Variant Convert(const Variants::Variant& input)
        {
            const T& arg = Variants::InternalVariantService::UnreflectUnchecked<const T&>(input);
            Variants::Variant result;
            Variants::InternalVariantService::SetVariantData<T>(result, arg);
            result.SetDecoratedTypeInfo(Types::TypeId<T>());
            return result;
        }

        virtual void IntrusiveConvert(Variants::Variant& variable)
        {
            variable = Convert(variable);
        }
    };

    template<typename T,typename Tag>
    void AddLValueConversionImpl(ConversionGraphPointer conversionGraph, Tag)
    {
    }

    template<typename T>
    std::enable_if_t<std::is_copy_constructible_v<T>> AddLValueConversionImpl(ConversionGraphPointer conversionGraph, Types::SmallPodTag)
    {
        conversionGraph->AddConversion(Types::TypeId<const T&>(), Types::TypeId<T>(), ConversionType::LValueConversion, std::make_shared<CreateLValueSmallPod<T>>());
    }
    template<typename T>
    std::enable_if_t<std::is_copy_constructible_v<T>> AddLValueConversionImpl(ConversionGraphPointer conversionGraph, Types::ValueTag)
    {
        conversionGraph->AddConversion(Types::TypeId<T>(), Types::TypeId<T&>(), ConversionType::LValueConversion, std::make_shared<CreateLValue<T>>());
    }

    template<typename T>
    void AddLValueConversion(ConversionGraphPointer conversionGraph) 
    {
        Types::TypeTag<T> tag;
        AddLValueConversionImpl<T>(conversionGraph, tag);
    }
}}}}
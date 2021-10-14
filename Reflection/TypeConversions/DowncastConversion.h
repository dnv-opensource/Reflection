#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IConversion.h"
#include "IConversionGraph.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeConversions {

    template<typename From, typename To>
    struct DynamicDowncastConversion : public IConversion
    {
        virtual Variants::Variant Convert(const Variants::Variant& other)
        {
            Variants::Variant result(other);
            IntrusiveConvert(result);
            result.SetDecoratedTypeInfo(Types::DecoratedTypeInfo(typeid(To), other.GetDecoratedTypeInfo().GetDecoration()));
            return result;
        }

        virtual void IntrusiveConvert( Variants::Variant& variable ) 
        {
            From* from = Variants::InternalVariantService::UnreflectUnchecked<From*>(variable);
            variable.SetData(static_cast<void*>(dynamic_cast<To*>(from)));
        }
    };

    template <typename From, typename To>
    inline void AddDowncastConversionsImpl(ConversionGraphPointer conversionGraph, ConversionPointer conversion)
    {
        conversionGraph->AddConversion(Types::TypeId<From*>(), Types::TypeId<To*>(), ConversionType::DowncastConversion, conversion);
        conversionGraph->AddConversion(Types::TypeId<const From*>(), Types::TypeId<const To*>(), ConversionType::DowncastConversion, conversion);
        conversionGraph->AddConversion(Types::TypeId<From&>(), Types::TypeId<To&>(), ConversionType::DowncastConversion, conversion);
        conversionGraph->AddConversion(Types::TypeId<const From&>(), Types::TypeId<const To&>(), ConversionType::DowncastConversion, conversion);
    }
    template <typename From, typename To>
    typename std::enable_if<std::is_base_of<From,To>::value && std::is_polymorphic<From>::value>::type 
        AddDowncastConversion(ConversionGraphPointer conversionGraph)
    {
        ConversionPointer conversion(new DynamicDowncastConversion<From, To>());
        AddDowncastConversionsImpl<From, To>(conversionGraph, conversion);
    }

    template <typename From, typename To>
    typename std::enable_if<!(std::is_base_of<From,To>::value && std::is_polymorphic<From>::value)>::type 
        AddDowncastConversion(ConversionGraphPointer conversionGraph)
    {        
    }
}}}}

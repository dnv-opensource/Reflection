#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/TypeConversions/IConversion.h"
#include <type_traits>
#include <TypeUtilities/Promote.h>

namespace DNVS {namespace MoFa { namespace Reflection { namespace TypeConversions {
#pragma warning(push)
#pragma warning(disable:4244)
#pragma warning(disable:4800)

    template<typename From, typename To>
    class ImplicitConversion : public IConversion 
    {
    public:
        virtual Variants::Variant Convert(const Variants::Variant& var) 
        {            
            Variants::Variant result(var);
            IntrusiveConvert(result);
            result.SetDecoratedTypeInfo(Types::TypeId<To>());
            return result;
        }
        virtual void IntrusiveConvert(Variants::Variant& variable) 
        {
            From from = Variants::InternalVariantService::UnreflectUnchecked<From>(variable);
            Variants::InternalVariantService::SetVariantData<To>(variable, (To)from);
        }
        static const ConversionType::Type ConversionTypeValue = 
            (std::is_same<To, typename TypeUtilities::Promote<From>::type>::value) ? 
                ConversionType::PromotionConversion :
                ConversionType::StandardConversion;
    };

    template<typename From, typename To>
    void AddImplicitConversion(ConversionGraphPointer conversionGraph) 
    {
        conversionGraph->AddConversion(Types::TypeId<From>(), Types::TypeId<To>(), ImplicitConversion<From, To>::ConversionTypeValue, std::make_shared<ImplicitConversion<From, To>>());
    }
#pragma warning(pop)    
}}}}
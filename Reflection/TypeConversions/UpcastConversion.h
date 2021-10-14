#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IConversion.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeConversions {
    template<typename From,typename To>
    struct UpcastConversion : public IConversion
    {
        virtual Variants::Variant Convert(const Variants::Variant& var)
        {
            //Extract value from var
            Variants::Variant result(var);
            IntrusiveConvert(result);
            Types::DecoratedTypeInfo newInfo(typeid(To),var.GetDecoratedTypeInfo().GetDecoration());
            result.SetDecoratedTypeInfo(newInfo);
            return result;
        }
        virtual void IntrusiveConvert(Variants::Variant& variable) {
            From* from=Variants::InternalVariantService::UnreflectUnchecked<From*>(variable);
            Variants::InternalVariantService::SetVariantData<To*>(variable,static_cast<To*>(from));
        }
    };

    template<typename From,typename To>
    void AddUpcastConversion(ConversionGraphPointer conversionGraph) {

        ConversionPointer conversion(new UpcastConversion<From,To>());
        conversionGraph->AddConversion(Types::TypeId<From*>(),Types::TypeId<To*>(),ConversionType::UpcastConversion,conversion);
        conversionGraph->AddConversion(Types::TypeId<const From*>(),Types::TypeId<const To*>(),ConversionType::UpcastConversion,conversion);
        conversionGraph->AddConversion(Types::TypeId<From&>(),Types::TypeId<To&>(),ConversionType::UpcastConversion,conversion);
        conversionGraph->AddConversion(Types::TypeId<const From&>(),Types::TypeId<const To&>(),ConversionType::UpcastConversion,conversion);
    }
    
}}}}
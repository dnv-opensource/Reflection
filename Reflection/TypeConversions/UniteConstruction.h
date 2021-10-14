#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IConversion.h"
#include "Reflection/Variants/InternalVariantService.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace TypeConversions {
    template<typename From, typename To, typename HolderT>
    struct UniteConstructConversionDummy : public TypeConversions::IConversion 
    {
    public:
        virtual Variants::Variant Convert( const Variants::Variant& variable ) 
        {
            Variants::Variant result(variable);
            IntrusiveConvert(result);
            result.SetDecoratedTypeInfo(Types::TypeId<To*>());
            return result;
        }

        virtual void IntrusiveConvert( Variants::Variant& variable ) 
        {
            Variants::InternalVariantService::SetVariantData<HolderT>(variable, HolderT(new To));
        }
    };

    template<typename From, typename To, typename HolderT>
    struct UniteConstructConversion : public TypeConversions::IConversion 
    {
    public:
        virtual Variants::Variant Convert( const Variants::Variant& variable ) 
        {
            Variants::Variant result(variable);
            IntrusiveConvert(result);
            result.SetDecoratedTypeInfo(Types::TypeId<To*>());
            return result;
        }

        virtual void IntrusiveConvert( Variants::Variant& variable ) 
        {
            From* from = Variants::InternalVariantService::UnreflectUnchecked<From*>(variable);
            Variants::InternalVariantService::SetVariantData<HolderT>(variable, HolderT(new To(from)));
        }
    };

    template<typename From, typename To, typename HolderT>
    struct UniteCastConversion : public TypeConversions::IConversion 
    {
    public:
        virtual Variants::Variant Convert( const Variants::Variant& variable ) 
        {
            Variants::Variant result(variable);
            IntrusiveConvert(result);
            result.SetDecoratedTypeInfo(Types::TypeId<To*>());
            return result;
        }

        virtual void IntrusiveConvert( Variants::Variant& variable ) 
        {            
            From* from = Variants::InternalVariantService::UnreflectUnchecked<From*>(variable);
            ///This required From to have a conversion to To.
            Variants::InternalVariantService::SetVariantData<HolderT>(variable, HolderT(*from));
        }
    };

    template<typename From, typename To, typename HolderFrom, typename HolderTo, bool EnableIf = std::is_convertible<From*, To>::value>
    struct AddUniteConstructorConversionIfConvertible {
        static void Apply(const TypeConversions::ConversionGraphPointer& conversionGraph) {
            typedef typename Types::UnwrapPointer<To>::type ToType;
            conversionGraph->AddConversion(
                Types::TypeId<From*>(), 
                Types::TypeId<To*>(), 
                TypeConversions::ConversionType::UserConversion, 
                std::make_shared<UniteConstructConversion<From, To, HolderTo>>());
            conversionGraph->AddConversion(
                Types::TypeId<To*>(), 
                Types::TypeId<From*>(), 
                TypeConversions::ConversionType::UserConversion, 
                std::make_shared<UniteCastConversion<To, From, HolderFrom>>());
        }
    };
    template<typename From, typename To, typename HolderFrom, typename HolderTo>
    struct AddUniteConstructorConversionIfConvertible<From, To, HolderFrom, HolderTo, false> {
        static void Apply(const TypeConversions::ConversionGraphPointer& conversionGraph) {
            conversionGraph->AddConversion(
                Types::TypeId<From*>(), 
                Types::TypeId<To*>(), 
                TypeConversions::ConversionType::UserConversion, 
                std::make_shared<UniteConstructConversionDummy<From, To, HolderTo>>());
        }
    };

    template<typename From, typename To, typename HolderFrom, typename HolderTo>
    void AddUniteConversion(const TypeConversions::ConversionGraphPointer& conversionGraph) {        
        AddUniteConstructorConversionIfConvertible<From, To, HolderFrom, HolderTo>::Apply(conversionGraph);
    }
}}}}
#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeConversions/IConversion.h"

#include "Reflection/Members/Modifier.h"
#include "Reflection/Members/IMember.h"

#include "TypeUtilities/FunctionTraits.h"
#include <type_traits>

namespace DNVS {namespace MoFa {namespace Reflection {namespace Constructors {
    template<typename From, typename To>
    struct ConstructorConversion : public TypeConversions::IConversion 
    {
    public:
        ConstructorConversion(Members::MemberPointer member) : m_member(member) {}
        virtual Variants::Variant Convert(const Variants::Variant& var)
        {
            std::vector<Variants::Variant> args(1, var);
            return m_member->FastInvoke(args);
        }
        virtual void IntrusiveConvert(Variants::Variant& var) 
        {
            std::vector<Variants::Variant> args(1, var);
            args[0].SetDecoratedTypeInfo(Types::TypeId<From>());
            var = m_member->FastInvoke(args);
        }
    private:
        Members::MemberPointer m_member;
    };

    template<typename Signature, int Arity>
    struct ReplaceArgumentWithVarargArgumentIfArityIsOne 
    {
        typedef typename TypeUtilities::FunctionTraits<Signature>::Argument0 type;
    };
    template<typename Signature>
    struct ReplaceArgumentWithVarargArgumentIfArityIsOne<Signature, 1> 
    {
        typedef typename TypeUtilities::FunctionTraits<Signature>::Argument0 Argument0;
        typedef typename std::remove_const<typename std::remove_reference<Argument0>::type>::type ContainerType;
        typedef typename ContainerType::value_type type;
    };

    template<typename Signature, typename Modifier, int Arity>
    struct ConstructorConversionAdder 
    {
    public:        
        //Use pointer syntax here in stead of smart pointer syntax, as ConstructorConversionAdder can be called from the constructor of the member.
        static void Apply(TypeConversions::ConversionGraphPointer conversionGraph, Members::IMember* member) 
        {
            ApplyImpl<Members::IsVararg<Modifier>::value>::Apply(conversionGraph, member, Members::IsExplicit<Modifier>::value, Members::IsAlias<Modifier>::value);
        }
        template<bool IsVararg>
        struct ApplyImpl 
        {
            static void Apply(TypeConversions::ConversionGraphPointer conversionGraph, Members::IMember* member, bool isExplicit, bool isAlias) 
            {
                if(member->GetMinimumArgumentCount()>1 || isExplicit) 
                    return;
                typedef typename TypeUtilities::FunctionTraits<Signature>::ReturnType To;
                typedef typename ReplaceArgumentWithVarargArgumentIfArityIsOne<Signature, Arity>::type From;
                TypeConversions::ConversionType::Type conversionType = (isAlias?TypeConversions::ConversionType::StandardConversion:TypeConversions::ConversionType::UserConversion);
                conversionGraph->AddConversion(Types::TypeId<From>(), Types::TypeId<To>(), conversionType, TypeConversions::ConversionPointer(new ConstructorConversion<From, To>(member)));
            }
        };
        template<>
        struct ApplyImpl<false> 
        {
            static void Apply(TypeConversions::ConversionGraphPointer conversionGraph, Members::IMember* member, bool isExplicit, bool isAlias) 
            {
                if(member->GetMinimumArgumentCount()>1 || isExplicit) 
                    return;
                typedef typename TypeUtilities::FunctionTraits<Signature>::ReturnType To;
                typedef typename TypeUtilities::FunctionTraits<Signature>::Argument0 From;
                TypeConversions::ConversionType::Type conversionType = (isAlias?TypeConversions::ConversionType::StandardConversion:TypeConversions::ConversionType::UserConversion);
                conversionGraph->AddConversion(Types::TypeId<From>(), Types::TypeId<To>(), conversionType, TypeConversions::ConversionPointer(new ConstructorConversion<From, To>(member)));
            }
        };
    };

    //No conversion if arity==0
    template<typename Signature, typename Modifier>
    struct ConstructorConversionAdder<Signature, Modifier, 0> 
    {
    public:        
        static void Apply(TypeConversions::ConversionGraphPointer conversionGraph, Members::IMember* member) {}
    };


}}}}

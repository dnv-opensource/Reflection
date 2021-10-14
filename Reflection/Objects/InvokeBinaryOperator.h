#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Variants/Variant.h"

#include "Reflection/Members/MemberWithArguments.h"
#include "Reflection/Members/OperatorNames.h"

#include "Reflection/TypeLibraries/ITypeLibrary.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Objects {
    inline Types::DecoratedTypeInfo GetType(TypeConversions::ConversionGraphPointer conversionGraph, Variants::Variant var) {
        return conversionGraph->GetDynamicType(var).GetDecoratedTypeInfo();
    }
    inline bool IsValidMemberPointer(Members::MemberWithArgumentsPointer overload)
    { 
        if (!overload)
            return false;
        return overload->IsOk();
    }
    template<typename Tag>
    Members::MemberWithArgumentsPointer PrepareInvokeBinaryOperator(const TypeLibraries::TypeLibraryPointer& typeLibrary,const Variants::Variant& lhs,const Variants::Variant& rhs)
    {
        std::vector<Variants::Variant> args(2);
        args[0]=typeLibrary->GetConversionGraph()->GetDynamicType(lhs);
        args[1]=typeLibrary->GetConversionGraph()->GetDynamicType(rhs);
        Members::MemberWithArgumentsPointer lhsOverload,rhsOverload;
        TypeLibraries::TypePointer typeLhs=typeLibrary->LookupType(args[0].GetDecoratedTypeInfo().GetTypeInfo());
        if(typeLhs) {
            lhsOverload=typeLhs->PrepareInvoke(Members::BinaryOperatorName<Tag>::Name(),args); 
        }
        TypeLibraries::TypePointer typeRhs=typeLibrary->LookupType(args[1].GetDecoratedTypeInfo().GetTypeInfo());
        if(typeRhs) {
            rhsOverload=typeRhs->PrepareInvoke(Members::BinaryOperatorRName<Tag>::Name(),args);
        }
        if(!IsValidMemberPointer(lhsOverload) && !IsValidMemberPointer(rhsOverload))
        {
            if (typeLibrary->GetConversionGraph()->GetConversionSequence(lhs, rhs.GetDecoratedTypeInfo())->IsValid() && typeRhs)
            {
                lhsOverload = typeRhs->PrepareInvoke(Members::BinaryOperatorName<Tag>::Name(), args);
            }
        }
        return Members::MemberWithArguments::SelectBestOverload(lhsOverload,rhsOverload);
    };
    template<typename Tag>
    Members::MemberWithArgumentsPointer PrepareInvokeBinaryModifyingOperator(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Variants::Variant& lhs, const Variants::Variant& rhs)
    {
        std::vector<Variants::Variant> args(2);
        args[0] = lhs;
        args[1] = typeLibrary->GetConversionGraph()->GetDynamicType(rhs);
        TypeLibraries::TypePointer typeLhs = typeLibrary->LookupType(args[0].GetDecoratedTypeInfo().GetTypeInfo());
        if (typeLhs) 
            return typeLhs->PrepareInvoke(Members::BinaryOperatorName<Tag>::Name(), args);
        return nullptr;
    };

    template<typename Tag>
    Variants::Variant InvokeBinaryOperator(const TypeLibraries::TypeLibraryPointer& typeLibrary,const Variants::Variant& lhs,const Variants::Variant& rhs)
    {
        Members::MemberWithArgumentsPointer delegate=PrepareInvokeBinaryOperator<Tag>(typeLibrary,lhs,rhs);
        if(!delegate) throw std::runtime_error("No operator overload found for " + DNVS::MoFa::Operators::Stringizer<Tag>::Stringize(GetTypeName(typeLibrary,lhs),GetTypeName(typeLibrary,rhs)));
        if(!delegate->IsOk()) {
            throw std::runtime_error("Unable to convert all the arguments for " + DNVS::MoFa::Operators::Stringizer<Tag>::Stringize(GetTypeName(typeLibrary,lhs),GetTypeName(typeLibrary,rhs)));
        }
        return delegate->Invoke();
    }

    template<typename Tag>
    Variants::Variant InvokeBinaryModifyingOperator(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Variants::Variant& lhs, const Variants::Variant& rhs)
    {
        Members::MemberWithArgumentsPointer delegate = PrepareInvokeBinaryModifyingOperator<Tag>(typeLibrary, lhs, rhs);
        if (!delegate) throw std::runtime_error("No operator overload found for " + DNVS::MoFa::Operators::Stringizer<Tag>::Stringize(GetTypeName(typeLibrary, lhs), GetTypeName(typeLibrary, rhs)));
        if (!delegate->IsOk()) {
            throw std::runtime_error("Unable to convert all the arguments for " + DNVS::MoFa::Operators::Stringizer<Tag>::Stringize(GetTypeName(typeLibrary, lhs), GetTypeName(typeLibrary, rhs)));
        }
        auto variant = delegate->Invoke();
        if (variant.GetData() == lhs.GetData())
            return lhs;
        else
            return variant;
    }
    

}}}}

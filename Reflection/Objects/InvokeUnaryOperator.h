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
    template<typename Tag>
    Members::MemberWithArgumentsPointer PrepareInvokeUnaryOperator(const TypeLibraries::TypeLibraryPointer& typeLibrary,const Variants::Variant& subject)
    {
        std::vector<Variants::Variant> args(1);
        args[0]=typeLibrary->GetConversionGraph()->GetDynamicType(subject);

        TypeLibraries::TypePointer type=typeLibrary->LookupType(args[0].GetDecoratedTypeInfo().GetTypeInfo());
        if(type) {
            return type->PrepareInvoke(Members::UnaryOperatorName<Tag>::Name(),args);
        }
        return Members::MemberWithArgumentsPointer();
    };

    template<typename Tag>
    Members::MemberWithArgumentsPointer PrepareInvokeUnaryModifyingOperator(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Variants::Variant& subject)
    {
        std::vector<Variants::Variant> args(1);
        args[0] = subject;

        TypeLibraries::TypePointer type = typeLibrary->LookupType(args[0].GetDecoratedTypeInfo().GetTypeInfo());
        if (type) {
            return type->PrepareInvoke(Members::UnaryOperatorName<Tag>::Name(), args);
        }
        return Members::MemberWithArgumentsPointer();
    };

    template<typename Tag>
    Variants::Variant InvokeUnaryOperator(const TypeLibraries::TypeLibraryPointer& typeLibrary,const Variants::Variant& subject)
    {
        Members::MemberWithArgumentsPointer delegate=PrepareInvokeUnaryOperator<Tag>(typeLibrary,subject);
        if(!delegate) throw std::runtime_error("No operator overload found for " + DNVS::MoFa::Operators::Stringizer<Tag>::Stringize(GetTypeName(typeLibrary,subject)));
        return delegate->Invoke();
    }

    template<typename Tag>
    Variants::Variant InvokeUnaryModifyingOperator(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Variants::Variant& subject)
    {
        Members::MemberWithArgumentsPointer delegate = PrepareInvokeUnaryModifyingOperator<Tag>(typeLibrary, subject);
        if (!delegate) throw std::runtime_error("No operator overload found for " + DNVS::MoFa::Operators::Stringizer<Tag>::Stringize(GetTypeName(typeLibrary, subject)));
        return delegate->Invoke();
    }
}}}}
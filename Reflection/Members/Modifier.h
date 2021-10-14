#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <type_traits>

namespace DNVS {namespace MoFa { namespace Reflection { namespace Members {

    namespace ModifierNamespace {
        enum {
            ///Default modifier, no modification to default behavior.
            NoModifierEnum = 0,
            ///Specifies that the last argument will be a container and that this container should be populated with all extra function arguments.
            VarargEnum = 1,
            ///Specifies that a constructor can not be used for implicit conversion.
            ExplicitEnum = 2,
            ///Specifies that a constructor should be treated as a Standard Conversion, not as a User Conversion.
            AliasEnum = 4,
            ///Specifies that the last argument of a constructor is the allocator. This argument must not be specified in the Constructor template list.
            AllocatorAsLastArgumentEnum = 8,
        };
        
        template<int ModifierValue>
        struct Modifier {
            template<int OtherModifier>
            Modifier<ModifierValue|OtherModifier> operator|(const Modifier<OtherModifier>&) const {return Modifier<ModifierValue|OtherModifier>();}
        };

        typedef Modifier<NoModifierEnum> NoModifier;
        typedef Modifier<ExplicitEnum> Explicit;
        typedef Modifier<VarargEnum> Vararg;
        typedef Modifier<AliasEnum> Alias;
        typedef Modifier<AllocatorAsLastArgumentEnum> AllocatorAsLastArgument;
        typedef Modifier<ExplicitEnum|AllocatorAsLastArgumentEnum> ExplicitAllocatorAsLastArgument;
		///Used when defining function overloads.
        struct Const {};

    }
    const ModifierNamespace::Modifier<ModifierNamespace::VarargEnum> Vararg;
    const ModifierNamespace::Modifier<ModifierNamespace::ExplicitEnum> Explicit;
    const ModifierNamespace::Modifier<ModifierNamespace::AliasEnum> Alias;
    const ModifierNamespace::Modifier<ModifierNamespace::AllocatorAsLastArgumentEnum> AllocatorAsLastArgument;


    template<typename T>
    struct IsVararg : std::integral_constant<bool, false> {};
    template<int ModifierValue>
    struct IsVararg<ModifierNamespace::Modifier<ModifierValue>> : std::integral_constant<bool, (ModifierValue&ModifierNamespace::VarargEnum)==ModifierNamespace::VarargEnum> {};

    template<typename T>
    struct IsAlias : std::integral_constant<bool, false> {};
    template<int ModifierValue>
    struct IsAlias<ModifierNamespace::Modifier<ModifierValue>> : std::integral_constant<bool, (ModifierValue&ModifierNamespace::AliasEnum)==ModifierNamespace::AliasEnum> {};

    template<typename T>
    struct IsExplicit : std::integral_constant<bool, false> {};
    template<int ModifierValue>
    struct IsExplicit<ModifierNamespace::Modifier<ModifierValue>> : std::integral_constant<bool, (ModifierValue&ModifierNamespace::ExplicitEnum)==ModifierNamespace::ExplicitEnum> {};

    template<typename T>
    struct IsAllocatorAsLastArgument : std::integral_constant<bool, false> {};
    template<int ModifierValue>
    struct IsAllocatorAsLastArgument<ModifierNamespace::Modifier<ModifierValue>> : std::integral_constant<bool, (ModifierValue&ModifierNamespace::AllocatorAsLastArgumentEnum)==ModifierNamespace::AllocatorAsLastArgumentEnum> {};
}}}}
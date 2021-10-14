#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Classes/Public.h"
#include "Reflection/Types/TypeTraits.h"
#include "Reflection/AutoReflect.h"
#include "Reflection/TypeConversions/DynamicTypeDeduction.h"

#include <memory>
#include "boost\mpl\not.hpp"
#include "boost\mpl\or.hpp"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Classes {
    template <class T>
    class IsAllocator
    {
        typedef char yes;
        typedef long no;

        // you can extend this with many more checks on the allocator interface
        template <class C> static yes check(typename C::template rebind<void>::other*);
        template <class C> static no  check(...);
    public:
        enum { value = sizeof(check<T>(nullptr)) == sizeof(yes) };
    };
    template<typename Reflected>
    class DynamicTypeDeduction {
    public:
        static void Add(const TypeConversions::ConversionGraphPointer& conversionGraph)
        {
            TypeConversions::AddDynamicTypeDeduction<Reflected>(conversionGraph);
        }
    };
    class NoDynamicTypeDeduction {
    public:
        static void Add(const TypeConversions::ConversionGraphPointer& conversionGraph) {}
    };
    template<typename T, typename EnableIf = void, typename... Args>
    struct ArgumentPack
    {
        static_assert(sizeof...(Args)==0,
            "Your type, TypeToBeChecked, is invalid:\n"
            "It can only be:\n"
            "- an allocator\n"
            "- a pointer or smart pointer\n"
            "- a Public<> declaration.\n"
            "This is typically a problem when defining a Class<>");
            using AllocatorType = std::allocator<void>;
            using HolderType = T*;
            using ConstructedObjectType = T*;
            using PublicType = Public<>;
            using AutoReflectorType = AutoReflector;
            using DynamicTypeDeductionAdder = DynamicTypeDeduction<T>;
    };
    template<typename Reflected, typename... Args>
    struct VerifyArgumentPack : public ArgumentPack<Reflected, void, Args...>
    {

    };
    template<typename Reflected, typename TypeToBeChecked, typename... Args>
    struct VerifyArgumentPack<Reflected, TypeToBeChecked, Args...> : public ArgumentPack<Reflected, void, TypeToBeChecked, Args...>
    {

    };

    template<typename T, typename... Args>
    struct ArgumentPack<T, void, NoDynamicTypeDeduction, Args...>
        : VerifyArgumentPack<T, Args...>
    {
        using DynamicTypeDeductionAdder = NoDynamicTypeDeduction;
    };

    template<typename T, typename PointerT, typename... Args>
    struct ArgumentPack<T, std::enable_if_t<Types::IsPointerWrapper<PointerT>::value, void>, PointerT, Args...>
        : VerifyArgumentPack<T, Args...>
    {
        using HolderType = PointerT;
        using ConstructedObjectType = T*;
    };

    template<typename T, typename... Args>
    struct ArgumentPack<T, void, T, Args...>
        : VerifyArgumentPack<T, Args...>
    {
        using HolderType = T;
        using ConstructedObjectType = T;
    };

    template<typename T, typename PublicT, typename... Args>
    struct ArgumentPack<T, std::enable_if_t<IsPublic<PublicT>::value, void>, PublicT, Args...>
        : VerifyArgumentPack<T, Args...>
    {
        using PublicType = PublicT;
    };

    template<typename T, typename AllocatorT, typename... Args>
    struct ArgumentPack<T, std::enable_if_t<IsAllocator<AllocatorT>::value, void>, AllocatorT, Args...>
        : VerifyArgumentPack<T, Args...>
    {
        using AllocatorType = AllocatorT;
    };

    template<typename T, typename ReflectorT, typename... Args>
    struct ArgumentPack<T, std::enable_if_t<std::is_base_of_v<AutoReflectorBase, ReflectorT>,void>, ReflectorT, Args...>
        : VerifyArgumentPack<T, Args...>
    {
        using AutoReflectorType = ReflectorT;
    };
    ///The void argument in the template here is for allowing enable_if
    template<typename Reflected, typename... Args>
    struct ClassArgumentPack : VerifyArgumentPack<Reflected, Args...>
    {
    };

}}}}
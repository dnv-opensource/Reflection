#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Types/DeduceTypeTag.h"
#include "TypeLibraries/ITypeLibrary.h"
#include "TypeConversions/IConversionGraph.h"
#include "Variants/VariantService.h"
#include <type_traits>

namespace DNVS { namespace MoFa { namespace Reflection {    
    template<typename PointerT>
    std::enable_if_t<std::is_polymorphic_v<PointerT>> ConditionallySetDynamicType(Variants::Variant& from, PointerT* pointer, const TypeLibraries::TypeLibraryPointer& typeLibrary, Types::PointerTag)
    {
        if (!typeLibrary->GetConversionGraph()->HasDynamicConversion(typeid(PointerT)))
        {
            from.SetDecoratedTypeInfo(Types::DecoratedTypeInfo(typeid(*pointer), Types::DecoratedTypeDeducer<PointerT*>::value));
            from.SetData(const_cast<void*>(dynamic_cast<const void*>(pointer)));
        }
    }

    template<typename SmartPointerT>
    void ConditionallySetDynamicType(Variants::Variant& from, SmartPointerT&& smartPointer, const TypeLibraries::TypeLibraryPointer& typeLibrary, Types::SmartPointerTag)
    {
        ConditionallySetDynamicType(from, get_pointer(std::forward<SmartPointerT>(smartPointer)), typeLibrary, Types::PointerTag());
    }

    template<typename T>
    void ConditionallySetDynamicType(Variants::Variant& from, T&& other, const TypeLibraries::TypeLibraryPointer& typeLibrary, Types::AnyTag)
    {
    }

    inline bool reflection_is_convertible(const Variants::Variant& from, const Types::DecoratedTypeInfo& to, const TypeLibraries::TypeLibraryPointer& typeLibrary)
    {
        auto conversionSequence = typeLibrary->GetConversionGraph()->GetConversionSequence(from, to);
        if (!conversionSequence)
            return false;
        return conversionSequence->IsValid();
    }

    inline bool reflection_is_convertible(Variants::Variant& from, const Types::DecoratedTypeInfo& to, const TypeLibraries::TypeLibraryPointer& typeLibrary)
    {
        if (!typeLibrary || !typeLibrary->GetConversionGraph())
            return false;
        auto conversionSequence = typeLibrary->GetConversionGraph()->GetConversionSequence(from, to);
        if (!conversionSequence)
            return false;
        return conversionSequence->IsValid();
    }

    template<typename From>
    bool reflection_is_convertible(From&& from, const Types::DecoratedTypeInfo& to, const TypeLibraries::TypeLibraryPointer& typeLibrary)
    {
        auto variant = Variants::VariantService().ReflectType<From>(std::forward<From>(from));
        ConditionallySetDynamicType(variant, from, typeLibrary, Types::TypeTag<From>());
        return reflection_is_convertible(variant, to, typeLibrary);
    }

    template<typename To, typename From>
    bool reflection_is_convertible(From&& from, const TypeLibraries::TypeLibraryPointer& typeLibrary)
    {
        return reflection_is_convertible(std::forward<From>(from), Types::TypeId<To>(), typeLibrary);
    }

    template<typename From>
    bool reflection_is_convertible(From&& from, const Types::DecoratedTypeInfo& to)
    {
        return reflection_is_convertible(std::forward<From>(from), to, Services::ServiceProvider::Instance().GetService<TypeLibraries::ITypeLibrary>());
    }

    template<typename To, typename From>
    bool reflection_is_convertible(From&& from)
    {
        return reflection_is_convertible<To>(std::forward<From>(from), Services::ServiceProvider::Instance().GetService<TypeLibraries::ITypeLibrary>());
    }
}}}
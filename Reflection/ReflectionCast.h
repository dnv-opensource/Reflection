#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ReflectionTraits.h"
#include <stdexcept>
#include <string>

namespace DNVS { namespace MoFa { namespace Reflection {    

    template<typename To, typename From>
    To reflection_cast(From&& from, const TypeLibraries::TypeLibraryPointer& typeLibrary)
    {
        auto variant = Variants::VariantService::ReflectType<From>(std::forward<From>(from));
        ConditionallySetDynamicType(variant, from, typeLibrary, Types::TypeTag<From>());
        auto conversionSequence = typeLibrary->GetConversionGraph()->GetConversionSequence(variant, Types::TypeId<To>());
        if (!conversionSequence || !conversionSequence->IsValid())
            throw std::runtime_error("Unable to convert from '" + std::string(typeid(From).name()) + "' to '" + std::string(typeid(To).name()) + ",");
        conversionSequence->IntrusiveConvert(variant);
        return Variants::InternalVariantService::UnreflectUnchecked<To>(variant);
    }

    template<typename To, typename From>
    To reflection_cast(From&& from)
    {
        return reflection_cast<To>(std::forward<From>(from), Services::ServiceProvider::Instance().GetService<TypeLibraries::ITypeLibrary>());
    }

}}}

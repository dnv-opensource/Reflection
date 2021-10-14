#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "AliasingSmartPointerStorage.h"
#include "MovableSmartPointerStorage.h"
#include "CopyableSmartPointerStorage.h"
#include "Reflection/Types/DeduceTypeTag.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Variants {
    template<typename T>
    bool IsVariantOfThisSmartPointerTypeImpl(const Variant& variant, Types::AliasingSmartPointerTag)
    {
        return AliasingSmartPointerSelector<T>::IsConvertible(variant.GetStorage());
    }

    template<typename T>
    bool IsVariantOfThisSmartPointerTypeImpl(const Variant& variant, Types::MovableSmartPointerTag)
    {
        return MovableSmartPointerStorage<T>::IsConvertible(variant.GetStorage());
    }

    template<typename T>
    bool IsVariantOfThisSmartPointerTypeImpl(const Variant& variant, Types::CopyableSmartPointerTag)
    {        
        return CopyableSmartPointerSelector<T>::IsConvertible(variant.GetStorage(), true);
    }

    template<typename T>
    bool IsVariantOfThisSmartPointerType(const Variant& variant)
    {
        return IsVariantOfThisSmartPointerTypeImpl<T>(variant, Types::TypeTag<T>());
    }
}}}}
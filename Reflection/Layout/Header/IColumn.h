#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"
#include "Reflection/Types/DecoratedTypeInfo.h"
#include "Reflection/Objects/Object.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Layout {
    class IColumn {
    public:
        virtual ~IColumn() {}
        virtual std::string GetHeader(TypeLibraries::TypeLibraryPointer typeLibrary, const Types::DecoratedTypeInfo& parentType, bool withPostfix = false) const = 0;
        virtual std::string GetDocumentation() const = 0;
        virtual Objects::Object Query(const Objects::Object& data) const = 0;
        virtual Types::DecoratedTypeInfo GetType(TypeLibraries::TypeLibraryPointer typeLibrary, const Types::DecoratedTypeInfo& parentType) const = 0;
        virtual Variants::Variant GetDefault() const = 0;
    };
}}}}


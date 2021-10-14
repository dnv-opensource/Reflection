//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DefaultColumn.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Layout {

    DefaultColumn::DefaultColumn(const std::string& header)
        : GenericColumn<DefaultColumn>(header)
    {
    }

    Objects::Object DefaultColumn::Query(const Objects::Object& data) const
    {
        return data;
    }

    Types::DecoratedTypeInfo DefaultColumn::GetType(TypeLibraries::TypeLibraryPointer typeLibrary, const Types::DecoratedTypeInfo& parentType) const
    {
        return parentType;
    }

}}}}



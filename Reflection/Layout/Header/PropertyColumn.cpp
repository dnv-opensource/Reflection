//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "PropertyColumn.h"
#include "Reflection/Members/IMember.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Layout {

    PropertyColumn::PropertyColumn(const std::string& propertyName, const std::string& header) 
        : GenericColumn<PropertyColumn>(header.empty() ? propertyName : header)
        , m_propertyName(propertyName)
    {

    }

    Objects::Object PropertyColumn::Query(const Objects::Object& data) const
    {
        return data.Invoke(m_propertyName, {});
    }

    Types::DecoratedTypeInfo PropertyColumn::GetType(TypeLibraries::TypeLibraryPointer typeLibrary, const Types::DecoratedTypeInfo& parentType) const
    {
        auto type = typeLibrary->LookupType(parentType.GetTypeInfo());
        if (type)
        {
            auto property = type->Lookup(m_propertyName, Variants::Variant());
            if (property)
                return property->GetReturnType();
        }
        return Types::TypeId<void>();
    }

}}}}



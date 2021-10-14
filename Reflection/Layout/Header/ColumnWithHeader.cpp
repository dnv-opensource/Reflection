//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ColumnWithHeader.h"
#include "Reflection/Attributes/PostfixAttribute.h"
#include "Reflection/Attributes/AttributeCollection.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Layout {

    ColumnWithHeader::ColumnWithHeader(const std::string& header) 
        : m_header(header)
    {

    }

    std::string ColumnWithHeader::GetHeader(TypeLibraries::TypeLibraryPointer typeLibrary, const Types::DecoratedTypeInfo& parentType, bool withPostfix) const
    {
        if (withPostfix)            
        {
            auto type = typeLibrary->LookupType(GetType(typeLibrary, parentType).GetTypeInfo());
            if (type && type->GetAttributeCollection().HasAttribute<Attributes::PostfixAttribute>())
            {
                return m_header + " [" + type->GetAttributeCollection().GetAttribute<Attributes::PostfixAttribute>().GetPostfix() + "]";
            }
        }
        return m_header;
    }

    std::string ColumnWithHeader::GetDocumentation() const
    {
        return m_documentation;
    }

    Variants::Variant ColumnWithHeader::GetDefault() const
    {
        return m_default;
    }

    void ColumnWithHeader::SetDocumentation(const std::string& documentation)
    {
        m_documentation = documentation;
    }

    void ColumnWithHeader::SetDefault(const Variants::Variant& object)
    {
        m_default = object;
    }

}}}}



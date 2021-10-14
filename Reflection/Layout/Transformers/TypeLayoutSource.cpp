//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypeLayoutSource.h"
#include "Reflection/Attributes/NameAttribute.h"
#include "Reflection/Attributes/AttributeCollection.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    TypeLayoutSource::TypeLayoutSource(const std::string& name, const Types::DecoratedTypeInfo& info, const Attributes::AttributeCollection& attributeCollection)
        : m_name(name)
        , m_attributeCollection(attributeCollection)
        , m_info(info)
    {

    }

    std::string TypeLayoutSource::GetName() const
    {
        return m_name;
    }

    const Attributes::AttributeCollection& TypeLayoutSource::GetAttributeCollection() const
    {
        return m_attributeCollection;
    }

    Types::DecoratedTypeInfo TypeLayoutSource::GetTypeInfo() const
    {
        return m_info;
    }

    Members::MemberPointer TypeLayoutSource::GetGetter() const
    {
        return nullptr;
    }

    Members::MemberPointer TypeLayoutSource::GetSetter() const
    {
        return nullptr;
    }

    std::string TypeLayoutSource::GetCaption(const TypeLibraries::TypeLibraryPointer& typeLibrary, const std::string& controlName, const std::string& controlCaption) const
    {
        if (!AllowEmbedElement() && controlCaption == "__USENAME__" && GetAttributeCollection().HasAttribute<Attributes::NameAttribute>())
            return GetAttributeCollection().GetAttribute<Attributes::NameAttribute>().GetCaption();
        return LayoutSource::GetCaption(typeLibrary, controlName, controlCaption);
    }

    bool TypeLayoutSource::AllowEmbedElement() const
    {
        return m_name != "__this__";
    }

}}}}


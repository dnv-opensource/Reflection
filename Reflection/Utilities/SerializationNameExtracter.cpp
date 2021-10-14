//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "SerializationNameExtracter.h"
#include "Reflection/Members/IMember.h"
#include "Reflection/TypeLibraries/ITypeLibrary.h"
#include "Reflection/Attributes/AttributeCollection.h"
#include "Reflection/Attributes/SerializationAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {    

    SerializationNameExtracter::SerializationNameExtracter(TypeLibraries::TypeLibraryPointer typeLibrary)
        : m_typeLibrary(typeLibrary)
    {

    }

    SerializationNameExtracter::~SerializationNameExtracter()
    {

    }

    std::string SerializationNameExtracter::GetName(const Members::MemberPointer& member) const
    {
        if (!member)
            return "";
        if (member->GetAttributeCollection().HasAttribute<Attributes::SerializationAttribute>())
            return member->GetAttributeCollection().GetAttribute<Attributes::SerializationAttribute>().GetName();
        if (member->GetName() == Members::ConstructorString && m_typeLibrary)
        {
            auto type = m_typeLibrary->LookupType(member->GetReturnType().GetTypeInfo());
            if (type)
                return type->GetName();
        }
        return member->GetName();
    }

    std::string SerializationNameExtracter::GetName(const TypeLibraries::TypePointer& type) const
    {
        if (!type)
            return "";
        if (type->GetAttributeCollection().HasAttribute<Attributes::SerializationAttribute>())
            return type->GetAttributeCollection().GetAttribute<Attributes::SerializationAttribute>().GetName();
        return type->GetName();
    }

}}}}




//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IconAttribute.h"
#include "Reflection/TypeLibraries/IType.h"
#include "AttributeCollection.h"
#include "Reflection/Objects/Object.h"
#include "Reflection/Utilities/MemberItem.h"
#include "Reflection/Members/IMember.h"
#include "Reflection/Types/DynamicTypeTraits.h"
#include "Reflection/Enums/EnumHelper.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {

    const IconAttribute* TryGetIconAttributeFromObject(const Objects::Object& object)
    {
        TypeLibraries::TypePointer type = nullptr;

        //1. Try to get icon from member.
        if (object.IsConvertibleTo<const Utilities::MemberItem&>())
        {
            const Utilities::MemberItem& memberItem = object.As<const Utilities::MemberItem&>();
            if (memberItem.GetMember() && memberItem.GetMember()->GetAttributeCollection().HasAttribute<IconAttribute>())
            {
                return &memberItem.GetMember()->GetAttributeCollection().GetAttribute<IconAttribute>();
            }
            if (object.GetTypeLibrary() && memberItem.GetMember())
            {
                type = object.GetTypeLibrary()->LookupType(memberItem.GetMember()->GetReturnType().GetTypeInfo());
            }
        }

        else if(object.IsConvertibleTo<TypeLibraries::TypePointer>())
            type = object.As<TypeLibraries::TypePointer>();
        else if(!type)
            type = object.ConvertToDynamicType().GetType();

        if (type && Types::IsEnum(type->GetType()))
        {
            auto member = Enums::EnumHelper(type).GetMember(object);
            if (member && member->GetAttributeCollection().HasAttribute<IconAttribute>())
            {
                return &member->GetAttributeCollection().GetAttribute<IconAttribute>();
            }
        }
        //Try to get icon from type
        if (type && type->GetAttributeCollection().HasAttribute<IconAttribute>())
            return &type->GetAttributeCollection().GetAttribute<IconAttribute>();
        return nullptr;
    }

}}}}



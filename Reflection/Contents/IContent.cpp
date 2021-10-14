//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IContent.h"
#include "Reflection/Members/IMember.h"
#include "ObjectContent.h"
#include "Reflection/Utilities/ArgumentFromObjectExtracter.h"
#include "Reflection/Utilities/ConstructorSelector.h"
#include "Reflection/Utilities/CallableConstructor.h"
#include "Reflection/TypeLibraries/ITypeLibrary.h"
#include "TypedTextContent.h"

namespace DNVS {namespace MoFa {namespace Reflection { namespace Contents { 

    bool IContent::IsTextEqual(const IContent& other) const
    {
        return GetDisplayText() == other.GetDisplayText();
    }

    std::shared_ptr<IContent> IContent::TryChangeType(const Types::DecoratedTypeInfo& typeInfo, const std::shared_ptr<IContent>& existingContent) const
    {
        return TypedTextContent::Parse(GetDisplayText(), typeInfo, existingContent);
    }

    std::shared_ptr<IContent> IContent::TryChangeMember(const Members::MemberPointer& member, const std::shared_ptr<IContent>& existingContent /*= nullptr*/) const
    {
        return nullptr;
    }

    Members::MemberPointer IContent::TryGetBestMember() const
    {
        return Utilities::ConstructorSelector(GetObject().ConvertToDynamicType()).GetBestConstructor().GetMember();
    }

    Members::MemberPointer IContent::GetStoredMember() const
    {
        return nullptr;
    }

}}}}


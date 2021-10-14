//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutSource.h"
#include "FunctionLayoutSourceProvider.h"
#include "Reflection/Attributes/AttributeCollection.h"
#include "Reflection/Attributes/CaptionAttribute.h"
#include "Reflection/TypeLibraries/ITypeLibrary.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Layout {

    std::string LayoutSource::GetCaption(const TypeLibraries::TypeLibraryPointer& typeLibrary, const std::string& controlName, const std::string& controlCaption) const
    {
        if (controlCaption == "__USENAME__" && !controlName.empty() && GetName() != controlName)
            return controlName;
        if (GetAttributeCollection().HasAttribute<Attributes::CaptionAttribute>())
            return GetAttributeCollection().GetAttribute<Attributes::CaptionAttribute>().GetCaption();
        if (controlCaption == "__USENAME__")
        {
            if (controlName == "__this__" && typeLibrary)
            {
                auto type = typeLibrary->LookupType(GetTypeInfo().GetTypeInfo());
                if (type)
                    return type->GetName();
            }
            return controlName;
        }
        return controlCaption;
    }

    LayoutContext LayoutSource::GetContext(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Members::MemberPointer& member, Types::DecoratedTypeInfo& parentTypeInfo) const
    {
        LayoutContext context;
        context.setter = GetSetter();
        context.getter = GetGetter();
        context.additionalArguments = GetAdditionalArguments();
        if (FunctionLayoutSourceProvider(typeLibrary, member).LookupLayoutSource(GetName()))
            context.function = member;
        context.type = GetTypeInfo();
        context.parentType = parentTypeInfo;
        return context;
    }

    bool LayoutSource::AllowEmbedElement() const
    {
        return true;
    }

    std::map<std::string, Variants::Variant, TypeUtilities::CompareCaseLess> LayoutSource::GetAdditionalArguments() const
    {
        return {};
    }

}}}} // namespace Layout

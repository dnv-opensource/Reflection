#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Members/IMember.h"
#include "Reflection/Attributes/AttributeCollection.h"
#include "TypeUtilities/CompareCaseLess.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class LayoutContext {
    public:
        Members::MemberPointer function;
        Members::MemberPointer getter;
        Members::MemberPointer setter;
        Types::DecoratedTypeInfo type;
        Types::DecoratedTypeInfo parentType;
        std::map<std::string, Variants::Variant, TypeUtilities::CompareCaseLess> additionalArguments;
        const Attributes::AttributeCollection& GetPropertyAttributeCollection() const
        {
            if (getter && &getter->GetAttributeCollection() != &Attributes::AttributeCollection::GetEmptyCollection())
                return getter->GetAttributeCollection();
            if (setter && &setter->GetAttributeCollection() != &Attributes::AttributeCollection::GetEmptyCollection())
                return setter->GetAttributeCollection();
            return Attributes::AttributeCollection::GetEmptyCollection();
        }
    };
}}}}

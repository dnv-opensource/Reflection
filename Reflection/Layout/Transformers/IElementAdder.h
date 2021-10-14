#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <memory>
#include "Reflection/Layout/ILayoutElement.h"
#include "Reflection/Attributes/AttributeCollection.h"
#include "Reflection/Members/IMemberFwd.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class IElementAdder {
    public:
        virtual ~IElementAdder() {}
        virtual void AddElement(const LayoutElementPointer& element, const Attributes::AttributeCollection& attributeCollection) = 0;
        virtual bool HasMember(const Members::MemberPointer& member) const = 0;
        virtual LayoutElementPointer GetLayoutElement() const = 0;
    };
}}}}
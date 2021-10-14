#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Layout/AutoControl.h"
#include "Reflection/Members/IMember.h"
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"
#include "Reflection/Attributes/AttributeCollection.h"
#include "Reflection/Config.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class OverloadSelector;
#pragma warning(push)
#pragma warning(disable:4275)
    class REFLECTION_IMPORT_EXPORT LayoutTransformer : virtual public TypeUtilities::IVisitor
    {
    public:
        LayoutTransformer(const TypeLibraries::TypeLibraryPointer& typeLibrary, bool isNested = false);
        void ChangeControlBasedOnType(const AutoControl& control, const Types::DecoratedTypeInfo& typeInfo);
        LayoutElementPointer TryEmbedElement(LayoutElementPointer element, const Attributes::AttributeCollection& attributeCollection);
        void AddMembersToOverloadSelector(OverloadSelector& selector, const Attributes::AttributeCollection& attributeCollection);
        LayoutElementPointer TryEmbedGroup(LayoutElementPointer element, const Attributes::AttributeCollection& attributeCollection);
        void SetAllowEmbedGroup(bool flag);
        void SetAllowRadioButtons(bool flag);
    protected:
        bool m_allowRadioButtons;
        TypeLibraries::TypeLibraryPointer m_typeLibrary;
        bool m_allowEmbedGroup;
        bool m_isNested;
    private:
        LayoutElementPointer TryAddOverloadSelector(LayoutElementPointer element, const Attributes::AttributeCollection& attributeCollection);
    };
#pragma warning(pop)

}}}}

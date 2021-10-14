//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutTransformer.h"
#include "Reflection/Layout/LayoutBuilder.h"

#include "Reflection/Types/DynamicTypeTraits.h"
#include "Reflection/TypeLibraries/ITypeLibrary.h"

#include "Reflection/Attributes/EnumerableAttribute.h"
#include "Reflection/Attributes/ContainerAttribute.h"
#include "Reflection/Attributes/ImageAttribute.h"
#include "Reflection/Attributes/GroupAttribute.h"
#include "Reflection/Attributes/SymmetryAttribute.h"
#include "Reflection/Attributes/LayoutAttribute.h"
#include "Reflection/Attributes/ConstructorSelectorAttribute.h"
#include "Reflection/Attributes/ExpandMemberAttribute.h"
#include "Reflection/Attributes/ContextAttribute.h"
#include "Reflection/Attributes/TreatAsBoolAttribute.h"
#include "Reflection/Attributes/GroupEnablerAttribute.h"
#include "Reflection/Attributes/AttributeCollectionService.h"
#include "Reflection/Utilities/ConstructorsFromTypeService.h"
#include "Reflection/Utilities/ConstructorsFromTypeExtracter.h"


namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    using namespace Attributes;

    LayoutTransformer::LayoutTransformer(const TypeLibraries::TypeLibraryPointer& typeLibrary, bool isNested)
        : m_typeLibrary(typeLibrary)
        , m_allowEmbedGroup(true)
        , m_isNested(isNested)
        , m_allowRadioButtons(true)
    {
    }

    void LayoutTransformer::ChangeControlBasedOnType(const AutoControl& control, const Types::DecoratedTypeInfo& typeInfo)
    {
        auto type = m_typeLibrary->LookupType(typeInfo.GetTypeInfo());
        if(const LayoutAttribute* attribute = GetPointerToAttributeOrNull<LayoutAttribute>(control.GetAttributeCollection(), type))
            return attribute->GetLayout()->Accept(*this);
        if (const TreatAsBoolAttribute* attribute = GetPointerToAttributeOrNull<TreatAsBoolAttribute>(control.GetAttributeCollection(), type))
            return CheckBox(control).Accept(*this);
        if (Types::IsSameAs(typeInfo, typeid(bool)))
            return CheckBox(control).Accept(*this);
        if (Types::IsEnum(typeInfo) && m_allowRadioButtons)
            return RadioButtons(control).Accept(*this);
        if (const EnumerableAttribute* attribute = GetPointerToAttributeOrNull<EnumerableAttribute>(control.GetAttributeCollection(), type))
            return ComboBox(control).Accept(*this);
        if (const ContainerAttribute* attribute = GetPointerToAttributeOrNull<ContainerAttribute>(control.GetAttributeCollection(), type))
            return Table(control).Accept(*this);
        return TextBox(control).Accept(*this);
    }
    class PrefixAdder : public TypeUtilities::Visitors
        <
        ControlElement,
        CollectionOfElements,
        ElementWithOneChild
        >
    {
    public:
        PrefixAdder(const std::string& prefix) : m_prefix(prefix) {}
        virtual void Visit(const ControlElement& control) override
        {
            const_cast<ControlElement&>(control).SetCaption(m_prefix + control.GetCaption());
        }
        virtual void Visit(const CollectionOfElements& control) override
        {
            for (const auto& ctrl : control)
                ctrl->Accept(*this);
        }
        virtual void Visit(const ElementWithOneChild& control) override
        {
            control.GetChild()->Accept(*this);
        }
    private:
        std::string m_prefix;
    };

    LayoutElementPointer LayoutTransformer::TryEmbedElement(LayoutElementPointer element, const Attributes::AttributeCollection& attributeCollection)
    {
        if (attributeCollection.HasAttribute<ExpandMemberAttribute>())
        {
            if (attributeCollection.GetAttribute<ExpandMemberAttribute>().CanExpand())
                element = TryAddOverloadSelector(element, attributeCollection);
        }
        else if (auto control = std::dynamic_pointer_cast<ControlElement>(element))
        {
            if (control->GetContext().type.GetTypeInfoPointer())
            {
                auto type = m_typeLibrary->LookupType(control->GetContext().type.GetTypeInfo());
                if (type && type->GetAttributeCollection().HasAttribute<ExpandMemberAttribute>() && type->GetAttributeCollection().GetAttribute<ExpandMemberAttribute>().CanExpand())
                {
                    element = TryAddOverloadSelector(element, attributeCollection);
                }
            }
        }
        if (attributeCollection.HasAttribute<ImageAttribute>())
        {
            const auto& imageAttribute = attributeCollection.GetAttribute<ImageAttribute>();
            Image image(imageAttribute.GetImageId(), imageAttribute.GetModule());
            switch (imageAttribute.GetImageSide())
            {
            case ImageAttribute::Top:
                element = MakeElement(VerticalElements(image, element));
                break;
            case ImageAttribute::Bottom:
                element = MakeElement(VerticalElements(element, image));
                break;
            case ImageAttribute::Left:
                element = MakeElement(HorizontalElements(image, element));
                break;
            case ImageAttribute::Right:
                element = MakeElement(HorizontalElements(element, image));
                break;
            }
        }
        element = TryEmbedGroup(element, attributeCollection);
        return element;
    }

    void LayoutTransformer::AddMembersToOverloadSelector(OverloadSelector& selector, const Attributes::AttributeCollection& attributeCollection)
    {
        auto members = Utilities::ConstructorsFromTypeService::GetService(m_typeLibrary)->CollectAllConstructorMembersOfThisAndDerivedClasses(selector.GetContext().type, false);
        if (attributeCollection.HasAttribute<ConstructorSelectorAttribute>())
            members = attributeCollection.GetAttribute<ConstructorSelectorAttribute>().GetMembers(m_typeLibrary, members);

        std::set<Members::MemberPointer> filteredMembers;
        for (const auto& member : members)
        {
            if (member->GetAttributeCollection().HasAttribute<ContextAttribute>())
            {
                if (attributeCollection.HasAttribute<ContextAttribute>() && attributeCollection.GetAttribute<ContextAttribute>().SharedContext(member->GetAttributeCollection().GetAttribute<ContextAttribute>()))
                    filteredMembers.insert(member);
            }
            else
                filteredMembers.insert(member);
        }
        selector.SetMembers(filteredMembers);
    }

    LayoutElementPointer LayoutTransformer::TryEmbedGroup(LayoutElementPointer element, const Attributes::AttributeCollection& attributeCollection)
    {
        if (!m_allowEmbedGroup)
            return element;
        if (attributeCollection.HasAttribute<GroupAttribute>())
        {
            auto groupBox = std::make_shared<GroupBox>(attributeCollection.GetAttribute<GroupAttribute>().GetGroup(), element);
            groupBox->AddDocumentation(attributeCollection.GetAttribute<GroupAttribute>().GetDocumentation());
            if (attributeCollection.HasAttribute<GroupEnablerAttribute>())
            {
                const auto& groupEnabler = attributeCollection.GetAttribute<GroupEnablerAttribute>();
                if (groupEnabler.HasFunction())
                {
                    groupBox->SetEnabler(std::make_shared<CheckBox>());
                    groupBox->GetEnabler()->SetCaption(groupBox->GetGroupName());
                    groupBox->GetEnabler()->SetAttributeCollection(attributeCollection);
                }
                else
                {
                    if (auto checkBox = std::dynamic_pointer_cast<CheckBox>(element))
                    {
                        groupBox->SetEnabler(checkBox);
                        groupBox->GetEnabler()->SetCaption(groupBox->GetGroupName());
                        groupBox->Clear();
                    }
                    else
                    {
                        throw std::runtime_error("element of a group box with a GroupEnablerAttribute must represent a boolean value.");
                    }
                }
            }
            element = groupBox;
        }
        if (attributeCollection.HasAttribute<SymmetryAttribute>())
        {
            const auto& symmetry = attributeCollection.GetAttribute<SymmetryAttribute>();
            element = std::make_shared<Symmetry>(symmetry.GetMasterProperty(), symmetry.GetSymmetryName(), symmetry.GetDocumentation(), element);
        }
        return element;
    }

    void LayoutTransformer::SetAllowEmbedGroup(bool flag)
    {
        m_allowEmbedGroup = flag;
    }

    void LayoutTransformer::SetAllowRadioButtons(bool flag)
    {
        m_allowRadioButtons = flag;
    }

    LayoutElementPointer LayoutTransformer::TryAddOverloadSelector(LayoutElementPointer element, const Attributes::AttributeCollection& attributeCollection)
    {
        if (auto controlElement = std::dynamic_pointer_cast<ControlElement>(element))
        {
            OverloadSelector selector(controlElement->GetName(),
                VerticalElements(
                    HorizontalElements(Selector(DropdownButton()), element),
                    Collapsable(
                        GroupBox("", DynamicArea())
                    )
                )
            );
            selector.GetContext() = controlElement->GetContext();
            AddMembersToOverloadSelector(selector, attributeCollection);
            element = MakeElement(std::move(selector));
        }
        return element;
    }

}}}}



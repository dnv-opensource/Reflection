//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Members/IMember.h"
#include "Reflection/Attributes/LayoutAttribute.h"
#include "Reflection/Attributes/IncludeInViewAttribute.h"
#include "Reflection/Utilities/MemberItem.h"
#include "AutoControl.h"
#include "LayoutBuilder.h"
#include "Transformers/PropertyLayoutTransformer.h"
#include "Transformers/PropertyLayoutSource.h"
#include "Transformers/TypeLayoutSource.h"
#include "Transformers/FunctionLayoutSourceProvider.h"
#include "EmbeddedElement.h"
#include "Transformers/CombinedLayoutSourceProvider.h"
#include "Transformers/MembersLayoutSourceProvider.h"
#include "Reflection/Attributes/NameAttribute.h"
#include "Reflection/Attributes/ContextAttribute.h"
#include "Reflection/Attributes/TabAttribute.h"
#include "Reflection/Attributes/RecordAttribute.h"
#include "Reflection/Attributes/IncludeInViewAttribute.h"
#include "Reflection/Attributes/AttributeCollectionService.h"
#include "Transformers/TabControlElementAdder.h"
#include "Transformers/VerticalElementAdder.h"
#include "Transformers/ConstantLayoutSourceProvider.h"
#include "Transformers/RecordIndexLayoutSource.h"
#include "Transformers/NestedPropertyHandler.h"


namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    using namespace Attributes;
    LayoutBuilder::LayoutBuilder(const TypeLibraries::TypeLibraryPointer& typeLibrary, bool isNested)
        : m_typeLibrary(typeLibrary)
        , m_isNested(isNested)
    {
    }

    LayoutElementPointer LayoutBuilder::CreateLayoutFromProperty(Members::MemberPointer property, Members::MemberPointer function)
    {
        if (!property)
            return nullptr;
        //Verify that this is actually a property
        if ((property->GetMemberType()&Members::MemberType::TypePropertyGetSet) == 0)
            return nullptr;
        if (property->GetMemberType()&Members::MemberType::TypePropertyGet && property->GetMinimumArgumentCount() == 0)
            return nullptr;
        std::shared_ptr<ILayoutSourceProvider> provider;
        if (function)
        {
            provider = std::make_shared<CombinedLayoutSourceProvider>(
                std::make_shared<MemberLayoutSourceProvider>(m_typeLibrary, function->GetReturnType()),
                std::make_shared<ConstantLayoutSourceProvider>(std::make_shared<PropertyLayoutSource>(property)));
        }
        else
            provider = std::make_shared<ConstantLayoutSourceProvider>(std::make_shared<PropertyLayoutSource>(property));
        PropertyLayoutTransformer transformer(m_typeLibrary, provider, true, true, m_isNested);
        transformer.SetFunction(function);
        if(function)
            transformer.SetParentType(function->GetReturnType());
        AutoControl(property->GetName()).Accept(transformer);
        return transformer.GetLayoutElement();
    }

    LayoutElementPointer LayoutBuilder::CreateLayoutFromFunctionArgument(Members::MemberPointer function, size_t index)
    {
        std::shared_ptr<ILayoutSource> source;
        const auto argumentInfo = function->GetArgumentInfo(index);        
        auto typeInfo = argumentInfo->GetTypeInfo();

        const auto& attributeCollection = TryGetAttributeCollection(function, index);
        source = std::make_shared<TypeLayoutSource>(argumentInfo->GetName(), typeInfo, attributeCollection);

        PropertyLayoutTransformer transformer(m_typeLibrary, source, false, false, m_isNested);
        transformer.SetFunction(function);
        AutoControl(argumentInfo->GetName()).Accept(transformer);
        if(!attributeCollection.IsEmpty())
            return transformer.TryEmbedElement(transformer.GetLayoutElement(), attributeCollection);
        else
            return transformer.GetLayoutElement();
    }

    const Attributes::AttributeCollection& LayoutBuilder::TryGetAttributeCollection(Members::MemberPointer function, size_t index)
    {
        const auto argumentInfo = function->GetArgumentInfo(index);
        auto typeInfo = argumentInfo->GetTypeInfo();
        if (!argumentInfo->GetAttributeCollection().IsEmpty())
            return argumentInfo->GetAttributeCollection();
        if (auto type = m_typeLibrary->LookupType(typeInfo.GetTypeInfo()))
            return type->GetAttributeCollection();
        return AttributeCollection::GetEmptyCollection();
    }

    LayoutElementPointer LayoutBuilder::CreateLayoutFromType(const std::string& name, const Types::DecoratedTypeInfo& typeInfo, Members::MemberPointer function)
    {
        auto type = m_typeLibrary->LookupType(typeInfo.GetTypeInfo());
        if (type)
            return CreateLayoutFromType(name, typeInfo, type->GetAttributeCollection(), function);
        else
            return CreateLayoutFromType(name, typeInfo, AttributeCollection::GetEmptyCollection(), function);
    }

    Layout::LayoutElementPointer LayoutBuilder::CreateLayoutFromType(const std::string& name, const Types::DecoratedTypeInfo& typeInfo, const Attributes::AttributeCollection& typeCollection, Members::MemberPointer function /*= nullptr*/)
    {
        std::shared_ptr<ILayoutSource> source = std::make_shared<TypeLayoutSource>(name, typeInfo, typeCollection);
        PropertyLayoutTransformer transformer(m_typeLibrary, source, false, false, m_isNested);
        transformer.SetFunction(function);
        AutoControl(name).Accept(transformer);
        if (!function)
            return transformer.TryEmbedElement(transformer.GetLayoutElement(), typeCollection);
        else
            return transformer.GetLayoutElement();

    }

    LayoutElementPointer LayoutBuilder::CreateLayoutFromTypeUsingLayoutAttribute(const Types::DecoratedTypeInfo& typeInfo, const TypeLibraries::TypePointer& type)
    {
        PropertyLayoutTransformer transformer(m_typeLibrary, std::make_shared<MemberLayoutSourceProvider>(m_typeLibrary, typeInfo), true, true);
        transformer.SetParentType(typeInfo);
        type->GetAttributeCollection().GetAttribute<LayoutAttribute>().GetLayout()->Accept(transformer);
        if (transformer.GetLayoutElement())
            return transformer.GetLayoutElement();
        else
            return nullptr;
    }

    LayoutElementPointer LayoutBuilder::CreateLayoutFromTypeUsingElementAdder(const Types::DecoratedTypeInfo& typeInfo, const TypeLibraries::TypePointer& type, std::unique_ptr<IElementAdder> adder)
    {
        for (const auto& memberName : type->GetUnsortedMembers())
        {
            if (auto member = type->Lookup(memberName, Variants::Variant()))
            {
                if (member->GetMemberType() == Members::MemberType::TypePropertyGetSet)
                {
                    if(adder->HasMember(member))
                        continue;
                    PropertyLayoutTransformer transformer(m_typeLibrary, std::make_shared<MemberLayoutSourceProvider>(m_typeLibrary, typeInfo), true, true, m_isNested);
                    transformer.SetParentType(typeInfo);
                    AutoControl(member->GetName()).Accept(transformer);
                    if (transformer.GetLayoutElement())
                        adder->AddElement(transformer.GetLayoutElement(), member->GetAttributeCollection());
                }
            }
        }
        return adder->GetLayoutElement();
    }

    std::unique_ptr<IElementAdder> CreateElementAdder(const Attributes::AttributeCollection& collection, bool fallbackToDefault = true)
    {
        if (collection.HasAttribute<Attributes::TabAttribute>())
            return std::make_unique<TabControlElementAdder>(collection.GetAttribute<Attributes::TabAttribute>().GetName());
        else if (fallbackToDefault)
            return std::make_unique<VerticalElementAdder>();
        else
            return nullptr;
    }

    LayoutElementPointer LayoutBuilder::CreateLayoutFromType(const Types::DecoratedTypeInfo& typeInfo)
    {
        auto type = m_typeLibrary->LookupType(typeInfo.GetTypeInfo());
        if (type)
        {
            if (type->GetAttributeCollection().HasAttribute<LayoutAttribute>())
                return CreateLayoutFromTypeUsingLayoutAttribute(typeInfo, type);
            else 
                return CreateLayoutFromTypeUsingElementAdder(typeInfo, type, CreateElementAdder(type->GetAttributeCollection()));
        }
        return nullptr;
    }
    bool HasSharedContext(Members::MemberPointer prop, Members::MemberPointer masterFunction)
    {
        if (prop->GetAttributeCollection().HasAttribute<Attributes::ContextAttribute>())
        {
            if (!masterFunction->GetAttributeCollection().HasAttribute<Attributes::ContextAttribute>())
                return false;
            return prop->GetAttributeCollection().GetAttribute<Attributes::ContextAttribute>().SharedContext(masterFunction->GetAttributeCollection().GetAttribute<Attributes::ContextAttribute>());
        }
        return true;
    }


    LayoutElementPointer LayoutBuilder::CreateGridLayout(const Members::MemberPointer& function)
    {
        const RecordAttribute& recordAttribute = function->GetAttributeCollection().GetAttribute<RecordAttribute>();
        //Unable to create grid that is not 2*2 yet.
        if (recordAttribute.GetRanges().size() != 2)
            return nullptr;
        HorizontalElements columnHeader;
        auto columnHeaders = recordAttribute.GetRanges().back()->GetHeadersInRange(Variants::Variant());
        auto rowHeaders = recordAttribute.GetRanges().front()->GetHeadersInRange(Variants::Variant());
        auto rows = recordAttribute.GetRanges().front()->GetIndicesInRange(Variants::Variant());
        auto columns = recordAttribute.GetRanges().back()->GetIndicesInRange(Variants::Variant());
        columnHeader.PushBack(Caption(""));
        for (const std::string& header : columnHeaders)
            columnHeader.PushBack(Caption(header));
        int rowCount = (int)rowHeaders.size();
        VerticalElements fixedGrid;
        std::shared_ptr<NestedPropertyHandler> nestedPropertyHandler;
        if(!recordAttribute.GetAliases().empty())
            nestedPropertyHandler = std::make_shared<NestedPropertyHandler>(m_typeLibrary, m_typeLibrary->LookupType(function->GetArgumentInfo(0)->GetTypeInfo().GetTypeInfo()));
        for (size_t row = 0; row < rows.size(); ++row)
        {
            HorizontalElements fixedRow;
            fixedRow.PushBack(Caption(rowHeaders[row]));
            for(size_t col = 0; col < columns.size(); ++col)
            {
                auto source = std::make_shared<RecordIndexLayoutSource>(m_typeLibrary, *nestedPropertyHandler, function, rows[row], columns[col], rowHeaders[row], columnHeaders[col]);
                PropertyLayoutTransformer transformer(m_typeLibrary, source, false, false, m_isNested);
                transformer.SetFunction(function);
                transformer.SetAllowRadioButtons(false);
                AutoControl(source->GetName(), "").Accept(transformer);
                fixedRow.PushBack(transformer.GetLayoutElement());
            }
            fixedGrid.PushBack(fixedRow);
        }
        if (nestedPropertyHandler)
            nestedPropertyHandler->Finalize();
        auto grid = std::make_shared<Grid>(function->GetName(),"", columnHeader, fixedGrid, rowCount, false);
        grid->SetPropertyHandler(std::move(nestedPropertyHandler));
        grid->SetFunction(function);
        return grid;
    }

    bool LayoutBuilder::HasAlias(const TypeLibraries::TypePointer& type, const std::string& name, const Attributes::IncludeInViewAttribute* includeInView) const
    {
        if (!type || !includeInView)
            return false;
        for (const std::string& method : includeInView->GetMethods())
        {
            auto member = type->Lookup(method, Variants::Variant());
            if (auto recordAttribute = GetPointerToAttributeOrNull<RecordAttribute>(member))
            {
                for (const PropertyAlias& aliasAttribute : recordAttribute->GetAliases())
                {
                    if (aliasAttribute.IsAliasOf(name))
                        return true;
                }
            }
        }
        return false;
    }

    LayoutElementPointer LayoutBuilder::CreateLayoutFromFunctionArgumentOrProperty(Members::MemberPointer function, size_t index, const TypeLibraries::TypePointer& type, IElementAdder* adder, const Attributes::IncludeInViewAttribute* includeInView)
    {
        std::string name = function->GetArgumentInfo(index)->GetName();
        if (name.empty())
            return nullptr;
        if (HasAlias(type, name, includeInView))
            return nullptr;
        Members::MemberPointer member;
        if (type)
            member = type->Lookup(name, Variants::Variant());
        if(member && adder && adder->HasMember(member))
           return nullptr;
        LayoutElementPointer result;
        if (member)
            result = CreateLayoutFromProperty(member, function);
        if (!result)
            result = CreateLayoutFromFunctionArgument(function, index);
        if(result && adder)
            adder->AddElement(result, member ? member->GetAttributeCollection() : Attributes::AttributeCollection::GetEmptyCollection());
        return result;
    }

    LayoutElementPointer LayoutBuilder::CreateLayoutFromFunction(Members::MemberPointer function)
    {
        if (!function)
            return nullptr;
        if (function->GetAttributeCollection().HasAttribute<LayoutAttribute>())
        {
            PropertyLayoutTransformer transformer(m_typeLibrary, std::make_shared<CombinedLayoutSourceProvider>(                
                std::make_shared<MemberLayoutSourceProvider>(m_typeLibrary, function->GetReturnType()),
                std::make_shared<FunctionLayoutSourceProvider>(m_typeLibrary, function)),
                true, true, m_isNested);
            transformer.SetFunction(function);
            transformer.SetParentType(function->GetReturnType());
            function->GetAttributeCollection().GetAttribute<LayoutAttribute>().GetLayout()->Accept(transformer);
            return transformer.GetLayoutElement();
        }
        if (function->GetMemberType() == Members::MemberType::TypePropertyGet && function->GetMinimumArgumentCount() == 0)
            return nullptr;
        if(function->GetAttributeCollection().HasAttribute<RecordAttribute>())
        {
            return CreateGridLayout(function);    
        }

        Utilities::MemberItem memberItem(function, m_typeLibrary);
        TypeLibraries::TypePointer type = m_typeLibrary->LookupType(memberItem.GetTypeInfo().GetTypeInfo());
        std::unique_ptr<IElementAdder> adder = CreateElementAdder(function->GetAttributeCollection(), type == nullptr);
        if (!adder && type)
            adder = CreateElementAdder(type->GetAttributeCollection());
        auto includeInView = GetPointerToAttributeOrNull<IncludeInViewAttribute>(function);
        for (size_t i = 0; i < function->GetArity(); ++i)
        {
            CreateLayoutFromFunctionArgumentOrProperty(function, i, type, adder.get(), includeInView);
        }
        if (type)
        {
            if (memberItem.IsUtilityClassFunction())
            {
                for (const auto& unsortedMember : type->GetUnsortedMembers())
                {
                    if (auto member = type->Lookup(unsortedMember, Variants::Variant()))
                    {
                        if (member->GetMemberType() == Members::MemberType::TypePropertyGetSet)
                        {
                            if (HasSharedContext(member, function))
                                adder->AddElement(CreateLayoutFromProperty(member, nullptr), member->GetAttributeCollection());
                        }
                    }
                }
            }
            else if (includeInView)
            {
                for (const std::string& memberName : includeInView->GetMethods())
                {
                    if (auto member = type->Lookup(memberName, Variants::Variant()))
                    {
                        if (member->GetMemberType() == Members::MemberType::TypePropertyGetSet)
                            adder->AddElement(CreateLayoutFromProperty(member, nullptr), member->GetAttributeCollection());
                        else 
                            adder->AddElement(CreateLayoutFromFunction(member), member->GetAttributeCollection());
                    }
                }
            }
            LayoutTransformer transformer(m_typeLibrary);
            auto layoutElement = transformer.TryEmbedElement(adder->GetLayoutElement(), type->GetAttributeCollection());
            if (type->GetAttributeCollection().HasAttribute<NameAttribute>() || memberItem.IsUtilityClassFunction())
                return MakeElement(VerticalElements(CreateLayoutFromType("__this__", memberItem.GetTypeInfo(), function), layoutElement));
            else
            {
                if (!layoutElement && function->GetArity() == 1)
                    return std::make_shared<VerticalElements>();
                else
                    return layoutElement;
            }
        }
        else
            return adder->GetLayoutElement();
    }

    LayoutElementPointer LayoutBuilder::CreateLayoutFromMember(Members::MemberPointer member, bool allowProperty)
    {
        if (!member)
            return nullptr;
        LayoutElementPointer layout;
        if(allowProperty)
            layout = CreateLayoutFromProperty(member);        
        if (!layout)
            layout = CreateLayoutFromFunction(member);
        if (layout)
        {
            Utilities::MemberItem memberItem(member, m_typeLibrary);
            std::string name = ToString(memberItem, Formatting::FormattingService());
            auto embeddedElement = std::make_shared<EmbeddedElement>(name, layout);
            embeddedElement->GetContext().type = memberItem.GetTypeInfo();
            embeddedElement->GetContext().function = member;
            layout = embeddedElement;
        }        
        return layout;
    }

}}}}

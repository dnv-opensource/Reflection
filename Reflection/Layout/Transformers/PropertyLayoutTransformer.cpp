//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "PropertyLayoutTransformer.h"
#include "Reflection/Attributes/CaptionAttribute.h"
#include "Reflection/Attributes/AttributeCollection.h"
#include "Reflection/Attributes/GroupAttribute.h"
#include "Reflection/Attributes/ImageAttribute.h"
#include "Reflection/Attributes/LayoutAttribute.h"
#include "Reflection/Attributes/HeaderAttribute.h"
#include "Reflection/Attributes/ContainerAttribute.h"
#include "Reflection/Attributes/DeprecatedAttribute.h"
#include "Reflection/Attributes/AttributeCollectionService.h"

#include "Reflection/Layout/GroupBox.h"
#include "Reflection/Layout/Header\DefaultColumn.h"
#include "Reflection/Layout/LayoutBuilder.h"
#include "Reflection/Layout/OverloadSelector.h"

#include "Reflection/Utilities/ConstructorsFromTypeService.h"

#include "ConstantLayoutSourceProvider.h"
#include "FunctionLayoutSourceProvider.h"
#include "ILayoutSource.h"
#include "CombinedLayoutSourceProvider.h"
#include "Reflection/Utilities/ConstructorsFromTypeExtracter.h"
#include "Reflection/Utilities/ArgumentFromObjectExtracter.h"
#include "MembersLayoutSourceProvider.h"
#include "ConstructorFinderSourceProvider.h"


namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    using namespace Attributes;
    PropertyLayoutTransformer::PropertyLayoutTransformer(const TypeLibraries::TypeLibraryPointer& typeLibrary, const std::shared_ptr<ILayoutSourceProvider>& provider, bool allowEmbedElement, bool useLayout, bool isNested)
        : LayoutTransformer(typeLibrary, isNested)
        , m_provider(provider)
        , m_allowEmbedElement(allowEmbedElement)
        , m_useLayout(useLayout)
    {
    }

    PropertyLayoutTransformer::PropertyLayoutTransformer(const TypeLibraries::TypeLibraryPointer& typeLibrary, const std::shared_ptr<ILayoutSource>& source, bool allowEmbedElement, bool useLayout, bool isNested)
        : PropertyLayoutTransformer(typeLibrary, std::make_shared<ConstantLayoutSourceProvider>(source), allowEmbedElement, useLayout, isNested)
    {
    }

    void PropertyLayoutTransformer::Visit(const AutoControl& control)
    {
        auto source = m_provider->LookupLayoutSource(control.GetName());
        if (source)
        {
            if (source->GetAttributeCollection().HasAttribute<Attributes::DeprecatedAttribute>())
                return;
            PropertyLayoutTransformer transformer(m_typeLibrary, std::make_shared<CombinedLayoutSourceProvider>(m_provider,std::make_shared<ConstantLayoutSourceProvider>(source)), m_allowEmbedElement);
            transformer.SetFunction(m_function);
            transformer.SetParentType(m_parentType);
            transformer.SetAllowRadioButtons(m_allowRadioButtons);
            if (m_useLayout)
            {
                if (source->GetAttributeCollection().HasAttribute<LayoutAttribute>())
                {
                    const auto& layout = source->GetAttributeCollection().GetAttribute<LayoutAttribute>().GetLayout();
                    if (!layout)
                        return;
                    transformer.SetAllowEmbedGroup(false);
                    layout->Accept(transformer);
                    transformer.SetAllowEmbedGroup(true);                 
                    m_element = transformer.GetLayoutElement();
                    if (std::dynamic_pointer_cast<EmbeddedElement>(layout))
                        m_element = TryEmbedElement(m_element, source->GetAttributeCollection());
                    else
                        m_element = transformer.TryEmbedGroup(m_element, source->GetAttributeCollection());
                    return;
                }
            }
            AutoControl copiedControl(control);
            if (m_useLayout)
            {
                copiedControl.GetContext().getter = source->GetGetter();
                copiedControl.GetContext().setter = source->GetSetter();
                copiedControl.SetAttributeCollection(source->GetAttributeCollection());
            }
            transformer.ChangeControlBasedOnType(copiedControl, source->GetTypeInfo());
            m_element = transformer.GetLayoutElement();
        }
    }

    void PropertyLayoutTransformer::Visit(const ControlElement& control)
    {        
        auto source = m_provider->LookupLayoutSource(control.GetName());
        if (source)
        {
            m_element = ModifyControlElement(control.Clone(), source);
        }
    }

    void PropertyLayoutTransformer::Visit(const EmbeddedElement& control)
    {
        auto source = m_provider->LookupLayoutSource(control.GetName());
        if (source)
        {
            LayoutBuilder builder(m_typeLibrary, m_isNested);
            LayoutElementPointer layout;
            Members::MemberPointer constructor;
            if(control.GetChild())
            {
                auto constructorFinder = std::make_shared<ConstructorFinderSourceProvider>(m_typeLibrary, source->GetTypeInfo());
                PropertyLayoutTransformer transformer(m_typeLibrary, constructorFinder, m_allowEmbedElement, false, m_isNested);
                control.GetChild()->Accept(transformer);
                layout = transformer.GetLayoutElement();
                constructor = constructorFinder->GetBestConstructor();
            }
            else
                layout = builder.CreateLayoutFromType(source->GetTypeInfo());
            auto embeddedElement = std::make_shared<EmbeddedElement>(source->GetName(), layout);
            embeddedElement->GetContext().type = source->GetTypeInfo();
            embeddedElement->GetContext().parentType = m_parentType;
            embeddedElement->GetContext().setter = source->GetSetter();
            embeddedElement->GetContext().getter = source->GetGetter();
            embeddedElement->GetContext().function = constructor;
            m_element = embeddedElement;
        }
    }

    void PropertyLayoutTransformer::Visit(const OverloadSelector& control)
    {
        auto source = m_provider->LookupLayoutSource("");
        if (source)
        {            
            PropertyLayoutTransformer transformer(m_typeLibrary, std::make_shared<ConstantLayoutSourceProvider>(source), m_allowEmbedElement, false, m_isNested);
            control.GetChild()->Accept(transformer);
            OverloadSelector selector(source->GetName(), transformer.GetLayoutElement());
            selector.GetContext() = source->GetContext(m_typeLibrary, m_function, m_parentType);
            selector.SetCaption(source->GetCaption(m_typeLibrary, selector.GetName(), selector.GetCaption()));
            AddMembersToOverloadSelector(selector, source->GetAttributeCollection());
            m_element = MakeElement(std::move(selector));
        }
    }

    void PropertyLayoutTransformer::Visit(const Grid& control)
    {
        auto source = m_provider->LookupLayoutSource(control.GetName());
        if (source)
        {
            std::shared_ptr<Grid> grid = std::make_shared<Grid>(control);
            m_element = ModifyControlElement(grid, source);
            auto type = m_typeLibrary->LookupType(grid->GetContext().type.GetTypeInfo());
            if (type && type->GetAttributeCollection().HasAttribute<Attributes::ContainerAttribute>())
            {
                auto valueTypeInfo = type->GetAttributeCollection().GetAttribute<Attributes::ContainerAttribute>().GetValueType();
                auto constructors = Utilities::ConstructorsFromTypeService::GetService(m_typeLibrary)->CollectAllConstructorMembersOfThisAndDerivedClasses(valueTypeInfo, true);
                auto constructor = Utilities::ConstructorsFromTypeExtracter(m_typeLibrary).GetBestMemberFromType(constructors, valueTypeInfo);
                if (!constructor)
                    throw std::runtime_error("When creating layout for '" + source->GetName() + "': Could not find a constructor that can be used to defer the columns of the grid.");
                if (constructor)
                {
                    grid->SetFunction(constructor);
                    HorizontalElements header;
                    header.SetName("Header");
                    auto valueType = m_typeLibrary->LookupType(valueTypeInfo.GetTypeInfo());
                    if (control.GetCanAddRemoveRows())
                        header.PushBack(Caption(""));
                    for (size_t i = 0; i < constructor->GetArity(); ++i)
                        header.PushBack(LayoutBuilder(m_typeLibrary).CreateLayoutFromFunctionArgumentOrProperty(constructor, i, valueType));
                    grid->SetHeader(std::move(header));
                }
            }
            else {
                throw std::runtime_error("Unable to create grid for type '" + m_typeLibrary->GetTypeFormatter()->FormatType(grid->GetContext().type) + " type needs to be a container (such as std::vector or std::set)");
            }
            if (TypeUtilities::IsIndeterminate(grid->HasSorting()))
            {
                auto type = m_typeLibrary->LookupType(grid->GetContext().type.GetTypeInfo());
                if (type && type->GetAttributeCollection().HasAttribute<Attributes::ContainerAttribute>())
                {
                    grid->SetHasSorting(type->GetAttributeCollection().GetAttribute<Attributes::ContainerAttribute>().HasSorting());
                }
            }
        }
    }

    LayoutElementPointer PropertyLayoutTransformer::ModifyControlElement(LayoutElementPointer element, std::shared_ptr<ILayoutSource> source)
    {
        auto controlElement = std::dynamic_pointer_cast<ControlElement>(element);
        if (controlElement)
        {            
            controlElement->GetContext() = source->GetContext(m_typeLibrary, m_function, m_parentType);
            controlElement->SetCaption(source->GetCaption(m_typeLibrary, controlElement->GetName(), controlElement->GetCaption()));
            controlElement->SetName(source->GetName());
            controlElement->SetAttributeCollection(source->GetAttributeCollection());
        }
        if (m_allowEmbedElement && source->AllowEmbedElement())
            return TryEmbedElement(element, source->GetAttributeCollection());
        else
            return element;
    }

    void PropertyLayoutTransformer::Visit(const CollectionOfElements& control)
    {
        m_element = control.Clone();
        auto element = std::dynamic_pointer_cast<CollectionOfElements>(m_element);
        if (element)
        {
            element->Clear();
            for (auto it = control.begin(); it != control.end(); ++it)
            {
                PropertyLayoutTransformer transformer(m_typeLibrary, m_provider, true, m_useLayout);
                transformer.SetAllowEmbedGroup(m_allowEmbedGroup);
                transformer.SetFunction(m_function);
                transformer.SetParentType(m_parentType);
                (*it)->Accept(transformer);
                if(transformer.GetLayoutElement())
                    element->PushBack(transformer.GetLayoutElement());
            }
        }
    }

    void PropertyLayoutTransformer::Visit(const SelectorLookup& control)
    {
        m_element = control.Clone();
        m_element->SetParentType(m_typeLibrary, m_parentType);
    }

    void PropertyLayoutTransformer::Visit(const ILayoutElement& control)
    {
        m_element = control.Clone();
        m_element->SetParentType(m_typeLibrary, m_parentType);
    }

    void PropertyLayoutTransformer::Visit(const Table& control)
    {
        auto source = m_provider->LookupLayoutSource(control.GetName());
        if (source)
        {
            std::shared_ptr<Table> table = std::make_shared<Table>(control);            
            m_element = ModifyControlElement(table, source);
            if (source->GetAttributeCollection().HasAttribute<Attributes::HeaderAttribute>())
                table->SetHeader(source->GetAttributeCollection().GetAttribute<Attributes::HeaderAttribute>().GetHeader());

            if (table->GetHeader().GetColumns().size() == 0)
            {
                std::string name = source->GetName();
                if (name.back() == 's')
                    name = name.substr(0, name.size() - 1);
                table->SetHeader(Header(DefaultColumn(name), false));
            }
            if (TypeUtilities::IsIndeterminate(table->HasSorting()))
            {
                auto type = m_typeLibrary->LookupType(table->GetContext().type.GetTypeInfo());
                if (type && type->GetAttributeCollection().HasAttribute<Attributes::ContainerAttribute>())
                {
                    table->SetHasSorting(type->GetAttributeCollection().GetAttribute<Attributes::ContainerAttribute>().HasSorting());
                }
            }
        }
    }

    LayoutElementPointer PropertyLayoutTransformer::GetLayoutElement() const
    {
        return m_element;
    }

    void PropertyLayoutTransformer::SetFunction(Members::MemberPointer function)
    {
        m_function = function;
    }

    Members::MemberPointer PropertyLayoutTransformer::GetFunction() const
    {
        return m_function;
    }

    void PropertyLayoutTransformer::SetParentType(const Types::DecoratedTypeInfo& parentType)
    {
        m_parentType = parentType;
    }

    Types::DecoratedTypeInfo PropertyLayoutTransformer::GetParentType() const
    {
        return m_parentType;
    }

}}}}



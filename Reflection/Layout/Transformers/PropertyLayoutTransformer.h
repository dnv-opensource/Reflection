#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Members/IMember.h"
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"
#include "LayoutTransformer.h"
#include "ILayoutSourceProvider.h"
#include "Reflection/Layout/SelectorLookup.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class AutoControl;
    class ControlElement;
    class CollectionOfElements;
    class ILayoutElement;
    class EmbeddedElement;
    class Table;
    class Grid;
    class OverloadSelector;

    class PropertyLayoutTransformer 
        : public TypeUtilities::Visitors
        <
            AutoControl, 
            ControlElement, 
            CollectionOfElements, 
            ILayoutElement,
            Table,
            Grid,
            EmbeddedElement,
            OverloadSelector,
            SelectorLookup
        >
        , public LayoutTransformer
    {
    public:
        PropertyLayoutTransformer(const TypeLibraries::TypeLibraryPointer& typeLibrary, const std::shared_ptr<ILayoutSourceProvider>& provider, bool allowEmbedElement, bool useLayout = false, bool isNested = false);
        PropertyLayoutTransformer(const TypeLibraries::TypeLibraryPointer& typeLibrary, const std::shared_ptr<ILayoutSource>& source, bool allowEmbedElement, bool useLayout = false, bool isNested = false);
        virtual void Visit(const AutoControl& control) override;
        virtual void Visit(const OverloadSelector& control) override;
        virtual void Visit(const ControlElement& control) override;
        virtual void Visit(const CollectionOfElements& control) override;
        virtual void Visit(const ILayoutElement& control) override;
        virtual void Visit(const Table& control) override;
        virtual void Visit(const Grid& control) override;
        virtual void Visit(const EmbeddedElement& control) override;
        virtual void Visit(const SelectorLookup& control) override;
        LayoutElementPointer GetLayoutElement() const;
        void SetFunction(Members::MemberPointer function);
        Members::MemberPointer GetFunction() const;
        void SetParentType(const Types::DecoratedTypeInfo& parentType);
        Types::DecoratedTypeInfo GetParentType() const;
    private:
        Types::DecoratedTypeInfo m_parentType;
        Members::MemberPointer m_function;
        std::shared_ptr<ILayoutSourceProvider> m_provider;
        LayoutElementPointer m_element;
        bool m_allowEmbedElement;
        bool m_useLayout;
        LayoutElementPointer ModifyControlElement(LayoutElementPointer element, std::shared_ptr<ILayoutSource> source);
    };
}}}}

#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Config.h"
#include "TypeUtilities/Visitors/CompareVisitor.h"
#include "ControlElement.h"
#include "CollectionOfElements.h"
#include "GroupBox.h"
#include "EmbeddedElement.h"
#include "Table.h"
#include "Symmetry.h"
#include "ElementWithOneChild.h"
#include "OverloadSelector.h"
#include "Caption.h"
#include "Grid.h"
#include "GridElements.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
#pragma warning(push)
#pragma warning(disable:4275)

    class REFLECTION_IMPORT_EXPORT CompareVisitor : public TypeUtilities::CompareVisitors<
        ControlElement, 
        CollectionOfElements, 
        GroupBox, 
        ILayoutElement, 
        GridElements,
        ElementWithOneChild,
        EmbeddedElement,
        Table,
        Grid,
        Symmetry,
        OverloadSelector,
        Caption
        >
    {
    public:
        CompareVisitor(const ILayoutElement& lhs, const ILayoutElement& rhs);
        virtual bool Compare(const ControlElement& lhs, const ControlElement& rhs) override;
        virtual bool Compare(const CollectionOfElements& lhs, const CollectionOfElements& rhs) override;
        virtual bool Compare(const GridElements& lhs, const GridElements& rhs) override;
        virtual bool Compare(const GroupBox& lhs, const GroupBox& rhs) override;
        virtual bool Compare(const ILayoutElement& lhs, const ILayoutElement& rhs) override;
        virtual bool Compare(const ElementWithOneChild& lhs, const ElementWithOneChild& rhs) override;
        virtual bool Compare(const EmbeddedElement& lhs, const EmbeddedElement& rhs) override;
        virtual bool Compare(const Table& lhs, const Table& rhs) override;
        virtual bool Compare(const Grid& lhs, const Grid& rhs) override;
        virtual bool Compare(const Symmetry& lhs, const Symmetry& rhs) override;
        virtual bool Compare(const OverloadSelector& lhs, const OverloadSelector& rhs) override;    
        virtual bool Compare(const Caption& lhs, const Caption& rhs) override;
    };
#pragma warning(pop)
}}}}

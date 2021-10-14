#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Config.h"
#include <string>
#include "TypeUtilities/Visitors/UnaryVisitorWithReturn.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class ControlElement;
    class CollectionOfElements;
    class GroupBox;
    class Image;
    class ILayoutElement;
    class EmbeddedElement;
    class ElementWithOneChild;
    class Table;
    class Symmetry;
    class OverloadSelector;
    class DynamicArea;
    class Caption;
    class GridElements;
    class Grid;
#pragma warning(push)
#pragma warning(disable:4275)
    class REFLECTION_IMPORT_EXPORT StringizeVisitor : public TypeUtilities::UnaryVisitorsWithReturn<
        std::string, 
        ControlElement, 
        CollectionOfElements, 
        GroupBox, 
        Image, 
        EmbeddedElement,
        GridElements,
        ElementWithOneChild,
        Table,
        Symmetry,
        OverloadSelector,
        DynamicArea,
        Caption,
        Grid>
    {
    public:
        StringizeVisitor(const ILayoutElement& element);
        virtual std::string VisitAndReturn(const ControlElement& element) override;
        virtual std::string VisitAndReturn(const CollectionOfElements& element) override;
        virtual std::string VisitAndReturn(const GroupBox& element) override;
        virtual std::string VisitAndReturn(const GridElements& elements) override;
        virtual std::string VisitAndReturn(const Image& element) override;
        virtual std::string VisitAndReturn(const EmbeddedElement& element) override;
        virtual std::string VisitAndReturn(const ElementWithOneChild& element) override;
        virtual std::string VisitAndReturn(const Table& element) override;
        virtual std::string VisitAndReturn(const Grid& element) override;
        virtual std::string VisitAndReturn(const Symmetry& element) override;
        virtual std::string VisitAndReturn(const OverloadSelector& element) override;
        virtual std::string VisitAndReturn(const DynamicArea& element) override;
        virtual std::string VisitAndReturn(const Caption& element) override;
    private:
        std::string StripType(std::string type) const;
    };
#pragma warning(pop)
}}}}

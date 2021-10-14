//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "CompareVisitor.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    CompareVisitor::CompareVisitor(const ILayoutElement& lhs, const ILayoutElement& rhs)
        : BaseClass(lhs,rhs)
    {

    }

    bool CompareVisitor::Compare(const ControlElement& lhs, const ControlElement& rhs)
    {
        if (lhs.GetName() != rhs.GetName())
            return false;
        if (lhs.GetCaption() != rhs.GetCaption())
            return false;
        return true;
    }

    bool CompareVisitor::Compare(const CollectionOfElements& lhs, const CollectionOfElements& rhs)
    {
        if (lhs.size() != rhs.size())
            return false;
        auto ithis = lhs.begin();
        auto iother = rhs.begin();
        for (; ithis != lhs.end(); ++ithis, ++iother)
        {
            if (**ithis != **iother)
                return false;
        }
        return true;
    }

    bool CompareVisitor::Compare(const GroupBox& lhs, const GroupBox& rhs)
    {
        if (lhs.GetGroupName() != rhs.GetGroupName())
            return false;
        if ((lhs.GetEnabler() == nullptr) != (rhs.GetEnabler() == nullptr))
            return false;
        if (lhs.GetEnabler())
        {
            if (*lhs.GetEnabler() != *rhs.GetEnabler())
                return false;
        }
        return Compare((const CollectionOfElements&)lhs, (const CollectionOfElements&)rhs);
    }

    bool CompareVisitor::Compare(const ILayoutElement& lhs, const ILayoutElement& rhs)
    {
        return true;
    }

    bool CompareVisitor::Compare(const EmbeddedElement& lhs, const EmbeddedElement& rhs)
    {
        if (lhs.GetName() != rhs.GetName())
            return false;
        return Compare((const ElementWithOneChild&)lhs, (const ElementWithOneChild&)rhs);
    }

    bool CompareVisitor::Compare(const ElementWithOneChild& lhs, const ElementWithOneChild& rhs)
    {
        return *lhs.GetChild() == *rhs.GetChild();
    }

    bool CompareVisitor::Compare(const Table& lhs, const Table& rhs)
    {
        if (!Compare((const ControlElement&)lhs, (const ControlElement&)rhs))
            return false;
        if (lhs.GetInitialNumberOfRows() != rhs.GetInitialNumberOfRows())
            return false;
        if (lhs.GetHeader().ShowHeader() != rhs.GetHeader().ShowHeader())
            return false;
        if (lhs.GetHeader().GetColumns().size() != rhs.GetHeader().GetColumns().size())
            return false;
        for (size_t i = 0; i < lhs.GetHeader().GetColumns().size(); ++i)
        {
            if (lhs.GetHeader().GetColumns()[i]->GetHeader(nullptr, typeid(void)) != rhs.GetHeader().GetColumns()[i]->GetHeader(nullptr, typeid(void)))
                return false;
            if (lhs.GetHeader().GetColumns()[i]->GetDocumentation() != rhs.GetHeader().GetColumns()[i]->GetDocumentation())
                return false;
        }
        return true;
    }

    bool CompareVisitor::Compare(const Symmetry& lhs, const Symmetry& rhs)
    {
        if (lhs.GetMasterPropertyName() != rhs.GetMasterPropertyName())
            return false;
        if (lhs.GetSymmetryCaption() != rhs.GetSymmetryCaption())
            return false;
        if (lhs.GetDocumentation() != rhs.GetDocumentation())
            return false;
        return *lhs.GetChild() == *rhs.GetChild();
    }

    bool CompareVisitor::Compare(const OverloadSelector& lhs, const OverloadSelector& rhs)
    {
        if (lhs.GetName() != rhs.GetName())
            return false;
        return Compare((const ElementWithOneChild&)lhs, (const ElementWithOneChild&)rhs);
    }

    bool CompareVisitor::Compare(const Caption& lhs, const Caption& rhs)
    {
        return lhs.GetCaption() == rhs.GetCaption();
    }

    bool CompareVisitor::Compare(const Grid& lhs, const Grid& rhs)
    {
        if (!Compare((const ControlElement&)lhs, (const ControlElement&)rhs))
            return false;
        if (lhs.GetInitialNumberOfRows() != rhs.GetInitialNumberOfRows())
            return false;
        if(!Compare(lhs.GetHeader(),rhs.GetHeader()))
            return false;
        if((lhs.GetFixedGrid() == nullptr) != (rhs.GetFixedGrid()==nullptr))
           return false;
        if (lhs.GetFixedGrid() && !Compare(*lhs.GetFixedGrid(), *rhs.GetFixedGrid()))
            return false;
        return true;
    }

    bool CompareVisitor::Compare(const GridElements& lhs, const GridElements& rhs)
    {
        if (lhs.GetRows() != rhs.GetRows())
            return false;
        if (lhs.GetCols() != rhs.GetCols())
            return false;
        return Compare((const CollectionOfElements&)lhs, (const CollectionOfElements&)rhs);
    }

}}}}



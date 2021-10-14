//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "RecordAttributeGroupCreator.h"
#include "Reflection/Layout/Caption.h"
#include "Reflection/Layout/VerticalElements.h"
#include "Reflection/Layout/HorizontalElements.h"
#include "Reflection/Controls/Aspects/IndexedPropertyAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    RecordAttributeGroupCreator::RecordAttributeGroupCreator(const Attributes::RecordAttribute& recordAttribute) 
        : m_recordAttribute(recordAttribute)
        , m_rows(recordAttribute.GetRanges().front()->GetIndicesInRange(Variants::Variant()))
        , m_rowIndexName(recordAttribute.GetRanges().front()->GetName())
        , m_columns(recordAttribute.GetRanges().back()->GetIndicesInRange(Variants::Variant()))
        , m_columnIndexName(recordAttribute.GetRanges().back()->GetName())
    {
        m_grid.resize(m_rows.size()*(m_columns.size() + 1));

        auto rowHeaders = recordAttribute.GetRanges().front()->GetHeadersInRange(Variants::Variant());
        for (size_t row = 0; row < rowHeaders.size(); ++row)
            SetAt(row, 0, std::make_shared<Caption>(rowHeaders.at(row)));
    }

    void RecordAttributeGroupCreator::AddLayoutElement(const std::shared_ptr<ILayoutElement>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode)
    {
        auto indexedPropertyAspect = controlNode->GetAspect<Controls::IndexedPropertyAspect>();
        size_t row = GetRowIndex(indexedPropertyAspect.GetIndices().find(m_rowIndexName)->second);
        size_t column = GetColumnIndex(indexedPropertyAspect.GetIndices().find(m_columnIndexName)->second);
        SetAt(row, column, layoutElement);
    }

    std::shared_ptr<DNVS::MoFa::Reflection::Layout::ILayoutElement> RecordAttributeGroupCreator::GetGroup() const
    {
        auto group = std::make_shared<VerticalElements>();
        for (size_t irow = 0; irow < m_rows.size(); ++irow)
        {
            auto row = std::make_shared<HorizontalElements>();
            for (size_t icol = 0; icol < m_columns.size() + 1; ++icol)
                row->PushBack(GetAt(irow, icol));
            group->PushBack(row);
        }
        return group;
    }

    std::shared_ptr<DNVS::MoFa::Reflection::Layout::ILayoutElement> RecordAttributeGroupCreator::GetAt(size_t row, size_t col) const
    {
        return m_grid.at(GetIndex(row, col));
    }

    void RecordAttributeGroupCreator::SetAt(size_t row, size_t col, const std::shared_ptr<ILayoutElement>& layoutElement)
    {
        m_grid[GetIndex(row, col)] = layoutElement;
    }

    size_t RecordAttributeGroupCreator::GetIndex(size_t row, size_t col) const
    {
        return row * (m_columns.size() + 1) + col;
    }

    size_t RecordAttributeGroupCreator::GetRowIndex(const Objects::Object& row) const
    {
        for (size_t i = 0; i < m_rows.size(); ++i)
        {
            if (row == Objects::Object(row.GetTypeLibrary(), m_rows[i]))
                return i;
        }
        throw std::runtime_error("Row not found");
    }

    size_t RecordAttributeGroupCreator::GetColumnIndex(const Objects::Object& col) const
    {
        for (size_t i = 0; i < m_columns.size(); ++i)
        {
            if (col == Objects::Object(col.GetTypeLibrary(), m_columns[i]))
                return i + 1; //Include the caption column
        }
        throw std::runtime_error("Column not found");
    }

}}}}


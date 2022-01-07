//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutGridElementsMergerHandler.h"
#include "Reflection/Layout/HorizontalElements.h"
#include "Reflection/Layout/GridElements.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class GridElementsCreator {
    public:
        bool TryAddRow(std::shared_ptr<ILayoutElement> layout)
        {
            auto row = std::dynamic_pointer_cast<HorizontalElements>(layout);
            if (!row)
                return false;
            if (m_rows.empty())
                m_columnCount = row->size();
            else if (m_columnCount != row->size())
                return false;
            if (m_columnCount < 2)
                return false;
            m_rows.push_back(row);
            return true;
        }
        void AddLayout(std::shared_ptr<ILayoutElement> layout)
        {
            m_layouts.push_back(layout);
        }
        bool TryConvertRowsIntoGridElements()
        {
            if (m_rows.size() == 1)
            {
                AddLayout(m_rows.front());
                m_rows.clear();
                return false;
            }
            else if (m_rows.empty())
                return false;
            auto cells = std::make_shared<GridElements>(m_rows.size(), m_columnCount);
            for (const auto& row : m_rows)
            {
                for (const auto& cell : *row)
                    cells->PushBack(cell);
            }
            AddLayout(cells);
            m_rows.clear();
            return true;
        }
        std::shared_ptr<ILayoutElement> TryCreateVerticalElements()
        {
            auto elements = std::make_shared<VerticalElements>();
            for (const auto& layout : m_layouts)
                elements->PushBack(layout);
            return elements;
        }
    private:
        size_t m_columnCount;
        std::vector<std::shared_ptr<HorizontalElements>> m_rows;
        std::vector<std::shared_ptr<ILayoutElement>> m_layouts;
    };
    std::shared_ptr<ILayoutElement> LayoutGridElementsMergerHandler::ModifyLayout(const std::shared_ptr<VerticalElements>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {       
        GridElementsCreator creator;
        for (auto& element : *layoutElement)
        {
            auto layout = ModifyLayout(element, controlNode, context);
            if (!creator.TryAddRow(layout))
            {
                creator.TryConvertRowsIntoGridElements();
                creator.AddLayout(layout);
            }
        }
        creator.TryConvertRowsIntoGridElements();
        return creator.TryCreateVerticalElements();
    }

}}}}


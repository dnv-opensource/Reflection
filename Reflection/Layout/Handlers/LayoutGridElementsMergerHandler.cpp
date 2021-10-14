//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutGridElementsMergerHandler.h"
#include "Reflection/Layout/HorizontalElements.h"
#include "Reflection/Layout/GridElements.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    std::shared_ptr<ILayoutElement> LayoutGridElementsMergerHandler::ModifyLayout(const std::shared_ptr<VerticalElements>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {       
        std::set<size_t> columns;
        std::vector<std::shared_ptr<HorizontalElements>> rows;
        for (auto& element : *layoutElement)
        {
            element = ModifyLayout(element, controlNode, context);
            if (auto row = std::dynamic_pointer_cast<HorizontalElements>(element))
            {
                rows.push_back(row);
                columns.insert(row->size());
            }
        }
        if (rows.size() == layoutElement->size() && columns.size() == 1 && *columns.begin() > 1)
        {
            auto cells = std::make_shared<GridElements>(rows.size(), *columns.begin());
            for (const auto& row : rows)
            {
                for (const auto& cell : *row)
                    cells->PushBack(cell);
            }
            return cells;
        }
        return layoutElement;
    }

}}}}


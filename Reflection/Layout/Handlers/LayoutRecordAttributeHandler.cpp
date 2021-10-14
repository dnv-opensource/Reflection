//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutRecordAttributeHandler.h"
#include "CellLayoutManager.h"
#include "GroupCreators/RecordAttributeGroupCreator.h"

#include "Reflection/Attributes/RecordAttribute.h"
#include "Reflection/Layout/Caption.h"
#include "Reflection/Controls/Aspects/IndexedPropertyAspect.h"
#include "LayoutTreatAsBoolAttributeHandler.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    std::shared_ptr<ILayoutElement> LayoutRecordAttributeHandler::ModifyLayout(const std::shared_ptr<AutoControl>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        if (layoutElement->TryGetMatchingControlNode(controlNode) && TryGetRecordAttributeForGrid(controlNode))
            return ModifyLayout(std::make_shared<Grid>(*layoutElement), controlNode, context);
        return layoutElement;
    }

    std::shared_ptr<ILayoutElement> LayoutRecordAttributeHandler::ModifyLayout(const std::shared_ptr<Grid>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        auto recordAttribute = TryGetRecordAttributeForGrid(controlNode);
        if (!recordAttribute)
            return layoutElement;

        auto actualNode = layoutElement->TryGetMatchingControlNode(controlNode);
        if (!actualNode)
            return layoutElement;

        HorizontalElements header;
        const auto& rowRange = *recordAttribute->GetRanges().begin();
        const auto& colRange = *recordAttribute->GetRanges().rbegin();
        header.PushBack(Caption(""));
        for (const auto& colHeader : colRange->GetHeadersInRange(Variants::Variant()))
            header.PushBack(Caption(colHeader));
        layoutElement->SetHeader(header);
        
		CellLayoutManager gridManager;
        auto group = CreateGroupCreator(actualNode);
        if (group)
        {
            for (const auto& child : controlNode->GetChildren())
                group->AddLayoutElement(gridManager.ModifyLayout(std::make_shared<AutoControl>(child->GetName(), child->GetCaption()), child, context), child);
            layoutElement->SetFixedGrid(std::dynamic_pointer_cast<VerticalElements>(group->GetGroup()));
        }
        return layoutElement;
    }


    std::shared_ptr<ILayoutElementGroupCreator> LayoutRecordAttributeHandler::CreateGroupCreator(const std::shared_ptr<Controls::ControlNode>& controlNode)
    {
        auto recordAttribute = TryGetRecordAttributeForGrid(controlNode);
        if (!recordAttribute)
            return nullptr;
        return std::make_shared<RecordAttributeGroupCreator>(*recordAttribute);
    }

    const Attributes::RecordAttribute* LayoutRecordAttributeHandler::TryGetRecordAttributeForGrid(const std::shared_ptr<Controls::ControlNode>& controlNode) const
    {
        if (auto recordAttribute = controlNode->TryGetAttribute<Attributes::RecordAttribute>())
            if (recordAttribute->GetRanges().size() == 2)
                return recordAttribute;
        return nullptr;
    }

}}}}


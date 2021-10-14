//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutGroupMergeHandler.h"
#include "Reflection/Layout/GroupBox.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    std::shared_ptr<ILayoutElement> LayoutGroupMergeHandler::ModifyLayout(const std::shared_ptr<CollectionOfElements>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        std::map<std::string, std::shared_ptr<GroupBox>> existingGroupBoxes;
        auto it = layoutElement->begin();
        while(it != layoutElement->end())
        {
            auto icurrent = it++;
            *icurrent = ModifyLayout(*icurrent, controlNode, context);
            if (auto groupBox = std::dynamic_pointer_cast<GroupBox>(*icurrent))
            {
                auto& existingGroupBox = existingGroupBoxes[groupBox->GetGroupName()];
                if (!existingGroupBox)
                    existingGroupBox = groupBox;
                else
                {
                    if(existingGroupBox->Merge(*groupBox))
                        layoutElement->erase(icurrent);
                }
            }
        }
        return layoutElement;
    }

}}}}


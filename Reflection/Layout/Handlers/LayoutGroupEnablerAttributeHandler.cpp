//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutGroupEnablerAttributeHandler.h"
#include "Reflection/Attributes/GroupEnablerAttribute.h"
#include "Reflection/Controls/Aspects/GroupEnablerAspect.h"
#include "Contexts/ForwardingLayoutControlContext.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    class CurrentGroupBoxControlContext : public ForwardingLayoutControlContext
    {
    public:
        CurrentGroupBoxControlContext(ILayoutControlContext& context, const std::shared_ptr<GroupBox>& currentGroupBox) 
            : ForwardingLayoutControlContext(context) 
            , m_currentGroupBox(currentGroupBox) 
        {}
        std::shared_ptr<GroupBox> GetCurrentGroupBox() const { return m_currentGroupBox; }
    private:
        std::shared_ptr<GroupBox> m_currentGroupBox;
    };

    std::shared_ptr<ILayoutElement> LayoutGroupEnablerAttributeHandler::ModifyLayout(const std::shared_ptr<GroupBox>& groupBox, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        if (auto attribute = controlNode->TryGetAttribute<Attributes::GroupEnablerAttribute>())
        {
            auto it = groupBox->begin();
            while (it != groupBox->end())
            {
                auto icurrent = it++;
                *icurrent = ModifyLayout(*icurrent, controlNode, CurrentGroupBoxControlContext(context, groupBox));
                if (!*icurrent)
                    groupBox->erase(icurrent);
            }
        }
        return groupBox;
    }

    std::shared_ptr<ILayoutElement> LayoutGroupEnablerAttributeHandler::ModifyLayout(const std::shared_ptr<CheckBox>& checkBox, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        if (checkBox->GetControlNode() && checkBox->GetControlNode()->TryGetAttribute<Attributes::GroupEnablerAttribute>())
        {
            if(CurrentGroupBoxControlContext* currentGroupBoxContext = dynamic_cast<CurrentGroupBoxControlContext*>(&context))
            {
                currentGroupBoxContext->GetCurrentGroupBox()->SetEnabler(checkBox);
                if (currentGroupBoxContext->GetCurrentGroupBox()->GetName() == checkBox->GetCaption())
                    currentGroupBoxContext->GetCurrentGroupBox()->SetName("");
                return nullptr;
            }
        }
        return checkBox;
    }

}}}}


#include "GroupAspectAdder.h"
#include "../ControlNode.h"
#include "../Aspects/GroupAspect.h"
#include "ControlNodeHandlerUtilities.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    GroupAspectAdder::GroupAspectAdder(const std::string& groupName)
        : m_groupName(groupName)
    {
    }

    bool GroupAspectAdder::OnInitialize(ControlNode& node, const std::shared_ptr<Layout::ControlElement>& element, bool priorState /*= false*/)
    {
        if (!m_groupName.empty())
        {
            ControlNode* related = LookupRelatedNode(node, element);
            if(!related && node.GetParent())
                related = LookupRelatedNode(*node.GetParent(), element);
            if (related)
            {
                if (!related->TryGetAspect<GroupAspect>())
                    related->AddAspect<GroupAspect>(m_groupName);
            }
        }
        return priorState;
    }

    bool GroupAspectAdder::OnInitialize(ControlNode& node, const std::shared_ptr<Layout::GroupBox>& element, bool priorState /*= false*/)
    {
        GroupAspectAdder childAdder(element->GetGroupName());
        for (const auto& child : *element)
        {
            childAdder.OnInitialize(node, child, priorState);
        }
        return priorState;
    }

}}}}


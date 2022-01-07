#pragma once
#include "ControlNodeLayoutAttributeHandler.h"
#include "../../Layout/ControlElement.h"
#include "../../Layout/GroupBox.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    class GroupAspectAdder : ControlNodeLayoutAttributeHandler<GroupAspectAdder, Layout::ControlElement, Layout::GroupBox>
    {
    public:
        GroupAspectAdder(const std::string& groupName);
        using BaseClass::OnInitialize;
        bool OnInitialize(ControlNode& node, const std::shared_ptr<Layout::ControlElement>& element, bool priorState = false);
        bool OnInitialize(ControlNode& node, const std::shared_ptr<Layout::GroupBox>& element, bool priorState = false);
    private:
        std::string m_groupName;
    };
}}}}
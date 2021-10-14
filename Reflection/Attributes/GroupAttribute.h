#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include "IAttribute.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    /**
    The GroupAttribute is used in reflection in order to group together many different properties.
    Example:
    Class<BendingMomentMemOpt> cls(typeLibrary, "BendingMomentMemOpt");
    cls.SetGet("bendingMomentChoice", &BendingMomentMemOpt::SetBendingMomentChoice, &BendingMomentMemOpt::GetBendingMomentChoice)
    .AddAttribute<GroupAttribute>("Axial compression and bending");

    When auto generating GUI for this, a group box will be generated with the title "Axial compression and bending".
    If multiple properties have the same group name, they will be placed in the same group box.
    */
    class GroupAttribute : public IAttribute {
    public:
        GroupAttribute(const std::string& group, const std::string& documentation = "") : m_group(group) , m_documentation(documentation) {}
        const std::string& GetGroup() const { return m_group; }
        const std::string& GetDocumentation() const { return m_documentation; }
    private:
        std::string m_group;
        std::string m_documentation;
    };
}}}}

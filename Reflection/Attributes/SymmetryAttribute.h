#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>

namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {
    /**
    The SymmetryAttribute is used in reflection in order to indicate that two values should be linked.
    Example:
    Class<DSBucklingMemOpt> cls(typeLibrary, "DSBucklingMemOpt");
    cls.SetGet("boundaryConditionY", &DSBucklingMemOpt::SetBoundaryConditionY,&DSBucklingMemOpt::GetBoundaryConditionY)
    .AddAttribute<CaptionAttribute>("C1 factor")
    .AddAttribute<GroupAttribute>("about y-axis");

    cls.SetGet("boundaryConditionZ", &DSBucklingMemOpt::SetBoundaryConditionZ,&DSBucklingMemOpt::GetBoundaryConditionZ)
    .AddAttribute<CaptionAttribute>("C1 factor")
    .AddAttribute<GroupAttribute>("about z-axis")
    .AddAttribute<SymmetryAttribute>("boundaryConditionY", "y-z symmetry");

    You can use this symmetry attribute for multiple properties. 
    When auto generating GUI, you will see two group boxes. One named "about y-axis" and one named "about z-axis".
    The second group box will have a check box, and the values here will be copied from the "about y-axis" box if
    the check box is ticked.

    You can add optional documentation for the symmetry by passing in a third string argument to AddAttribute<SymmetryAttribute>(...);
    */
    class SymmetryAttribute
    {
    public:
        SymmetryAttribute(const std::string& masterProperty, const std::string& symmetryName, const std::string& documentation = "")
            : m_masterProperty(masterProperty)
            , m_symmetryName(symmetryName)
            , m_documentation(documentation)
        {}
        std::string GetMasterProperty() const { return m_masterProperty; }
        std::string GetSymmetryName() const { return m_symmetryName; }                
        /// <summary>
        /// Gets the documentation for the given symmetry option.
        /// </summary>
        /// <returns></returns>
        std::string GetDocumentation() const { return m_documentation; }
    private:
        std::string m_masterProperty;
        std::string m_symmetryName;
        std::string m_documentation;
    };
}}}}
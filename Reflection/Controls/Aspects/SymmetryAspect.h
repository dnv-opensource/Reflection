#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlAspect.h"
#include "Reflection/Members/IMemberFwd.h"
#include <set>
namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    //This aspect represents symmetry between two properties. It will manifest in a check box in the dialog. When this check box is checked, 
    //the node that this aspect belongs to will inherit the values of the masterPropertyName.
    class REFLECTION_IMPORT_EXPORT SymmetryAspect : public IControlAspectT<SymmetryAspect>
    {
    public:
        SymmetryAspect(const std::string& masterPropertyName, const std::string& documentation);
        ~SymmetryAspect();
        const Attributes::AttributeCollection& GetAttributeCollection() const override;
        bool operator==(const SymmetryAspect& other) const override;
        std::string ToString() const override;
        const std::string& GetMasterPropertyName() const;
        const std::string& GetDocumentation() const;
    private:
        std::string m_masterPropertyName;
        std::string m_documentation;
    };
}}}}
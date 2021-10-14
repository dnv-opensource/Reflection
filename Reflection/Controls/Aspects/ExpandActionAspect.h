#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlAspect.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    //This aspect represents that a node can be used as an argument to a constructor. It mentions both the constructor and the index of the argument.
    class REFLECTION_IMPORT_EXPORT ExpandActionAspect : public IControlAspectT<ExpandActionAspect>
    {
    public:
        ExpandActionAspect(bool defaultExpanded);
        const Attributes::AttributeCollection& GetAttributeCollection() const override;
        bool operator==(const ExpandActionAspect& other) const override;
        std::string ToString() const override;
        bool IsDefaultExpanded() const;
    private:
        bool m_defaultExpanded;
    };
}}}}
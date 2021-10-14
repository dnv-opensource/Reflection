#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    //Ignore the LayoutAttribute for this node.
    //This is relevant if we are generating a layout for a constructor and there is a predefined layout on the base class 
    //telling us to insert an OverloadSelector for the current type.
    //Then we must ignore this layout.
    class REFLECTION_IMPORT_EXPORT IgnoreLayoutAspect : public IControlAspectT<IgnoreLayoutAspect>
    {
    public:
        IgnoreLayoutAspect();
        const Attributes::AttributeCollection& GetAttributeCollection() const override;
        bool operator==(const IgnoreLayoutAspect& other) const override;
        std::string ToString() const override;
    };

}}}}
#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    //Force this element to be disabled always.
    class REFLECTION_IMPORT_EXPORT DisabledAspect : public IControlAspectT<DisabledAspect>
    {
    public:
        const Attributes::AttributeCollection& GetAttributeCollection() const override;
        bool operator==(const DisabledAspect& other) const override;
        std::string ToString() const override;
    };

}}}}
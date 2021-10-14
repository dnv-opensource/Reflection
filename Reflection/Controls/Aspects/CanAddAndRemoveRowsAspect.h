#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    class REFLECTION_IMPORT_EXPORT CanAddAndRemoveRowsAspect : public IControlAspectT<CanAddAndRemoveRowsAspect>
    {
    public:
        const Attributes::AttributeCollection& GetAttributeCollection() const override;
        bool operator==(const CanAddAndRemoveRowsAspect& other) const override;
        std::string ToString() const override;
    };

}}}}
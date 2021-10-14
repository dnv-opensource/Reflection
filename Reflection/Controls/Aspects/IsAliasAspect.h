#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    //This aspect represents a constant value stored on the node. This is used when parsing the RecordAttribute to store the row and column information for a given node.
    class REFLECTION_IMPORT_EXPORT IsAliasAspect : public IControlAspectT<IsAliasAspect>
    {
    public:
        const Attributes::AttributeCollection& GetAttributeCollection() const override;
        bool operator==(const IsAliasAspect& other) const override;
        std::string ToString() const override;
    };

}}}}
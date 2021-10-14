#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    /*
    This aspect is assigned to the root node in order to allow assignment to a named variable.
    */
    class REFLECTION_IMPORT_EXPORT RootAspect : public IControlAspectT<RootAspect>
    {
    public:
        const Attributes::AttributeCollection& GetAttributeCollection() const override;
        bool operator==(const RootAspect& other) const override;
        std::string ToString() const override;
        Contents::Content TryGetPropertyContent(const Contents::Content& parent, bool trySimplify = true) override;
        Contents::Content TrySetPropertyContent(const Contents::Content& parent, const Contents::Content& value) override;
        bool SupportAssign() const override;
    };

}}}}
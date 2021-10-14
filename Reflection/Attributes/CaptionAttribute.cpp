//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "CaptionAttribute.h"
#include "AttributeCollection.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {

    std::string GetCaptionOrDefault(const AttributeCollection& collection, const std::string& defaultCaption)
    {
        if (collection.HasAttribute<CaptionAttribute>())
            return collection.GetAttribute<CaptionAttribute>().GetCaption();
        else
            return defaultCaption;
    }

}}}}

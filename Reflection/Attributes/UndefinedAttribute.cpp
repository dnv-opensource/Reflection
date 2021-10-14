//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "UndefinedAttribute.h"
#include "AttributeCollectionService.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {

    bool REFLECTION_IMPORT_EXPORT IsUndefined(const Objects::Object& object)
    {
        auto type = object.GetType();
        if (!type)
            return false;
        auto undefinedAttribute = GetPointerToAttributeOrNull<UndefinedAttribute>(type->GetAttributeCollection());
        if (undefinedAttribute)
            return undefinedAttribute->InvokeFunction({ object });
        return false;
    }

}}}}


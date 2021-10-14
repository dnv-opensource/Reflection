#include "AttributeCollection.h"
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt



namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {

    const AttributeCollection& AttributeCollection::GetEmptyCollection()
    {
        return s_emptyCollection;
    }

    AttributeCollection AttributeCollection::s_emptyCollection;

}}}}
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ContainerElementAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    ContainerElementAspect::ContainerElementAspect()
    {

    }

    ContainerElementAspect::~ContainerElementAspect()
    {

    }

    const Attributes::AttributeCollection& ContainerElementAspect::GetAttributeCollection() const
    {
        return Attributes::AttributeCollection::GetEmptyCollection();
    }

    std::string ContainerElementAspect::ToString() const
    {
        return "ContainerElementAspect()";
    }

    bool ContainerElementAspect::operator==(const ContainerElementAspect& other) const
    {
        return true;
    }

}}}}


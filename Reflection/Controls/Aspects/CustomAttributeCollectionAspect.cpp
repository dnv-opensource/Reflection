//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "CustomAttributeCollectionAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    CustomAttributeCollectionAspect::CustomAttributeCollectionAspect(Attributes::AttributeCollection&& attributeCollection)
        : m_attributeCollection(std::move(attributeCollection))
    {}

    CustomAttributeCollectionAspect::~CustomAttributeCollectionAspect()
    {

    }

    const Attributes::AttributeCollection& CustomAttributeCollectionAspect::GetAttributeCollection() const
    {
        return m_attributeCollection;
    }

    std::string CustomAttributeCollectionAspect::ToString() const
    {
        return "CustomAttributeCollectionAspect(AttriuteCollection())";
    }

    bool CustomAttributeCollectionAspect::operator==(const CustomAttributeCollectionAspect& other) const
    {
        return true;
    }

}}}}


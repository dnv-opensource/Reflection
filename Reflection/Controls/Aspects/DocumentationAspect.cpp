//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DocumentationAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    DocumentationAspect::DocumentationAspect(const Attributes::DocumentationAttribute& attribute)
    {
        m_attributeCollection.AddAttribute(Attributes::DocumentationAttribute(attribute));
    }

    const Attributes::AttributeCollection& DocumentationAspect::GetAttributeCollection() const
    {
        return m_attributeCollection;
    }

    std::string DocumentationAspect::ToString() const
    {
        return "DocumentationAspect()";
    }

    bool DocumentationAspect::operator==(const DocumentationAspect& other) const
    {
        return true;
    }

}}}}


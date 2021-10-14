#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlAspect.h"
#include "Reflection/Members/IMemberFwd.h"
#include <set>
namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    //This aspect represents that a node can be constructed using one of the constructors mentioned.
    class REFLECTION_IMPORT_EXPORT CustomAttributeCollectionAspect : public IControlAspectT<CustomAttributeCollectionAspect>
    {
    public:
        CustomAttributeCollectionAspect(Attributes::AttributeCollection&& attributeCollection);
        template<typename AttribT, typename... Attribs>
        CustomAttributeCollectionAspect(AttribT&& attrib, Attribs&&... attribs)
            : CustomAttributeCollectionAspect(attribs...)
        {
            m_attributeCollection.AddAttribute(std::forward<AttribT>(attrib));
        }
        ~CustomAttributeCollectionAspect();
        const Attributes::AttributeCollection& GetAttributeCollection() const override;
        bool operator==(const CustomAttributeCollectionAspect& other) const override;
        std::string ToString() const override;
    private:
        CustomAttributeCollectionAspect() {}
        Attributes::AttributeCollection m_attributeCollection;
    };
}}}}
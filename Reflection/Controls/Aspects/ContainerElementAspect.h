#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlAspect.h"
#include "Reflection/Attributes/ContainerAttribute.h"
#include <set>
namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    //This aspect represents an element in a container. It implements the SupportUpdateParents method
    class REFLECTION_IMPORT_EXPORT ContainerElementAspect : public IControlAspectT<ContainerElementAspect>
    {
    public:
        ContainerElementAspect();
        ~ContainerElementAspect();
        const Attributes::AttributeCollection& GetAttributeCollection() const override;
        bool operator==(const ContainerElementAspect& other) const override;
        std::string ToString() const override;
        virtual bool SupportUpdateParent() const { return true; }
    private:
    };
}}}}
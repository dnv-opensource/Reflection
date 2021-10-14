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
    //This aspect represents that a node can be constructed using one of the constructors mentioned.
    class REFLECTION_IMPORT_EXPORT ContainerAspect : public IControlAspectT<ContainerAspect>
    {
    public:
        ContainerAspect(const Types::DecoratedTypeInfo& containerType, const Attributes::ContainerAttribute& containerAttribute);
        ~ContainerAspect();
        const Attributes::AttributeCollection& GetAttributeCollection() const override;
        bool operator==(const ContainerAspect& other) const override;
        std::string ToString() const override;
        const Types::DecoratedTypeInfo& GetContainerType() const;
        const Attributes::ContainerAttribute& GetContainerAttribute() const;
        std::vector<Contents::Content> Split(const Contents::Content& content) const;
        Contents::Content Join(const TypeLibraries::TypeLibraryPointer& typeLibrary, const std::vector<Contents::Content>& contents, bool simplify = true) const;
    private:
        Contents::Content JoinNoSimplify(const TypeLibraries::TypeLibraryPointer& typeLibrary, const std::vector<Contents::Content>& contents) const;
        Types::DecoratedTypeInfo m_containerType;
        const Attributes::ContainerAttribute& m_containerAttribute;
    };
}}}}
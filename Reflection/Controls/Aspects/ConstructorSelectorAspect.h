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
    class REFLECTION_IMPORT_EXPORT ConstructorSelectorAspect : public IControlAspectT<ConstructorSelectorAspect>
    {
    public:
        ConstructorSelectorAspect(const Types::DecoratedTypeInfo& decoratedTypeInfo, const std::set<Members::MemberPointer>& constructors);
        ~ConstructorSelectorAspect();
        const Attributes::AttributeCollection& GetAttributeCollection() const override;
        bool operator==(const ConstructorSelectorAspect& other) const override;
        std::string ToString() const override;
        const Types::DecoratedTypeInfo& GetDecoratedTypeInfo() const { return m_decoratedTypeInfo; }
        const std::set<Members::MemberPointer>& GetConstructors() const { return m_constructors; }
        virtual bool HasMember() const { return true; }
    private:
        Types::DecoratedTypeInfo m_decoratedTypeInfo;
        std::set<Members::MemberPointer> m_constructors;
    };
}}}}
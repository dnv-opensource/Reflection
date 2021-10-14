#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Members/IMember.h"
#include "LayoutSource.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class PropertyLayoutSource : public LayoutSource 
    {
    public:
        PropertyLayoutSource(const Members::MemberPointer& member);
        virtual std::string GetName() const override;
        virtual const Attributes::AttributeCollection& GetAttributeCollection() const override;
        virtual Types::DecoratedTypeInfo GetTypeInfo() const override;
        virtual Members::MemberPointer GetGetter() const override;
        virtual Members::MemberPointer GetSetter() const override;
    private:
        Members::MemberPointer m_member;
    };
}}}}
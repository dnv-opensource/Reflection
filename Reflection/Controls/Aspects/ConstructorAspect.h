#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlAspect.h"
#include "Reflection/Members/IMemberFwd.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    //This aspect represents that a node can be constructed using this constructor.
    class REFLECTION_IMPORT_EXPORT ConstructorAspect : public IControlAspectT<ConstructorAspect>
    {
    public:
        ConstructorAspect(const Members::MemberPointer& constructor);
        ~ConstructorAspect();
        const Attributes::AttributeCollection& GetAttributeCollection() const override;
        bool operator==(const ConstructorAspect& other) const override;
        std::string ToString() const override;
        const Members::MemberPointer& GetConstructor() const { return m_constructor; }
        virtual bool HasMember() const { return true; }
    private:
        Members::MemberPointer m_constructor;
    };
}}}}
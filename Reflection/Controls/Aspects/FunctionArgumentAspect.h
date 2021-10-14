#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlAspect.h"
#include "Reflection/Members/IMemberFwd.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    //This aspect represents that a node can be used as an argument to a constructor. It mentions both the constructor and the index of the argument.
    class REFLECTION_IMPORT_EXPORT FunctionArgumentAspect : public IControlAspectT<FunctionArgumentAspect>
    {
    public:
        FunctionArgumentAspect(const Members::MemberPointer& member, size_t index);
        ~FunctionArgumentAspect();
        const Attributes::AttributeCollection& GetAttributeCollection() const override;
        bool operator==(const FunctionArgumentAspect& other) const override;
        std::string ToString() const override;
        const Members::MemberPointer& GetMember() const { return m_member; }
        size_t GetIndex() const { return m_index; }
        Contents::Content TryGetPropertyContent(const Contents::Content& parent, bool trySimplify = true) override;
        virtual bool SupportUpdateParent() const override { return true; }
    private:
        Members::MemberPointer m_member;
        size_t m_index;
    };
}}}}
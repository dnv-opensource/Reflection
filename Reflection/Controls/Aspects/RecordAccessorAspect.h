#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlAspect.h"
#include "Reflection/Members/IMemberFwd.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    /*
    This aspect will be created if a function has the RecordAttribute. It will store the get and set method indicated by the RecordAttribute.
    It is used when creating grids from methods such as
    SetStiffness(i,j,Stiffness);
    GetStiffness(i,j);
    */
    class REFLECTION_IMPORT_EXPORT RecordAccessorAspect : public IControlAspectT<RecordAccessorAspect>
    {
    public:
        RecordAccessorAspect(const Members::MemberPointer& setFunction, const Members::MemberPointer& getFunction);
        ~RecordAccessorAspect();
        const Attributes::AttributeCollection& GetAttributeCollection() const override;
        bool operator==(const RecordAccessorAspect& other) const override;
        std::string ToString() const override;
        const Members::MemberPointer& GetGetFunction() const { return m_getFunction; }
        const Members::MemberPointer& GetSetFunction() const { return m_setFunction; }
        virtual bool SupportAssign() const { return true; }
        virtual bool HasMember() const { return true; }
    private:
        Members::MemberPointer m_setFunction;
        Members::MemberPointer m_getFunction;
    };

}}}}
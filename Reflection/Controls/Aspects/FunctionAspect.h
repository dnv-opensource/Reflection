#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlAspect.h"
#include "Reflection/Members/IMemberFwd.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    //This aspect represents that a node can be called using this function.
    class REFLECTION_IMPORT_EXPORT FunctionAspect : public IControlAspectT<FunctionAspect>
    {
    public:
        FunctionAspect(const Members::MemberPointer& function);
        ~FunctionAspect();
        const Attributes::AttributeCollection& GetAttributeCollection() const override;
        bool operator==(const FunctionAspect& other) const override;
        std::string ToString() const override;
        const Members::MemberPointer& GetFunction() const { return m_function; }
        virtual bool HasMember() const { return true; }
    private:
        Members::MemberPointer m_function;
    };
}}}}
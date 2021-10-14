#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlAspect.h"
#include "Reflection/Members/IMemberFwd.h"
#include <list>

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    //This aspect represents that a node can be constructed using one of the constructors mentioned.
    class REFLECTION_IMPORT_EXPORT FunctionSelectorAspect : public IControlAspectT<FunctionSelectorAspect>
    {
    public:
        FunctionSelectorAspect(const std::list<Members::MemberPointer>& functions);
        ~FunctionSelectorAspect();
        const Attributes::AttributeCollection& GetAttributeCollection() const override;
        bool operator==(const FunctionSelectorAspect& other) const override;
        std::string ToString() const override;
        const std::list<Members::MemberPointer>& GetFunctions() const { return m_functions; }
        virtual bool HasMember() const { return true; }
    private:
        std::list<Members::MemberPointer> m_functions;
    };
}}}}
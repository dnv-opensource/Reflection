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
    //This aspect represents a single constructor and allows extracting data.
    class REFLECTION_IMPORT_EXPORT SelectedConstructorAspect : public IControlAspectT<SelectedConstructorAspect>
    {
    public:
        SelectedConstructorAspect(const Members::MemberPointer& constructor);
        ~SelectedConstructorAspect();
        const Attributes::AttributeCollection& GetAttributeCollection() const override;
        bool operator==(const SelectedConstructorAspect& other) const override;
        std::string ToString() const override;
        const Members::MemberPointer GetConstructor() const { return m_constructor; }
        Contents::Content TryGetPropertyContent(const Contents::Content& parent, bool trySimplify = true) override;
        bool SupportUpdateParent() const override { return true; }
        bool SupportAssign() const override;
        Contents::Content TrySetPropertyContent(const Contents::Content& parent, const Contents::Content& value) override;
    private:
        Members::MemberPointer m_constructor;
    };
}}}}
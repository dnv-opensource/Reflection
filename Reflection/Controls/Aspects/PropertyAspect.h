#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlAspect.h"
#include "Reflection/Members/IMemberFwd.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    //This aspect is used in order to represent a property, such as Vector3d.X.
    //It will support getting and setting of this property.
    class REFLECTION_IMPORT_EXPORT PropertyAspect : public IControlAspectT<PropertyAspect>
    {
    public:
        PropertyAspect(const Members::MemberPointer& property);
        ~PropertyAspect();
        const Attributes::AttributeCollection& GetAttributeCollection() const override;
        bool operator==(const PropertyAspect& other) const override;
        std::string ToString() const override;
        Members::MemberPointer GetGetter() const;
        Members::MemberPointer GetSetter() const;
        Contents::Content TryGetPropertyContent(const Contents::Content& parent, bool trySimplify = true) override;
        Contents::Content TrySetPropertyContent(const Contents::Content& parent, const Contents::Content& value) override;
        bool SupportAssign() const override;
        virtual bool HasMember() const { return true; }
    private:
        Members::MemberPointer m_property;
    };

}}}}
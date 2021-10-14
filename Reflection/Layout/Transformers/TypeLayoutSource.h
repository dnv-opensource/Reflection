#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Members/IMember.h"
#include "LayoutSource.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class TypeLayoutSource : public LayoutSource
    {
    public:
        TypeLayoutSource(const std::string& name, const Types::DecoratedTypeInfo& info, const Attributes::AttributeCollection& attributeCollection);
        virtual std::string GetName() const override;
        virtual const Attributes::AttributeCollection& GetAttributeCollection() const override;
        virtual Types::DecoratedTypeInfo GetTypeInfo() const override;
        virtual Members::MemberPointer GetGetter() const override;
        virtual Members::MemberPointer GetSetter() const override;
        virtual std::string GetCaption(const TypeLibraries::TypeLibraryPointer& typeLibrary, const std::string& controlName, const std::string& controlCaption) const override;
        virtual bool AllowEmbedElement() const;
    private:
        std::string m_name;
        Types::DecoratedTypeInfo m_info;
        const Attributes::AttributeCollection& m_attributeCollection;
    };
}}}}
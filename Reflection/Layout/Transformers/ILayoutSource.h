#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Attributes/AttributeCollection.h"
#include "Reflection/Types/DecoratedTypeInfo.h"
#include "Reflection/Members/IMemberFwd.h"
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"
#include "Reflection/Layout/LayoutContext.h"
#include "TypeUtilities/CompareCaseLess.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class ILayoutSource 
    {
    public:
        virtual ~ILayoutSource() {}
        virtual std::string GetName() const = 0;
        virtual std::string GetCaption(const TypeLibraries::TypeLibraryPointer& typeLibrary, const std::string& controlName, const std::string& controlCaption) const = 0;
        virtual LayoutContext GetContext(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Members::MemberPointer& member, Types::DecoratedTypeInfo& parentTypeInfo) const = 0;
        virtual const Attributes::AttributeCollection& GetAttributeCollection() const = 0;
        virtual Types::DecoratedTypeInfo GetTypeInfo() const = 0;
        virtual Members::MemberPointer GetGetter() const = 0;
        virtual Members::MemberPointer GetSetter() const = 0;
        virtual std::map<std::string, Variants::Variant, TypeUtilities::CompareCaseLess> GetAdditionalArguments() const = 0;
        virtual bool AllowEmbedElement() const = 0;
    };
}}}}
#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ILayoutSource.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class LayoutSource : public ILayoutSource
    {
    public:
        virtual std::string GetCaption(const TypeLibraries::TypeLibraryPointer& typeLibrary, const std::string& controlName, const std::string& controlCaption) const override;
        virtual LayoutContext GetContext(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Members::MemberPointer& member, Types::DecoratedTypeInfo& parentTypeInfo) const override;
        virtual bool AllowEmbedElement() const;
        virtual std::map<std::string, Variants::Variant, TypeUtilities::CompareCaseLess> GetAdditionalArguments() const override;
    };
}}}}
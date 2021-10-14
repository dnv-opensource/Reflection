#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ILayoutSourceProvider.h"
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"
#include "Reflection/Members/IMember.h"
#include "TypeUtilities/CompareCaseLess.h"
#include <set>

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class ConstructorFinderSourceProvider
        : public ILayoutSourceProvider
    {
    public:
        ConstructorFinderSourceProvider(TypeLibraries::TypeLibraryPointer typeLibrary, const Types::DecoratedTypeInfo& info);
        virtual std::shared_ptr<ILayoutSource> LookupLayoutSource(const std::string& name) override;
        Members::MemberPointer GetBestConstructor() const;
    private:
        bool IsValidConstructor(const Members::MemberPointer& member) const;
        std::shared_ptr<ILayoutSourceProvider> m_actualProvider;
        TypeLibraries::TypeLibraryPointer m_typeLibrary;
        Types::DecoratedTypeInfo m_type;
        mutable std::set<std::string, TypeUtilities::CompareCaseLess> m_locatedNames;
    };
}}}}

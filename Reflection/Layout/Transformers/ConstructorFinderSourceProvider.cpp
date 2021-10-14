//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ConstructorFinderSourceProvider.h"
#include "Reflection/Utilities/ConstructorsFromTypeService.h"
#include "MembersLayoutSourceProvider.h"
#include "ILayoutSource.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    ConstructorFinderSourceProvider::ConstructorFinderSourceProvider(TypeLibraries::TypeLibraryPointer typeLibrary, const Types::DecoratedTypeInfo& info)
        : m_actualProvider(std::make_shared<MemberLayoutSourceProvider>(typeLibrary, info))
        , m_typeLibrary(typeLibrary)
        , m_type(info)
    {
    }

    std::shared_ptr<ILayoutSource> ConstructorFinderSourceProvider::LookupLayoutSource(const std::string& name)
    {
        auto layout = m_actualProvider->LookupLayoutSource(name);
        if (layout)
            m_locatedNames.insert(layout->GetName());
        return layout;
    }

    Members::MemberPointer ConstructorFinderSourceProvider::GetBestConstructor() const
    {
        auto constructors = Utilities::ConstructorsFromTypeService::GetService(m_typeLibrary)->CollectAllConstructorMembersOfThisAndDerivedClasses(m_type, false);
        for(const auto& constructor : constructors)
        {
            if (IsValidConstructor(constructor))
                return constructor;
        }
        return nullptr;
    }

    bool ConstructorFinderSourceProvider::IsValidConstructor(const Members::MemberPointer& member) const
    {
        if (m_locatedNames.size() < member->GetMinimumArgumentCount())
            return false;
        if (m_locatedNames.size() > member->GetMaximumArgumentCount())
            return false;
        size_t argumentsFound = 0;
        for (size_t i = 0; i < member->GetArity(); ++i)
        {
            if (m_locatedNames.count(member->GetArgumentInfo(i)->GetName()) > 0)
                ++argumentsFound;
            else if (!member->GetArgumentInfo(i)->HasDefault())
                return false;
        }
        return argumentsFound == m_locatedNames.size();
    }

}}}}



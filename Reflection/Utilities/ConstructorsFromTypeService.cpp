//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ConstructorsFromTypeService.h"
#include "ConstructorsFromTypeExtracter.h"
#include "Services/ServiceProvider.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {

    std::set<Members::MemberPointer> ConstructorsFromTypeService::CollectAllConstructorMembersOfThisAndDerivedClasses(const Types::DecoratedTypeInfo& decoratedTypeInfo, bool allowAll)
    {
        size_t count = m_constructorMembers.size();
        auto& members = m_constructorMembers[std::make_pair(decoratedTypeInfo, allowAll)];
        if (m_constructorMembers.size() > count)
        {
            members = ConstructorsFromTypeExtracter(m_typeLibrary).CollectAllConstructorMembersOfThisAndDerivedClasses(decoratedTypeInfo, allowAll);
        }
        return members;
    }

    std::set<Members::MemberPointer> ConstructorsFromTypeService::CollectAllSerializationConstructorMembersOfThisAndDerivedClasses(const Types::DecoratedTypeInfo& decoratedTypeInfo)
    {
        size_t count = m_serializationMembers.size();
        auto& members = m_serializationMembers[decoratedTypeInfo];
        if (m_serializationMembers.size() > count)
        {
            members = ConstructorsFromTypeExtracter(m_typeLibrary).CollectAllSerializationConstructorMembersOfThisAndDerivedClasses(decoratedTypeInfo);
        }            
        return members;
    }

    std::shared_ptr<ConstructorsFromTypeService> ConstructorsFromTypeService::GetService(const TypeLibraries::TypeLibraryPointer& typeLibrary)
    {
        auto service = Services::ServiceProvider::Instance().TryGetService<ConstructorsFromTypeService>();
        if (service)
            return service;
        else return std::make_shared<ConstructorsFromTypeService>(typeLibrary);
    }

    ConstructorsFromTypeService::ConstructorsFromTypeService(const TypeLibraries::TypeLibraryPointer& typeLibrary)
        : m_typeLibrary(typeLibrary)
    {
    }

}}}}


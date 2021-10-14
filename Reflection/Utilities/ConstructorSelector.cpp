//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ConstructorSelector.h"
#include "CallableConstructor.h"
#include "ConstructorsFromTypeService.h"
#include "Reflection/Types/DynamicTypeTraits.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {    

    ConstructorSelector::ConstructorSelector(const Objects::Object& object, bool checkRecursive)
        : m_object(object)
        , m_checkRecursive(checkRecursive)
    {
    }

    CallableConstructor ConstructorSelector::GetBestConstructor()
    {
        CallableConstructor best = GetBestConstructor(ConstructorsFromTypeService::GetService(m_object.GetTypeLibrary())->CollectAllConstructorMembersOfThisAndDerivedClasses(m_object.GetDecoratedTypeInfo(), true));
        if(!best.IsValid())
            best = GetBestConstructor(ConstructorsFromTypeService::GetService(m_object.GetTypeLibrary())->CollectAllConstructorMembersOfThisAndDerivedClasses(Types::AddConst(m_object.GetDecoratedTypeInfo()), true));
        return best;
    }

    Utilities::CallableConstructor ConstructorSelector::GetBestConstructor(const std::set<Members::MemberPointer>& members) const
    {
        std::set<CallableConstructor> sortedConstructors;
        for (auto member : members)
        {
            CallableConstructor constructor(member, m_object);
            if (constructor.IsValid())
            {
                if(!m_checkRecursive || !constructor.IsInfinitlyRecursive())
                    sortedConstructors.insert(constructor);
            }
        }
        if (sortedConstructors.empty())
            return CallableConstructor();
        else
            return *sortedConstructors.begin();
    }

}}}}

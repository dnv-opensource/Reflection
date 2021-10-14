#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Members/IMember.h"
#include <set>
#include <typeindex>
namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {
    class ConstructorFromTypeSubset {
    public:
        ConstructorFromTypeSubset(const std::set<std::type_index>& types, bool includeTypes = true)
            : m_types(types)
            , m_includeTypes(includeTypes)
        {}
        std::set<Members::MemberPointer> operator()(const TypeLibraries::TypeLibraryPointer& typeLibrary, std::set<Members::MemberPointer> members) const
        {
            std::set<Members::MemberPointer> subset;
            for(const auto& member : members)
            {
                bool isIncluded = m_types.count(member->GetReturnType().GetTypeInfo()) > 0;
                if (isIncluded == m_includeTypes)
                    subset.insert(member);
            }
            return subset;
        }
        void AddType(const std::type_index& type)
        {
            m_types.insert(type);
        }
    private:
        std::set<std::type_index> m_types;
        bool m_includeTypes;
    };

    template<typename... T>
    struct ConstructorPopulator
    {
        static void Populate(ConstructorFromTypeSubset&) {}
    };

    template<typename T, typename... Types>
    struct ConstructorPopulator<T,Types...>
    {
        static void Populate(ConstructorFromTypeSubset& subset)
        {
            subset.AddType(typeid(T));
            ConstructorPopulator<Types...>::Populate(subset);
        }
    };


    template<typename... Types>
    ConstructorFromTypeSubset IncludeConstructorsOfTypes()
    {
        ConstructorFromTypeSubset selector({}, true);
        ConstructorPopulator<Types...>::Populate(selector);
        return selector;
    }

    template<typename... Types>
    ConstructorFromTypeSubset ExcludeConstructorsOfTypes()
    {
        ConstructorFromTypeSubset selector({}, false);
        ConstructorPopulator<Types...>::Populate(selector);
        return selector;
    }

}}}}
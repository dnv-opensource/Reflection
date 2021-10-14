#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "IMemberFinder.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {
    template<typename T>
    class ConstructorFinder : public IMemberFinder
    {
    public:
        ConstructorFinder(const std::string& name)
            : m_name(name)
        {}
        virtual Members::MemberPointer GetMember(const TypeLibraries::TypeLibraryPointer& typeLibrary) const
        {
            return MemberUtilities(typeLibrary).GetMemberFromType(m_name, Types::TypeId<T>(), Members::MemberType::TypeConstructor);
        }
    private:
        std::string m_name;
    };

    template<typename R, typename... Args>
    class ConstructorFinder<R(Args...)> : public IMemberFinder
    {
    public:
        ConstructorFinder(const std::string& name)
            : m_name(name)
        {}
        virtual Members::MemberPointer GetMember(const TypeLibraries::TypeLibraryPointer& typeLibrary) const
        {
            TypedMemberUtilities utils(typeLibrary, Types::TypeId<R>());
            return utils.GetConstructor<Args...>(m_name);
        }
    };


    template<typename T>
    std::shared_ptr<IMemberFinder> FindConstructor(const std::string& name)
    {
        return std::make_shared<ConstructorFinder<T>>(name);
    }


    template<typename T>
    Members::MemberPointer FindConstructor(const std::string& name, const TypeLibraries::TypeLibraryPointer& typeLibrary)
    {
        return FindConstructor<T>(name)->GetMember(typeLibrary);
    }

    template<typename T>
    std::shared_ptr<IMemberFinder> FindConstructor()
    {
        return FindConstructor<T>(Members::ConstructorString);
    }


    template<typename T>
    Members::MemberPointer FindConstructor(const TypeLibraries::TypeLibraryPointer& typeLibrary)
    {
        return FindConstructor<T>(Members::ConstructorString, typeLibrary);
    }

}}}}

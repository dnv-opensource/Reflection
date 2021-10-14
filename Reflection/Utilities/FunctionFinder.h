#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "IMemberFinder.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {
    template<typename T, typename SignatureT = void>
    class FunctionFinder : public IMemberFinder
    {
    public:
        FunctionFinder(const std::string& name)
            : m_name(name)
        {}
        virtual Members::MemberPointer GetMember(const TypeLibraries::TypeLibraryPointer& typeLibrary) const
        {
            TypedMemberUtilities utils(typeLibrary, Types::TypeId<T>());
            return utils.GetMember(m_name);
        }
    private:
        std::string m_name;
    };

    template<typename R, typename C, typename... Args>
    class FunctionFinder<R(C::*)(Args...)> : public IMemberFinder
    {
    public:
        FunctionFinder(const std::string& name)
            : m_name(name)
        {}
        virtual Members::MemberPointer GetMember(const TypeLibraries::TypeLibraryPointer& typeLibrary) const
        {
            TypedMemberUtilities utils(typeLibrary, Types::TypeId<C>());
            return utils.GetMemberWithSignature<Args...>(m_name);
        }
    private:
        std::string m_name;
    };

    template<typename R, typename C, typename... Args>
    class FunctionFinder<R(C::*)(Args...) const> : public IMemberFinder
    {
    public:
        FunctionFinder(const std::string& name)
            : m_name(name)
        {}
        virtual Members::MemberPointer GetMember(const TypeLibraries::TypeLibraryPointer& typeLibrary) const
        {
            TypedMemberUtilities utils(typeLibrary, Types::TypeId<C>());
            return utils.GetMemberWithSignature<Args...>(m_name);
        }
    private:
        std::string m_name;
    };

    template<typename R, typename C, typename... Args>
    class FunctionFinder<R(C,Args...)> : public IMemberFinder
    {
    public:
        FunctionFinder(const std::string& name)
            : m_name(name)
        {}
        virtual Members::MemberPointer GetMember(const TypeLibraries::TypeLibraryPointer& typeLibrary) const
        {
            TypedMemberUtilities utils(typeLibrary, Types::TypeId<C>());
            return utils.GetMemberWithSignature<Args...>(m_name);
        }
    private:
        std::string m_name;
    };

    template<typename T, typename R, typename... Args>
    class FunctionFinder<T,R(Args...)> : public IMemberFinder
    {
    public:
        FunctionFinder(const std::string& name)
            : m_name(name)
        {}
        virtual Members::MemberPointer GetMember(const TypeLibraries::TypeLibraryPointer& typeLibrary) const
        {
            TypedMemberUtilities utils(typeLibrary, Types::TypeId<T>());
            return utils.GetMemberWithSignature<Args...>(m_name);
        }
    private:
        std::string m_name;
    };

    template<typename... Args>
    std::shared_ptr<IMemberFinder> FindFunction(const std::string& name)
    {
        return std::make_shared<FunctionFinder<Args...>>(name);
    }


    template<typename... Args>
    Members::MemberPointer FindFunction(const std::string& name, const TypeLibraries::TypeLibraryPointer& typeLibrary)
    {
        return FindFunction<Args...>(name)->GetMember(typeLibrary);
    }

}}}}

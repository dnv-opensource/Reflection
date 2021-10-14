#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"
#include "Reflection/TypeLibraries/ITypeLibrary.h"
#include "Reflection/Members/IMember.h"
#include "Reflection/Members/MemberType.h"
#include "Reflection/Operators/BinaryOperator.h"
#include "Reflection/Operators/UnaryOperator.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {    
    //Allow simplified access to reflected members. This is useful when writing unit tests. 
    //Typically, this helper class is used by the TypedMemberUtilities, so you will not use MemberUtilities directly in your code
    class MemberUtilities {
    public:
        MemberUtilities(const TypeLibraries::TypeLibraryPointer& typeLibrary)
            : m_typeLibrary(typeLibrary)
        {}
        template<typename T>
        Members::MemberPointer GetMemberFromType(const std::string& name, Members::MemberType memberType)
        {
            return GetMemberFromType(name, Types::TypeId<T>(), memberType);
        }
        Members::MemberPointer GetMemberFromType(const std::string& name, const Types::DecoratedTypeInfo& typeInfo, Members::MemberType memberType)
        {
            if (!m_typeLibrary)
                return nullptr;
            auto type = m_typeLibrary->LookupType(typeInfo.GetTypeInfo());
            if (!type)
                return nullptr;
            auto member = type->Lookup(name, Variants::Variant());
            if (!member)
                return nullptr;
            for (auto overload : member->GetOverloads())
            {
                if (overload->GetMemberType() & memberType)
                    return overload;
            }
            return nullptr;
        }
        template<typename... Args>
        struct ArgumentValidator {};
        template<>
        struct ArgumentValidator<> 
        {
            static bool Validate(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Members::MemberPointer& member, size_t iarg)
            {
                return iarg >= member->GetMinimumArgumentCount();
            }
        };
        template<typename Arg, typename... Args>
        struct ArgumentValidator<Arg, Args...>
        {
            static bool Validate(const TypeLibraries::TypeLibraryPointer& typeLibrary,const Members::MemberPointer& member, size_t iarg)
            {
                if (iarg >= member->GetMaximumArgumentCount())
                    return false;
                auto conversionSequence = typeLibrary->GetConversionGraph()->GetConversionSequence(Types::TypeId<Arg>(), member->GetArgumentInfo(iarg)->GetTypeInfo());
                if(!conversionSequence || !conversionSequence->IsValid())
                    return false;
                return ArgumentValidator<Args...>::Validate(typeLibrary, member, iarg + 1);
            }
        };
        template<typename... Args>
        Members::MemberPointer GetMemberFromTypeWithArguments(const std::string& name, const Types::DecoratedTypeInfo& typeInfo, Members::MemberType memberType)
        {
            if (!m_typeLibrary)
                return nullptr;
            auto type = m_typeLibrary->LookupType(typeInfo.GetTypeInfo());
            if (!type)
                return nullptr;
            auto member = type->Lookup(name, Variants::Variant());
            if (!member)
                return nullptr;
            for (auto overload : member->GetOverloads())
            {
                if (overload->GetMemberType() & memberType)
                {
                    size_t firstArgumentIndex = 0;
                    if (overload->GetMemberType() == Members::MemberType::TypeMemberFunction)
                        firstArgumentIndex = 1;
                    if (ArgumentValidator<Args...>::Validate(m_typeLibrary, overload, firstArgumentIndex))
                        return overload;
                }
            }
            return nullptr;
        }
    private:
        TypeLibraries::TypeLibraryPointer m_typeLibrary;
    };

    //TypedMemberUtilities is useful when you want to access a specific constructor or operator for a given type.
    //Example: If you want to get the constructor for vec3d, you do the following:
    //TypeMemberUtilities vec3dUtils(typeLibrary, TypeId<vec3d>());
    //
    //This will return the constructor that takes three length (x,y,z) as arguments.
    //vec3dUtils.GetConstructor<Length,Length,Length>("Vector3d");
    //
    //This will return the constructor that takes two points (p1, p2) as arguments.
    //vec3dUtils.GetConstructor<const pos3d&, const pos3d&>("Vector3d");
    //
    //You can also get operators. This will return the vec3d + vec3d operator:
    //vec3dUtils.GetOperator(This.Const + This.Const);
    class TypedMemberUtilities {
    public:
        TypedMemberUtilities(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Types::DecoratedTypeInfo& typeInfo)
            : m_typeLibrary(typeLibrary)
            , m_typeInfo(typeInfo)
        {}

        template<typename... Arguments>
        Members::MemberPointer GetConstructor()
        {
            return GetConstructor<Arguments...>(Members::ConstructorString);
        }
        template<typename... Arguments>
        Members::MemberPointer GetConstructor(const std::string& name)
        {
            return MemberUtilities(m_typeLibrary).GetMemberFromTypeWithArguments<Arguments...>(name, m_typeInfo, Members::MemberType::TypeConstructor);
        }
        template<typename... Arguments>
        Members::MemberPointer GetMemberWithSignature(const std::string& name)
        {
            return MemberUtilities(m_typeLibrary).GetMemberFromTypeWithArguments<Arguments...>(name, m_typeInfo, Members::MemberType::TypeFunction);
        }
        Members::MemberPointer GetMember(const std::string& name)
        {
            auto type = m_typeLibrary->LookupType(m_typeInfo.GetTypeInfo());
            if (!type)
                return nullptr;
            return type->Lookup(name, Variants::Variant());
        }
        template<typename Lhs, typename Rhs, typename Tag>
        Members::MemberPointer GetOperator(Operators::ThisOperators::BinaryOperatorGenerator<Lhs,Rhs,Tag>)
        {
            if (!m_typeLibrary)
                return nullptr;
            auto typeLhs = GetType(Operators::Other<Lhs>());
            auto typeRhs = GetType(Operators::Other<Rhs>());
            auto type = m_typeLibrary->LookupType(m_typeInfo.GetTypeInfo());
            if (!type)
                return nullptr;
            auto member = type->Lookup(Operators::ThisOperators::BinaryOperatorGenerator<Lhs, Rhs, Tag>::TypeName::Name(), Variants::Variant());
            if (!member)
                return nullptr;
            for (auto overload : member->GetOverloads())
            {
                if (overload->GetMemberType() & Members::MemberType::TypeOperator)
                {
                    if (!m_typeLibrary->GetConversionGraph()->GetConversionSequence(typeLhs, overload->GetArgumentInfo(0)->GetTypeInfo()))
                        continue;
                    if (!m_typeLibrary->GetConversionGraph()->GetConversionSequence(typeRhs, overload->GetArgumentInfo(1)->GetTypeInfo()))
                        continue;
                    return overload;
                }
            }
            return nullptr;
        }
        template<int Decoration>
        Types::DecoratedTypeInfo GetType(Operators::Other<Operators::ThisOperators::This<Decoration>>)
        {
            return m_typeInfo;
        }
        template<typename T>
        Types::DecoratedTypeInfo GetType(Operators::Other<T>)
        {
            return Types::TypeId<T>();
        }
        template<typename T>
        Types::DecoratedTypeInfo GetType(Operators::Other<Operators::Other<T>>)
        {
            return Types::TypeId<T>();
        }
    private:
        TypeLibraries::TypeLibraryPointer m_typeLibrary;
        Types::DecoratedTypeInfo m_typeInfo;
    };
}}}}

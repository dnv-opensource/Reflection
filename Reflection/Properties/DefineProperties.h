#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "PropertyGet.h"
#include "PropertySet.h"
#include "PropertyStaticGet.h"
#include <type_traits>
#include "Reflection/AutoReflect.h"
#include "Reflection/Members/GlobalType.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Properties {
    const Members::GlobalType Global;

    template<typename ReflectedT, typename AutoReflectorT = IgnoreAutoReflector,typename EnableIf=void>
    class DefineProperties
    {
    public:
        DefineProperties(const TypeLibraries::TypeLibraryPointer& typeLibrary, TypeLibraries::TypePointer type) 
            : m_type(type) 
            , m_typeLibrary(typeLibrary)
        {}
        template<typename Signature>
        PropertyGet<Signature>& Get(const std::string& name, Signature get) {
            static_assert(TypeUtilities::FunctionTraits<Signature>::Arity == 1, "Property Get must have one argument");
            typedef PropertyGet<Signature> FunctionType;
            boost::intrusive_ptr<FunctionType> method(new FunctionType(name, m_type->GetConversionGraph(), get));
            AutoReflectorT::AutoReflectSignature<Signature>(m_typeLibrary);
            return *boost::static_pointer_cast<FunctionType>(m_type->AddMember(name, method));
        }

        template<typename ClassType, typename ReturnType>
        PropertyGet<ReturnType (ClassType::*)() const>& Get(const std::string& name,ReturnType (ClassType::*get)() const) {
            typedef PropertyGet<ReturnType(ClassType::*)() const> FunctionType;
            boost::intrusive_ptr<FunctionType> method(new FunctionType(name,m_type->GetConversionGraph(),get));
            AutoReflectorT::AutoReflectSignature<ReturnType(ClassType::*)() const>(m_typeLibrary);
            return *boost::static_pointer_cast<FunctionType>(m_type->AddMember(name, method));
        }
        template<typename ClassType, typename ReturnType>
        PropertyGet<ReturnType (ClassType::*)>& Get(const std::string& name,ReturnType (ClassType::*get)) {
            typedef PropertyGet<ReturnType(ClassType::*)> FunctionType;
            boost::intrusive_ptr<FunctionType> method(new FunctionType(name,m_type->GetConversionGraph(),get));
            AutoReflectorT::AutoReflectSignature<ReturnType(ClassType::*)>(m_typeLibrary);
            return *boost::static_pointer_cast<FunctionType>(m_type->AddMember(name, method));
        }


        template<typename ReturnType>
        PropertyStaticGet<ReturnType>& StaticGet(const std::string& name, ReturnType(*get)(), Members::GlobalType)
        {
            typedef PropertyStaticGet<ReturnType> FunctionType;
            FunctionType& method = StaticGet(name, get);
            m_typeLibrary->LookupType(typeid(Members::GlobalType))->AddMember(name, &method);
            return method;
        }

        template<typename ReturnType>
        PropertyStaticGet<ReturnType>& StaticGet(const std::string& name, ReturnType(*get)()) 
        {
            typedef PropertyStaticGet<ReturnType> FunctionType;
            boost::intrusive_ptr<FunctionType> method(new FunctionType(name, m_type->GetConversionGraph(), get()));
            return *boost::static_pointer_cast<FunctionType>(m_type->AddMember(name, method));
        }

        template<typename ReturnType>
        PropertyStaticGet<ReturnType>& StaticGet(const std::string& name, ReturnType value, Members::GlobalType)
        {
            typedef PropertyStaticGet<ReturnType> FunctionType;
            FunctionType& method = StaticGet(name, value);
            m_typeLibrary->LookupType(typeid(Members::GlobalType))->AddMember(name, &method);
            return method;
        }

        template<typename ReturnType>
        PropertyStaticGet<ReturnType>& StaticGet(const std::string& name, ReturnType value) {
            typedef PropertyStaticGet<ReturnType> FunctionType;
            boost::intrusive_ptr<FunctionType> method(new FunctionType(name, m_type->GetConversionGraph(), value));
            return *boost::static_pointer_cast<FunctionType>(m_type->AddMember(name, method));
        }

        template<typename ClassType, typename ArgumentType>
        PropertySet<void (ClassType::*)(ArgumentType)>& Set(const std::string& name, void (ClassType::*set)(ArgumentType)) {
            typedef PropertySet<void (ClassType::*)(ArgumentType)> FunctionType;
            boost::intrusive_ptr<FunctionType> method(new FunctionType(name,m_type->GetConversionGraph(),set));
            AutoReflectorT::AutoReflectSignature<void (ClassType::*)(ArgumentType)>(m_typeLibrary);
            return *boost::static_pointer_cast<FunctionType>(m_type->AddMember(name, method));
        }

        template<typename Signature>
        PropertySet<Signature>& Set(const std::string& name, Signature set)
        {
            static_assert(TypeUtilities::FunctionTraits<Signature>::Arity == 2, "Property Get must have two argument");
            typedef PropertySet<Signature> FunctionType;
            boost::intrusive_ptr<FunctionType> method(new FunctionType(name, m_type->GetConversionGraph(), set));
            AutoReflectorT::AutoReflectSignature<Signature>(m_typeLibrary);
            return *boost::static_pointer_cast<FunctionType>(m_type->AddMember(name, method));
        }

        template<typename ClassType,typename ArgumentType, typename ReturnType>
        PropertyGet<ReturnType(ClassType::*)() const>& SetGet(const std::string& name, void (ClassType::*set)(ArgumentType), ReturnType (ClassType::*get)() const)
        {
            Set(name, set);
            return Get(name, get);
        }

        template<typename SignatureSet, typename SignatureGet>
        PropertyGet<SignatureGet>& SetGet(const std::string& name, SignatureSet set, SignatureGet get)
        {
            Set(name, set);
            return Get(name, get);
        }

    private:
        TypeLibraries::TypePointer m_type;
        TypeLibraries::TypeLibraryPointer m_typeLibrary;
    };
    ///Disable property definition for all native types, such as int, float etc.
    template<typename ReflectedT, typename AutoReflectorT>
    class DefineProperties<ReflectedT, AutoReflectorT, typename std::enable_if<!std::is_class<ReflectedT>::value>::type>
    {
    public:
        DefineProperties(const TypeLibraries::TypeLibraryPointer& typeLibrary, TypeLibraries::TypePointer type) {}
    };


}}}}
#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Function.h"
#include "Reflection/TypeLibraries/IType.h"
#include "Reflection/Members/Modifier.h"
#include "Reflection/Members/MemberFunctionFromSignature.h"
#include "Reflection/AutoReflect.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Functions {

    template<typename Reflected, typename AutoReflectorT = IgnoreAutoReflector>
    class DefineFunction
    {
    public:
        DefineFunction(const TypeLibraries::TypeLibraryPointer& typeLibrary,TypeLibraries::TypePointer type) 
            : m_type(type) 
            , m_typeLibrary(typeLibrary)
        {}
        
        //Member function
        template<typename Signature>
        Functions::Function<Signature,Members::ModifierNamespace::NoModifier,true>& Function(const std::string& name,Signature function) {
            typedef Functions::Function<Signature, Members::ModifierNamespace::NoModifier, true> FunctionType;
            boost::intrusive_ptr<FunctionType> method(new FunctionType(name,m_type->GetConversionGraph(),function));
            AutoReflectorT::AutoReflectSignature<Signature>(m_typeLibrary);
            return *boost::static_pointer_cast<FunctionType>(m_type->AddMember(name,method));
        }

		//Member function with simplified function deduction.
		template<typename Signature>
		Functions::Function<
			typename Members::MemberFunctionFromSignature<Signature>::template Rebind<Reflected>::Type,
			Members::ModifierNamespace::NoModifier,
			true
		>& Function(const std::string& name,typename Members::MemberFunctionFromSignature<Signature>::template Rebind<Reflected>::Type function) 
		{
            return Function(name,function);
		}

		//Member function with simplified function deduction, Const version.
		template<typename Signature,typename Modifier>
		Functions::Function<
			typename Members::MemberFunctionFromSignature<Signature,Modifier>::template Rebind<Reflected>::Type,
			Members::ModifierNamespace::NoModifier,
			true
		>& Function(const std::string& name,typename Members::MemberFunctionFromSignature<Signature,Modifier>::template Rebind<Reflected>::Type function) 
		{
            return Function(name,function);
		}

        //Member function with variable number of arguments
        template<typename Signature>
        Functions::Function<Signature,Members::ModifierNamespace::Vararg,true>& Function(const std::string& name,Signature function,Members::ModifierNamespace::Vararg) {
            typedef Functions::Function<Signature, Members::ModifierNamespace::Vararg, true> FunctionType;
            boost::intrusive_ptr<FunctionType> method(new FunctionType(name,m_type->GetConversionGraph(),function));
            AutoReflectorT::AutoReflectSignature<Signature>(m_typeLibrary);
            return *boost::static_pointer_cast<FunctionType>(m_type->AddMember(name, method));
        }

		//Member function with simplified function deduction. Vararg.
		template<typename Signature>
		Functions::Function<
			typename Members::MemberFunctionFromSignature<Signature>::template Rebind<Reflected>::Type,
			Members::ModifierNamespace::Vararg,
			true
		>& Function(const std::string& name,typename Members::MemberFunctionFromSignature<Signature>::template Rebind<Reflected>::Type function,Members::ModifierNamespace::Vararg) 
		{
            return Function(name,function,Vararg);
		}

		//Member function with simplified function deduction, const version. Vararg.
		template<typename Signature,typename Modifier>
		Functions::Function<
			typename Members::MemberFunctionFromSignature<Signature,Modifier>::template Rebind<Reflected>::Type,
			Members::ModifierNamespace::Vararg,
			true
		>& Function(const std::string& name,typename Members::MemberFunctionFromSignature<Signature,Modifier>::template Rebind<Reflected>::Type function,Members::ModifierNamespace::Vararg) 
		{
            return Function(name,function,Vararg);
		}

        //Static function
        template<typename Signature>
        Functions::Function<Signature,Members::ModifierNamespace::NoModifier,false>& StaticFunction(const std::string& name,Signature function) {
            typedef Functions::Function<Signature, Members::ModifierNamespace::NoModifier, false> FunctionType;
            boost::intrusive_ptr<FunctionType> method(new FunctionType(name,m_type->GetConversionGraph(),function));
            AutoReflectorT::AutoReflectSignature<Signature>(m_typeLibrary);
            return *boost::static_pointer_cast<FunctionType>(m_type->AddMember(name, method));
        }
        //Static function with variable number of arguments
        template<typename Signature>
        Functions::Function<Signature,Members::ModifierNamespace::Vararg,false>& StaticFunction(const std::string& name,Signature function,Members::ModifierNamespace::Vararg) {
            typedef Functions::Function<Signature, Members::ModifierNamespace::Vararg, false> FunctionType;
            boost::intrusive_ptr<FunctionType> method(new FunctionType(name,m_type->GetConversionGraph(),function));
            AutoReflectorT::AutoReflectSignature<Signature>(m_typeLibrary);
            return *boost::static_pointer_cast<FunctionType>(m_type->AddMember(name, method));
        }

    private:
        TypeLibraries::TypePointer m_type;
        TypeLibraries::TypeLibraryPointer m_typeLibrary;
    };

}}}}
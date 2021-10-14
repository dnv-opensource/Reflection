#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeConversions/InheritanceConversions.h"
#include "Reflection/TypeConversions/UserConversion.h"
#include "Reflection/TypeConversions/TrivialConversion.h"
#include "Reflection/TypeConversions/PointerReferenceConversion.h"
#include "Reflection/TypeConversions/ImplicitConversion.h"
#include "Reflection/TypeConversions/UniteConstruction.h"
#include "Reflection/TypeConversions/LValueConversion.h"
#include "Reflection/TypeConversions/WrapConversion.h"

#include "Reflection/TypeLibraries/ITypeLibrary.h"
#include "Reflection/TypeLibraries/TypeWithGlobalMemberRegistration.h"

#include "Reflection/Members/Modifier.h"

#include "Public.h"
#include "Reflection/Reflect.h"
#include "Reflection/Attributes/TypeAttribute.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Classes {

    template<class Reflected, typename ArgumentPackT>
    class ClassBase 
    {
    public:
        typedef Reflected ReflectedType;
        using AllocatorType = typename ArgumentPackT::AllocatorType;
        using HolderType = typename ArgumentPackT::HolderType;
        using PublicType = typename ArgumentPackT::PublicType;
        using AutoReflectorType = typename ArgumentPackT::AutoReflectorType;
        using ConstructedObjectType = typename ArgumentPackT::ConstructedObjectType;
        using DynamicTypeDeductionAdder = typename ArgumentPackT::DynamicTypeDeductionAdder;

        ClassBase(TypeLibraries::TypeLibraryPointer typeLibrary, const std::string& name, AllocatorType allocator = AllocatorType())
            :   m_type(typeLibrary->CreateType(typeid(Reflected), name))
            ,   m_allocator(allocator)
        {
            GetType()->GetAttributeCollection().AddAttribute(Attributes::TypeAttribute(Types::TypeId<HolderType>()));
            RegisterBases(typeLibrary, PublicType());
            TypeConversions::AddTrivialConversions<Reflected>(typeLibrary->GetConversionGraph());
            TypeConversions::AddPointerReferenceConversions<Reflected>(typeLibrary->GetConversionGraph());
            DynamicTypeDeductionAdder::Add(typeLibrary->GetConversionGraph());
            TypeConversions::AddLValueConversion<Reflected>(typeLibrary->GetConversionGraph());
            TypeConversions::AddWrapConversion<ConstructedObjectType, HolderType>(typeLibrary->GetConversionGraph());
        }
        TypeLibraries::TypePointer GetType() const {return m_type; }

        TypeLibraries::TypePointer GetTypeWithGlobalMemberRegistration(TypeLibraries::TypeLibraryPointer typeLibrary)
        {
            TypeLibraries::TypePointer globalType = typeLibrary->LookupType(typeid(Members::GlobalType));
            if(globalType)
                return TypeLibraries::TypePointer(new TypeLibraries::TypeWithGlobalMemberRegistration(GetType(), globalType));
            else
                return GetType();
        }

        ///Adds a custom user conversion from Reflected to T
        template<typename T>
        void ImplicitConversion() 
        {
            TypeConversions::AddImplicitConversion<const Reflected&, T>(GetType()->GetConversionGraph());
        }        
        ///Adds a custom user conversion from Reflected to T based on the given signature.
        template<typename Signature>
        void ImplicitConversion(Signature signature) 
        {
            typedef typename TypeUtilities::FunctionTraits<Signature>::Argument0 From;
            typedef typename TypeUtilities::FunctionTraits<Signature>::ReturnType To;
            TypeConversions::AddUserConversion<From, To>(GetType()->GetConversionGraph(), signature);
        }
        template<typename T>
        void ImplicitConversion(Members::ModifierNamespace::Alias) 
        {
            ImplicitConversion(&Reflected::operator T, Alias);
        }        
        ///Adds a custom user conversion from Reflected to T based on the given signature.
        template<typename Signature>
        void ImplicitConversion(Signature signature, Members::ModifierNamespace::Alias) 
        {
            typedef typename TypeUtilities::FunctionTraits<Signature>::Argument0 From;
            typedef typename TypeUtilities::FunctionTraits<Signature>::ReturnType To;
            GetType()->GetConversionGraph()->AddConversion(Types::TypeId<From>(), Types::TypeId<To>(), TypeConversions::ConversionType::StandardConversion, std::make_shared<TypeConversions::FunctionConversion<From, To, Signature, To>>(signature));
        }
        template<typename OtherT>
        void Unite()
        {
            Unite<OtherT, OtherT*>();
        }
        template<typename OtherT, typename HolderOther>
        void Unite()
        {
            TypeConversions::AddUniteConversion<OtherT, Reflected, HolderOther, HolderType>(GetType()->GetConversionGraph());
            GetType()->AddUniteType(typeid(OtherT));
        }
    protected:
        template<typename BaseT>
        void RegisterBase(TypeLibraries::TypeLibraryPointer typeLibrary) 
        {         
            AutoReflectorType::ReflectBase<BaseT>(typeLibrary, m_allocator);
            TypeConversions::AddInheritanceConversions<BaseT, Reflected>(typeLibrary->GetConversionGraph());
            TypeLibraries::TypePointer derived = GetType();
            TypeLibraries::TypePointer base = typeLibrary->CreateType(typeid(BaseT), "");
            derived->GetAttributeCollection().AddBaseAttribute(base->GetAttributeCollection());
            derived->AddBaseType(typeid(BaseT));
            base->AddDerivedType(typeid(Reflected));
        }

        template<typename Base, typename... Bases>
        void RegisterBases(TypeLibraries::TypeLibraryPointer typeLibrary, Public<Base, Bases...>) 
        {
            RegisterBase<Base>(typeLibrary);
            RegisterBases(typeLibrary, Public<Bases...>());
        }
        void RegisterBases(TypeLibraries::TypeLibraryPointer typeLibrary, Public<>) 
        {
        }
    private:
        TypeLibraries::TypePointer m_type;
        AllocatorType m_allocator;
    };
}}}}

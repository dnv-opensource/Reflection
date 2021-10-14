#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeLibraries/IType.h"
#include "Constructor.h"
#include "ConstructorFunction.h"
#include "Reflection/AutoReflect.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Constructors {
    template<typename Reflected,typename AllocatorT=std::allocator<Reflected>,typename HolderT=Reflected*, typename ConstructedObjectT = Reflected*, typename AutoReflectorT = IgnoreAutoReflector>
    class DefineConstructor
    {
    public:
        DefineConstructor(TypeLibraries::TypeLibraryPointer typeLibrary, TypeLibraries::TypePointer type,AllocatorT allocator=AllocatorT()) 
            : m_typeLibrary(typeLibrary)
            , m_type(type) 
            , m_allocator(allocator) 
        {}

        /*
        Define a constructor from a C++ function
        Usage:
        cls.Constructor(&LengthVector::CreateLengthVector);
        */
        template<typename Signature>
        ConstructorFunction<Signature,HolderT,Members::ModifierNamespace::NoModifier>& Constructor(const Signature& function) {
            typedef ConstructorFunction<Signature, HolderT, Members::ModifierNamespace::NoModifier> FunctionType;
            boost::intrusive_ptr<FunctionType> method(new FunctionType(Members::ConstructorString, m_type->GetName(), m_type->GetConversionGraph(),function));
            method->DefineImplicitConstructorConversion();
            AutoReflectorT::AutoReflectSignature<Signature>(m_typeLibrary);
            return *boost::static_pointer_cast<FunctionType>(m_type->AddMember(Members::ConstructorString, method));
        }
        /*
        Define a constructor from a C++ function + modifiers, such as Alias, Vararg etc.
        Usage:
        cls.Constructor(&CreateGuidePlane,Vararg);
        */
        template<typename Signature,int ModifierValue>
        ConstructorFunction<Signature,HolderT,Members::ModifierNamespace::Modifier<ModifierValue>>& Constructor(Signature function,Members::ModifierNamespace::Modifier<ModifierValue>) {
            typedef ConstructorFunction<Signature, HolderT, Members::ModifierNamespace::Modifier<ModifierValue>> FunctionType;
            boost::intrusive_ptr<FunctionType> method(new FunctionType(Members::ConstructorString, m_type->GetName(), m_type->GetConversionGraph(),function));
            method->DefineImplicitConstructorConversion();
            AutoReflectorT::AutoReflectSignature<Signature>(m_typeLibrary);
            return *boost::static_pointer_cast<FunctionType>(m_type->AddMember(Members::ConstructorString, method));
        }
        /*
        Define a named constructor from a C++ function
        Usage:
        cls.NamedConstructor("XVector3d",&UnitVector::XVector3d);
        */
        template<typename Signature>
        ConstructorFunction<Signature, HolderT, Members::ModifierNamespace::NoModifier>& NamedConstructor(const std::string& name, const Signature& function) {
            typedef ConstructorFunction<Signature, HolderT, Members::ModifierNamespace::NoModifier> FunctionType;
            boost::intrusive_ptr<FunctionType> method(new FunctionType(name, name, m_type->GetConversionGraph(), function));
            return *boost::static_pointer_cast<FunctionType>(m_type->AddMember(name, method));
        }
        /*
        Define a named constructor from a C++ function with modifiers
        Usage:
        cls.NamedConstructor("CreateLineFromTwoPoints",&ggGuideCurveNURBS::CreateLineFromTwoPoints, Vararg);
        */
        template<typename Signature, int ModifierValue>
        ConstructorFunction<Signature, HolderT, Members::ModifierNamespace::Modifier<ModifierValue>>& NamedConstructor(const std::string& name, Signature function, Members::ModifierNamespace::Modifier<ModifierValue>) {
            typedef ConstructorFunction<Signature, HolderT, Members::ModifierNamespace::Modifier<ModifierValue>> FunctionType;
            boost::intrusive_ptr<FunctionType> method(new FunctionType(name, name, m_type->GetConversionGraph(), function));
            AutoReflectorT::AutoReflectSignature<Signature>(m_typeLibrary);
            return *boost::static_pointer_cast<FunctionType>(m_type->AddMember(name, method));
        }

        /*
        Define a constructor from a C++ constructor
        Usage:
        Class<LengthVector> cls(...);
        cls.Constructor<Length,Length,Length>();
        */
        template<typename... Args>
        Constructors::Constructor<ConstructedObjectT(Args...), AllocatorT, HolderT, Members::ModifierNamespace::NoModifier>& Constructor()
        { 
            return CreateConstructorWithSignature<ConstructedObjectT(Args...), Members::ModifierNamespace::NoModifier>();
        }
        template<typename... Args>
        Constructors::Constructor<ConstructedObjectT(Args...), AllocatorT, HolderT, Members::ModifierNamespace::NoModifier>& NamedConstructor(const std::string& name)
        {
            return CreateConstructorWithSignature<ConstructedObjectT(Args...), Members::ModifierNamespace::NoModifier>(name);
        }
        /*
        Define a constructor from a C++ constructor, make it explicit.
        Prevent automatic conversion from input argument to class.
        Usage:
        Class<ConstantCurveOffset> cls(...);
        cls.Constructor<LengthVector, bool>(Explicit)
            .AddSignature("constantOffset", Arg("useLocalSystem") = true);        
        */
        template<typename... Args>
        Constructors::Constructor<ConstructedObjectT(Args...), AllocatorT, HolderT, Members::ModifierNamespace::Explicit>& Constructor(Members::ModifierNamespace::Explicit)
        {
            return CreateConstructorWithSignature<ConstructedObjectT(Args...), Members::ModifierNamespace::Explicit>();
        }
        template<typename... Args>
        Constructors::Constructor<ConstructedObjectT(Args...), AllocatorT, HolderT, Members::ModifierNamespace::Explicit>& NamedConstructor(const std::string& name, Members::ModifierNamespace::Explicit)
        {
            return CreateConstructorWithSignature<ConstructedObjectT(Args...), Members::ModifierNamespace::Explicit>(name);
        }
        /*
        Define a constructor from a C++ constructor, add the allocator as the last argument, in case someone wants to 
        use it in the construction phase. This is maybe more relevant when registering constructor as a function.
        class DummyClass 
        {
        public:
           DummyClass(double x, double y, MyAllocator alloc);
        };
        Class<DummyClass, MyAllocator> cls(...);
        cls.Constructor<double, double>(AllocatorAsLastArgument);
        */
        template<typename... Args>
        Constructors::Constructor<ConstructedObjectT(Args...), AllocatorT, HolderT, Members::ModifierNamespace::AllocatorAsLastArgument>& Constructor(Members::ModifierNamespace::AllocatorAsLastArgument)
        {
            return CreateConstructorWithSignature<ConstructedObjectT(Args...), Members::ModifierNamespace::AllocatorAsLastArgument>();
        }
        template<typename... Args>
        Constructors::Constructor<ConstructedObjectT(Args...), AllocatorT, HolderT, Members::ModifierNamespace::AllocatorAsLastArgument>& NamesConstructor(const std::string& name, Members::ModifierNamespace::AllocatorAsLastArgument)
        {
            return CreateConstructorWithSignature<ConstructedObjectT(Args...), Members::ModifierNamespace::AllocatorAsLastArgument>(name);
        }

        /*
        Combine AllocatorAsLastArgument with Explicit
        class DummyClass 
        {
        public:
           DummyClass(double x, double y, MyAllocator alloc);
        };
        Class<DummyClass, MyAllocator> cls(...);
        cls.Constructor<double, double>(AllocatorAsLastArgument|Explicit);
        */
        template<typename... Args>
        Constructors::Constructor<ConstructedObjectT(Args...), AllocatorT, HolderT, Members::ModifierNamespace::ExplicitAllocatorAsLastArgument>& Constructor(Members::ModifierNamespace::ExplicitAllocatorAsLastArgument) 
        {
            return CreateConstructorWithSignature<ConstructedObjectT(Args...), Members::ModifierNamespace::ExplicitAllocatorAsLastArgument>(); 
        }
        template<typename... Args>
        Constructors::Constructor<ConstructedObjectT(Args...), AllocatorT, HolderT, Members::ModifierNamespace::ExplicitAllocatorAsLastArgument>& Constructor(const std::string& name, Members::ModifierNamespace::ExplicitAllocatorAsLastArgument)
        {
            return CreateConstructorWithSignature<ConstructedObjectT(Args...), Members::ModifierNamespace::ExplicitAllocatorAsLastArgument>(name);
        }

        /*
        Promote conversion between two classes from user conversion to standard conversion.
        class LengthVector
        {
        public:
           LengthVector(const vec3d& vec);
        };
        Class<LengthVector> cls(...);
        cls.Constructor<const vec3d&>(Alias);
        */
        template<typename... Args>
        Constructors::Constructor<ConstructedObjectT(Args...), AllocatorT, HolderT, Members::ModifierNamespace::Alias>& Constructor(Members::ModifierNamespace::Alias)
        {
            static_assert(sizeof...(Args) > 0, "Must have at least one argument to define an alias");
            return CreateConstructorWithSignature<ConstructedObjectT(Args...), Members::ModifierNamespace::Alias>(); 
        }
        template<typename... Args>
        Constructors::Constructor<ConstructedObjectT(Args...), AllocatorT, HolderT, Members::ModifierNamespace::Alias>& NamedConstructor(const std::string& name, Members::ModifierNamespace::Alias)
        {
            static_assert(sizeof...(Args) > 0, "Must have at least one argument to define an alias");
            return CreateConstructorWithSignature<ConstructedObjectT(Args...), Members::ModifierNamespace::Alias>(name);
        }
        /*
        Adds vararg support for a constructor.
        class DummyClass
        {
        public:
           DummyClass(int, const std::vector<double>& args);
        };
        Class<DummyClass> cls(...);
        cls.Constructor<int, const std::vector<double>&>(Vararg);
        Allows unrolling of the last argument:
        'DummyClass(1, 4.5, 3.2, 3.3, 6.5)'
        will be automatically translated into
        DummyClass(1, {4.5, 3.2, 3.3, 6.5})
        */
        template<typename... Args>
        Constructors::Constructor<ConstructedObjectT(Args...), AllocatorT, HolderT, Members::ModifierNamespace::Vararg>& Constructor(Members::ModifierNamespace::Vararg)
        {
            static_assert(sizeof...(Args) > 0, "Must have at least one argument to define a vararg");
            return CreateConstructorWithSignature<ConstructedObjectT(Args...), Members::ModifierNamespace::Vararg>();
        }
        template<typename... Args>
        Constructors::Constructor<ConstructedObjectT(Args...), AllocatorT, HolderT, Members::ModifierNamespace::Vararg>& NamedConstructor(const std::string& name, Members::ModifierNamespace::Vararg)
        {
            static_assert(sizeof...(Args) > 0, "Must have at least one argument to define a vararg");
            return CreateConstructorWithSignature<ConstructedObjectT(Args...), Members::ModifierNamespace::Vararg>(name);
        }
    private:
        template<typename Signature,typename Modifier>
        Constructors::Constructor<Signature,AllocatorT,HolderT,Modifier>& CreateConstructorWithSignature() {
            typedef Constructors::Constructor<Signature, AllocatorT, HolderT, Modifier> ConstructorType;
            boost::intrusive_ptr<ConstructorType> method(new ConstructorType(Members::ConstructorString, m_type->GetName(), m_type->GetConversionGraph(),m_allocator));
            method->DefineImplicitConstructorConversion();
            AutoReflectorT::AutoReflectSignature<Signature>(m_typeLibrary);
            return *boost::static_pointer_cast<ConstructorType>(m_type->AddMember(Members::ConstructorString, method));
        }
        template<typename Signature, typename Modifier>
        Constructors::Constructor<Signature, AllocatorT, HolderT, Modifier>& CreateConstructorWithSignature(const std::string& name) 
        {
            typedef Constructors::Constructor<Signature, AllocatorT, HolderT, Modifier> ConstructorType;
            boost::intrusive_ptr<ConstructorType> method(new ConstructorType(name, name, m_type->GetConversionGraph(), m_allocator));
            method->DefineImplicitConstructorConversion();
            AutoReflectorT::AutoReflectSignature<Signature>(m_typeLibrary);
            return *boost::static_pointer_cast<ConstructorType>(m_type->AddMember(name, method));
        }

        TypeLibraries::TypeLibraryPointer m_typeLibrary;
        TypeLibraries::TypePointer m_type;
        AllocatorT m_allocator;
    };
}}}}
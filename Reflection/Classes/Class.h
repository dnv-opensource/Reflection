#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ClassArgumentPack.h"
#include "ClassBase.h"

#include "Reflection/Constructors/DefineConstructor.h"
#include "Reflection/Functions/DefineFunction.h"
#include "Reflection/Operators/DefineOperator.h"
#include "Reflection/TypeLibraries/ITypeLibrary.h"

#include "Reflection/TypeConversions/TrivialConversion.h"
#include "Reflection/TypeConversions/PointerReferenceConversion.h"
#include "Reflection/TypeConversions/UserConversion.h"

#include "Reflection/Properties/DefineProperties.h"
#include "Reflection/Members/Arg.h"
#include "Reflection/Reflect.h"
#include "Reflection/Functions/RegisterToStringFunction.h"
#include "Reflection/ArgumentException.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Classes {
    //To help visual assist give us autocompletion, we need to do add the following classes.
    using Functions::DefineFunction;
    using Constructors::DefineConstructor;
    using Operators::DefineOperator;
    using Properties::DefineProperties;

    using Properties::Global;
    using Members::GlobalType;
    
    /**Use to define operators:
    cls.Operator(This.Const+This.Const);
    cls.Operator(This+=int());
    */    
    using Operators::This;
    
    /**Use in an operator definition if the type in question is heavy/impossible to default construct
    cls.Operator(This.Const+Other<const NonDefaultConstructableType&>());
    */
    using Operators::Other;

    /**Use to define vararg functions or constructors:
    cls.Function("MyFunction",&MyClass::MyFunction,Vararg);
    or
    cls.Constructor<int,const std::vector<double>&>(Vararg);

    The last argument in the function/constructor must be a container, and all extra arguments will be put into this container.
    */
    using Members::Vararg;

    ///Use to define conversions with better conversion quality than UserConversion.
    using Members::Alias;

    /**Use to define explicit constructors:
    cls.Constructor<int>(Explicit);
    This means that no implicit conversion will be done from the constructor argument type (int) to the reflected type.
    */
    using Members::Explicit;    
    
    /**Use to define a constructor that takes an allocator as its last argument:
    Class<MyClass,CustomAllocator<MyClass>> cls(...);
    cls.Constructor<int>(AllocatorAsLastArgument);
    - Matches the following constructor:
    MyClass(int, const CustomAllocator<MyClass>& alloc);
    When calling this class using reflection, you only pass in the first argument.
    */
    using Members::AllocatorAsLastArgument;

    /**Use Arg when you want to add variable names + documentation+ default arguments to a function signature.
    cls.Function("SomeFunction",&SomeClass::SomeFunction)
       .AddSignature( (Arg("Length"),Arg("Width")=4.0) );
       ->Note the extra parenthesis around the signature. This is required.
    */
    using Members::Arg;

	///Used to simplify overloaded member function deduction.
    using Members::ModifierNamespace::Const;
    using Reflection::Reflect;
    using Functions::RegisterToStringFunction;
    using Types::TypeId;

    using Reflection::ArgumentException;
    using namespace Attributes;
    
    /**Reflects a class or type.
    You can do the following:
    1. Define inheritance:
    Class<scStraightBeam,Public<scBeam>> cls;
    2. Define allocator:
    Class<scStraightBeam,Public<scBeam>,Core::DatabaseAllocator<scStraightBeam> > cls;
    3. Define smart pointer storage (default is an unmanaged pointer)
    Class<std::string,std::shared_ptr<std::string> > cls;
    */
    template<typename Reflected, typename... Args>
    class Class 
        :   public ClassBase<Reflected, ClassArgumentPack<Reflected, Args...> >
        ,   public DefineConstructor<Reflected, typename ClassArgumentPack<Reflected, Args...>::AllocatorType, typename ClassArgumentPack<Reflected, Args...>::HolderType, typename ClassArgumentPack<Reflected, Args...>::ConstructedObjectType, typename ClassArgumentPack<Reflected, Args...>::AutoReflectorType>
        ,   public DefineOperator<Reflected, typename ClassArgumentPack<Reflected, Args...>::HolderType>
        ,   public DefineFunction<Reflected, typename ClassArgumentPack<Reflected, Args...>::AutoReflectorType>
        ,   public DefineProperties<Reflected, typename ClassArgumentPack<Reflected, Args...>::AutoReflectorType>
    {
    public:
        ///Defines a type. Assign a name to the type.
        Class(TypeLibraries::TypeLibraryPointer typeLibrary, const std::string& name, AllocatorType allocator = AllocatorType())
            :   ClassBase(typeLibrary, name, allocator)
            ,   DefineConstructor<Reflected, AllocatorType, HolderType, ConstructedObjectType, AutoReflectorType>(typeLibrary,GetTypeWithGlobalMemberRegistration(typeLibrary), allocator)
            ,   DefineOperator<Reflected,HolderType>(GetType())
            ,   DefineFunction<Reflected, AutoReflectorType>(typeLibrary, GetTypeWithGlobalMemberRegistration(typeLibrary))
            ,   DefineProperties<Reflected, AutoReflectorType>(typeLibrary, GetType())
        {
        }
        ///Makes T an alias of Reflected, T will always be converted into Reflected whenever T is encountered.
        template<typename T>
        void MakeAlias() 
        {
            GetType()->GetConversionGraph()->AddConversion(Types::TypeId<T>(), Types::TypeId<Reflected>(), TypeConversions::ConversionType::DynamicTypeConversion, std::make_shared<TypeConversions::ConstructorConversion<T, Reflected, HolderType>>());
        }
        template<typename AttributeT, typename... Args>
        void AddAttribute(Args&&... args)
        {
            GetType()->GetAttributeCollection().AddAttribute(AttributeT(std::forward<Args>(args)...));
        }
        void AddDocumentation(const std::string& doc)
        {
            GetType()->AddDocumentation(doc);
        }
    private:
    };

}}}}

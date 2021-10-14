//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest\gtest.h"
#include "Reflection\Classes\Class.h"
#include "Reflection\TypeLibraries\TypeLibraryFactory.h"
#include <Reflection/Containers/ReflectList.h>

#include <list>
#include <numeric>

namespace DNVS { namespace MoFa { namespace Reflection { namespace Classes { namespace Tests {
    class TestClassBase  {
    public:
        TestClassBase(double x = 0)
            :   m_x(x)
        {}
        double GetX() const {return m_x;}
        void SetX(double x) {m_x = x;}
    private:
        double m_x;
    };
    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, TestClassBase**)
    {
        Class<TestClassBase> cls(typeLibrary, "TestClassBase");
        //Define property
        cls.Get("X", &TestClassBase::GetX);
        cls.Set("X", &TestClassBase::SetX);
    }

    class TestClassA : public TestClassBase 
    {
    public:
        explicit TestClassA(double x)
            : TestClassBase(x)
        {}
        void Sum(const std::list<double>& args)
        {
            SetX(std::accumulate(args.begin(),args.end(),GetX()));
        }
        bool operator==(const TestClassA& other) const
        {
            return GetX() == other.GetX();
        }
        TestClassA operator+(const TestClassA& other) const
        {
            return TestClassA(GetX()+GetX());
        }
    };
    
    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, TestClassA**)
    {
        Class<TestClassA, Public<TestClassBase>> cls(typeLibrary, "TestClassA");
        //Define constructor. Do not allow default conversion from double to TestClassA.
        cls.Constructor<double>(Explicit);
        //Operator overload support
        cls.Operator(This.Const == This.Const);
        cls.Operator(This.Const + This.Const)
            .AddDocumentation("Adds these together!");
        //Define a vararg function. It will be listed as having 0 or more arguments. It will automatically create a list and pass it to the C++ function.
        cls.Function("Sum", &TestClassA::Sum, Vararg);
    }

    class TestClassB 
    {
    private:
        double m_value;
    public:
        int DoDummy(int a,int b=5)
        {
            return a*b;
        }
        TestClassB(const TestClassA& other)
        {
            m_value = other.GetX();
        }
    };

    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, TestClassB**)
    {
        Class<TestClassB> cls(typeLibrary, "TestClassB");
        //Implicit conversion from TestClassB to TestClassA allowed. Add signature with argument name.
        cls.Constructor<const TestClassA&>().AddSignature("other");
        cls.Function("DoDummy", &TestClassB::DoDummy)
            .AddSignature("a", Arg("b") = 5)
            .AddDocumentation("Multiply numbers");
    }

    TEST(RealisticClassTests,RegisterAll)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary();
        Reflect<TestClassA>(typeLibrary);
        Reflect<TestClassB>(typeLibrary);

/*        //You need to add function on ITypeLibrary to get all types:
        std::list<TypePointer> types = typeLibrary->GetAllTypes();
        BOOST_FOREACH(TypePointer type, types)
        {
            ExtendedTypeConversionService etcs;
            Construct new dynamic type:
            System::Type^ managedType;
            //DecoratedTypeInfo nativeType = We need to store the exact storage type of NativeType on IType. Today this is not stored. The value is:
            //In ClassBase constructor:
            //m_type->SetDecoratedTypeInfo(TypeId<HolderType>());                        
            typeLibrary->GetConversionGraph()->AddConversion(type->GetDecoratedTypeInfo(), etcs.GetManagedTypeId(type), ConversionType::StandardConversion, std::make_shared<TypeConversions::SomeConversionClassThatDoesTheWrapping>());
            typeLibrary->GetConversionGraph()->AddConversion(etcs.GetManagedTypeId(type), type->GetDecoratedTypeInfo(), ConversionType::StandardConversion, std::make_shared<TypeConversions::SomeConversionClassThatDoesTheUnWrapping>());

            std:string className = type->GetName();
            //type->GetBaseClasses(), you need to implement this.
            //You need to add function on IType to get all members:
            std::list<MemberPointer> members = type->GetAllMembers();
            BOOST_FOREACH(MemberPointer member, members)
            {
                std:string functionName = member->GetName();
                //If this is a member function, the first argument indicates 'this'.                
                size_t numberOfArgumentsIncludingThis = member->GetArity();
                //If isVararg is true, Arity will not give the correct number of arguments, instead it will count the vararg argument pack as one argument.
                bool isVararg = member->IsVarargMember();
                MemberType memberType = member->GetMemberType();
                //One of
//                     TypeNone = 0,
//                     TypeConstructor = 1,
//                     TypeMemberFunction = 2,
//                     TypeStaticFunction = 4,
//                     TypeFunction = 6,
//                     TypePropertyGet = 8,
//                     TypePropertySet = 16,
//                     TypePropertyGetSet = TypePropertyGet | TypePropertySet,
//                     TypeOperator = 32,
//                     TypeAll = TypeConstructor | TypeFunction | TypePropertyGetSet | TypeOperator
                ///Returns the minimum number of required arguments. Differs from arity if default arguments have been supplied.
                size_t minCount = member->GetMinimumArgumentCount();
                ///Returns the maximum number of arguments. For varargs this can be numeric_limits<size_t>::max()
                size_t maxCount = member->GetMaximumArgumentCount();
                for(size_t i = 0; i < numberOfArgumentsIncludingThis)
                {
                    ArgumentInfoPointer info = member->GetArgumentInfo(i);
                    //Type of argument.
                    auto typeOfArgument = info->GetTypeInfo();                    
                    if(info->HasDefault())
                    {
                        //Default value of argument (if supported in C#)
                        Variants::Variant v = info->GetDefault();
                    }
                    //Name of argument.
                    std::string name = info->GetName();
                    //Documentation of argument.
                    std::string doc = info->GetDocumentation();
                    
                }
                //Actual return type.
                member->GetReturnType();
                //Probably not relevant?
                member->IsConst();
                //Not implemented. Member data stored on Method. You need to implement function for this on interface and access function on Method class.
                member->GetDocumentation();                    
            }
        }*/

        /*
        Invocation. Use the ExtendedTypeConversionService in Marshalling.Net
        ExtendedTypeConversionService etcs;
        You have a list of System::Object^, you need to use
        std::vector<Variants::Variant> nativeArgs;
        nativeArgs.push_back(etcs.ReflectManaged(arg));
        Variants::Variant result = member->Invoke(nativeArgs);
        return etcs.AsManaged<System::Object^>(result);
        */
    }
}}}}}

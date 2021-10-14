//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <gtest/gtest.h>
#include "Reflection/TypeLibraries/TypeLibraryFactory.h"
#include "Reflection/Classes/Class.h"
#include "Reflection/Members/MemberLoggerImpl.h"
#include "Reflection/Members/MemberLoggerContext.h"
#include "Reflection/Members/OperatorNames.h"
#include "Reflection/Operators/DefineOperator.h"
#include "Reflection/Types/DefaultTypeFormatter.h"
#include "Reflection/TypeLibraries/TypeLibraryNamedTypeFormatter.h"
#include "Reflection/Members/IMemberFwd.h"

namespace DNVS {namespace MoFa { namespace Reflection {namespace Tests {
    using namespace DNVS::MoFa::Operators;
    class MemberLoggerTestClass {
    public:
        MemberLoggerTestClass(int a, double b) {};
        int MyMemberFunction(double a, int b, const std::string& c = "Hello World") { return 1; }
        int GetAVal() { return 1; };
        void SetAVal(int a) { };
        bool operator==(const MemberLoggerTestClass& pos) const { return true; };
        static bool MyStaticFunction(std::string a, double b, const int& c = 1) { return true; }
    };
    double MyFreeFunction(double a) { return 1.0; }

    //This function will implement reflection for MemberLoggerTestClass
    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, MemberLoggerTestClass**)
    {
        using namespace Classes;
        Class<MemberLoggerTestClass> cls(typeLibrary, "MemberLoggerTestClass");

        cls.Constructor<int, double>()
            .AddSignature("a", "b");
        cls.Function("MyMemberFunction", &MemberLoggerTestClass::MyMemberFunction)
            .AddSignature("a", "b", Arg("c", "Unused argument, for fun") = std::string("Hello World"));
        cls.SetGet("AVal", &MemberLoggerTestClass::SetAVal, &MemberLoggerTestClass::GetAVal);
        cls.Operator(This.Const == This.Const);
        cls.StaticFunction("MyStaticFunction", &MemberLoggerTestClass::MyStaticFunction)
            .AddSignature("a", "b", Arg("c", "Unused argument, for fun") = int(1));
        cls.StaticFunction("MyFreeFunction", &MyFreeFunction)
            .AddSignature("a");
    }

    TEST(MemberLoggerTests, LogMemberFunction)
    {
        //Create the reflection library
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary();
        typeLibrary->CreateType(typeid(std::string), "string");

        //Actually register MemberLoggerTestClass to reflection library
        Reflect<MemberLoggerTestClass>(typeLibrary);

        auto context = std::make_shared<Members::MemberLoggerContext>();
        //context->SetFunctionDetailService(DNVS::MoFa::Services::ServiceProvider::Instance().TryGetService<DNVS::MoFa::TypeUtilities::IFunctionDetailService>());
        auto defaultTypeFormatter = std::make_shared< TypeLibraries::TypeLibraryNamedTypeFormatter>(typeLibrary.get(), std::make_shared<Types::DefaultTypeFormatter>());
        context->SetTypeFormatter(defaultTypeFormatter);
        context->SetVariantFormatter(typeLibrary->GetVariantFormatter());

        Members::MemberPointer memberFunction = typeLibrary->LookupType(typeid(MemberLoggerTestClass))->Lookup("MyMemberFunction");
        memberFunction->SetMemberLoggerContext(context);
        DNVS::MoFa::Reflection::Members::MemberLoggerImpl memberFunctionLogger(memberFunction.get(), false);

        EXPECT_EQ("Reflection.Test.exe", memberFunctionLogger.GetDLL());
        EXPECT_EQ("MemberLoggerTestClass", memberFunctionLogger.GetClass());
        EXPECT_EQ("MyMemberFunction", memberFunctionLogger.GetFunction());
        EXPECT_EQ("int(double a, int b, string c)", memberFunctionLogger.GetSignature());
    }

    TEST(MemberLoggerTests, LogConstructor)
    {
        //Create the reflection library
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary();
        typeLibrary->CreateType(typeid(std::string), "string");

        //Actually register MemberLoggerTestClass to reflection library
        Reflect<MemberLoggerTestClass>(typeLibrary);

        auto context = std::make_shared<Members::MemberLoggerContext>();
        //context->SetFunctionDetailService(DNVS::MoFa::Services::ServiceProvider::Instance().TryGetService<DNVS::MoFa::TypeUtilities::IFunctionDetailService>());
        auto defaultTypeFormatter = std::make_shared< TypeLibraries::TypeLibraryNamedTypeFormatter>(typeLibrary.get(), std::make_shared<Types::DefaultTypeFormatter>());
        context->SetTypeFormatter(defaultTypeFormatter);
        context->SetVariantFormatter(typeLibrary->GetVariantFormatter());

        auto constructor = typeLibrary->LookupType(typeid(MemberLoggerTestClass))->Lookup(Members::ConstructorString);
        DNVS::MoFa::Reflection::Members::MemberLoggerImpl constructorLogger(constructor.get(), context, false);

        EXPECT_EQ("Reflection.Test.exe", constructorLogger.GetDLL());
        EXPECT_EQ("MemberLoggerTestClass", constructorLogger.GetClass());
        EXPECT_EQ("MemberLoggerTestClass", constructorLogger.GetFunction());
        EXPECT_EQ("MemberLoggerTestClass(int a, double b)", constructorLogger.GetSignature());
    }

    TEST(MemberLoggerTests, LogPropertyGet)
    {
        //Create the reflection library
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary();
        typeLibrary->CreateType(typeid(std::string), "string");

        //Actually register MemberLoggerTestClass to reflection library
        Reflect<MemberLoggerTestClass>(typeLibrary);

        auto context = std::make_shared<Members::MemberLoggerContext>();
        //context->SetFunctionDetailService(DNVS::MoFa::Services::ServiceProvider::Instance().TryGetService<DNVS::MoFa::TypeUtilities::IFunctionDetailService>());
        auto defaultTypeFormatter = std::make_shared< TypeLibraries::TypeLibraryNamedTypeFormatter>(typeLibrary.get(), std::make_shared<Types::DefaultTypeFormatter>());
        context->SetTypeFormatter(defaultTypeFormatter);
        context->SetVariantFormatter(typeLibrary->GetVariantFormatter());

        auto propertyGet = typeLibrary->LookupType(typeid(MemberLoggerTestClass))->Lookup("AVal")->GetOverloads()[1];
        DNVS::MoFa::Reflection::Members::MemberLoggerImpl propertyGetLogger(propertyGet.get(), context, false);

        EXPECT_EQ("Reflection.Test.exe", propertyGetLogger.GetDLL());
        EXPECT_EQ("MemberLoggerTestClass", propertyGetLogger.GetClass());
        EXPECT_EQ("AVal", propertyGetLogger.GetFunction());
        EXPECT_EQ("int()", propertyGetLogger.GetSignature());
    }

    TEST(MemberLoggerTests, LogPropertySet)
    {
        //Create the reflection library
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary();
        typeLibrary->CreateType(typeid(std::string), "string");

        //Actually register MemberLoggerTestClass to reflection library
        Reflect<MemberLoggerTestClass>(typeLibrary);

        auto context = std::make_shared<Members::MemberLoggerContext>();
        //context->SetFunctionDetailService(DNVS::MoFa::Services::ServiceProvider::Instance().TryGetService<DNVS::MoFa::TypeUtilities::IFunctionDetailService>());
        auto defaultTypeFormatter = std::make_shared< TypeLibraries::TypeLibraryNamedTypeFormatter>(typeLibrary.get(), std::make_shared<Types::DefaultTypeFormatter>());
        context->SetTypeFormatter(defaultTypeFormatter);
        context->SetVariantFormatter(typeLibrary->GetVariantFormatter());

        auto propertySet = typeLibrary->LookupType(typeid(MemberLoggerTestClass))->Lookup("AVal")->GetOverloads()[0];
        DNVS::MoFa::Reflection::Members::MemberLoggerImpl propertySetLogger(propertySet.get(), context, false);

        EXPECT_EQ("Reflection.Test.exe", propertySetLogger.GetDLL());
        EXPECT_EQ("MemberLoggerTestClass", propertySetLogger.GetClass());
        EXPECT_EQ("AVal", propertySetLogger.GetFunction());
        EXPECT_EQ("void(int value)", propertySetLogger.GetSignature());
    }

    TEST(MemberLoggerTests, LogOperator)
    {
        //Create the reflection library
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary();
        typeLibrary->CreateType(typeid(std::string), "string");

        //Actually register MemberLoggerTestClass to reflection library
        Reflect<MemberLoggerTestClass>(typeLibrary);

        auto context = std::make_shared<Members::MemberLoggerContext>();
        //context->SetFunctionDetailService(DNVS::MoFa::Services::ServiceProvider::Instance().TryGetService<DNVS::MoFa::TypeUtilities::IFunctionDetailService>());
        auto defaultTypeFormatter = std::make_shared< TypeLibraries::TypeLibraryNamedTypeFormatter>(typeLibrary.get(), std::make_shared<Types::DefaultTypeFormatter>());
        context->SetTypeFormatter(defaultTypeFormatter);
        context->SetVariantFormatter(typeLibrary->GetVariantFormatter());

        auto operatorType = typeLibrary->LookupType(typeid(MemberLoggerTestClass))->Lookup(Members::BinaryOperatorName<Tags::EqualTo>::Name());
        DNVS::MoFa::Reflection::Members::MemberLoggerImpl operatorTypeLogger(operatorType.get(), context, false);

        EXPECT_EQ("Reflection.Test.exe", operatorTypeLogger.GetDLL());
        EXPECT_EQ("MemberLoggerTestClass", operatorTypeLogger.GetClass());
        EXPECT_EQ(" == ", operatorTypeLogger.GetFunction());
        EXPECT_EQ("bool()", operatorTypeLogger.GetSignature());
    }

    TEST(MemberLoggerTests, LogStaticFunction)
    {
        //Create the reflection library
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary();
        typeLibrary->CreateType(typeid(std::string), "string");

        //Actually register MemberLoggerTestClass to reflection library
        Reflect<MemberLoggerTestClass>(typeLibrary);

        auto context = std::make_shared<Members::MemberLoggerContext>();
        //context->SetFunctionDetailService(DNVS::MoFa::Services::ServiceProvider::Instance().TryGetService<DNVS::MoFa::TypeUtilities::IFunctionDetailService>());
        auto defaultTypeFormatter = std::make_shared< TypeLibraries::TypeLibraryNamedTypeFormatter>(typeLibrary.get(), std::make_shared<Types::DefaultTypeFormatter>());
        context->SetTypeFormatter(defaultTypeFormatter);
        context->SetVariantFormatter(typeLibrary->GetVariantFormatter());

        auto staticFunction = typeLibrary->LookupType(typeid(MemberLoggerTestClass))->Lookup("MyStaticFunction");
        DNVS::MoFa::Reflection::Members::MemberLoggerImpl staticFunctionLogger(staticFunction.get(), context, false);

        EXPECT_EQ("Reflection.Test.exe", staticFunctionLogger.GetDLL());
        EXPECT_EQ("void", staticFunctionLogger.GetClass());
        //EXPECT_EQ("MemberLoggerTestClass", staticFunctionLogger.GetClass());
        EXPECT_EQ("MyStaticFunction", staticFunctionLogger.GetFunction());
        EXPECT_EQ("bool(string a, double b, int c)", staticFunctionLogger.GetSignature());
    }

    TEST(MemberLoggerTests, LogFreeFunction)
    {
        //Create the reflection library
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary();
        typeLibrary->CreateType(typeid(std::string), "string");

        //Actually register MemberLoggerTestClass to reflection library
        Reflect<MemberLoggerTestClass>(typeLibrary);

        auto context = std::make_shared<Members::MemberLoggerContext>();
        //context->SetFunctionDetailService(DNVS::MoFa::Services::ServiceProvider::Instance().TryGetService<DNVS::MoFa::TypeUtilities::IFunctionDetailService>());
        auto defaultTypeFormatter = std::make_shared< TypeLibraries::TypeLibraryNamedTypeFormatter>(typeLibrary.get(), std::make_shared<Types::DefaultTypeFormatter>());
        context->SetTypeFormatter(defaultTypeFormatter);
        context->SetVariantFormatter(typeLibrary->GetVariantFormatter());

        auto freeFunction = typeLibrary->LookupType(typeid(MemberLoggerTestClass))->Lookup("MyFreeFunction");
        DNVS::MoFa::Reflection::Members::MemberLoggerImpl freeFunctionLogger(freeFunction.get(), context, false);

        EXPECT_EQ("Reflection.Test.exe", freeFunctionLogger.GetDLL());
        EXPECT_EQ("void", freeFunctionLogger.GetClass());
        //EXPECT_EQ("MemberLoggerTestClass", freeFunctionLogger.GetClass());
        EXPECT_EQ("MyFreeFunction", freeFunctionLogger.GetFunction());
        EXPECT_EQ("double(double a)", freeFunctionLogger.GetSignature());
    }
}}}}
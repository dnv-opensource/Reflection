//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection\TypeLibraries\TypeLibraryFactory.h"
#include "Reflection\Functions\DefineFunction.h"
#include "Reflection\Variants\VariantService.h"
#include "Reflection\Objects\Object.h"
#include "Reflection\TypeConversions\TrivialConversion.h"
#include "Reflection\TypeConversions\PointerReferenceConversion.h"
#include "Reflection\TypeConversions\BuiltInConversions.h"
#include "Reflection\Members\Arg.h"
#include "Reflection\Members\MemberWithArguments.h"
#include <numeric>
#include <gtest/gtest.h>
#include "GoogleTest/Profiler.h"
#include "Reflection/Members/Invoker/FastInvoker.h"

using namespace DNVS::MoFa::Reflection::Functions;
using namespace DNVS::MoFa::Reflection::TypeLibraries;
using namespace DNVS::MoFa::Reflection::TypeConversions;
using namespace DNVS::MoFa::Reflection::Objects;
using namespace DNVS::MoFa::Reflection::Variants;
using namespace DNVS::MoFa::Reflection::Members;

TEST(FunctionTests, DefineFunctionSimple)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    DefineFunction<std::string> def(typeLibrary,typeLibrary->CreateType(typeid(std::string), "string"));
    def.Function("clear", &std::string::clear);
}

TEST(FunctionTests, DefineFunctionSimple_Invoke)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    DefineFunction<std::string> def(typeLibrary, typeLibrary->CreateType(typeid(std::string), "string"));
    AddTrivialConversions<std::string>(typeLibrary->GetConversionGraph());
    AddPointerReferenceConversions<std::string>(typeLibrary->GetConversionGraph());
    def.Function("clear", &std::string::clear);

    std::string text("Hello");
    Object var(typeLibrary, VariantService().ReflectType<std::string&>(text));

    EXPECT_EQ("Hello", var.As<std::string&>());
    var.Invoke("clear", std::vector<Object>());
    EXPECT_EQ("", var.As<std::string&>());

}
class TestClassWithFunctionArguments 
{
public:
    TestClassWithFunctionArguments() {}
    __declspec(noinline) int Add(int a, int b = 4) 
    {
        return a + b;
    }
};

TEST(FunctionTests, DefineFunctionDocumentArguments_HasDefaultArgument)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    DefineFunction<TestClassWithFunctionArguments> def(typeLibrary, typeLibrary->CreateType(typeid(TestClassWithFunctionArguments), "TestClassWithFunctionArguments"));
    AddTrivialConversions<TestClassWithFunctionArguments>(typeLibrary->GetConversionGraph());
    AddPointerReferenceConversions<TestClassWithFunctionArguments>(typeLibrary->GetConversionGraph());
    AddBuiltInConversions(typeLibrary->GetConversionGraph());
    //Reflect a function with one default argument
    def.Function("Add", &TestClassWithFunctionArguments::Add)
       .AddSignature("a", Arg("b") = 4);

    TestClassWithFunctionArguments testClass;
    Object var(typeLibrary, &testClass);
    Object a(typeLibrary, 4);
    std::vector<Object> args(1, a);
    //Invoke a function, but don't add a second argument. Rely on the default argument in stead.
    EXPECT_EQ(8, var.Invoke("Add", args).As<int>());
}

TEST(FunctionTests, DefineFunctionDocumentArguments_HasDefaultArgument_InvokeWithoutDefaultArgument)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    DefineFunction<TestClassWithFunctionArguments> def(typeLibrary, typeLibrary->CreateType(typeid(TestClassWithFunctionArguments), "TestClassWithFunctionArguments"));
    AddTrivialConversions<TestClassWithFunctionArguments>(typeLibrary->GetConversionGraph());
    AddPointerReferenceConversions<TestClassWithFunctionArguments>(typeLibrary->GetConversionGraph());
    AddBuiltInConversions(typeLibrary->GetConversionGraph());
    //Reflect a function with one default argument
    def.Function("Add", &TestClassWithFunctionArguments::Add)
        .AddSignature("a", Arg("b") = 4);

    TestClassWithFunctionArguments testClass;
    Object var(typeLibrary, &testClass);
    Object a(typeLibrary, 4);
    std::vector<Object> args;
    args.push_back(Object(typeLibrary, 4));
    args.push_back(Object(typeLibrary, 3));
    //Invoke a function, but this time add a second argument.
    EXPECT_EQ(7, var.Invoke("Add", args).As<int>());

}
class ClassWithVarargArguments 
{
public:
    double TestFunction(int a, const std::vector<double>& arg) 
    {
        return a*std::accumulate(arg.begin(), arg.end(), 0.0);
    }
};

TEST(FunctionTests, DefineVarargFunction)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    DefineFunction<ClassWithVarargArguments> def(typeLibrary, typeLibrary->CreateType(typeid(ClassWithVarargArguments), "ClassWithVarargArguments"));
    def.Function("TestFunction", &ClassWithVarargArguments::TestFunction, Vararg);
}

TEST(FunctionTests, DefineVarargFunction_Invoke)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    AddBuiltInConversions(typeLibrary->GetConversionGraph());
    AddPointerReferenceConversions<ClassWithVarargArguments>(typeLibrary->GetConversionGraph());
    AddTrivialConversions<ClassWithVarargArguments>(typeLibrary->GetConversionGraph());
    
    DefineFunction<ClassWithVarargArguments> def(typeLibrary, typeLibrary->CreateType(typeid(ClassWithVarargArguments), "ClassWithVarargArguments"));
    def.Function("TestFunction", &ClassWithVarargArguments::TestFunction, Vararg);

    ClassWithVarargArguments cls;    
    
    std::vector<Object> args;
    Object self(typeLibrary, &cls);
    args.push_back(Object(typeLibrary, 5));
    args.push_back(Object(typeLibrary, 4.2));
    args.push_back(Object(typeLibrary, 2.6));
    EXPECT_NEAR(5*(4.2 + 2.6), self.Invoke("TestFunction", args).As<double>(), 1e-6);
}

TEST(FunctionTests, DefineVarargFunction_DocumentArguments)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();

    DefineFunction<ClassWithVarargArguments> def(typeLibrary, typeLibrary->CreateType(typeid(ClassWithVarargArguments), "ClassWithVarargArguments"));
    def.Function("TestFunction", &ClassWithVarargArguments::TestFunction, Vararg)
       .AddSignature("Multiplier");
    TypePointer type = typeLibrary->LookupType(typeid(ClassWithVarargArguments));
    EXPECT_EQ("Multiplier", type->Lookup("TestFunction")->GetArgumentInfo(1)->GetName());
}

TEST(FunctionTests, RegisterStaticAndMemberOverload_Fail)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();

    DefineFunction<std::string> def(typeLibrary, typeLibrary->CreateType(typeid(std::string), "string"));
    def.Function("clear", &std::string::clear);
    ASSERT_THROW(def.StaticFunction<double(*)(double)>("clear", &sin), std::runtime_error);
}

TEST(FunctionTests, InvokingFunctionWithNoConversionSpecifiedIncursNoConversionCost)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();

    DefineFunction<TestClassWithFunctionArguments> def(typeLibrary, typeLibrary->CreateType(typeid(TestClassWithFunctionArguments), "TestClassWithFunctionArguments"));
    AddTrivialConversions<TestClassWithFunctionArguments>(typeLibrary->GetConversionGraph());
    AddPointerReferenceConversions<TestClassWithFunctionArguments>(typeLibrary->GetConversionGraph());
    AddBuiltInConversions(typeLibrary->GetConversionGraph());
    //Reflect a function with one default argument
    auto& function = def.Function("Add", &TestClassWithFunctionArguments::Add)
        .AddSignature("a", Arg("b") = 4);

    std::vector<Variant> args(3);
    TestClassWithFunctionArguments testClass;

    args[0] = VariantService::Reflect<TestClassWithFunctionArguments*>(&testClass);
    args[1] = VariantService::Reflect<int>(4);
    args[2] = VariantService::Reflect<int>(7);
    Variant result = function.InvokeNative(args);
    ASSERT_EQ(11, VariantService::Unreflect<int>(result));
}

TEST(FunctionTests, DISABLED_PerformanceOfInvokeNativeIsSameOrderOfMagnitudeAsRegularFunctionCall)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();

    DefineFunction<TestClassWithFunctionArguments> def(typeLibrary, typeLibrary->CreateType(typeid(TestClassWithFunctionArguments), "TestClassWithFunctionArguments"));
    AddTrivialConversions<TestClassWithFunctionArguments>(typeLibrary->GetConversionGraph());
    AddPointerReferenceConversions<TestClassWithFunctionArguments>(typeLibrary->GetConversionGraph());
    AddBuiltInConversions(typeLibrary->GetConversionGraph());
    //Reflect a function with one default argument
    auto& function = def.Function("Add", &TestClassWithFunctionArguments::Add)
        .AddSignature("a", Arg("b") = 4);

    std::vector<Variant> args(3);
    TestClassWithFunctionArguments testClass;

    args[0] = VariantService::Reflect<TestClassWithFunctionArguments*>(&testClass);
    args[1] = VariantService::Reflect<int>(4);
    args[2] = VariantService::Reflect<int>(7);
    auto baseline = [&]() -> int
    {
        volatile double a = 4;
        volatile int b = 7;
        return testClass.Add((int)a, b);
    };
    auto contender = [&]()
    {
        return InternalVariantService::UnreflectUnchecked<int>(function.InvokeNative(args));
    };
    auto contender2 = [&]()
    {
        return InternalVariantService::UnreflectUnchecked<int>(function.PrepareInvoke(args)->Invoke());
    };
    auto contender3 = [&]()
    {
        return InternalVariantService::UnreflectUnchecked<int>(function.Invoke(args));
    };
    auto contender3Fast = [&]()
    {
        return InternalVariantService::UnreflectUnchecked<int>(function.FastInvoke(args));
    };
    DNVS::MoFa::Reflection::Members::Invoker::FastInvoker invoker(&function);
    auto contender4 = [&]() -> int
    {
        args[1] = VariantService::Reflect<double>(4.0);
        return InternalVariantService::UnreflectUnchecked<int>(invoker.Invoke(args));
    };

    Object var(typeLibrary, &testClass);
    std::vector<Object> args2;
    args2.push_back(Object(typeLibrary, 4));
    args2.push_back(Object(typeLibrary, 7));
    auto contender5 = [&]()
    {
        return var.Invoke("Add", args2).As<int>();
    };
    TypePointer type = typeLibrary->LookupType(typeid(TestClassWithFunctionArguments));
    auto contender6_getMember = [&]() -> int
    {
        MemberPointer member = type->Lookup("Add");
        std::vector<Variant> args;
        args.reserve(3);
        args.push_back(var.GetVariant());
        args.push_back(args2[0].GetVariant());
        args.push_back(args2[1].GetVariant());
        return InternalVariantService::UnreflectUnchecked<int>(function.FastInvoke(args));
    };
    auto contender6 = [&]() -> int
    {
        std::vector<Variant> args;
        args.resize(3);
        args[0] = var.GetVariant();
        args[1] = args2[0].GetVariant();
        args[2] = args2[1].GetVariant();
        return InternalVariantService::UnreflectUnchecked<int>(function.FastInvoke(args));
    };
    Profile(contender3, contender3Fast, 0.1);
    Profile(baseline, contender3Fast, 30);
//    Profile(contender3Fast,contender6_getMember,2);
}
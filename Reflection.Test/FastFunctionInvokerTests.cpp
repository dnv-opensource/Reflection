//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest\gtest.h"
#include "Reflection\Variants\Variant.h"
#include "Reflection\Members\Invoker\FastInvoker.h"
#include "Reflection\TypeLibraries\TypeLibraryFactory.h"
#include "Reflection\TypeConversions\BuiltInConversions.h"
#include "Reflection\Functions\DefineFunction.h"
#include <numeric>
#include "Reflection\Members\Arg.h"
#include <string>
namespace DNVS {namespace MoFa { namespace Reflection { namespace Functions { namespace Tests {
    class FastFunctionInvokerTestsClass 
    {
    public:
        double Pow(double a, int b) {return pow(a, b); }
        double Sum(const std::vector<double>& args) 
        {
            return std::accumulate(args.begin(), args.end(), 0.0);
        }
        double Multiply(double a, double b = 5.5)
        {
            return a * b;
        }
    };
    class FastFunctionInvokerTests : public ::testing::Test
    {
    public:
        virtual void SetUp()
        {
            typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary();
            conversionGraph = typeLibrary->GetConversionGraph();
            TypeConversions::AddBuiltInConversions(conversionGraph);
            DefineFunction<FastFunctionInvokerTestsClass> def(typeLibrary,typeLibrary->CreateType(typeid(FastFunctionInvokerTestsClass), "FastFunctionInvokerTestsClass"));
            def.Function("Pow", &FastFunctionInvokerTestsClass::Pow);
            def.Function("Sum", &FastFunctionInvokerTestsClass::Sum, Members::Vararg);
            def.Function("Multiply", &FastFunctionInvokerTestsClass::Multiply).AddSignature("a", Members::Arg("b") = 5.5);
        }

    protected:
        TypeConversions::ConversionGraphPointer conversionGraph;
        TypeLibraries::TypeLibraryPointer typeLibrary;
    };
    TEST_F(FastFunctionInvokerTests, InvokeMemberFunctionWithoutConversion)
    {
        FastFunctionInvokerTestsClass cls;
        std::vector<Variants::Variant> args(3);
        args[0] = Variants::VariantService::Reflect<FastFunctionInvokerTestsClass*>(&cls);
        args[1] = Variants::VariantService::Reflect<double>(5.6);
        args[2] = Variants::VariantService::Reflect<int>(3);
        Members::MemberPointer member = typeLibrary->LookupType(typeid(FastFunctionInvokerTestsClass))->Lookup("Pow");
        ASSERT_DOUBLE_EQ(pow(5.6, 3), Variants::VariantService::Unreflect<double>(Members::Invoker::FastInvoker(member.get()).Invoke(args)));
    }

    TEST_F(FastFunctionInvokerTests, DISABLED_InvokeMemberFunctionWithConversion)
    {
        FastFunctionInvokerTestsClass cls;
        std::vector<Variants::Variant> args(3);
        args[0] = Variants::VariantService::Reflect<FastFunctionInvokerTestsClass*>(&cls);
        args[1] = Variants::VariantService::Reflect<int>(2);
        args[2] = Variants::VariantService::Reflect<const short&>(2);
        Members::MemberPointer member = typeLibrary->LookupType(typeid(FastFunctionInvokerTestsClass))->Lookup("Pow");
        ASSERT_DOUBLE_EQ(4, Variants::VariantService::Unreflect<double>(Members::Invoker::FastInvoker(member.get()).Invoke(args)));
    }

    TEST_F(FastFunctionInvokerTests, InvokeVarargMemberFunctionWithoutConversion)
    {
        FastFunctionInvokerTestsClass cls;
        std::vector<Variants::Variant> args(3);
        args[0] = Variants::VariantService::Reflect<FastFunctionInvokerTestsClass*>(&cls);
        args[1] = Variants::VariantService::Reflect<double>(1.2);
        args[2] = Variants::VariantService::Reflect<double>(2.2);
        Members::MemberPointer member = typeLibrary->LookupType(typeid(FastFunctionInvokerTestsClass))->Lookup("Sum");
        ASSERT_DOUBLE_EQ(3.4, Variants::VariantService::Unreflect<double>(Members::Invoker::FastInvoker(member.get()).Invoke(args)));
    }

    TEST_F(FastFunctionInvokerTests, InvokeVarargMemberFunctionWithConversion)
    {
        FastFunctionInvokerTestsClass cls;
        std::vector<Variants::Variant> args(3);
        args[0] = Variants::VariantService::Reflect<FastFunctionInvokerTestsClass*>(&cls);
        args[1] = Variants::VariantService::Reflect<int>(1);
        args[2] = Variants::VariantService::Reflect<int>(6);
        Members::MemberPointer member = typeLibrary->LookupType(typeid(FastFunctionInvokerTestsClass))->Lookup("Sum");
        ASSERT_DOUBLE_EQ(7.0, Variants::VariantService::Unreflect<double>(Members::Invoker::FastInvoker(member.get()).Invoke(args)));
    }

    TEST_F(FastFunctionInvokerTests, InvokeMemberFunctionWithDefaultArguments)
    {
        FastFunctionInvokerTestsClass cls;
        std::vector<Variants::Variant> args(2);
        args[0] = Variants::VariantService::Reflect<FastFunctionInvokerTestsClass*>(&cls);
        args[1] = Variants::VariantService::Reflect<double>(1.6);
        Members::MemberPointer member = typeLibrary->LookupType(typeid(FastFunctionInvokerTestsClass))->Lookup("Multiply");
        ASSERT_DOUBLE_EQ(1.6*5.5, Variants::VariantService::Unreflect<double>(Members::Invoker::FastInvoker(member.get()).Invoke(args)));
    }

    TEST_F(FastFunctionInvokerTests, InvokeMemberFunctionWithIncorrectNumberOfArgumentsThrows)
    {
        FastFunctionInvokerTestsClass cls;
        std::vector<Variants::Variant> args(2);
        args[0] = Variants::VariantService::Reflect<FastFunctionInvokerTestsClass*>(&cls);
        args[1] = Variants::VariantService::Reflect<double>(1.6);
        Members::MemberPointer member = typeLibrary->LookupType(typeid(FastFunctionInvokerTestsClass))->Lookup("Pow");
        ASSERT_THROW(Members::Invoker::FastInvoker(member.get()).Invoke(args), std::runtime_error);
    }

    TEST_F(FastFunctionInvokerTests, InvokeMemberFunctionWithUnconvertableArgumentsThrows)
    {
        FastFunctionInvokerTestsClass cls;
        std::vector<Variants::Variant> args(3);
        args[0] = Variants::VariantService::Reflect<FastFunctionInvokerTestsClass*>(&cls);
        args[1] = Variants::VariantService::Reflect<std::string>(std::string("1.6"));
        args[2] = Variants::VariantService::Reflect<double>(1.6);
        Members::MemberPointer member = typeLibrary->LookupType(typeid(FastFunctionInvokerTestsClass))->Lookup("Pow");
        ASSERT_THROW(Members::Invoker::FastInvoker(member.get()).Invoke(args), std::runtime_error);
    }
}}}}}

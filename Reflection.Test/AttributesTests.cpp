//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest\gtest.h"
#include "Reflection\TypeLibraries\TypeLibraryPointer.h"
#include "Reflection\Classes\Class.h"
#include "Reflection\Attributes\GenerateConstructorArgumentsAttribute.h"
#include "Reflection\Attributes\FunctionCallbackAttributeVararg.h"
#include "Reflection\TypeLibraries\TypeLibraryFactory.h"
#include "Reflection\Objects\Object.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Test {
    class Plane3d {
    public:
        Plane3d(double a, double b, double c, double d) : m_a(a), m_b(b) ,m_c(c), m_d(d) {}
        static Plane3d XPlane3d(double a) 
        {
            return Plane3d(a, 0, 0, 0);
        }
        double m_a, m_b, m_c, m_d;
    };

    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, Plane3d**)
    {
        using namespace Classes;
        Class<Plane3d> cls(typeLibrary, "Plane3d");
        cls.Constructor<double, double, double, double>();
        cls.NamedConstructor("XPlane3d", Plane3d::XPlane3d)
            .AddAttribute<GenerateConstructorArgumentsAttribute>([](const Plane3d& plane, double& arg1) 
        {
            if (plane.m_b == 0.0 && plane.m_c == 0.0 && plane.m_d == 0.0)
            {
                arg1 = plane.m_a;
                return true;
            }
            else
                return false; 
        });
    }
    
    TEST(AttributesTests, TestGenerateConstructorArgumentsAttribute)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary();
        Reflect<Plane3d>(typeLibrary);
        auto type = typeLibrary->LookupType(typeid(Plane3d));
        auto plane = type->Lookup("XPlane3d");
        const Attributes::GenerateConstructorArgumentsAttribute& attrib = plane->GetAttributeCollection().GetAttribute<Attributes::GenerateConstructorArgumentsAttribute>();
        std::vector<Variants::Variant> args;
        ASSERT_TRUE(attrib.GenerateConstructorArguments(Objects::Object(typeLibrary, Plane3d(1, 0, 0, 0)), args));
        EXPECT_EQ(1, args.size());
    }

    TEST(AttributesTests, TestFunctionWrapperWithSignature)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary();
        Attributes::FunctionCallbackAttribute<void(int)> wrapper0([](int a) {});
        wrapper0.InvokeFunction(5, {});
        EXPECT_EQ(wrapper0.GetStringArguments().size(), 0);
        Attributes::FunctionCallbackAttribute<void(int)> wrapper1([](int a, double b) {}, "b");
        wrapper1.InvokeFunction(5, {Objects::Object(typeLibrary, 5.4)});
        EXPECT_EQ(wrapper1.GetStringArguments().size(), 1);
    }
}}}}
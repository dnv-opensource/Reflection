//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"
#include "Reflection/Classes/Class.h"
#include "Reflection/TypeLibraries/TypeLibraryFactory.h"
#include "Reflection/Utilities/ArgumentFromObjectExtracter.h"
#include "Reflection/Variants/VariantService.h"
#include "Reflection/Objects/Object.h"
#include "Reflection/Attributes/GenerateConstructorArgumentsAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities { namespace Test {
    class TestClassForArgumentFromObjectExtracter
    {
    public:
        TestClassForArgumentFromObjectExtracter(double x, double y, double z) : m_x(x),m_y(y),m_z(z) {}
        static TestClassForArgumentFromObjectExtracter CreateX(double x) { return TestClassForArgumentFromObjectExtracter(x, 0, 0); }
        void SetX(double x) { m_x = x; }
        double GetX() const { return m_x; }
        void SetY(double y) { m_y = y; }
        double GetY() const { return m_y; }
        void SetZ(double z) { m_z = z; }
        double GetZ() const { return m_z; }
    private:
        double m_x, m_y, m_z;
    };
    
    TEST(ArgumentFromObjectExtracterTest, FailToGetArgumentFromConstructorWithNoGettersAndSetters)
    {
        auto typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        using namespace Classes;
        Class<TestClassForArgumentFromObjectExtracter> cls(typeLibrary, "TestClassForArgumentFromObjectExtracter");
        Members::MemberPointer member = &cls.Constructor<double, double, double>();
        ArgumentFromObjectExtracter extracter(typeLibrary, member, 0);
        EXPECT_FALSE(extracter(Variants::VariantService::Reflect(TestClassForArgumentFromObjectExtracter(1,2,3))).IsValid());
    }

    TEST(ArgumentFromObjectExtracterTest, GetNamedArgumentFromConstructorWithGettersAndSetters)
    {
        auto typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        using namespace Classes;
        Class<TestClassForArgumentFromObjectExtracter> cls(typeLibrary, "TestClassForArgumentFromObjectExtracter");
        Members::MemberPointer member = &cls.Constructor<double, double, double>().AddSignature("x", "y", "z");
        cls.SetGet("Y", &TestClassForArgumentFromObjectExtracter::SetY, &TestClassForArgumentFromObjectExtracter::GetY);
        ArgumentFromObjectExtracter extracter(typeLibrary, member, 1);
        auto y = extracter(Variants::VariantService::Reflect(TestClassForArgumentFromObjectExtracter(1, 2, 3)));
        ASSERT_TRUE(y.IsValid());
        EXPECT_DOUBLE_EQ(2, Objects::Object(typeLibrary, y).As<double>());
    }

    TEST(ArgumentFromObjectExtracterTest, GetArgumentValueUsingGenerateConstructorArgumentsAttribute)
    {
        auto typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        using namespace Classes;
        Class<TestClassForArgumentFromObjectExtracter> cls(typeLibrary, "TestClassForArgumentFromObjectExtracter");
        cls.Constructor<double, double, double>().AddSignature("x", "y", "z");
        Members::MemberPointer member = &cls.NamedConstructor("CreateX", &TestClassForArgumentFromObjectExtracter::CreateX)
            .AddAttribute<GenerateConstructorArgumentsAttribute>([](const TestClassForArgumentFromObjectExtracter& object, double& x) 
        {
            if (object.GetY() == 0 && object.GetZ() == 0)
            {
                x = object.GetX();
                return true;
            }
            return false;
        });
        ArgumentFromObjectExtracter extracter(typeLibrary, member, 0);
        EXPECT_FALSE(extracter(Variants::VariantService::Reflect(TestClassForArgumentFromObjectExtracter(1, 2, 3))).IsValid());
        auto x = extracter(Variants::VariantService::Reflect(TestClassForArgumentFromObjectExtracter(1.45, 0, 0)));
        ASSERT_TRUE(x.IsValid());
        EXPECT_DOUBLE_EQ(1.45, Objects::Object(typeLibrary, x).As<double>());
    }

    TEST(ArgumentFromObjectExtracterTest, GetArgumentValueUsingGenerateConstructorArgumentsAttributeAndName)
    {
        auto typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        using namespace Classes;
        Class<TestClassForArgumentFromObjectExtracter> cls(typeLibrary, "TestClassForArgumentFromObjectExtracter");
        cls.Constructor<double, double, double>().AddSignature("x", "y", "z");
        Members::MemberPointer member = &cls.NamedConstructor("CreateX", &TestClassForArgumentFromObjectExtracter::CreateX)
            .AddSignature("x")
            .AddAttribute<GenerateConstructorArgumentsAttribute>([](const TestClassForArgumentFromObjectExtracter& object, double& x)
        {
            if (object.GetY() == 0 && object.GetZ() == 0)
            {
                x = object.GetX();
                return true;
            }
            return false;
        });
        ArgumentFromObjectExtracter extracter(typeLibrary, typeid(TestClassForArgumentFromObjectExtracter),"X", member);
        EXPECT_FALSE(extracter(Variants::VariantService::Reflect(TestClassForArgumentFromObjectExtracter(1, 2, 3))).IsValid());
        auto x = extracter(Variants::VariantService::Reflect(TestClassForArgumentFromObjectExtracter(1.45, 0, 0)));
        ASSERT_TRUE(x.IsValid());
        EXPECT_DOUBLE_EQ(1.45, Objects::Object(typeLibrary, x).As<double>());
    }

}}}}}
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest\gtest.h"
#include "Reflection\TypeLibraries\TypeLibraryFactory.h"
#include "Reflection\Utilities\SerializationNameExtracter.h"
#include "Reflection\Classes\Class.h"
#include "Reflection\Attributes\SerializationAttribute.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {    
    class TestClassForNameExtracterTests
    {
    public:
        TestClassForNameExtracterTests(double, int) {}
        double GetX() { return 0.0; }
    };
    TEST(SerializationNameExtracterTests, ExtractDefaultNameOfConstructor)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        using namespace Classes;
        Class<TestClassForNameExtracterTests> cls(typeLibrary, "TestClassForNameExtracterTests");
        Members::MemberPointer member = &cls.Constructor<double, int>();
        EXPECT_EQ("TestClassForNameExtracterTests", SerializationNameExtracter(typeLibrary).GetName(member));
    }

    TEST(SerializationNameExtracterTests, ExtractSerializationNameOfConstructor)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        using namespace Classes;
        Class<TestClassForNameExtracterTests> cls(typeLibrary, "TestClassForNameExtracterTests");
        Members::MemberPointer member = &cls.Constructor<double, int>().AddAttribute<SerializationAttribute>("test_class_for_name_extracter_tests");
        EXPECT_EQ("test_class_for_name_extracter_tests", SerializationNameExtracter(typeLibrary).GetName(member));
    }

    TEST(SerializationNameExtracterTests, ExtractGetterName)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        using namespace Classes;
        Class<TestClassForNameExtracterTests> cls(typeLibrary, "TestClassForNameExtracterTests");
        Members::MemberPointer member = &cls.Get("x", &TestClassForNameExtracterTests::GetX);
        EXPECT_EQ("x", SerializationNameExtracter(typeLibrary).GetName(member));
    }

}}}}
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest\gtest.h"
#include "Reflection\Utilities\ConstructibleItem.h"
#include "Reflection\TypeLibraries\TypeLibraryFactory.h"
#include "Reflection\Reflect.h"
#include "Reflection\Classes\Class.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities { namespace Test {
    using namespace Classes;
    class MyTestClass
    {
    public:
        MyTestClass(int) {}
        void SetArgument(int) {}
        int GetArgument() const { return 1; }
        void TestFunction(int, double) {}
    };

    TEST(ConstructibleItemTests, CreateTestItemWithOneUnnamedConstructorArgument_CheckErrorMessage)
    {
        TypeLibraries::TypeLibraryPointer lib = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Reflect<ConstructibleItem>(lib);
        Class<MyTestClass> cls(lib, "MyTestClass");
        cls.Constructor<int>();

        ConstructibleItem item(lib->LookupType(typeid(MyTestClass)), lib);
        try {
            item.Validate();
            ASSERT_FALSE(true) << "Need to define error handling for unnamed constructor arguments";
        }
        catch(std::exception& e)
        { 
            EXPECT_EQ("Constructor registration is incorrect: Argument 0 of MyTestClass(int) is unnamed\n"
                      "You can fix this in your class where you registering your constructor:\n"
                      "cls.Constructor<int>()\n"
                      "   .AddSignature(\"<Argument name 0>\")", std::string(e.what()));
        }
    }

    TEST(ConstructibleItemTests, CreateTestClassWithOneNamedMismatchingConstructorArgument_CheckErrorMessage)
    {
        TypeLibraries::TypeLibraryPointer lib = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Reflect<ConstructibleItem>(lib);
        Class<MyTestClass> cls(lib, "MyTestClass");
        cls.Constructor<int>().AddSignature("argument1");
        cls.SetGet("Argument", &MyTestClass::SetArgument, &MyTestClass::GetArgument);
        ConstructibleItem item(lib->LookupType(typeid(MyTestClass)), lib);
        try {
            item.Validate();
            ASSERT_FALSE(true) << "Need to define error handling for mismatched named constructor arguments";
        }
        catch (std::exception& e)
        {
            EXPECT_EQ("Constructor registration is incorrect: Argument 0, 'argument1' of MyTestClass(int argument1) does not match any of the named properties in the class", std::string(e.what()));
        }
    }

    TEST(ConstructibleItemTests, CreateTestClassWithOneCorrectlyNamedButInvalidPropertyDefinitionArgument_CheckErrorMessage)
    {
        TypeLibraries::TypeLibraryPointer lib = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Reflect<ConstructibleItem>(lib);
        Class<MyTestClass> cls(lib, "MyTestClass");
        cls.Constructor<int>().AddSignature("argument1");
        cls.Set("Argument1", &MyTestClass::SetArgument);
        ConstructibleItem item(lib->LookupType(typeid(MyTestClass)), lib);
        try {
            item.Validate();
            ASSERT_FALSE(true) << "Need to define error handling for invalid named constructor arguments";
        }
        catch (std::exception& e)
        {
            EXPECT_EQ("Constructor registration is incorrect: Argument 0, 'argument1' of MyTestClass(int argument1) represents a Write-Only Property. Only Read/Write properties (defined using SetGet(...)) can be matched with constructor arguments", std::string(e.what()));
        }
    }

    TEST(ConstructibleItemTests, CreateCorrectTestClass_NoErrorMessages)
    {
        TypeLibraries::TypeLibraryPointer lib = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Reflect<ConstructibleItem>(lib);
        Class<MyTestClass> cls(lib, "MyTestClass");
        cls.Constructor<int>().AddSignature("argument1");
        cls.SetGet("Argument1", &MyTestClass::SetArgument, &MyTestClass::GetArgument);
        ConstructibleItem item(lib->LookupType(typeid(MyTestClass)), lib);
        EXPECT_NO_THROW(item.Validate());
    }

    TEST(ConstructibleItemTests, CreateTestClass_ToString)
    {
        TypeLibraries::TypeLibraryPointer lib = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Reflect<ConstructibleItem>(lib);
        Class<MyTestClass> cls(lib, "MyTestClass");
        cls.Constructor<int>().AddSignature("argument1");
        cls.SetGet("Argument1", &MyTestClass::SetArgument, &MyTestClass::GetArgument);
        ConstructibleItem item(lib->LookupType(typeid(MyTestClass)), lib);
        EXPECT_EQ("New MyTestClass", ToString(item, Formatting::FormattingService()));
    }
}}}}}
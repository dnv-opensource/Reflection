//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"
#include <list>
#include <Reflection/TypeLibraries/ITypeLibrary.h>
#include "Reflection/TypeLibraries/TypeLibraryFactory.h"
#include "Reflection/Classes/Class.h"
#include "Reflection/Objects/Object.h"
#include <Reflection/Containers/ReflectList.h>
#include "Reflection/TypeConversions/BuiltInConversions.h"

namespace DNVS { namespace MoFa {namespace Reflection {namespace Tests {
    class TestClassWithList {
    public:
        TestClassWithList(const std::list<int>& spacings) : m_spacings(spacings) {}
        const std::list<int>& GetSpacings() const { return m_spacings; }
        void SetSpacings(const std::list<int>& spacings) { m_spacings = spacings; }
    private:
        std::list<int> m_spacings;
    };
    typedef std::shared_ptr<TestClassWithList> TestClassWithListPointer;
    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, TestClassWithList**)
    {
        using namespace Classes;
        Class<TestClassWithList, TestClassWithListPointer> cls(typeLibrary, "TestClassWithList");
        cls.Constructor<const std::list<int>&>(Explicit).AddSignature("spacings");
        cls.SetGet("Spacings", &TestClassWithList::SetSpacings, &TestClassWithList::GetSpacings);
    }
    std::string IntToString(int var, const Formatting::FormattingService& formattingService)
    {
        return boost::lexical_cast<std::string>(var);
    }
    TEST(ToStringTests, ListToStringTest)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        TypeConversions::AddBuiltInConversions(typeLibrary->GetConversionGraph());
        Classes::Class<int> cls(typeLibrary, "");
        cls.Function("ToString", IntToString);
        Reflect<TestClassWithList>(typeLibrary);
        Objects::Object object(typeLibrary, std::make_shared<TestClassWithList>(std::list<int>{1, 4, 5, 2}));
        Formatting::FormattingService formattingService;
        EXPECT_EQ("TestClassWithList(Array(1, 4, 5, 2))", ToString(object, formattingService));
    }
}}}}
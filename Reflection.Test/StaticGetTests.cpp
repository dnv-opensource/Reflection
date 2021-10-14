//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <gtest/gtest.h>
#include "Reflection/Attributes/DefaultConstructorSelectorAttribute.h"
#include "Reflection/Attributes/ListAsConstructorAttribute.h"
#include "Reflection/Classes/Class.h"
#include "Reflection/TypeLibraries/TypeLibraryFactory.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Tests {
    class StaticGetTests {
    public:
        StaticGetTests(int value) : m_value(value) {}
        static StaticGetTests General() { return StaticGetTests(0); }
        int GetVal() const { return m_value; }
        bool operator==(const StaticGetTests& other) const { return m_value == other.m_value; }
    private:
        int m_value;
    };
    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, StaticGetTests**)
    {
        using namespace Classes;
        Class<StaticGetTests, StaticGetTests> cls(typeLibrary, "StaticGetTests");
        cls.StaticGet("General", &StaticGetTests::General)
            .AddAttribute<DefaultConstructorSelectorAttribute>([](const StaticGetTests& other) {return other.GetVal() == 0; })
            .AddAttribute<ListAsConstructorAttribute>();
        cls.Constructor<int>(Explicit)
            .AddSignature("val")
            .AddAttribute<DefaultConstructorSelectorAttribute>([](const StaticGetTests& other) {return other.GetVal() != 0; });
        cls.Get("Val", &StaticGetTests::GetVal);
        cls.Operator(This.Const == This.Const);
    }
    TEST(StaticGetTests, TestToString)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Reflect<StaticGetTests>(typeLibrary);
        EXPECT_EQ("General", ToString(Objects::Object(typeLibrary, StaticGetTests::General()), Formatting::FormattingService()));
        EXPECT_EQ("StaticGetTests(No suitable constructors found to generate string from int)", ToString(Objects::Object(typeLibrary, StaticGetTests(4)), Formatting::FormattingService()));
    }
}}}}
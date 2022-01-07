//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest\gtest.h"
#include "Reflection\Attributes\LayoutAttribute.h"


#include "Reflection\TypeLibraries\TypeLibraryFactory.h"
#include "Reflection\Enums\Enum.h"
#include "Reflection\Classes\Class.h"

#include "Reflection\TypeConversions\BuiltInConversions.h"
#include "LayoutTestClasses.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Attributes { namespace Tests {

    class LayoutAttributeTests : public testing::Test
    {
    public:
        void SetUp()
        {
            typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
            Reflect<MyEnumForLayout2>(typeLibrary);
            Reflect<MyEnumForLayout4>(typeLibrary);
            Reflect<MyEnumAsCombo>(typeLibrary);
            using namespace Classes;
            Class<int>(typeLibrary, "int").Operator(This.Const == This.Const);
            TypeConversions::AddBuiltInConversions(typeLibrary->GetConversionGraph());
        }
        
    protected:
        TypeLibraries::TypeLibraryPointer typeLibrary;
    };
    TEST_F(LayoutAttributeTests, CreateAttributeWithTextBox)
    {
        LayoutAttribute attribute(TextBox("Hello"));
    }

    TEST_F(LayoutAttributeTests, CreateAttributeWithVerticalElements)
    {
        LayoutAttribute attribute(VerticalElements(TextBox("Hello"), TextBox("Bye")));
    }

    TEST_F(LayoutAttributeTests, CreateAttributeWithHorizontalElements)
    {
        LayoutAttribute attribute(HorizontalElements(VerticalElements(TextBox("Hello"), TextBox("Bye")), TextBox("Text")));
    }

    TEST_F(LayoutAttributeTests, ParseLayout)
    {
        LayoutAttribute attribute("Hello World!");
    }

}}}}}

//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest\gtest.h"
#include "Reflection\Attributes\LayoutAttribute.h"
#include "Reflection\Attributes\CaptionAttribute.h"
#include "Reflection\Attributes\GroupAttribute.h"
#include "Reflection\Attributes\ImageAttribute.h"
#include "Reflection\Attributes\HeaderAttribute.h"
#include "Reflection\Attributes\SymmetryAttribute.h"

#include "Reflection\TypeLibraries\TypeLibraryFactory.h"
#include "Reflection\Enums\Enum.h"
#include "Reflection\Classes\Class.h"
#include "Reflection\Layout\LayoutBuilder.h"
#include "Reflection\Layout\Header\DefaultColumn.h"
#include "Reflection\Containers\ReflectVector.h"

#include "EnumOrValue.h"
#include "Reflection\Attributes\ExpandMemberAttribute.h"
#include "Reflection\Attributes\NameAttribute.h"
#include "Reflection\Containers\ReflectVector.h"
#include "Reflection\Layout\Header\PropertyColumn.h"
#include "Reflection\Attributes\UtilityClassAttribute.h"
#include "Reflection\Attributes\TabAttribute.h"
#include "Reflection\Attributes\GroupEnablerAttribute.h"
#include "Reflection\Layout\SelectorLookup.h"
#include "Vector3d.h"
#include "Reflection\Attributes\EnumerableAttribute.h"
#include "Units\TranslationalStiffness.h"
#include "Units\RotationalStiffness.h"
#include "Units\CoupledStiffness.h"
#include "Units\Runtime\DynamicQuantity.h"
#include "Matrix.h"
#include "Reflection\Attributes\IncludeInViewAttribute.h"
#include "Reflection\Attributes\RecordAttribute.h"
#include "Reflection\Utilities\MemberUtilities.h"
#include "Reflection\TypeConversions\BuiltInConversions.h"
#include "Reflection\Attributes\DefaultAttribute.h"
#include "BoundaryStiffnessMatrix.h"
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
   
    TEST_F(LayoutAttributeTests, AutodetectLayoutFromPropertyGet_NoPredefinedLayout)
    {
        using namespace Classes;
        Reflect<MyTestClassForLayout::EnumOrDouble>(typeLibrary, "EnumOrDouble");
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(TextBox("A","A"), *builder.CreateLayoutFromProperty(&cls.Get("A", &MyTestClassForLayout::GetA)));
        EXPECT_EQ(RadioButtons("Enum1", "Enum1"), *builder.CreateLayoutFromProperty(&cls.Get("Enum1", &MyTestClassForLayout::GetEnum1)));
        EXPECT_EQ(RadioButtons("Enum2", "Enum2"), *builder.CreateLayoutFromProperty(&cls.Get("Enum2", &MyTestClassForLayout::GetEnum2)));
        EXPECT_EQ(ComboBox("EnumOrValue", "EnumOrValue"), *builder.CreateLayoutFromProperty(&cls.Get("EnumOrValue", &MyTestClassForLayout::GetEnumOrValue)));
        EXPECT_EQ(CheckBox("Bool", "Bool"), *builder.CreateLayoutFromProperty(&cls.Get("Bool", &MyTestClassForLayout::GetBool)));
        EXPECT_EQ(Table("Vector", "Vector", Header(DefaultColumn("Vector"), false), 6), *builder.CreateLayoutFromProperty(&cls.Get("Vector", &MyTestClassForLayout::GetVector)));
        EXPECT_EQ(Table("Vector", "Vector", Header(DefaultColumn("Item"), true), 6), *builder.CreateLayoutFromProperty(&cls.Get("Vector", &MyTestClassForLayout::GetVector).AddAttribute<HeaderAttribute>(DefaultColumn("Item"))));
        EXPECT_EQ(Table("Vector", "Vector", Header(DefaultColumn("Item"), DefaultColumn("Test").AddDocumentation("Hello"), true), 6),
            *builder.CreateLayoutFromProperty(&cls.Get("Vector", &MyTestClassForLayout::GetVector)
                .AddAttribute<HeaderAttribute>(
                    DefaultColumn("Item"), 
                    CallbackColumn("Test", [](double value) { return value*value; }).AddDefault(4.5).AddDocumentation("Hello")
                )
            )
        );
    }

    TEST_F(LayoutAttributeTests, CreateLayoutFromSplitNumberOrEnum)
    {
        using namespace Classes;
        Reflect<MyTestClassForLayout::EnumOrDouble>(typeLibrary, "EnumOrDouble");
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        auto& member = cls.Get("Enum1", &MyTestClassForLayout::GetEnumOrValue).AddAttribute<LayoutAttribute>(EmbeddedElement(HorizontalElements(Slider("option", "Enum1"), TextBox("value",""))));
        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(EmbeddedElement("Enum1",HorizontalElements(Slider("Option", "Enum1"), TextBox("Value", ""))), *builder.CreateLayoutFromProperty(&member));
        auto embedded = std::dynamic_pointer_cast<EmbeddedElement>(builder.CreateLayoutFromProperty(&member));
        ASSERT_NE(embedded, nullptr);
        ASSERT_NE(embedded->GetContext().function, nullptr);
        EXPECT_EQ(embedded->GetContext().function->ToString({ "a","b" }), "EnumOrDouble(a, b)");
        auto slider = std::dynamic_pointer_cast<Slider>(*std::dynamic_pointer_cast<HorizontalElements>(embedded->GetChild())->begin());
        EXPECT_TRUE(slider->GetAttributeCollection().HasAttribute<EnumerableAttribute>());
    }

    TEST_F(LayoutAttributeTests, AutodetectLayoutFromPropertyGet_WithCaption)
    {
        using namespace Classes;
        Reflect<MyTestClassForLayout::EnumOrDouble>(typeLibrary, "EnumOrDouble");
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(TextBox("A", "MyA"), *builder.CreateLayoutFromProperty(&cls.Get("A", &MyTestClassForLayout::GetA).AddAttribute<CaptionAttribute>("MyA")));
    }

    TEST_F(LayoutAttributeTests, AutodetectLayoutFromPropertyGet_WithGroup)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(GroupBox("MyGroup",TextBox("A","A")), *builder.CreateLayoutFromProperty(&cls.Get("A", &MyTestClassForLayout::GetA).AddAttribute<GroupAttribute>("MyGroup")));
    }

    TEST_F(LayoutAttributeTests, AutodetectLayoutFromPropertyGet_WithSymmetryGroup)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(Symmetry("B", "My symmetry","My doc",GroupBox("MyGroup", TextBox("A","A"))), *builder.CreateLayoutFromProperty(&cls.Get("A", &MyTestClassForLayout::GetA).AddAttribute<GroupAttribute>("MyGroup").AddAttribute<SymmetryAttribute>("B", "My symmetry", "My doc")));
    }

    TEST_F(LayoutAttributeTests, AutodetectLayoutFromPropertyGet_WithImage)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(VerticalElements(Image(1), TextBox("A","A")), *builder.CreateLayoutFromProperty(&cls.Get("A", &MyTestClassForLayout::GetA).AddAttribute<ImageAttribute>(1,ImageAttribute::Top)));
    }

    TEST_F(LayoutAttributeTests, AutodetectLayoutFromPropertyGet_LayoutWithCaptionGiven)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(TextBox("A", "MyA"), *builder.CreateLayoutFromProperty(&cls.Get("A", &MyTestClassForLayout::GetA).AddAttribute<LayoutAttribute>("MyA")));
    }

    TEST_F(LayoutAttributeTests, AutodetectLayoutFromPropertyGet_LayoutWithImage)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(HorizontalElements(Image(1),TextBox("A", "MyA")), *builder.CreateLayoutFromProperty(&cls.Get("A", &MyTestClassForLayout::GetA).AddAttribute<LayoutAttribute>(HorizontalElements(Image(1), "MyA"))));
    }

    TEST_F(LayoutAttributeTests, AutodetectLayoutFromTypeAndName)
    {
        using namespace Classes;
        AutoReflect<std::vector<long>>::Reflect(typeLibrary);
        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(TextBox("A","A"), *builder.CreateLayoutFromType("A", typeid(double)));
        EXPECT_EQ(RadioButtons("B","B"), *builder.CreateLayoutFromType("B", typeid(MyEnumForLayout2)));
        EXPECT_EQ(Table("C", "C", Header(DefaultColumn("C"), false), 6), *builder.CreateLayoutFromType("C", typeid(std::vector<long>)));
    }

    TEST_F(LayoutAttributeTests, AutodetectLayoutFromConstructor_NoPredefinedLayout)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        cls.Constructor<double, MyEnumForLayout2, MyEnumForLayout4>().AddSignature("a", "enum1", "enum2");
        cls.Get("A", &MyTestClassForLayout::GetA);
        cls.Get("Enum1", &MyTestClassForLayout::GetEnum1);
        cls.Get("Enum2", &MyTestClassForLayout::GetEnum2);

        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(VerticalElements(TextBox("A", "A"), RadioButtons("Enum1", "Enum1"), RadioButtons("Enum2", "Enum2")), *builder.CreateLayoutFromFunction(cls.GetType()->Lookup(Members::ConstructorString)));
    }

    TEST_F(LayoutAttributeTests, AutodetectLayoutFromConstructor_ModifyLayoutForMemberToIncludeMultiple)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        cls.Constructor<double, MyEnumForLayout2, MyEnumForLayout4>().AddSignature("a", "enum1", "enum2");
        cls.Get("A", &MyTestClassForLayout::GetA).AddAttribute<LayoutAttribute>(VerticalElements("a", "enum2"));
        cls.Get("Enum1", &MyTestClassForLayout::GetEnum1);
        cls.Get("Enum2", &MyTestClassForLayout::GetEnum2);

        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(VerticalElements(VerticalElements(TextBox("A", "a"), RadioButtons("Enum2", "enum2")), RadioButtons("Enum1", "Enum1")), *builder.CreateLayoutFromFunction(cls.GetType()->Lookup(Members::ConstructorString)));
    }


    TEST_F(LayoutAttributeTests, AutodetectLayoutFromConstructor_AddImage)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        cls.Constructor<double, MyEnumForLayout2, MyEnumForLayout4>().AddSignature("a", "enum1", "enum2");
        cls.AddAttribute<ImageAttribute>(1, ImageAttribute::Left);
        cls.Get("A", &MyTestClassForLayout::GetA);
        cls.Get("Enum1", &MyTestClassForLayout::GetEnum1);
        cls.Get("Enum2", &MyTestClassForLayout::GetEnum2);

        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(HorizontalElements(Image(1),VerticalElements(TextBox("A", "A"), RadioButtons("Enum1", "Enum1"), RadioButtons("Enum2", "Enum2"))), *builder.CreateLayoutFromFunction(cls.GetType()->Lookup(Members::ConstructorString)));
    }

    TEST_F(LayoutAttributeTests, AutodetectLayoutFromConstructor_PredefinedPropertyLayout)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        cls.Constructor<double, MyEnumForLayout2, MyEnumForLayout4>().AddSignature("a", "enum1", "enum2");
        cls.AddAttribute<ImageAttribute>(1, ImageAttribute::Left);
        cls.Get("A", &MyTestClassForLayout::GetA).AddAttribute<LayoutAttribute>(GroupBox("Hello world", ComboBox("Tada")));
        cls.Get("Enum1", &MyTestClassForLayout::GetEnum1);
        cls.Get("Enum2", &MyTestClassForLayout::GetEnum2);

        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(HorizontalElements(Image(1), VerticalElements(GroupBox("Hello world", ComboBox("A", "Tada")), RadioButtons("Enum1", "Enum1"), RadioButtons("Enum2", "Enum2"))), *builder.CreateLayoutFromFunction(cls.GetType()->Lookup(Members::ConstructorString)));
    }

    TEST_F(LayoutAttributeTests, UseLayoutFromConstructor_PredefinedPropertyLayout)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        cls.Constructor<double, MyEnumForLayout2, MyEnumForLayout4>().AddSignature("a", "enum1", "enum2")
            .AddAttribute<LayoutAttribute>(VerticalElements("Enum1", "A", ComboBox("Enum2")));
        cls.AddAttribute<ImageAttribute>(1, ImageAttribute::Left);
        cls.Get("A", &MyTestClassForLayout::GetA).AddAttribute<LayoutAttribute>(GroupBox("Hello world", ComboBox("Tada")));
        cls.Get("Enum1", &MyTestClassForLayout::GetEnum1);
        cls.Get("Enum2", &MyTestClassForLayout::GetEnum2);

        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(VerticalElements(RadioButtons("Enum1", "Enum1"), GroupBox("Hello world", ComboBox("A", "Tada")), ComboBox("Enum2", "Enum2")), *builder.CreateLayoutFromFunction(cls.GetType()->Lookup(Members::ConstructorString)));
    }

    TEST_F(LayoutAttributeTests, UseLayoutFromConstructor_IncludeGroups)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        cls.Constructor<double, MyEnumForLayout2, MyEnumForLayout4>().AddSignature("a", "enum1", "enum2")
            .AddAttribute<LayoutAttribute>(VerticalElements("A", "Enum1", "Enum2"));
        cls.AddAttribute<ImageAttribute>(1, ImageAttribute::Left);
        cls.Get("A", &MyTestClassForLayout::GetA);
        cls.Get("Enum1", &MyTestClassForLayout::GetEnum1).AddAttribute<GroupAttribute>("G1");
        cls.Get("Enum2", &MyTestClassForLayout::GetEnum2).AddAttribute<GroupAttribute>("G1");

        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(VerticalElements(TextBox("A","A"),GroupBox("G1",RadioButtons("Enum1","Enum1")),GroupBox("G1",RadioButtons("Enum2","Enum2"))), *builder.CreateLayoutFromFunction(cls.GetType()->Lookup(Members::ConstructorString)));
    }

    TEST_F(LayoutAttributeTests, EnumAsCombo)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");

        auto member = &cls.Function("CallWithEnumAsCombo", &MyTestClassForLayout::CallWithEnumAsCombo).AddSignature("enumAsCombo");
        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(VerticalElements(ComboBox("enumAsCombo","")), *builder.CreateLayoutFromFunction(member));
    }
    TEST_F(LayoutAttributeTests, EnumAsComboWithCaption)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");

        cls.Get("EnumAsCombo", &MyTestClassForLayout::GetEnumAsCombo)
            .AddAttribute<CaptionAttribute>("Enum As Combo");

        auto member = &cls.Function("CallWithEnumAsCombo", &MyTestClassForLayout::CallWithEnumAsCombo).AddSignature("enumAsCombo");
        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(VerticalElements(ComboBox("EnumAsCombo", "Enum As Combo")), *builder.CreateLayoutFromFunction(member));
    }

    TEST_F(LayoutAttributeTests, EmbedMember)
    {
        using namespace Classes;
        Reflect<TypeToEmbed>(typeLibrary);
        LayoutBuilder builder(typeLibrary);
        Class<EmbedderType> cls(typeLibrary, "EmbedderType");
        auto member = &cls.SetGet("Point", &EmbedderType::SetPoint, &EmbedderType::GetPoint).AddAttribute<LayoutAttribute>(GroupBox("Y", EmbeddedElement()));
        EXPECT_EQ(GroupBox("Y", EmbeddedElement("Point", VerticalElements(TextBox("X","X"),TextBox("Y","Y")))), *builder.CreateLayoutFromProperty(member));
    }

    TEST_F(LayoutAttributeTests, EmbedMemberIncludeAttributesOnMember)
    {
        using namespace Classes;
        Reflect<TypeToEmbed>(typeLibrary);
        LayoutBuilder builder(typeLibrary);
        Class<EmbedderType> cls(typeLibrary, "EmbedderType");
        auto member = &cls.SetGet("Point", &EmbedderType::SetPoint, &EmbedderType::GetPoint)
            .AddAttribute<LayoutAttribute>(EmbeddedElement())
            .AddAttribute<GroupAttribute>("Y");
        EXPECT_EQ(GroupBox("Y", EmbeddedElement("Point", VerticalElements(TextBox("X","X"), TextBox("Y","Y")))), *builder.CreateLayoutFromProperty(member));
    }



    TEST_F(LayoutAttributeTests, DetectOverloadSelector)
    {
        using namespace Classes;
        Reflect<A>(typeLibrary);

        Class<B> cls(typeLibrary, "B");
        Members::MemberPointer member = &cls.SetGet("A", &B::SetA, &B::GetA)
            .AddAttribute<ExpandMemberAttribute>(ExpandOptions::CanExpand)
            .AddAttribute<CaptionAttribute>("My A");
        
        LayoutBuilder builder(typeLibrary);

        EXPECT_EQ(OverloadSelector("A",
            VerticalElements(
                HorizontalElements(Selector(DropdownButton()), TextBox("A", "My A")),
                Collapsable(GroupBox("", DynamicArea()))
            )
        ), *builder.CreateLayoutFromProperty(member));
    }

    TEST_F(LayoutAttributeTests, DetectOverloadSelector_SameSelectorForTwoDynamicAreas)
    {
        using namespace Classes;
        Reflect<A>(typeLibrary);

        Class<B> cls(typeLibrary, "B");
        cls.SetGet("A", &B::SetA, &B::GetA)
            .AddAttribute<ExpandMemberAttribute>(ExpandOptions::CanExpand)
            .AddAttribute<CaptionAttribute>("My A")
            .AddAttribute<LayoutAttribute>(OverloadSelector(VerticalElements(SelectorLookup("A"), DynamicArea())));
        cls.SetGet("A2", &B::SetA2, &B::GetA2)
            .AddAttribute<ExpandMemberAttribute>(ExpandOptions::CanExpand)
            .AddAttribute<CaptionAttribute>("My A2")
            .AddAttribute<LayoutAttribute>(OverloadSelector(VerticalElements(SelectorLookup("A"), DynamicArea())));

        Members::MemberPointer member =
            &cls.Constructor<const A&, const A&>()
            .AddSignature("a", "a2")
            .AddAttribute<LayoutAttribute>(VerticalElements(
                Selector(Buttons("A")), 
                HorizontalElements("A", "A2")));
        LayoutBuilder builder(typeLibrary);

        EXPECT_EQ(*builder.CreateLayoutFromMember(member),
                  EmbeddedElement("B(a, a2)", VerticalElements(
                      Selector(Buttons("A")),
                      HorizontalElements(
                          OverloadSelector("A",  VerticalElements(SelectorLookup("A"),DynamicArea())),
                          OverloadSelector("A2", VerticalElements(SelectorLookup("A"), DynamicArea()))
                      )
                  )));
    }

    TEST_F(LayoutAttributeTests, DetectOverloadSelector_Custom)
    {
        using namespace Classes;
        Reflect<A>(typeLibrary);

        Class<B> cls(typeLibrary, "B");
        Members::MemberPointer member = &cls.SetGet("A", &B::SetA, &B::GetA)
            .AddAttribute<LayoutAttribute>(OverloadSelector(HorizontalElements(Selector(RadioButtons("Curve")), DynamicArea())))
            .AddAttribute<CaptionAttribute>("My A");

        LayoutBuilder builder(typeLibrary);

        EXPECT_EQ(OverloadSelector("A", HorizontalElements(Selector(RadioButtons("Curve")), DynamicArea())), *builder.CreateLayoutFromProperty(member));
        EXPECT_NE(nullptr, std::dynamic_pointer_cast<OverloadSelector>(builder.CreateLayoutFromProperty(member))->GetContext().getter);
    }

    TEST_F(LayoutAttributeTests, DetectOverloadSelector_AttributeOnClass)
    {
        using namespace Classes;
        Reflect<A>(typeLibrary);
        LayoutBuilder builder(typeLibrary);

        EXPECT_EQ(OverloadSelector("A",VerticalElements(HorizontalElements(Selector(DropdownButton("")), TextBox("A", "A")), Collapsable(GroupBox("", DynamicArea())))), *builder.CreateLayoutFromType("A",typeid(A), nullptr));
    }

    TEST_F(LayoutAttributeTests, DetectOverloadSelector_FromType)
    {
        using namespace Classes;
        Reflect<A>(typeLibrary);

        Class<B> cls(typeLibrary, "B");
        Members::MemberPointer member = &cls.SetGet("A", &B::SetA, &B::GetA)
            .AddAttribute<CaptionAttribute>("My A");

        LayoutBuilder builder(typeLibrary);

        EXPECT_EQ(OverloadSelector("A",
            VerticalElements(
                HorizontalElements(Selector(DropdownButton()), TextBox("A", "My A")),
                Collapsable(GroupBox("", DynamicArea()))
            )
        ), *builder.CreateLayoutFromProperty(member));
    }

    TEST_F(LayoutAttributeTests, DetectOverloadSelector_FromType_UseTabControl)
    {
        using namespace Classes;
        Reflect<A>(typeLibrary);

        Class<B> cls(typeLibrary, "B");
        Members::MemberPointer member = &cls.SetGet("A", &B::SetA, &B::GetA)
            .AddAttribute<LayoutAttribute>(OverloadSelector(GroupBox("My A" ,TabControl())));

        LayoutBuilder builder(typeLibrary);

        EXPECT_EQ(OverloadSelector("A",GroupBox("My A", TabControl())), *builder.CreateLayoutFromProperty(member));
    }

    TEST_F(LayoutAttributeTests, StaticPropertyGivesEmptyLayout)
    {
        using namespace Classes;
        Class<B> cls(typeLibrary, "B");
        Members::MemberPointer member = &cls.StaticGet("A", 5);

        LayoutBuilder builder(typeLibrary);

        EXPECT_EQ(nullptr,builder.CreateLayoutFromProperty(member));
        EXPECT_EQ(nullptr, builder.CreateLayoutFromMember(member));
    }

    TEST_F(LayoutAttributeTests, AutodetectLayoutFromConstructorOfNamedObject_NoPredefinedLayout)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        cls.AddAttribute<NameAttribute>([](MyTestClassForLayout*) {return ""; }, [](MyTestClassForLayout*, std::string) {});
        cls.Constructor<double, MyEnumForLayout2, MyEnumForLayout4>().AddSignature("a", "enum1", "enum2");
        cls.Get("A", &MyTestClassForLayout::GetA);
        cls.Get("Enum1", &MyTestClassForLayout::GetEnum1);
        cls.Get("Enum2", &MyTestClassForLayout::GetEnum2);

        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(VerticalElements(TextBox("__this__", "Name"), VerticalElements(TextBox("A", "A"), RadioButtons("Enum1", "Enum1"), RadioButtons("Enum2", "Enum2"))), *builder.CreateLayoutFromFunction(cls.GetType()->Lookup(Members::ConstructorString)));
    }

    TEST_F(LayoutAttributeTests, TableWithPropertyColumn)
    {
        using namespace Classes;
        Class<B> cls(typeLibrary, "B");
        auto& prop = cls.Get("AllA", &B::GetAllA)
            .AddAttribute<HeaderAttribute>(DefaultColumn("A"), PropertyColumn("X"), PropertyColumn("I", "Index"));

        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(Table("AllA", "AllA", Header(DefaultColumn("A"), PropertyColumn("X"), PropertyColumn("I","Index")), 6), *builder.CreateLayoutFromProperty(&prop));
    }


    TEST_F(LayoutAttributeTests, GridWithAutomatedColumn)
    {
        using namespace Classes;
        Reflect<MyRow>(typeLibrary);
        Class<MyGrid> cls(typeLibrary, "MyGrid");
        auto& prop = cls.SetGet("Rows", &MyGrid::SetRows,&MyGrid::GetRows)
            .AddAttribute<LayoutAttribute>(Grid());

        LayoutBuilder builder(typeLibrary);
        auto layout = builder.CreateLayoutFromProperty(&prop);
        auto myGrid = std::dynamic_pointer_cast<Grid>(layout);
        ASSERT_NE(myGrid, nullptr);
        EXPECT_EQ(Grid("Rows", "", HorizontalElements(TextBox("X", "X"), TextBox("Y", "Y"), TextBox("Layers", "Number of layers")), 6), *layout);
        ASSERT_NE(myGrid->GetFunction(), nullptr);
    }


    TEST_F(LayoutAttributeTests, TestFunctionWithArgumentAttributes)
    {
        using namespace Classes;        
        Class<C> cls(typeLibrary, "C");
        auto& fn = cls.StaticFunction("TestFunction", &C::TestFunction)
            .AddSignature("a", "b")
            .AddArgumentAttribute<CaptionAttribute>("a", "A");

        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(VerticalElements(TextBox("a", "A"), TextBox("b", "b")), *builder.CreateLayoutFromFunction(&fn));
    }


    TEST_F(LayoutAttributeTests, TestUtilityFunction)
    {
        using namespace Classes;
        Class<UtilityClass> cls(typeLibrary, "UtilityClass");
        cls.AddAttribute<UtilityClassAttribute>();
        auto& fn = cls.Function("TestFunction", &UtilityClass::TestFunction)
            .AddSignature("a", "b")
            .AddArgumentAttribute<CaptionAttribute>("a", "A");
        cls.SetGet("Option", &UtilityClass::SetOption, &UtilityClass::GetOption);
        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(VerticalElements(TextBox("__this__","UtilityClass"),VerticalElements(TextBox("a", "A"), TextBox("b", "b"), TextBox("Option","Option"))), *builder.CreateLayoutFromFunction(&fn));
    }

    TEST_F(LayoutAttributeTests, TestDefaultTab)
    {
        using namespace Classes;
        Class<UtilityClass> cls(typeLibrary, "UtilityClass");
        cls.AddAttribute<TabAttribute>("Hello");
        cls.SetGet("Option", &UtilityClass::SetOption, &UtilityClass::GetOption);
        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(TabControl(EmbeddedElement("Hello", VerticalElements(TextBox("Option","Option")))), *builder.CreateLayoutFromType(typeid(UtilityClass)));
    }

    TEST_F(LayoutAttributeTests, TestTwoTabs)
    {
        using namespace Classes;
        Class<UtilityClass> cls(typeLibrary, "UtilityClass");
        cls.AddAttribute<TabAttribute>("Hello");
        cls.SetGet("Option", &UtilityClass::SetOption, &UtilityClass::GetOption);
        cls.SetGet("Option2", &UtilityClass::SetOption2, &UtilityClass::GetOption2).AddAttribute<TabAttribute>("Two");
        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(TabControl(
            EmbeddedElement("Hello", VerticalElements(TextBox("Option", "Option"))),
            EmbeddedElement("Two",VerticalElements(TextBox("Option2","Option2")))), *builder.CreateLayoutFromType(typeid(UtilityClass)));
    }

    TEST_F(LayoutAttributeTests, TestElementOutsideTabs)
    {
        using namespace Classes;
        Class<UtilityClass> cls(typeLibrary, "UtilityClass");
        cls.AddAttribute<TabAttribute>("Hello");
        cls.SetGet("Option", &UtilityClass::SetOption, &UtilityClass::GetOption);
        cls.SetGet("Option2", &UtilityClass::SetOption2, &UtilityClass::GetOption2).AddAttribute<TabAttribute>(TabPosition::AboveTab);
        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(VerticalElements(
            TextBox("Option2", "Option2"),
            TabControl(EmbeddedElement("Hello", VerticalElements(TextBox("Option", "Option"))))
        ), *builder.CreateLayoutFromType(typeid(UtilityClass)));
    }

    TEST_F(LayoutAttributeTests, TestCustomPartialLayout)
    {
        using namespace Classes;
        Class<UtilityClass> cls(typeLibrary, "UtilityClass");
        cls.SetGet("OptionBool", &UtilityClass::SetOptionBool, &UtilityClass::GetOptionBool)
            .AddAttribute<LayoutAttribute>(HorizontalElements("OptionBool", "Option"))
            .AddAttribute<CaptionAttribute>("Choice");
        cls.SetGet("Option", &UtilityClass::SetOption, &UtilityClass::GetOption)
            .AddAttribute<CaptionAttribute>("Value");
        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(VerticalElements(HorizontalElements(CheckBox("OptionBool", "Choice"), TextBox("Option", "Value"))), *builder.CreateLayoutFromType(typeid(UtilityClass)));
    }


    
    TEST_F(LayoutAttributeTests, TestIndividualRadioButtonLayout)
    {
        using namespace Classes;
        Class<EliminateEdgeClass> cls(typeLibrary, "EliminateEdgeClass");
        cls.SetGet("EliminateRelativeLength", &EliminateEdgeClass::SetEliminateRelativeLength, &EliminateEdgeClass::GetEliminateRelativeLength)
            .AddAttribute<LayoutAttribute>(RadioButton())
            .AddAttribute<CaptionAttribute>("Relative length to mesh density:")
            ;
        cls.SetGet("EliminateAbsoluteLength", &EliminateEdgeClass::SetEliminateAbsoluteLength, &EliminateEdgeClass::GetEliminateAbsoluteLength)
            .AddAttribute<LayoutAttribute>(RadioButton())
            .AddAttribute<CaptionAttribute>("Absolute length:")
            ;
        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(VerticalElements(RadioButton("EliminateRelativeLength", "Relative length to mesh density:"), RadioButton("EliminateAbsoluteLength", "Absolute length:")), *builder.CreateLayoutFromType(typeid(EliminateEdgeClass)));
    }

    TEST_F(LayoutAttributeTests, TestIndividualRadioButtonWithTextBoxBehind)
    {
        using namespace Classes;
        Class<EliminateEdgeClass> cls(typeLibrary, "EliminateEdgeClass");
        cls.SetGet("EliminateRelativeLength", &EliminateEdgeClass::SetEliminateRelativeLength, &EliminateEdgeClass::GetEliminateRelativeLength)
            .AddAttribute<LayoutAttribute>(HorizontalElements(RadioButton(), "RelativeLengthLimit"))
            .AddAttribute<CaptionAttribute>("Relative length to mesh density:")
            .AddAttribute<GroupAttribute>("Hello")
            ;
        cls.SetGet("RelativeLengthLimit", &EliminateEdgeClass::SetRelativeLengthLimit, &EliminateEdgeClass::GetRelativeLengthLimit)
            .AddAttribute<CaptionAttribute>("")
            ;
        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(VerticalElements(GroupBox("Hello",HorizontalElements(RadioButton("EliminateRelativeLength", "Relative length to mesh density:"), TextBox("RelativeLengthLimit", "")))), *builder.CreateLayoutFromType(typeid(EliminateEdgeClass)));
    }

    TEST_F(LayoutAttributeTests, TestGroupBoxWithEnableControl)
    {
        using namespace Classes;
        Class<EliminateEdgeClass> cls(typeLibrary, "EliminateEdgeClass");
        cls.SetGet("EliminateRelativeLength", &EliminateEdgeClass::SetEliminateRelativeLength, &EliminateEdgeClass::GetEliminateRelativeLength)
            .AddAttribute<LayoutAttribute>(RadioButton())
            .AddAttribute<CaptionAttribute>("Relative length to mesh density:")
            .AddAttribute<GroupAttribute>("Eliminate edges shorter than")
            .AddAttribute<GroupEnablerAttribute>([](bool eliminateRelativeLength, bool eliminateAbsoluteLength) {return eliminateRelativeLength || eliminateAbsoluteLength; }, "EliminateRelativeLength", "EliminateAbsoluteLength")
            ;
        cls.SetGet("EliminateAbsoluteLength", &EliminateEdgeClass::SetEliminateAbsoluteLength, &EliminateEdgeClass::GetEliminateAbsoluteLength)
            .AddAttribute<LayoutAttribute>(RadioButton())
            .AddAttribute<GroupAttribute>("Eliminate edges shorter than")
            .AddAttribute<CaptionAttribute>("Absolute length:")
            ;
        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(VerticalElements(GroupBox(CheckBox("","Eliminate edges shorter than"), RadioButton("EliminateRelativeLength", "Relative length to mesh density:")),GroupBox("Eliminate edges shorter than", RadioButton("EliminateAbsoluteLength", "Absolute length:"))), *builder.CreateLayoutFromType(typeid(EliminateEdgeClass)));
    }

    TEST_F(LayoutAttributeTests, TestGroupBoxWithEnableControlConnectedToProperty)
    {
        using namespace Classes;
        Class<EliminateEdgeClass> cls(typeLibrary, "EliminateEdgeClass");
        cls.SetGet("eliminateShortEdges", &EliminateEdgeClass::SetEliminateShortEdges, &EliminateEdgeClass::GetEliminateShortEdges)
            .AddAttribute<GroupAttribute>("Eliminate edges shorter than")
            .AddAttribute<GroupEnablerAttribute>()
            ;

        cls.SetGet("EliminateRelativeLength", &EliminateEdgeClass::SetEliminateRelativeLength, &EliminateEdgeClass::GetEliminateRelativeLength)
            .AddAttribute<LayoutAttribute>(RadioButton())
            .AddAttribute<CaptionAttribute>("Relative length to mesh density:")
            .AddAttribute<GroupAttribute>("Eliminate edges shorter than")
            ;
        cls.SetGet("EliminateAbsoluteLength", &EliminateEdgeClass::SetEliminateAbsoluteLength, &EliminateEdgeClass::GetEliminateAbsoluteLength)
            .AddAttribute<LayoutAttribute>(RadioButton())
            .AddAttribute<GroupAttribute>("Eliminate edges shorter than")
            .AddAttribute<CaptionAttribute>("Absolute length:")
            ;
        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(VerticalElements(
            GroupBox(CheckBox("eliminateShortEdges", "Eliminate edges shorter than")),
            GroupBox("Eliminate edges shorter than", RadioButton("EliminateRelativeLength", "Relative length to mesh density:")),
            GroupBox("Eliminate edges shorter than", RadioButton("EliminateAbsoluteLength", "Absolute length:"))), *builder.CreateLayoutFromType(typeid(EliminateEdgeClass)));
    }

    TEST_F(LayoutAttributeTests, TestCaption)
    {
        using namespace Classes;
        Class<EliminateEdgeClass> cls(typeLibrary, "EliminateEdgeClass");
        cls.SetGet("eliminateShortEdges", &EliminateEdgeClass::SetEliminateShortEdges, &EliminateEdgeClass::GetEliminateShortEdges)
            .AddAttribute<GroupAttribute>("Eliminate edges shorter than")
            .AddAttribute<LayoutAttribute>(VerticalElements(Caption("Hello"),CheckBox()))
            ;

        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(VerticalElements(
            GroupBox("Eliminate edges shorter than",VerticalElements(Caption("Hello"), CheckBox("eliminateShortEdges", "")))
        ), *builder.CreateLayoutFromType(typeid(EliminateEdgeClass)));
    }

    TEST_F(LayoutAttributeTests, TestEmptyLayout)
    {
        using namespace Classes;
        Class<EliminateEdgeClass> cls(typeLibrary, "EliminateEdgeClass");
        cls.SetGet("eliminateShortEdges", &EliminateEdgeClass::SetEliminateShortEdges, &EliminateEdgeClass::GetEliminateShortEdges)
            .AddAttribute<LayoutAttribute>()
            ;
        cls.SetGet("EliminateRelativeLength", &EliminateEdgeClass::SetEliminateRelativeLength, &EliminateEdgeClass::GetEliminateRelativeLength)
            .AddAttribute<LayoutAttribute>(RadioButton())
            .AddAttribute<CaptionAttribute>("Relative length to mesh density:")
            .AddAttribute<GroupAttribute>("Eliminate edges shorter than")
            ;

        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(VerticalElements(
            GroupBox("Eliminate edges shorter than", RadioButton("EliminateRelativeLength", "Relative length to mesh density:"))
        ), *builder.CreateLayoutFromType(typeid(EliminateEdgeClass)));
    }

    TEST_F(LayoutAttributeTests, TestCreateLayoutFromNamedMemberMethods)
    {
        Reflect<Plate>(typeLibrary);
        LayoutBuilder builder(typeLibrary);
        EXPECT_EQ(*builder.CreateLayoutFromMembers<Plate>("Divide", RadioButtons(), "Divide", "Explode"),
            EmbeddedElement("Divide",
                VerticalElements(
                    TextBox("__this__", "Plate"),
                    OverloadSelector(
                        "Functions",
                        VerticalElements(
                            Selector(RadioButtons()),
                            DynamicArea())))));
    }




    TEST_F(LayoutAttributeTests, CreateViewModelWithFunctionGrid)
    {
        using namespace Classes;
        Reflect<BoundaryStiffnessMatrix>(typeLibrary);
        Utilities::TypedMemberUtilities utils(typeLibrary, Types::TypeId<BoundaryStiffnessMatrix*>());
        auto constructor = utils.GetConstructor<const TranslationalBoundary&, const TranslationalBoundary&, const TranslationalBoundary&, const RotationalBoundary&, const RotationalBoundary&, const RotationalBoundary&>();
        ASSERT_NE(constructor, nullptr);
        LayoutBuilder builder(typeLibrary);
        auto layout = builder.CreateLayoutFromFunction(constructor);
        EXPECT_EQ(*layout,
                  VerticalElements(
                      TextBox("__this__", "Name"), 
                      VerticalElements(
                          Grid("SetStiffness", "",
                               HorizontalElements(Caption(""), Caption("1"), Caption("2"), Caption("3"), Caption("4"), Caption("5"), Caption("6"), Caption("boundary type")), 
                               VerticalElements(
                                   HorizontalElements(Caption("1"), TextBox("Dx.value", ""), TextBox("1,2", ""), TextBox("1,3", ""), TextBox("1,4", ""), TextBox("1,5", ""), TextBox("1,6", ""), ComboBox("Dx.option", "")),
                                   HorizontalElements(Caption("2"), TextBox("2,1", ""), TextBox("Dy.value", ""), TextBox("2,3", ""), TextBox("2,4", ""), TextBox("2,5", ""), TextBox("2,6", ""), ComboBox("Dy.option", "")),
                                   HorizontalElements(Caption("3"), TextBox("3,1", ""), TextBox("3,2", ""), TextBox("Dz.value", ""), TextBox("3,4", ""), TextBox("3,5", ""), TextBox("3,6", ""), ComboBox("Dz.option", "")),
                                   HorizontalElements(Caption("4"), TextBox("4,1", ""), TextBox("4,2", ""), TextBox("4,3", ""), TextBox("Rx.value", ""), TextBox("4,5", ""), TextBox("4,6", ""), ComboBox("Rx.option", "")),
                                   HorizontalElements(Caption("5"), TextBox("5,1", ""), TextBox("5,2", ""), TextBox("5,3", ""), TextBox("5,4", ""), TextBox("Ry.value", ""), TextBox("5,6", ""), ComboBox("Ry.option", "")),
                                   HorizontalElements(Caption("6"), TextBox("6,1", ""), TextBox("6,2", ""), TextBox("6,3", ""), TextBox("6,4", ""), TextBox("6,5", ""), TextBox("Rz.value", ""), ComboBox("Rz.option", ""))
                               ), 6
                          )
                      )
                  )
        );
    }

}}}}}

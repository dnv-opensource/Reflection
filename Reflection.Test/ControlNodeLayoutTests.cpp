//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"
#include "LayoutTestClasses.h"
#include "Reflection/TypeLibraries/TypeLibraryFactory.h"
#include "Reflection/TypeConversions/BuiltInConversions.h"
#include "Reflection/Controls/ControlNode.h"
#include "Reflection/Layout/Handlers/LayoutControl.h"
#include "Reflection/Attributes/TreatAsBoolAttribute.h"
#include "Reflection/Attributes/GroupAttribute.h"
#include "Reflection/Attributes/ImageAttribute.h"
#include "Reflection/Attributes/NameAttribute.h"
#include <Reflection/Containers/ReflectVector.h>
#include "Reflection/Controls/Aspects/ContainerAspect.h"
#include "Reflection/Attributes/UtilityClassAttribute.h"
#include "Reflection/Attributes/TabAttribute.h"
#include "Reflection/Attributes/GroupEnablerAttribute.h"
#include "Reflection/Attributes/FunctionSelectorAttribute.h"
#include "BoundaryStiffnessMatrix.h"
#include "Reflection/Utilities/MemberUtilities.h"
#include "BoundaryCondition.h"
#include "Reflection/Layout/GridElements.h"
#include "Reflection/Attributes/SymmetryAttribute.h"
#include "Reflection/Attributes/HeaderAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Attributes { namespace Tests {

    class ControlNodeLayoutTests : public testing::Test
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
            Class<size_t>(typeLibrary, "size_t").Operator(This.Const == This.Const);
            TypeConversions::AddBuiltInConversions(typeLibrary->GetConversionGraph());
        }
        std::shared_ptr<ILayoutElement> BuildMemberLayout(const Members::MemberPointer& member)
        {
            return LayoutControl(std::make_shared<Controls::ControlNode>(typeLibrary, member)).GetLayout();
        }
        template<typename T>
        std::shared_ptr<ILayoutElement> BuildTypeLayout()
        {
            return LayoutControl(std::make_shared<Controls::ControlNode>(typeLibrary, Types::TypeId<T>())).GetLayout();
        }
    protected:
        TypeLibraries::TypeLibraryPointer typeLibrary;
    };

    TEST_F(ControlNodeLayoutTests, CreateLayoutFromSplitNumberOrEnum)
    {
        using namespace Classes;
        Reflect<MyTestClassForLayout::EnumOrDouble>(typeLibrary, "EnumOrDouble");
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        auto& member = cls.Get("Enum1", &MyTestClassForLayout::GetEnumOrValue).AddAttribute<LayoutAttribute>(EmbeddedElement(HorizontalElements(Slider("option", "Enum1"), TextBox("value", ""))));
        EXPECT_EQ(HorizontalElements(Slider("Option", "Enum1"), TextBox("Value", "")), *BuildMemberLayout(&member));
    }
    
    TEST_F(ControlNodeLayoutTests, AutodetectLayoutFromPropertyGet_NoPredefinedLayout)
    {
        using namespace Classes;
        Reflect<MyTestClassForLayout::EnumOrDouble>(typeLibrary, "EnumOrDouble");
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        EXPECT_EQ(TextBox("A", "A"), *BuildMemberLayout(&cls.Get("A", &MyTestClassForLayout::GetA)));
        EXPECT_EQ(CheckBox("Bool", "Bool"), *BuildMemberLayout(&cls.Get("Bool", &MyTestClassForLayout::GetBool)));       
        EXPECT_EQ(CheckBox("Enum1", "Enum1"), *BuildMemberLayout(&cls.Get("Enum1", &MyTestClassForLayout::GetEnum1).AddAttribute<TreatAsBoolAttribute>(first, second)));
        EXPECT_EQ(RadioButtons("Enum2", "Enum2"), *BuildMemberLayout(&cls.Get("Enum2", &MyTestClassForLayout::GetEnum2)));
        EXPECT_EQ(ComboBox("EnumOrValue", "EnumOrValue"), *BuildMemberLayout(&cls.Get("EnumOrValue", &MyTestClassForLayout::GetEnumOrValue)));

//         EXPECT_EQ(Table("Vector", "Vector", Header(DefaultColumn("Vector"), false), 6), *builder.CreateLayoutFromProperty(&cls.Get("Vector", &MyTestClassForLayout::GetVector)));
//         EXPECT_EQ(Table("Vector", "Vector", Header(DefaultColumn("Item"), true), 6), *builder.CreateLayoutFromProperty(&cls.Get("Vector", &MyTestClassForLayout::GetVector).AddAttribute<HeaderAttribute>(DefaultColumn("Item"))));
//         EXPECT_EQ(Table("Vector", "Vector", Header(DefaultColumn("Item"), DefaultColumn("Test").AddDocumentation("Hello"), true), 6),
//             *builder.CreateLayoutFromProperty(&cls.Get("Vector", &MyTestClassForLayout::GetVector)
//                 .AddAttribute<HeaderAttribute>(
//                     DefaultColumn("Item"),
//                     CallbackColumn("Test", [](double value) { return value * value; }).AddDefault(4.5).AddDocumentation("Hello")
//                     )
//             )
//         );
    }

    TEST_F(ControlNodeLayoutTests, AutodetectLayoutFromPropertyGet_WithCaption)
    {
        using namespace Classes;
        Reflect<MyTestClassForLayout::EnumOrDouble>(typeLibrary, "EnumOrDouble");
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        EXPECT_EQ(TextBox("A", "MyA"), *BuildMemberLayout(&cls.Get("A", &MyTestClassForLayout::GetA).AddAttribute<CaptionAttribute>("MyA")));
    }

    TEST_F(ControlNodeLayoutTests, AutodetectLayoutFromPropertyGet_WithGroup)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        EXPECT_EQ(GroupBox("MyGroup", TextBox("A", "A")), *BuildMemberLayout(&cls.Get("A", &MyTestClassForLayout::GetA).AddAttribute<GroupAttribute>("MyGroup")));
    }

    TEST_F(ControlNodeLayoutTests, AutodetectLayoutFromPropertyGet_WithImage)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        EXPECT_EQ(VerticalElements(Image(1), TextBox("A", "A")), *BuildMemberLayout(&cls.Get("A", &MyTestClassForLayout::GetA).AddAttribute<ImageAttribute>(1, ImageAttribute::Top)));
    }

    TEST_F(ControlNodeLayoutTests, AutodetectLayoutFromPropertyGet_LayoutWithCaptionGiven)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        EXPECT_EQ(TextBox("A", "MyA"), *BuildMemberLayout(&cls.Get("A", &MyTestClassForLayout::GetA).AddAttribute<LayoutAttribute>("MyA")));
    }

    TEST_F(ControlNodeLayoutTests, AutodetectLayoutFromPropertyGet_LayoutWithImage)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        EXPECT_EQ(HorizontalElements(Image(1), TextBox("A", "MyA")), *BuildMemberLayout(&cls.Get("A", &MyTestClassForLayout::GetA).AddAttribute<LayoutAttribute>(HorizontalElements(Image(1), "MyA"))));
    }

    TEST_F(ControlNodeLayoutTests, AutodetectLayoutFromConstructor_NoPredefinedLayout)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        cls.Constructor<double, MyEnumForLayout2, MyEnumForLayout4>().AddSignature("a", "enum1", "enum2");
        cls.Get("A", &MyTestClassForLayout::GetA);
        cls.Get("Enum1", &MyTestClassForLayout::GetEnum1);
        cls.Get("Enum2", &MyTestClassForLayout::GetEnum2);

        EXPECT_EQ(VerticalElements(TextBox("A", "A"), RadioButtons("Enum1", "Enum1"), RadioButtons("Enum2", "Enum2")), *BuildMemberLayout(cls.GetType()->Lookup(Members::ConstructorString)));
    }

    TEST_F(ControlNodeLayoutTests, AutodetectLayoutFromConstructor_ModifyLayoutForMemberToIncludeMultiple)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        cls.Constructor<double, MyEnumForLayout2, MyEnumForLayout4>().AddSignature("a", "enum1", "enum2");
        cls.Get("A", &MyTestClassForLayout::GetA).AddAttribute<LayoutAttribute>(VerticalElements("a", "enum2"));
        cls.Get("Enum1", &MyTestClassForLayout::GetEnum1);
        cls.Get("Enum2", &MyTestClassForLayout::GetEnum2);

        EXPECT_EQ(VerticalElements(TextBox("A", "a"), RadioButtons("Enum2", "enum2"), RadioButtons("Enum1", "Enum1")), *BuildMemberLayout(cls.GetType()->Lookup(Members::ConstructorString)));
    }

    TEST_F(ControlNodeLayoutTests, AutodetectLayoutFromConstructor_AddImage)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        cls.Constructor<double, MyEnumForLayout2, MyEnumForLayout4>().AddSignature("a", "enum1", "enum2");
        cls.AddAttribute<ImageAttribute>(1, ImageAttribute::Left);
        cls.Get("A", &MyTestClassForLayout::GetA);
        cls.Get("Enum1", &MyTestClassForLayout::GetEnum1);
        cls.Get("Enum2", &MyTestClassForLayout::GetEnum2);

        EXPECT_EQ(HorizontalElements(Image(1), VerticalElements(TextBox("A", "A"), RadioButtons("Enum1", "Enum1"), RadioButtons("Enum2", "Enum2"))), *BuildMemberLayout(cls.GetType()->Lookup(Members::ConstructorString)));
    }

    TEST_F(ControlNodeLayoutTests, AutodetectLayoutFromConstructor_PredefinedPropertyLayout)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        cls.Constructor<double, MyEnumForLayout2, MyEnumForLayout4>().AddSignature("a", "enum1", "enum2");
        cls.AddAttribute<ImageAttribute>(1, ImageAttribute::Left);
        cls.Get("A", &MyTestClassForLayout::GetA).AddAttribute<LayoutAttribute>(GroupBox("Hello world", ComboBox("Tada")));
        cls.Get("Enum1", &MyTestClassForLayout::GetEnum1);
        cls.Get("Enum2", &MyTestClassForLayout::GetEnum2);

        EXPECT_EQ(HorizontalElements(Image(1), VerticalElements(GroupBox("Hello world", ComboBox("A", "Tada")), RadioButtons("Enum1", "Enum1"), RadioButtons("Enum2", "Enum2"))), *BuildMemberLayout(cls.GetType()->Lookup(Members::ConstructorString)));
    }

    TEST_F(ControlNodeLayoutTests, UseLayoutFromConstructor_PredefinedPropertyLayout)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        cls.Constructor<double, MyEnumForLayout2, MyEnumForLayout4>().AddSignature("a", "enum1", "enum2")
            .AddAttribute<LayoutAttribute>(VerticalElements("Enum1", "A", ComboBox("Enum2")));
        cls.AddAttribute<ImageAttribute>(1, ImageAttribute::Left);
        cls.Get("A", &MyTestClassForLayout::GetA).AddAttribute<LayoutAttribute>(GroupBox("Hello world", ComboBox("Tada")));
        cls.Get("Enum1", &MyTestClassForLayout::GetEnum1);
        cls.Get("Enum2", &MyTestClassForLayout::GetEnum2);

        EXPECT_EQ(HorizontalElements(Image(1), VerticalElements(RadioButtons("Enum1", "Enum1"), GroupBox("Hello world", ComboBox("A", "Tada")), ComboBox("Enum2", "Enum2"))), *BuildMemberLayout(cls.GetType()->Lookup(Members::ConstructorString)));
    }

    TEST_F(ControlNodeLayoutTests, UseLayoutFromConstructor_IncludeGroups)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        cls.Constructor<double, MyEnumForLayout2, MyEnumForLayout4>().AddSignature("a", "enum1", "enum2")
            .AddAttribute<LayoutAttribute>(VerticalElements("A", "Enum1", "Enum2"));
        cls.AddAttribute<ImageAttribute>(1, ImageAttribute::Left);
        cls.Get("A", &MyTestClassForLayout::GetA);
        cls.Get("Enum1", &MyTestClassForLayout::GetEnum1).AddAttribute<GroupAttribute>("G1");
        cls.Get("Enum2", &MyTestClassForLayout::GetEnum2).AddAttribute<GroupAttribute>("G1");
        
        EXPECT_EQ(HorizontalElements(Image(1), VerticalElements(TextBox("A", "A"), GroupBox("G1", RadioButtons("Enum1", "Enum1"), RadioButtons("Enum2", "Enum2")))), *BuildMemberLayout(cls.GetType()->Lookup(Members::ConstructorString)));
    }

    TEST_F(ControlNodeLayoutTests, CombineGroups_OneFromLayoutOneFromAttribute_Ok)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        cls.Constructor<double, MyEnumForLayout2, MyEnumForLayout4>().AddSignature("a", "enum1", "enum2");
        cls.Get("A", &MyTestClassForLayout::GetA);
        cls.Get("Enum1", &MyTestClassForLayout::GetEnum1)
            .AddAttribute<ImageAttribute>(1, ImageAttribute::Left)
            .AddAttribute<LayoutAttribute>(GroupBox("G1", "Enum 1", "Enum2"));
        cls.Get("Enum2", &MyTestClassForLayout::GetEnum2);

        EXPECT_EQ(VerticalElements(TextBox("A", "A"), HorizontalElements(Image(1), GroupBox("G1", RadioButtons("Enum1", "Enum 1"), RadioButtons("Enum2", "Enum2")))), *BuildMemberLayout(cls.GetType()->Lookup(Members::ConstructorString)));
    }

    TEST_F(ControlNodeLayoutTests, EnumAsCombo)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");

        auto member = &cls.Function("CallWithEnumAsCombo", &MyTestClassForLayout::CallWithEnumAsCombo).AddSignature("enumAsCombo");
        EXPECT_EQ(VerticalElements(ComboBox("enumAsCombo", "")), *BuildMemberLayout(member));
    }

    TEST_F(ControlNodeLayoutTests, EnumAsComboWithCaption)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");

        cls.Get("EnumAsCombo", &MyTestClassForLayout::GetEnumAsCombo)
            .AddAttribute<CaptionAttribute>("Enum As Combo");

        auto member = &cls.Function("CallWithEnumAsCombo", &MyTestClassForLayout::CallWithEnumAsCombo).AddSignature("enumAsCombo");
        EXPECT_EQ(VerticalElements(ComboBox("EnumAsCombo", "Enum As Combo")), *BuildMemberLayout(member));
    }

    TEST_F(ControlNodeLayoutTests, EmbedMember)
    {
        using namespace Classes;
        Reflect<TypeToEmbed>(typeLibrary);
        Class<EmbedderType> cls(typeLibrary, "EmbedderType");
        auto member = &cls.SetGet("Point", &EmbedderType::SetPoint, &EmbedderType::GetPoint).AddAttribute<LayoutAttribute>(GroupBox("Y", EmbeddedElement()));
        EXPECT_EQ(GroupBox("Y", VerticalElements(TextBox("X", "X"), TextBox("Y", "Y"))), *BuildMemberLayout(member));
    }

    TEST_F(ControlNodeLayoutTests, EmbedMemberIncludeAttributesOnMember)
    {
        using namespace Classes;
        Reflect<TypeToEmbed>(typeLibrary);
        Class<EmbedderType> cls(typeLibrary, "EmbedderType");
        auto member = &cls.SetGet("Point", &EmbedderType::SetPoint, &EmbedderType::GetPoint)
            .AddAttribute<LayoutAttribute>(EmbeddedElement())
            .AddAttribute<GroupAttribute>("Y");
        EXPECT_EQ(GroupBox("Y", VerticalElements(TextBox("X", "X"), TextBox("Y", "Y"))), *BuildMemberLayout(member));
    }

    TEST_F(ControlNodeLayoutTests, EmbedMember_SpecifyEmbeddedLayout)
    {
        using namespace Classes;
        Reflect<TypeToEmbed>(typeLibrary);
        Class<EmbedderType> cls(typeLibrary, "EmbedderType");
        auto member = &cls.SetGet("Point", &EmbedderType::SetPoint, &EmbedderType::GetPoint).AddAttribute<LayoutAttribute>(GroupBox("Y", EmbeddedElement(VerticalElements(TextBox("Y"), TextBox("X", "MyX")))));
        EXPECT_EQ(GroupBox("Y", VerticalElements(TextBox("Y", "Y"), TextBox("X", "MyX"))), *BuildMemberLayout(member));
    }

    TEST_F(ControlNodeLayoutTests, DetectOverloadSelector)
    {
        using namespace Classes;
        Reflect<A>(typeLibrary);

        Class<B> cls(typeLibrary, "B");
        Members::MemberPointer member = &cls.SetGet("A", &B::SetA, &B::GetA)
            .AddAttribute<ExpandMemberAttribute>(ExpandOptions::CanExpand)
            .AddAttribute<CaptionAttribute>("My A");

        EXPECT_EQ(OverloadSelector("A",
            VerticalElements(
                HorizontalElements(Selector(DropdownButton("__SELECTOR__",""), false), TextBox("A", "My A")),
                Collapsable(GroupBox("", DynamicArea()))
            )
        ), *BuildMemberLayout(member));
    }

    TEST_F(ControlNodeLayoutTests, DetectOverloadSelector_Custom)
    {
        using namespace Classes;
        Reflect<A>(typeLibrary);

        Class<B> cls(typeLibrary, "B");
        Members::MemberPointer member = &cls.SetGet("A", &B::SetA, &B::GetA)
            .AddAttribute<LayoutAttribute>(OverloadSelector(HorizontalElements(Selector(RadioButtons("Curve")), DynamicArea())))
            .AddAttribute<CaptionAttribute>("My A");

       
        ASSERT_EQ(OverloadSelector("A", HorizontalElements(Selector(RadioButtons("__SELECTOR__", "Curve"), false), DynamicArea())), *BuildMemberLayout(member));
        EXPECT_NE(nullptr, std::dynamic_pointer_cast<OverloadSelector>(BuildMemberLayout(member))->GetControlNode());
    }


    TEST_F(ControlNodeLayoutTests, DetectOverloadSelector_AttributeOnClass)
    {
        using namespace Classes;
        Reflect<A>(typeLibrary);
        EXPECT_EQ(OverloadSelector("A", VerticalElements(HorizontalElements(Selector(DropdownButton("__SELECTOR__", ""), false), TextBox("A", "A")), Collapsable(GroupBox("", DynamicArea())))), *BuildTypeLayout<A>());
    }
    TEST_F(ControlNodeLayoutTests, DetectOverloadSelector_FromType)
    {
        using namespace Classes;
        Reflect<A>(typeLibrary);

        Class<B> cls(typeLibrary, "B");
        Members::MemberPointer member = &cls.SetGet("A", &B::SetA, &B::GetA)
            .AddAttribute<CaptionAttribute>("My A");

        EXPECT_EQ(OverloadSelector("A",
            VerticalElements(
                HorizontalElements(Selector(DropdownButton("__SELECTOR__", ""), false), TextBox("A", "My A")),
                Collapsable(GroupBox("", DynamicArea()))
            )
        ), *BuildMemberLayout(member));
    }
    TEST_F(ControlNodeLayoutTests, DetectOverloadSelector_FromType_UseTabControl)
    {
        using namespace Classes;
        Reflect<A>(typeLibrary);

        Class<B> cls(typeLibrary, "B");
        Members::MemberPointer member = &cls.SetGet("A", &B::SetA, &B::GetA)
            .AddAttribute<LayoutAttribute>(OverloadSelector(GroupBox("My A", TabControl())));

        ASSERT_EQ(OverloadSelector("A", GroupBox("My A", TabControl())), *BuildMemberLayout(member));
        EXPECT_NE(std::dynamic_pointer_cast<TabControl>(*std::dynamic_pointer_cast<GroupBox>(std::dynamic_pointer_cast<OverloadSelector>(BuildMemberLayout(member))->GetChild())->begin())->GetControlNode(), nullptr);
    }

    TEST_F(ControlNodeLayoutTests, StaticPropertyGivesEmptyLayout)
    {
        using namespace Classes;
        Class<B> cls(typeLibrary, "B");
        Members::MemberPointer member = &cls.StaticGet("A", 5);

        EXPECT_EQ(nullptr, BuildMemberLayout(member));
    }

    TEST_F(ControlNodeLayoutTests, AutodetectLayoutFromConstructorOfNamedObject_NoPredefinedLayout)
    {
        using namespace Classes;
        Class<MyTestClassForLayout> cls(typeLibrary, "MyTestClassForLayout");
        cls.AddAttribute<NameAttribute>([](MyTestClassForLayout*) {return ""; }, [](MyTestClassForLayout*, std::string) {});
        cls.Constructor<double, MyEnumForLayout2, MyEnumForLayout4>().AddSignature("a", "enum1", "enum2");
        cls.Get("A", &MyTestClassForLayout::GetA);
        cls.Get("Enum1", &MyTestClassForLayout::GetEnum1);
        cls.Get("Enum2", &MyTestClassForLayout::GetEnum2);

        EXPECT_EQ(VerticalElements(TextBox("__this__", "Name"), TextBox("A", "A"), RadioButtons("Enum1", "Enum1"), RadioButtons("Enum2", "Enum2")), *BuildMemberLayout(cls.GetType()->Lookup(Members::ConstructorString)));
    }

    TEST_F(ControlNodeLayoutTests, GridWithContainer)
    {
        using namespace Classes;
        Reflect<MyRow>(typeLibrary);
        Class<MyGrid> cls(typeLibrary, "MyGrid");
        auto& prop = cls.SetGet("Rows", &MyGrid::SetRows, &MyGrid::GetRows)
            .AddAttribute<LayoutAttribute>(Grid());

        auto layout = BuildMemberLayout(&prop);
        auto myGrid = std::dynamic_pointer_cast<Grid>(layout);
        ASSERT_NE(myGrid, nullptr);
        EXPECT_EQ(Grid("Rows", "", HorizontalElements(TextBox("X", "X"), TextBox("Y", "Y"), TextBox("Layers", "Number of layers"))), *layout);
        ASSERT_NE(myGrid->GetControlNode(), nullptr);
        EXPECT_NE(myGrid->GetControlNode()->TryGetAspect<Controls::ContainerAspect>(), nullptr);
    }

    TEST_F(ControlNodeLayoutTests, TestFunctionWithArgumentAttributes)
    {
        using namespace Classes;
        Class<C> cls(typeLibrary, "C");
        auto& fn = cls.StaticFunction("TestFunction", &C::TestFunction)
            .AddSignature("a", "b")
            .AddArgumentAttribute<CaptionAttribute>("a", "A");

        EXPECT_EQ(VerticalElements(TextBox("a", "A"), TextBox("b", "b")), *BuildMemberLayout(&fn));
    }
    TEST_F(ControlNodeLayoutTests, TestUtilityFunction)
    {
        using namespace Classes;
        Class<UtilityClass> cls(typeLibrary, "UtilityClass");
        cls.AddAttribute<UtilityClassAttribute>();
        auto& fn = cls.Function("TestFunction", &UtilityClass::TestFunction)
            .AddSignature("a", "b")
            .AddArgumentAttribute<CaptionAttribute>("a", "A");
        cls.SetGet("Option", &UtilityClass::SetOption, &UtilityClass::GetOption);
        EXPECT_EQ(VerticalElements(TextBox("a", "A"), TextBox("b", "b"), TextBox("Option", "Option")), *BuildMemberLayout(&fn));
    }

    TEST_F(ControlNodeLayoutTests, TestDefaultTab)
    {
        using namespace Classes;
        Class<UtilityClass> cls(typeLibrary, "UtilityClass");
        cls.AddAttribute<TabAttribute>("Hello");
        cls.SetGet("Option", &UtilityClass::SetOption, &UtilityClass::GetOption);
        EXPECT_EQ(TabControl(EmbeddedElement("Hello", VerticalElements(TextBox("Option", "Option")))), *BuildTypeLayout<UtilityClass>());
    }

    TEST_F(ControlNodeLayoutTests, TestTwoTabs)
    {
        using namespace Classes;
        Class<UtilityClass> cls(typeLibrary, "UtilityClass");
        cls.AddAttribute<TabAttribute>("Hello");
        cls.SetGet("Option", &UtilityClass::SetOption, &UtilityClass::GetOption);
        cls.SetGet("Option2", &UtilityClass::SetOption2, &UtilityClass::GetOption2).AddAttribute<TabAttribute>("Two");

        EXPECT_EQ(TabControl(
            EmbeddedElement("Hello", VerticalElements(TextBox("Option", "Option"))),
            EmbeddedElement("Two", VerticalElements(TextBox("Option2", "Option2")))), *BuildTypeLayout<UtilityClass>());
    }

    TEST_F(ControlNodeLayoutTests, TestElementOutsideTabs)
    {
        using namespace Classes;
        Class<UtilityClass> cls(typeLibrary, "UtilityClass");
        cls.AddAttribute<TabAttribute>("Hello");
        cls.SetGet("Option", &UtilityClass::SetOption, &UtilityClass::GetOption);
        cls.SetGet("Option2", &UtilityClass::SetOption2, &UtilityClass::GetOption2).AddAttribute<TabAttribute>(TabPosition::AboveTab);

        EXPECT_EQ(VerticalElements(
            TextBox("Option2", "Option2"),
            TabControl(EmbeddedElement("Hello", VerticalElements(TextBox("Option", "Option"))))
        ), *BuildTypeLayout<UtilityClass>());
    }

    TEST_F(ControlNodeLayoutTests, TestCustomPartialLayout)
    {
        using namespace Classes;
        Class<UtilityClass> cls(typeLibrary, "UtilityClass");
        cls.SetGet("OptionBool", &UtilityClass::SetOptionBool, &UtilityClass::GetOptionBool)
            .AddAttribute<LayoutAttribute>(HorizontalElements("OptionBool", "Option"))
            .AddAttribute<CaptionAttribute>("Choice");
        cls.SetGet("Option", &UtilityClass::SetOption, &UtilityClass::GetOption)
            .AddAttribute<CaptionAttribute>("Value");
        EXPECT_EQ(VerticalElements(HorizontalElements(CheckBox("OptionBool", "Choice"), TextBox("Option", "Value"))), *BuildTypeLayout<UtilityClass>());
    }

    TEST_F(ControlNodeLayoutTests, TestIndividualRadioButtonLayout)
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

        EXPECT_EQ(VerticalElements(RadioButton("EliminateRelativeLength", "Relative length to mesh density:"), RadioButton("EliminateAbsoluteLength", "Absolute length:")), *BuildTypeLayout<EliminateEdgeClass>());
    }

    TEST_F(ControlNodeLayoutTests, TestIndividualRadioButtonWithTextBoxBehind)
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
        EXPECT_EQ(VerticalElements(GroupBox("Hello", HorizontalElements(RadioButton("EliminateRelativeLength", "Relative length to mesh density:"), TextBox("RelativeLengthLimit", "")))), *BuildTypeLayout<EliminateEdgeClass>());
    }

    TEST_F(ControlNodeLayoutTests, TestGroupAttribute_HasDocumentation)
    {
        using namespace Classes;
        Class<EliminateEdgeClass> cls(typeLibrary, "EliminateEdgeClass");
        cls.SetGet("EliminateRelativeLength", &EliminateEdgeClass::SetEliminateRelativeLength, &EliminateEdgeClass::GetEliminateRelativeLength)
            .AddAttribute<LayoutAttribute>(HorizontalElements(RadioButton(), "RelativeLengthLimit"))
            .AddAttribute<CaptionAttribute>("Relative length to mesh density:")
            .AddAttribute<GroupAttribute>("Hello", "MyDoc")
            ;
        cls.SetGet("RelativeLengthLimit", &EliminateEdgeClass::SetRelativeLengthLimit, &EliminateEdgeClass::GetRelativeLengthLimit)
            .AddAttribute<CaptionAttribute>("")
            ;
        auto layout = BuildTypeLayout<EliminateEdgeClass>();
        auto groupBox = std::dynamic_pointer_cast<GroupBox>(*std::dynamic_pointer_cast<VerticalElements>(layout)->begin());
        EXPECT_EQ(groupBox->GetDocumentation(), "MyDoc");
    }
    TEST_F(ControlNodeLayoutTests, TestGroupBoxWithEnableControl)
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

        EXPECT_EQ(VerticalElements(GroupBox(CheckBox("Eliminate edges shorter than", "Eliminate edges shorter than"), RadioButton("EliminateRelativeLength", "Relative length to mesh density:"), RadioButton("EliminateAbsoluteLength", "Absolute length:"))), *BuildTypeLayout<EliminateEdgeClass>());
    }

    TEST_F(ControlNodeLayoutTests, TestGroupBoxWithEnableControlConnectedToProperty)
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
        EXPECT_EQ(VerticalElements(
            GroupBox(
                CheckBox("eliminateShortEdges", "Eliminate edges shorter than"),
                RadioButton("EliminateRelativeLength", "Relative length to mesh density:"),
                RadioButton("EliminateAbsoluteLength", "Absolute length:")
            )), *BuildTypeLayout<EliminateEdgeClass>());
    }

    TEST_F(ControlNodeLayoutTests, TestCaption)
    {
        using namespace Classes;
        Class<EliminateEdgeClass> cls(typeLibrary, "EliminateEdgeClass");
        cls.SetGet("eliminateShortEdges", &EliminateEdgeClass::SetEliminateShortEdges, &EliminateEdgeClass::GetEliminateShortEdges)
            .AddAttribute<GroupAttribute>("Eliminate edges shorter than")
            .AddAttribute<LayoutAttribute>(VerticalElements(Caption("Hello"), CheckBox()))
            ;

        EXPECT_EQ(VerticalElements(
            GroupBox("Eliminate edges shorter than", Caption("Hello"), CheckBox("eliminateShortEdges", ""))
        ), *BuildTypeLayout<EliminateEdgeClass>());
    }

    TEST_F(ControlNodeLayoutTests, TestEmptyLayout)
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

        EXPECT_EQ(VerticalElements(
            GroupBox("Eliminate edges shorter than", RadioButton("EliminateRelativeLength", "Relative length to mesh density:"))
        ), *BuildTypeLayout<EliminateEdgeClass>());
    }

    TEST_F(ControlNodeLayoutTests, TestNestedMemberLayout)
    {
        using namespace Classes;
        Reflect<Vector3d>(typeLibrary);
        Class<ForceMoment> cls(typeLibrary, "ForceMoment");
        cls.Constructor<const Vector3d&, const Vector3d&>().AddSignature("force", "moment");
        cls.SetGet("force", &ForceMoment::SetForce, &ForceMoment::GetForce)
            .AddAttribute<LayoutAttribute>(EmbeddedElement(VerticalElements(TextBox("X", "Fx"), TextBox("Y", "Fy"), TextBox("Z", "Fz"))));
        cls.SetGet("moment", &ForceMoment::SetMoment, &ForceMoment::GetMoment)
            .AddAttribute<LayoutAttribute>(EmbeddedElement(VerticalElements(TextBox("X", "Mx"), TextBox("Y", "My"), TextBox("Z", "Mz"))));



        EXPECT_EQ(
            VerticalElements(
                TextBox("X", "Fx"), TextBox("Y", "Fy"), TextBox("Z", "Fz"),
                TextBox("X", "Mx"), TextBox("Y", "My"), TextBox("Z", "Mz")),
            *BuildTypeLayout<ForceMoment>());

        auto layout = BuildTypeLayout<ForceMoment>();
        auto verticalElements = std::dynamic_pointer_cast<VerticalElements>(layout);
        auto forceX = std::dynamic_pointer_cast<TextBox>(*verticalElements->begin());
        EXPECT_NE(forceX->GetControlNode(), nullptr);
    }

    TEST_F(ControlNodeLayoutTests, TestCreateLayoutFromNamedMemberMethods)
    {
        using namespace Classes;
        Reflect<Plate>(typeLibrary);
        auto controlNode = std::make_shared<Controls::ControlNode>(typeLibrary, Types::TypeId<Plate>(), FunctionSelectorAttribute(RadioButtons(), "Divide", "Explode"));
        auto layout = LayoutControl(controlNode).GetLayout();

        EXPECT_EQ(
            VerticalElements(
                TextBox("__this__", "Plate"),
                OverloadSelector(
                    "Functions",
                    VerticalElements(
                        Selector(RadioButtons("__SELECTOR__", ""), false),
                        DynamicArea()
                    )
                )
            ),
            *layout
        );
    }

    TEST_F(ControlNodeLayoutTests, CreateViewModelWithFunctionGrid)
    {
        using namespace Classes;
        Reflect<BoundaryStiffnessMatrix>(typeLibrary);
        Utilities::TypedMemberUtilities utils(typeLibrary, Types::TypeId<BoundaryStiffnessMatrix*>());
        auto constructor = utils.GetConstructor<const TranslationalBoundary&, const TranslationalBoundary&, const TranslationalBoundary&, const RotationalBoundary&, const RotationalBoundary&, const RotationalBoundary&>();
        ASSERT_NE(constructor, nullptr);
        auto layout = BuildMemberLayout(constructor);
        EXPECT_EQ(*layout,
            VerticalElements(
                TextBox("__this__", "Name"),
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
        );
    }

    TEST_F(ControlNodeLayoutTests, CreateBoundaryConditionLayout_MakeGrid)
    {
        using namespace Classes;
        Reflect<BoundaryCondition>(typeLibrary);
        Utilities::TypedMemberUtilities utils(typeLibrary, Types::TypeId<BoundaryCondition*>());
        auto constructor = utils.GetConstructor<const TranslationalBoundary&, const TranslationalBoundary&, const TranslationalBoundary&, const RotationalBoundary&, const RotationalBoundary&, const RotationalBoundary&>();
        ASSERT_NE(constructor, nullptr);
        auto layout = BuildMemberLayout(constructor);
        EXPECT_EQ(*layout,
            VerticalElements(
                CheckBox("TranslationalSymmetry", "Let Dy and Dz equal Dx"),
                GridElements(3, 2,
                    Slider("Option", "Dx"), VerticalElements(Caption("Spring stiffness"), TextBox("Value", "")),
                    Slider("Option", "Dy"), TextBox("Value", ""),
                    Slider("Option", "Dz"), TextBox("Value", "")
                ),
                CheckBox("RotationalSymmetry", "Let Ry and Rz equal Rx"),
                GridElements(3, 2,
                    Slider("Option", "Rx"), VerticalElements(Caption("Spring stiffness"), TextBox("Value", "")),
                    Slider("Option", "Ry"), TextBox("Value", ""),
                    Slider("Option", "Rz"), TextBox("Value", "")
                )
            )
        );
    }

    TEST_F(ControlNodeLayoutTests, CreateFileExporterNode)
    {
        using namespace Classes;
        Reflect<FileExporter>(typeLibrary, false);
        Utilities::TypedMemberUtilities util(typeLibrary, Types::TypeId<FileExporter>());
        auto layout = BuildMemberLayout(util.GetMember("DoExport"));
        EXPECT_EQ(*layout, VerticalElements(CheckBox("journalled", "Journal export operation"), TextBox("Value", "Value")));
    }

    TEST_F(ControlNodeLayoutTests, CreateFileExporterNode_RequireJournalling)
    {
        using namespace Classes;
        Reflect<FileExporter>(typeLibrary, true);
        Utilities::TypedMemberUtilities util(typeLibrary, Types::TypeId<FileExporter>());
        auto layout = BuildMemberLayout(util.GetMember("DoExport"));
        EXPECT_EQ(*layout, VerticalElements(TextBox("Value", "Value")));
    }


    TEST_F(ControlNodeLayoutTests, DetectOverloadSelector_SameSelectorForTwoDynamicAreas)
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

        auto layout = BuildMemberLayout(member);
        EXPECT_EQ(*layout,
            VerticalElements(
                Selector(Buttons("__SELECTOR__", "A"), false),
                HorizontalElements(
                    OverloadSelector("A", VerticalElements(SelectorLookup("A"), DynamicArea())),
                    OverloadSelector("A2", VerticalElements(SelectorLookup("A"), DynamicArea()))
                )
            ));
    }

    TEST_F(ControlNodeLayoutTests, GridWithAutomatedColumn)
    {
        using namespace Classes;
        Reflect<MyRow>(typeLibrary);
        Class<MyGrid> cls(typeLibrary, "MyGrid");
        auto& prop = cls.SetGet("Rows", &MyGrid::SetRows, &MyGrid::GetRows)
            .AddAttribute<LayoutAttribute>(Grid());

        auto layout = BuildMemberLayout(&prop);
        auto myGrid = std::dynamic_pointer_cast<Grid>(layout);
        ASSERT_NE(myGrid, nullptr);
        EXPECT_EQ(Grid("Rows", "", HorizontalElements(TextBox("X", "X"), TextBox("Y", "Y"), TextBox("Layers", "Number of layers")), 6), *layout);
    }


    TEST_F(ControlNodeLayoutTests, DISABLED_TableWithPropertyColumn)
    {
        using namespace Classes;
        Class<B> cls(typeLibrary, "B");
        auto& prop = cls.Get("AllA", &B::GetAllA)
            .AddAttribute<HeaderAttribute>(DefaultColumn("A"), PropertyColumn("X"), PropertyColumn("I", "Index"));

        auto layout = BuildMemberLayout(&prop);
        EXPECT_EQ(Table("AllA", "AllA", Header(DefaultColumn("A"), PropertyColumn("X"), PropertyColumn("I", "Index")), 6), *layout);
    }





}}}}}
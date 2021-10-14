//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"
#include "Reflection/Classes/Class.h"
#include "Reflection/Enums/Enum.h"
#include "Reflection/TypeLibraries/TypeLibraryFactory.h"
#include "Reflection/Controls/ControlNode.h"
#include "Reflection/Controls/Aspects/PropertyAspect.h"
#include "Reflection/Controls/Aspects/FunctionArgumentAspect.h"
#include "Reflection/Controls/Aspects/ConstructorAspect.h"
#include "Reflection/Controls/Aspects/ConstructorSelectorAspect.h"
#include "Reflection/Controls/Aspects/TypeAspect.h"
#include "Reflection/Controls/Aspects/IsAliasAspect.h"
#include "Reflection/Controls/Aspects/AliasAspect.h"
#include "Reflection/Controls/Aspects/RootAspect.h"

#include "Reflection/Attributes/LayoutAttribute.h"
#include "Reflection/Attributes/IncludeInViewAttribute.h"
#include "Reflection/Attributes/RecordAttribute.h"
#include "Reflection/Attributes/ExpandMemberAttribute.h"

#include "Reflection/Contents/Content.h"

#include "Reflection/Utilities/MemberUtilities.h"

#include "Reflection/TypeConversions/BuiltInConversions.h"

#include "Vector3d.h"
#include "EnumOrValue.h"
#include "Services/ScopedServiceProvider.h"
#include "MockReflectionContentsService.h"
#include "BoundaryStiffnessMatrix.h"
#include "Reflection/Attributes/FunctionSelectorAttribute.h"
#include "Reflection/Layout/RadioButtons.h"
#include "Reflection/Controls/Aspects/FunctionSelectorAspect.h"
#include "Reflection/Attributes/UtilityClassAttribute.h"
#include "Reflection/Attributes/DeprecatedAttribute.h"
#include "Reflection/Attributes/ContextAttribute.h"
#include "Reflection/Attributes/SymmetryAttribute.h"
#include <Reflection/Containers/ReflectVector.h>
#include "Reflection/Containers/ReflectMap.h"
#include "Reflection/Containers/ReflectSet.h"
#include "Reflection/Containers/ReflectPair.h"
#include "Reflection/Controls/Aspects/ContainerAspect.h"
#include "LayoutTestClasses.h"
#include "Reflection/Attributes/BindToSelectionAttribute.h"
#include "Reflection/Attributes/BindingAttribute.h"
#include "Reflection/Controls/Aspects/HeaderAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls { namespace Tests {
    using namespace Attributes::Tests;
    class ControlNodeTestClass {
    public:
        ControlNodeTestClass(double value) : m_value(value) 
        {
            m_vector.resize(4);
        }
        ControlNodeTestClass(double value, int a) : m_value(value)
        {
            m_vector.resize(4);
        }
        enum class MyEnum {
            Manual,
            Red,
            Blue,
        };
        void SetValue(double value) { m_value = value; }
        double GetValue() const { return m_value; }

        using MyEnumOrValue = EnumOrValue<MyEnum, MyEnum::Manual>;
        MyEnumOrValue GetEnumOrValue() const { return m_enumOrValue; }
        void SetEnumOrValue(const MyEnumOrValue& value) { m_enumOrValue = value; }

        void SetScale(size_t i, size_t j, double scale) { m_vector[i + j * 2] = scale; }
        double GetScale(size_t i, size_t j) const { return m_vector.at(i + j * 2); }
    private:

        double m_value;
        MyEnumOrValue m_enumOrValue;
        std::vector<double> m_vector;
    };
    Types::DecoratedTypeInfo GetScaleType(size_t i, size_t j)
    {
        if (i == 0 && j == 0)
            return Types::TypeId<float>();
        return Types::TypeId<double>();
    }
    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, ControlNodeTestClass::MyEnum**)
    {
        using namespace Enums;
        Enum<ControlNodeTestClass::MyEnum> enm(typeLibrary, "MyEnum");
        enm.EnumValue("Red", ControlNodeTestClass::MyEnum::Red);
        enm.EnumValue("Blue", ControlNodeTestClass::MyEnum::Blue);
        enm.InternalEnumValue("Manual", ControlNodeTestClass::MyEnum::Manual);
    }
    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, ControlNodeTestClass**)
    {
        using namespace Classes;
        Reflect<ControlNodeTestClass::MyEnum>(typeLibrary);
        Reflect<ControlNodeTestClass::MyEnumOrValue>(typeLibrary, "MyEnumOrValue");
        Class<ControlNodeTestClass> cls(typeLibrary, "ControlNodeTestClass");
        cls.SetGet("Value", &ControlNodeTestClass::SetValue, &ControlNodeTestClass::GetValue);
        cls.SetGet("EnumOrValue", &ControlNodeTestClass::SetEnumOrValue, &ControlNodeTestClass::GetEnumOrValue)
            .AddAttribute<LayoutAttribute>(EmbeddedElement(HorizontalElements(Slider("option", "Enum1"), TextBox("value", ""))));
        cls.Constructor<double>()
            .AddSignature("value")
            .AddAttribute<IncludeInViewAttribute>("EnumOrValue");

        cls.Function("SetScale", &ControlNodeTestClass::SetScale)
            .AddSignature("i", "j", "scale")
            .AddAttribute<RecordAttribute>(
                "GetScale", //Define reflection method for extracting values from the matrix
                IndexRange(0, 1, "i"), //Fixed size number of rows
                IndexRange(0, 1, "j"), //Fixed size number of columns 
                RecordTypeCallback(GetScaleType,"i","j")
                );
        cls.Function("GetScale", &ControlNodeTestClass::GetScale).AddSignature("i", "j");
    }

    class ControlNodeTests : public ::testing::Test
    {
    public:
        void SetUp() override
        {
            typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
            provider.RegisterService(typeLibrary);
            provider.RegisterService<Contents::IReflectionContentsService>(std::make_shared<Contents::Tests::MockReflectionContentsService>(typeLibrary));
            Reflect<ControlNodeTestClass>(typeLibrary);
            controlNodeUtils = std::make_unique<Utilities::TypedMemberUtilities>(typeLibrary, Types::TypeId<ControlNodeTestClass>());
            using namespace Classes;
            Class<double>(typeLibrary, "double").Function("toString", [](double val) {return std::to_string(val); });
            Class<int>(typeLibrary, "int").Operator(This.Const == This.Const);
            TypeConversions::AddBuiltInConversions(typeLibrary->GetConversionGraph());
        }
    protected:
        Services::ScopedServiceProvider provider;
        TypeLibraries::TypeLibraryPointer typeLibrary;
        std::unique_ptr<Utilities::TypedMemberUtilities> controlNodeUtils;
    };
    TEST_F(ControlNodeTests, ControlNodeOfProperty_SetGet)
    {        
        ControlNode node(typeLibrary, controlNodeUtils->GetMember("Value"));        
        ControlNode baseline(typeLibrary);
        baseline.SetDecoratedTypeInfo(Types::TypeId<double>());
        baseline.SetName("Value");
        baseline.AddAspect<TypeAspect>(typeLibrary->LookupType(typeid(double)));
        baseline.AddAspect<PropertyAspect>(controlNodeUtils->GetMember("Value"));
        baseline.AddAspect<RootAspect>();

        EXPECT_EQ(node, baseline);
    }

    TEST_F(ControlNodeTests, ControlNodeOfConstructor)
    {
        Reflect<Vector3d>(typeLibrary);

        Utilities::TypedMemberUtilities utils(typeLibrary, Types::TypeId<Vector3d>());
        ControlNode node(typeLibrary, utils.GetConstructor<double,double,double>());
        
        ASSERT_EQ(node.GetChildren().size(),3);
        ControlNode baseline(typeLibrary);
        baseline.SetDecoratedTypeInfo(Types::TypeId<double>());
        baseline.SetName("X");
        baseline.AddAspect<TypeAspect>(typeLibrary->LookupType(typeid(double)));
        baseline.AddAspect<PropertyAspect>(utils.GetMember("X"));
        baseline.AddAspect<FunctionArgumentAspect>(utils.GetConstructor<double, double, double>(), 0);
        EXPECT_EQ(*node.GetChildren()[0], baseline);
        ASSERT_NE(node.TryGetAspect<ConstructorAspect>(), nullptr);
        EXPECT_EQ(*node.TryGetAspect<ConstructorAspect>(), ConstructorAspect(utils.GetConstructor<double, double, double>()));
    }

    TEST_F(ControlNodeTests, ContolNodeWithLayout_NestedProperties)
    {
        ControlNode node(typeLibrary, controlNodeUtils->GetMember("EnumOrValue"));
        EXPECT_EQ(node.GetName(), "EnumOrValue");
        EXPECT_EQ(node.GetDecoratedTypeInfo(), Types::TypeId<ControlNodeTestClass::MyEnumOrValue>());
        EXPECT_EQ(node.GetAspect<PropertyAspect>(), PropertyAspect(controlNodeUtils->GetMember("EnumOrValue")));
        EXPECT_NE(node.TryGetAspect<ConstructorAspect>(), nullptr);
        ASSERT_TRUE(node.HasChild("value"));
        ASSERT_TRUE(node.HasChild("option"));
        EXPECT_NE(node.GetChild("value")->TryGetAspect<FunctionArgumentAspect>(), nullptr);
        EXPECT_NE(node.GetChild("value")->TryGetAspect<PropertyAspect>(), nullptr);
    }

    TEST_F(ControlNodeTests, Constructor_IncludeInViewAttribute)
    {
        ControlNode node(typeLibrary, controlNodeUtils->GetConstructor<double>());
        EXPECT_EQ(node.GetChildren().size(), 2);
        EXPECT_NE(node.GetChild("EnumOrValue")->TryGetAspect<ConstructorAspect>(), nullptr);
        EXPECT_EQ(node.GetChild("EnumOrValue")->GetChildren().size(), 2);
    }

    TEST_F(ControlNodeTests, Constructor_SimpleRecordAttribute)
    {
        ControlNode node(typeLibrary, controlNodeUtils->GetMember("SetScale"));
        EXPECT_EQ(node.GetChildren().size(), 4);        
        ASSERT_NE(node.GetChild("0,1"), nullptr);
        auto leaf = node.GetChild("0,1");
        EXPECT_EQ(leaf->GetDecoratedTypeInfo(), Types::TypeId<double>());
    }

    TEST_F(ControlNodeTests, Constructor_SimpleRecordAttribute_CustomReturnType)
    {
        ControlNode node(typeLibrary, controlNodeUtils->GetMember("SetScale"));
        EXPECT_EQ(node.GetChildren().size(), 4);
        ASSERT_NE(node.GetChild("0,0"), nullptr);
        auto leaf = node.GetChild("0,0");
        EXPECT_EQ(leaf->GetDecoratedTypeInfo(), Types::TypeId<float>());
    }

    TEST_F(ControlNodeTests, Constructor_RecordAttribute_NestedAlias)
    {
        Reflect<BoundaryStiffnessMatrix>(typeLibrary);
        ControlNode node(typeLibrary, Types::TypeId<BoundaryStiffnessMatrix>());
        ASSERT_NE(node.GetChild("Dx"), nullptr);
        ASSERT_NE(node.GetChild("Dx")->GetChild("value"), nullptr);
        ASSERT_NE(node.GetChild("Dx")->TryGetAspect<ConstructorAspect>(), nullptr);
        ASSERT_NE(node.GetChild("Dx")->TryGetAspect<IsAliasAspect>(), nullptr);
    }

    TEST_F(ControlNodeTests, Constructor_RecordAttribute_GetStiffness_CheckAlias)
    {
        Reflect<BoundaryStiffnessMatrix>(typeLibrary);
        ControlNode node(typeLibrary, Types::TypeId<BoundaryStiffnessMatrix>());
        ASSERT_NE(node.GetChild("setStiffness"), nullptr);
        ASSERT_NE(node.GetChild("setStiffness")->GetChild("Dx.value"), nullptr);
        EXPECT_EQ(node.GetChild("setStiffness")->GetChild("Dx.value")->GetAspect<AliasAspect>().GetAliasNode(), node.GetChild("Dx")->GetChild("value"));
        ASSERT_NE(node.GetChild("setStiffness")->GetChild("Dx.option"), nullptr);
        EXPECT_EQ(node.GetChild("setStiffness")->GetChild("Dx.option")->GetAspect<AliasAspect>().GetAliasNode(), node.GetChild("Dx")->GetChild("option"));
        EXPECT_EQ(node.GetChild("setStiffness")->GetChild("1,2")->TryGetAspect<AliasAspect>(), nullptr);
    }

    TEST_F(ControlNodeTests, VectorNode_TryGetXFromVector)
    {
        Reflect<Vector3d>(typeLibrary);
        Utilities::TypedMemberUtilities utils(typeLibrary, Types::TypeId<Vector3d>());
        ControlNode node(typeLibrary, utils.GetConstructor<double, double, double>());
        Contents::Content content(typeLibrary, Vector3d(3, 5, 7));
        Contents::Content child = node.GetChild("X")->TryGetPropertyContent(content);
        EXPECT_EQ(child.GetScriptText(), "3.000000");
        EXPECT_DOUBLE_EQ(child.As<double>(), 3);
    }

    TEST_F(ControlNodeTests, InitializeConstructorArgumentWithoutProperty_SetNameCorrectly)
    {
        using namespace Classes;
        Class<ControlNodeTestClass> cls(typeLibrary, "ControlNodeTestClass");
        cls.Constructor<double, int>()
            .AddSignature("value", "a");

        ControlNode node(typeLibrary, controlNodeUtils->GetConstructor<double,int>());
        EXPECT_NE(node.GetChild("a"), nullptr);
    }

    TEST_F(ControlNodeTests, ControlNodeOfTypeWithSingleConstructor_PopulateWithConstructor)
    {
        ControlNode node(typeLibrary, Types::TypeId<ControlNodeTestClass>());
        EXPECT_EQ(node.GetChildren().size(), 2);
        EXPECT_EQ(node.GetAspect<ConstructorAspect>(), ConstructorAspect(controlNodeUtils->GetConstructor<double>()));        
    }

    class TestBaseClass
    {
    public:
        virtual ~TestBaseClass() {}
    };

    class TestDerivedClassA : public TestBaseClass
    {
    public:
        TestDerivedClassA(double a) : m_a(a) {}
        double GetA() const { return m_a; }
        void SetA(double a) { m_a = a; }
    private:
        double m_a;
    };

    class TestDerivedClassB : public TestBaseClass
    {
    public:
        TestDerivedClassB(double b) : m_b(b) {}
        double GetB() const { return m_b; }
        void SetB(double b) { m_b = b; }
    private:
        double m_b;
    };
    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, TestBaseClass**)
    {
        using namespace Classes;
        Class<TestBaseClass> cls(typeLibrary, "TestBaseClass");
    }

    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, TestDerivedClassA**)
    {
        using namespace Classes;
        Class<TestDerivedClassA, Public<TestBaseClass>> cls(typeLibrary, "TestDerivedClassA");
        cls.Constructor<double>().AddSignature("a");
        cls.SetGet("A", &TestDerivedClassA::SetA, &TestDerivedClassA::GetA);
    }

    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, TestDerivedClassB**)
    {
        using namespace Classes;
        Class<TestDerivedClassB, Public<TestBaseClass>> cls(typeLibrary, "TestDerivedClassB");
        cls.Constructor<double>().AddSignature("b");
        cls.SetGet("B", &TestDerivedClassB::SetB, &TestDerivedClassB::GetB);
    }

    TEST_F(ControlNodeTests, TestControlNodeOfExpandableType)
    {
        using namespace Classes;
        Class<TestBaseClass> cls(typeLibrary, "TestBaseClass");
        cls.AddAttribute<ExpandMemberAttribute>(ExpandOptions::CanExpand);
        Reflect<TestDerivedClassA>(typeLibrary);
        Reflect<TestDerivedClassB>(typeLibrary);
        ControlNode node(typeLibrary, Types::TypeId<TestBaseClass*>());
        ASSERT_EQ(node.GetChildren().size(), 1);
        EXPECT_EQ(node.GetChildren().front()->GetAspect<ConstructorSelectorAspect>().GetConstructors().size(), 2);
    }

    TEST_F(ControlNodeTests, TestControlNodeOfLayoutWithOverloadManagement)
    {
        using namespace Classes;
        Class<TestBaseClass> cls(typeLibrary, "TestBaseClass");
        cls.AddAttribute<LayoutAttribute>(OverloadSelector(VerticalElements(Selector(RadioButtons()), DynamicArea())));
        Reflect<TestDerivedClassA>(typeLibrary);
        Reflect<TestDerivedClassB>(typeLibrary);
        ControlNode node(typeLibrary, Types::TypeId<TestBaseClass*>());
        ASSERT_EQ(node.GetChildren().size(), 1);
        EXPECT_EQ(node.GetChildren().front()->GetAspect<ConstructorSelectorAspect>().GetConstructors().size(), 2);
    }

    class ClassWithFunctionOverrides
    {
    public:
        void Explode() {}
        bool SplitAt(double param) { return false; }
        void Divide(double a, double b) {}
        void SetDummy(bool) {}
        bool GetDummy() const { return true; }
    private:
    };

    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, ClassWithFunctionOverrides**)
    {
        using namespace Classes;
        Class<ClassWithFunctionOverrides> cls(typeLibrary, "ClassWithFunctionOverrides");
        cls.Constructor();
        cls.Function("Explode", &ClassWithFunctionOverrides::Explode);
        cls.Function("SplitAt", &ClassWithFunctionOverrides::SplitAt).AddSignature("param").AddAttribute<ContextAttribute>("TestContext");
        cls.Function("Divide", &ClassWithFunctionOverrides::Divide).AddSignature("a", "b");
    }

    TEST_F(ControlNodeTests, TestFunction)
    {
        Reflect<ClassWithFunctionOverrides>(typeLibrary);
        Utilities::TypedMemberUtilities util(typeLibrary, Types::TypeId<ClassWithFunctionOverrides>());
        ControlNode node(typeLibrary, util.GetMember("Divide"));
        ASSERT_EQ(node.GetChildren().size(), 2);
        ASSERT_NE(node.GetChild("a"), nullptr);
        ASSERT_NE(node.GetChild("a"), nullptr);
    }

    TEST_F(ControlNodeTests, TestFunctionOnExpandableType)
    {
        Reflect<ClassWithFunctionOverrides>(typeLibrary);
        using namespace Classes;
        Class<ClassWithFunctionOverrides> cls(typeLibrary, "ClassWithFunctionOverrides");
        cls.AddAttribute<ExpandMemberAttribute>(ExpandOptions::CanExpand);
        Utilities::TypedMemberUtilities util(typeLibrary, Types::TypeId<ClassWithFunctionOverrides>());
        ControlNode node(typeLibrary, util.GetMember("Divide"));
        ASSERT_EQ(node.GetChildren().size(), 2);
        ASSERT_NE(node.GetChild("a"), nullptr);
        ASSERT_NE(node.GetChild("a"), nullptr);
    }

    TEST_F(ControlNodeTests, TestFunctionSelector)
    {
        Reflect<ClassWithFunctionOverrides>(typeLibrary);
        ControlNode node(typeLibrary, Types::TypeId<ClassWithFunctionOverrides>(), Attributes::FunctionSelectorAttribute(Layout::RadioButtons(), "Explode", "SplitAt", "Divide"));
        ASSERT_EQ(node.GetChildren().size(), 1);
        EXPECT_EQ(node.TryGetAspect<ConstructorAspect>(), nullptr);
        EXPECT_EQ(node.GetChildren().front()->GetAspect<FunctionSelectorAspect>().GetFunctions().size(), 3);
    }

    TEST_F(ControlNodeTests, TestFunctionSelectorOnExpandableType)
    {
        Reflect<ClassWithFunctionOverrides>(typeLibrary);
        using namespace Classes;
        Class<ClassWithFunctionOverrides> cls(typeLibrary, "ClassWithFunctionOverrides");
        cls.AddAttribute<ExpandMemberAttribute>(ExpandOptions::CanExpand);
        ControlNode node(typeLibrary, Types::TypeId<ClassWithFunctionOverrides>(), Attributes::FunctionSelectorAttribute(Layout::RadioButtons(), "Explode", "SplitAt", "Divide"));
        ASSERT_EQ(node.GetChildren().size(), 1);
        EXPECT_EQ(node.TryGetAspect<ConstructorAspect>(), nullptr);
        EXPECT_EQ(node.TryGetAspect<ConstructorSelectorAspect>(), nullptr);
        EXPECT_EQ(node.GetChildren().front()->GetAspect<FunctionSelectorAspect>().GetFunctions().size(), 3);
    }

    TEST_F(ControlNodeTests, TestFunctionSelector_UtilityClassAttribute)
    {
        Reflect<ClassWithFunctionOverrides>(typeLibrary);
        using namespace Classes;
        Class<ClassWithFunctionOverrides> cls(typeLibrary, "ClassWithFunctionOverrides");
        cls.AddAttribute<ExpandMemberAttribute>(ExpandOptions::CanExpand);
        ControlNode node(typeLibrary, Types::TypeId<ClassWithFunctionOverrides>(), Attributes::FunctionSelectorAttribute(Layout::RadioButtons(), "Explode", "SplitAt", "Divide"));
        ASSERT_EQ(node.GetChildren().size(), 1);
    }

    TEST_F(ControlNodeTests, TestFunction_UtilityClassAttribute)
    {
        Reflect<ClassWithFunctionOverrides>(typeLibrary);
        using namespace Classes;
        Class<ClassWithFunctionOverrides> cls(typeLibrary, "ClassWithFunctionOverrides");
        cls.SetGet("Dummy", &ClassWithFunctionOverrides::SetDummy, &ClassWithFunctionOverrides::GetDummy);
        cls.AddAttribute<UtilityClassAttribute>();
        Utilities::TypedMemberUtilities util(typeLibrary, Types::TypeId<ClassWithFunctionOverrides>());
        ControlNode node(typeLibrary, util.GetMember("Divide"));
        EXPECT_EQ(node.GetChildren().size(), 3);
        ASSERT_NE(node.GetChild("a"), nullptr);
        ASSERT_NE(node.GetChild("b"), nullptr);
        ASSERT_NE(node.GetChild("Dummy"), nullptr);
    }

    TEST_F(ControlNodeTests, TestFunction_DeprecatedAttribute)
    {
        Reflect<ClassWithFunctionOverrides>(typeLibrary);
        using namespace Classes;
        Class<ClassWithFunctionOverrides> cls(typeLibrary, "ClassWithFunctionOverrides");
        cls.SetGet("Dummy", &ClassWithFunctionOverrides::SetDummy, &ClassWithFunctionOverrides::GetDummy).AddAttribute<DeprecatedAttribute>();
        cls.AddAttribute<UtilityClassAttribute>();
        Utilities::TypedMemberUtilities util(typeLibrary, Types::TypeId<ClassWithFunctionOverrides>());
        ControlNode node(typeLibrary, util.GetMember("Divide"));
        EXPECT_EQ(node.GetChildren().size(), 2);
        ASSERT_NE(node.GetChild("a"), nullptr);
        ASSERT_NE(node.GetChild("b"), nullptr);
        ASSERT_EQ(node.GetChild("Dummy"), nullptr);
    }

    TEST_F(ControlNodeTests, TestFunction_ContextAttribute_NotInContext)
    {
        Reflect<ClassWithFunctionOverrides>(typeLibrary);
        using namespace Classes;
        Class<ClassWithFunctionOverrides> cls(typeLibrary, "ClassWithFunctionOverrides");
        cls.SetGet("Dummy", &ClassWithFunctionOverrides::SetDummy, &ClassWithFunctionOverrides::GetDummy);
        cls.AddAttribute<UtilityClassAttribute>();
        Utilities::TypedMemberUtilities util(typeLibrary, Types::TypeId<ClassWithFunctionOverrides>());
        ControlNode node(typeLibrary, util.GetMember("SplitAt"));
        EXPECT_EQ(node.GetChildren().size(), 1);
        ASSERT_NE(node.GetChild("param"), nullptr);
    }

    TEST_F(ControlNodeTests, TestFunction_ContextAttribute_InContext)
    {
        Reflect<ClassWithFunctionOverrides>(typeLibrary);
        using namespace Classes;
        Class<ClassWithFunctionOverrides> cls(typeLibrary, "ClassWithFunctionOverrides");
        cls.SetGet("Dummy", &ClassWithFunctionOverrides::SetDummy, &ClassWithFunctionOverrides::GetDummy).AddAttribute<ContextAttribute>("TestContext");
        cls.AddAttribute<UtilityClassAttribute>();
        Utilities::TypedMemberUtilities util(typeLibrary, Types::TypeId<ClassWithFunctionOverrides>());
        ControlNode node(typeLibrary, util.GetMember("SplitAt"));
        EXPECT_EQ(node.GetChildren().size(), 2);
        ASSERT_NE(node.GetChild("param"), nullptr);
        ASSERT_NE(node.GetChild("dummy"), nullptr);
    }

    TEST_F(ControlNodeTests, TestFunction_ContextAttribute_InDifferentContext)
    {
        Reflect<ClassWithFunctionOverrides>(typeLibrary);
        using namespace Classes;
        Class<ClassWithFunctionOverrides> cls(typeLibrary, "ClassWithFunctionOverrides");
        cls.SetGet("Dummy", &ClassWithFunctionOverrides::SetDummy, &ClassWithFunctionOverrides::GetDummy).AddAttribute<ContextAttribute>("X");
        cls.AddAttribute<UtilityClassAttribute>();
        Utilities::TypedMemberUtilities util(typeLibrary, Types::TypeId<ClassWithFunctionOverrides>());
        ControlNode node(typeLibrary, util.GetMember("SplitAt"));
        EXPECT_EQ(node.GetChildren().size(), 1);
        ASSERT_NE(node.GetChild("param"), nullptr);
    }

    class ClassWithSymmetry {
    public:
        double GetA() { return 0; }
        void SetA(double) {}
        double GetB() { return 0; }
        void SetB(double) {}
    };

    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, ClassWithSymmetry**)
    {
        using namespace Classes;
        Class<ClassWithSymmetry> cls(typeLibrary, "ClassWithSymmetry");
        cls.AddAttribute<UtilityClassAttribute>();
        cls.SetGet("A", &ClassWithSymmetry::SetA, &ClassWithSymmetry::GetA);
    }


    TEST_F(ControlNodeTests, TestSymmetry_SymmetryAttribute)
    {
        Reflect<ClassWithSymmetry>(typeLibrary);
        using namespace Classes;
        Class<ClassWithSymmetry> cls(typeLibrary, "ClassWithSymmetry");
        cls.SetGet("B", &ClassWithSymmetry::SetB, &ClassWithSymmetry::GetB)
            .AddAttribute<SymmetryAttribute>("A", "Syncronize Values", "Please syncronize values")
            ;

        ControlNode node(typeLibrary, Types::TypeId<ClassWithSymmetry>());
        ASSERT_EQ(node.GetChildren().size(), 2);
        EXPECT_NE(node.GetChild("B")->GetChild("Syncronize Values"), nullptr);       
    }

    class RowClass {
    public:
        RowClass(double a, double b) : m_a(a) , m_b(b) {}
        double GetA() const { return m_a; }
        void SetA(double val) { m_a = val; }
        double GetB() const { return m_b; }
        void SetB(double val) { m_b = val; }
        bool operator ==(const RowClass& other) const { return m_a == other.m_a && m_b == other.m_b; }
    private:
        double m_a;
        double m_b;
    };
    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, RowClass**)
    {
        using namespace Classes;
        Class<RowClass> cls(typeLibrary, "RowClass");
        cls.Constructor<double, double>().AddSignature("a", "b");
        cls.Get("A", &RowClass::GetA);
        cls.SetGet("B", &RowClass::SetB, &RowClass::GetB);
    }

    class ClassWithVectorProperty {
    public:
        std::vector<RowClass> GetVec() const { return m_vec; }
        void SetVec(const std::vector<RowClass>& vec) { m_vec = vec; }
    private:
        std::vector<RowClass> m_vec;
    };

    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, ClassWithVectorProperty**)
    {
        using namespace Classes;
        Reflect<RowClass>(typeLibrary);
        Class<ClassWithVectorProperty> cls(typeLibrary, "ClassWithSymmetry");
        cls.AddAttribute<UtilityClassAttribute>();
        cls.SetGet("Vec", &ClassWithVectorProperty::SetVec, &ClassWithVectorProperty::GetVec).AddAttribute<LayoutAttribute>(Grid());
    }
    class Array {
    public:
        Array(const std::vector<Objects::Object>& args) {}
    };
    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, Array**)
    {
        using namespace Classes;
        Class<Array> cls(typeLibrary, "Array");
        cls.Constructor<const std::vector<Objects::Object>&>(Vararg);
    }

    TEST_F(ControlNodeTests, TestContrainerAspect)
    {
        Reflect<ClassWithVectorProperty>(typeLibrary);
        Reflect<Array>(typeLibrary);
        ControlNode node(typeLibrary, Types::TypeId<ClassWithVectorProperty>());
        ASSERT_EQ(node.GetChildren().size(), 1);
        auto vec = node.GetChild("Vec");
        ASSERT_NE(vec, nullptr);
        EXPECT_NE(vec->TryGetAspect<ContainerAspect>(), nullptr);
        std::vector<RowClass> rows{RowClass(1, 2), RowClass(5, 6)};
        auto result = vec->GetAspect<ContainerAspect>().Split(Contents::Content(typeLibrary, rows));
        ASSERT_EQ(result.size(), 2);
        ASSERT_TRUE(result.at(0).IsConvertibleTo<RowClass>());
        EXPECT_EQ(result.at(0).GetScriptText(), "RowClass(1.000000, 2.000000)");
        auto joined = vec->GetAspect<ContainerAspect>().Join(typeLibrary, result);
        EXPECT_EQ(joined.As<std::vector<RowClass>>(), rows);
        EXPECT_EQ(joined.GetScriptText(), "Array(RowClass(1.000000, 2.000000), RowClass(5.000000, 6.000000))");
        result.push_back(Contents::Content("RowClass(9,1.4)"));
        joined = vec->GetAspect<ContainerAspect>().Join(typeLibrary, result, false);
        EXPECT_EQ(joined.GetScriptText(), "Array(RowClass(1.000000, 2.000000), RowClass(5.000000, 6.000000), RowClass(9,1.4))");
        auto split = vec->GetAspect<ContainerAspect>().Split(joined);
        ASSERT_EQ(split.size(), 3);
        EXPECT_EQ(split.at(2).GetScriptText(), "RowClass(9,1.4)");
    }

    TEST_F(ControlNodeTests, EmbedMember)
    {
        using namespace Classes;
        Reflect<TypeToEmbed>(typeLibrary);
        Class<EmbedderType> cls(typeLibrary, "EmbedderType");
        auto member = &cls.SetGet("Point", &EmbedderType::SetPoint, &EmbedderType::GetPoint).AddAttribute<LayoutAttribute>(GroupBox("Y", EmbeddedElement()));
        ControlNode node(typeLibrary, member);
        EXPECT_EQ(node.GetChildren().size(), 2);
    }

    TEST_F(ControlNodeTests, EmbedMember_SpecifyEmbeddedLayout)
    {
        using namespace Classes;
        Reflect<TypeToEmbed>(typeLibrary);
        Class<EmbedderType> cls(typeLibrary, "EmbedderType");
        auto member = &cls.SetGet("Point", &EmbedderType::SetPoint, &EmbedderType::GetPoint).AddAttribute<LayoutAttribute>(GroupBox("Y", EmbeddedElement(VerticalElements(TextBox("Y"), TextBox("X", "MyX")))));
        ControlNode node(typeLibrary, member);
        ASSERT_EQ(node.GetChildren().size(), 2);
        EXPECT_EQ(node.GetChildren().at(0)->GetName(), "Y");
        EXPECT_EQ(node.GetChildren().at(1)->GetName(), "X");
    }

    TEST_F(ControlNodeTests, DetectOverloadSelector_Custom)
    {
        using namespace Classes;
        Reflect<A>(typeLibrary);

        Class<B> cls(typeLibrary, "B");
        Members::MemberPointer member = &cls.SetGet("A", &B::SetA, &B::GetA)
            .AddAttribute<LayoutAttribute>(OverloadSelector(HorizontalElements(Selector(RadioButtons("Curve")), DynamicArea())))
            .AddAttribute<CaptionAttribute>("My A");


        ControlNode node(typeLibrary, member);
        auto child = node.GetChild("__SELECTOR__");
        ASSERT_NE(child, nullptr);
        EXPECT_EQ(child->GetCaption(), "Curve");
    }

    TEST_F(ControlNodeTests, DetectOverloadSelector_DropdownButton_AddExpand)
    {
        using namespace Classes;
        Reflect<A>(typeLibrary);

        Class<B> cls(typeLibrary, "B");
        Members::MemberPointer member = &cls.SetGet("A", &B::SetA, &B::GetA)
            .AddAttribute<LayoutAttribute>(OverloadSelector(HorizontalElements(Selector(DropdownButton("Curve")), Collapsable(DynamicArea()))))
            .AddAttribute<CaptionAttribute>("My A");


        ControlNode node(typeLibrary, member);
        auto child = node.GetChild("__SELECTOR__");
        ASSERT_NE(child, nullptr);
        EXPECT_EQ(child->GetCaption(), "Curve");
        EXPECT_NE(child->GetChild("Expanded"), nullptr);
    }

    TEST_F(ControlNodeTests, CreateFileExporterNode)
    {
        using namespace Classes;
        Reflect<FileExporter>(typeLibrary, false);
        Utilities::TypedMemberUtilities util(typeLibrary, Types::TypeId<FileExporter>());
        ControlNode node(typeLibrary, util.GetMember("DoExport"));
        EXPECT_NE(node.GetChild("fileName"), nullptr);
        EXPECT_NE(node.GetChild("extension"), nullptr);
        EXPECT_NE(node.GetChild("value"), nullptr);
        EXPECT_NE(node.GetChild("journalled"), nullptr);
        EXPECT_NE(node.GetChild("InitialDir"), nullptr);
    }

    TEST_F(ControlNodeTests, CreateFileExporterNode_RequireJournalling)
    {
        using namespace Classes;
        Reflect<FileExporter>(typeLibrary, true);
        Utilities::TypedMemberUtilities util(typeLibrary, Types::TypeId<FileExporter>());
        ControlNode node(typeLibrary, util.GetMember("DoExport"));
        EXPECT_NE(node.GetChild("fileName"), nullptr);
        EXPECT_NE(node.GetChild("extension"), nullptr);
        EXPECT_NE(node.GetChild("value"), nullptr);
        EXPECT_EQ(node.GetChild("journalled"), nullptr);
        EXPECT_NE(node.GetChild("InitialDir"), nullptr);
    }
	class ConnectivityManager
	{
	public:
		void Connect(int a, int b) {}
		void Disconnect(int a, int b) {}
		bool IsConnected(int a, int b) const { return true; }
		bool IsTopologicallyConnected(int a, int b) const { return true; }
	};
	
	void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, ConnectivityManager**)
	{
		using namespace Classes;
		Class<ConnectivityManager> cls(typeLibrary, "ConnectivityManager");
		cls.Function("Connect", &ConnectivityManager::Connect).AddSignature("a", "b");
		cls.Function("Disconnect", &ConnectivityManager::Disconnect).AddSignature("a", "b");
		cls.Function("IsConnected", &ConnectivityManager::IsConnected).AddSignature("a", "b");
		cls.Function("IsTopologicallyConnected", &ConnectivityManager::IsTopologicallyConnected).AddSignature("a", "b");
		cls.AddAttribute<LayoutAttribute>(VerticalElements(
			Auto<int>("a").AddAttribute<BindToSelectionAttribute>(),
			Grid("ConnectionGrid", "", HorizontalElements("b","IsConnected"))
				.AddAttribute<BindingAttribute>(typeLibrary, [](int a) 
				{
					std::map<int, std::set<bool>> connected;
					return connected;
				}, "a")
		));
	}
	TEST_F(ControlNodeTests, ConnectivityManager_TestGridNodeHasTypeFromBinding)
	{
		Reflect<ConnectivityManager>(typeLibrary);
		ControlNode node(typeLibrary, Types::TypeId<ConnectivityManager>());
		EXPECT_EQ(node.GetChildren().size(), 2);
		EXPECT_EQ(node.GetChild("ConnectionGrid")->GetDecoratedTypeInfo(), (Reflection::Types::TypeId<std::map<int, std::set<bool>>>()));
		const HeaderAspect& aspect = node.GetChild("ConnectionGrid")->GetAspect<HeaderAspect>();
		EXPECT_EQ(aspect.GetHeader()->size(), 2);
		auto item0 = std::dynamic_pointer_cast<Layout::ControlElement>(*aspect.GetHeader()->begin());
		auto item1 = std::dynamic_pointer_cast<Layout::ControlElement>(*std::next(aspect.GetHeader()->begin()));
		ASSERT_NE(item0->GetControlNode(), nullptr);
		ASSERT_NE(item1->GetControlNode(), nullptr);
		EXPECT_EQ(item0->GetControlNode()->GetName(), "b");
		EXPECT_EQ(item1->GetControlNode()->GetName(), "IsConnected");
		EXPECT_EQ(item0->GetControlNode()->GetDecoratedTypeInfo(), Types::TypeId<int>());
		EXPECT_EQ(item1->GetControlNode()->GetDecoratedTypeInfo(), Types::TypeId<bool>());
	}

	TEST_F(ControlNodeTests, ConnectivityManager_TestTupleHasChildElements)
	{
		Reflect<ConnectivityManager>(typeLibrary);
		ControlNode node(typeLibrary, Types::TypeId<std::pair<int, std::set<bool>>>());
		EXPECT_EQ(node.GetChildren().size(), 2);
		EXPECT_EQ(node.GetChild("0")->GetDecoratedTypeInfo(), Types::TypeId<int>());
		EXPECT_EQ(node.GetChild("1")->GetDecoratedTypeInfo(), Types::TypeId<std::set<bool>>());
	}
}}}}}
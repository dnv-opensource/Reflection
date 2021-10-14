//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"
#include "Reflection/Objects/Object.h"
#include "Reflection/Variants/ConversionError.h"
#include "Reflection/TypeLibraries/TypeLibraryFactory.h"
#include "Reflection/TypeConversions/BuiltInConversions.h"
#include "TestModels.h"
#include "Formatting/DefaultFormatterRules.h"
#include "Reflection/Utilities/DefaultRegistration.h"
#include "Reflection/Attributes/DefaultConstructorSelectorAttribute.h"
#include "Reflection/Attributes/GenerateConstructorArgumentsAttribute.h"
#include "Reflection/Attributes/ListAsConstructorAttribute.h"
#include "Reflection/Attributes/ObjectNameAttribute.h"
#include "Reflection/Attributes/AlternativeNullPointerNameAttribute.h"
#include "Reflection/Attributes/ShortSyntaxAttribute.h"
#include "Vector3d.h"
#include "FakeExpressionEvaluator.h"
#include "Reflection/Attributes/ExpressionExpanderAttribute.h"
#include "Reflection/ReflectionCast.h"
#include "Reflection/ReflectionTraits.h"


namespace DNVS {namespace MoFa {namespace Reflection {namespace Objects { namespace Test {
    TEST(ObjectTests, CreateEmptyObject_ConversionInvalid)
    {
        EXPECT_FALSE(Object().ConvertTo(typeid(int)).IsValid());
        EXPECT_FALSE(Object().ConvertTo<int>().IsValid());
        EXPECT_FALSE(Object().IsConvertibleTo(typeid(int)));
        EXPECT_FALSE(Object().IsConvertibleTo<int>());
    }
    TEST(ObjectTests, CreateObjectWithoutTypeLibrary_ConversionToSameTypeWorks)
    {
        EXPECT_TRUE(Object(nullptr, 1).ConvertTo(Types::TypeId<int>()).IsValid());
        EXPECT_TRUE(Object(nullptr, 1).ConvertTo<int>().IsValid());
        EXPECT_EQ(42, Object(nullptr, 42).As<int>());
        EXPECT_TRUE(Object(nullptr, 42).IsConvertibleTo(typeid(int)));
        EXPECT_TRUE(Object(nullptr, 42).IsConvertibleTo<int>());
    }

    TEST(ObjectTests, CreateObjectWithoutTypeLibrary_ConversionToDifferentTypeInvalid)
    {
        EXPECT_FALSE(Object(nullptr, 1).ConvertTo(Types::TypeId<short>()).IsValid());
        EXPECT_FALSE(Object(nullptr, 1).ConvertTo<short>().IsValid());
        EXPECT_THROW(Object(nullptr, 1).As<short>(), Variants::ConversionError);
        EXPECT_FALSE(Object(nullptr, 42).IsConvertibleTo(typeid(short)));
        EXPECT_FALSE(Object(nullptr, 42).IsConvertibleTo<short>());
    }

    TEST(ObjectTests, CreateObjectWithTypeLibrary_ConversionToSameTypeWorks)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(true);
        TypeConversions::AddBuiltInConversions(typeLibrary->GetConversionGraph());

        EXPECT_TRUE(Object(typeLibrary, 1).ConvertTo(Types::TypeId<int>()).IsValid());
        EXPECT_TRUE(Object(typeLibrary, 1).ConvertTo<int>().IsValid());
        EXPECT_EQ(42, Object(typeLibrary, 42).As<int>());
        EXPECT_TRUE(Object(typeLibrary, 42).IsConvertibleTo(typeid(int)));
        EXPECT_TRUE(Object(typeLibrary, 42).IsConvertibleTo<int>());
    }

    TEST(ObjectTests, CreateObjectWithTypeLibrary_ConversionToDifferentTypeWorks)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(true);
        TypeConversions::AddBuiltInConversions(typeLibrary->GetConversionGraph());

        EXPECT_TRUE(Object(typeLibrary, 1).ConvertTo(Types::TypeId<short>()).IsValid());
        EXPECT_TRUE(Object(typeLibrary, 1).ConvertTo<short>().IsValid());
        EXPECT_EQ(42, Object(typeLibrary, 42).As<short>());
        EXPECT_TRUE(Object(typeLibrary, 42).IsConvertibleTo(typeid(short)));
        EXPECT_TRUE(Object(typeLibrary, 42).IsConvertibleTo<short>());
    }

    TEST(ObjectTests, CreateObjectWithTypeLibrary_ConversionToUnrelatedTypeInvalid)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(true);
        TypeConversions::AddBuiltInConversions(typeLibrary->GetConversionGraph());

        EXPECT_FALSE(Object(typeLibrary, 1).ConvertTo(Types::TypeId<std::string>()).IsValid());
        EXPECT_FALSE(Object(typeLibrary, 1).ConvertTo<std::string>().IsValid());
        EXPECT_THROW(Object(typeLibrary, 42).As<std::string>(), Variants::ConversionError);
        EXPECT_FALSE(Object(typeLibrary, 42).IsConvertibleTo(typeid(std::string)));
        EXPECT_FALSE(Object(typeLibrary, 42).IsConvertibleTo<std::string>());
    }

    TEST(ObjectTests, CreateObjectOfBaseType_ConvertToDynamicType)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(true);
        TypeConversions::AddBuiltInConversions(typeLibrary->GetConversionGraph());
        Reflect<TestDerivedClass>(typeLibrary);
        std::shared_ptr<TestBaseClass> value(new TestDerivedClass);
        Object base(typeLibrary, value);
        EXPECT_EQ(Types::TypeId<TestBaseClass*>(), base.GetDecoratedTypeInfo());
        Object derived = base.ConvertToDynamicType();
        EXPECT_EQ(Types::TypeId<TestDerivedClass*>(), derived.GetDecoratedTypeInfo());
    }

    class ObjectWithAutomaticToStringSupport 
    {
    public:
        ObjectWithAutomaticToStringSupport() : m_firstArgument(0) {}
        ObjectWithAutomaticToStringSupport(const ObjectWithAutomaticToStringSupport& other) : m_firstArgument(other.m_firstArgument) {}
        ObjectWithAutomaticToStringSupport(int firstArgument) : m_firstArgument(firstArgument) {}
        ObjectWithAutomaticToStringSupport(int tens, int ones) : m_firstArgument(tens*10+ones) {}
        int GetFirstArgument() const { return m_firstArgument; }
        void SetFirstArgument(int firstArgument) { m_firstArgument = firstArgument; }
        bool operator==(const ObjectWithAutomaticToStringSupport& other) const { return m_firstArgument == other.m_firstArgument; }
    private:
        int m_firstArgument;
    };
    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, ObjectWithAutomaticToStringSupport**)
    {
        using namespace Classes;
        Class<ObjectWithAutomaticToStringSupport> cls(typeLibrary, "ObjectWithAutomaticToStringSupport");
        cls.Constructor();
        cls.Constructor<int>(Explicit).AddSignature("firstArgument");
        cls.SetGet("FirstArgument", &ObjectWithAutomaticToStringSupport::SetFirstArgument, &ObjectWithAutomaticToStringSupport::GetFirstArgument);
    }
    Formatting::FormattingService CreateScriptFormattingService(bool expandAllowed = true, bool shortSyntax = false)
    {
        using namespace Formatting;
        FormattingService service;
        service.AddFormatter<IFormatterRules>(std::make_shared<DefaultFormatterRules>(false, shortSyntax?IFormatterRules::SimplifiedGui:IFormatterRules::Exact, true, expandAllowed));
        return service;
    }
    TEST(ObjectTests, AutoimplementToStringFromConstructor)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Utilities::DefaultRegistration::Reflect(typeLibrary);
        TypeConversions::AddBuiltInConversions(typeLibrary->GetConversionGraph());

        using namespace Classes;
        Class<ObjectWithAutomaticToStringSupport> cls(typeLibrary, "ObjectWithAutomaticToStringSupport");
        cls.Constructor<int>(Explicit)
            .AddSignature("firstArgument");

        cls.SetGet("FirstArgument", &ObjectWithAutomaticToStringSupport::SetFirstArgument, &ObjectWithAutomaticToStringSupport::GetFirstArgument);

        Object base(typeLibrary, ObjectWithAutomaticToStringSupport(42));
        EXPECT_EQ("ObjectWithAutomaticToStringSupport(42)", ToString(base, CreateScriptFormattingService()));
    }
    
    TEST(ObjectTests, AutoimplementToString_HideEmptyConstructorIfPossible)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Utilities::DefaultRegistration::Reflect(typeLibrary);
        TypeConversions::AddBuiltInConversions(typeLibrary->GetConversionGraph());

        using namespace Classes;
        Class<ObjectWithAutomaticToStringSupport> cls(typeLibrary, "ObjectWithAutomaticToStringSupport");
        cls.Constructor();
        cls.Constructor<int>(Explicit)
            .AddSignature("firstArgument");

        cls.SetGet("FirstArgument", &ObjectWithAutomaticToStringSupport::SetFirstArgument, &ObjectWithAutomaticToStringSupport::GetFirstArgument);
        cls.Operator(This.Const == This.Const);
        EXPECT_EQ("ObjectWithAutomaticToStringSupport(42)", ToString(Object(typeLibrary, ObjectWithAutomaticToStringSupport(42)), CreateScriptFormattingService()));
        EXPECT_EQ("ObjectWithAutomaticToStringSupport()", ToString(Object(typeLibrary, ObjectWithAutomaticToStringSupport()), CreateScriptFormattingService()));
    }

    /*
    Dummy Selector to decide whether a special constructor should be used if the first argument is 42.
    This is a contrived example, but it can be useful, for example for
    LinearCurveOffset which has two constructors:
    LinearCurveOffset(vec3d end1, vec3d end2, bool useLocalSystem);
    and
    LinearCurveOffset(CurveOffsetPointer end1, CurveOffsetPointer end2, bool useLocalSystem);
    If end1 and end2 are constant curve offsets, we want the default constructor to be the one taking vec3d.
    Otherwise, we need to use the second constructor.
    */
    bool UseMagicConstructor(const ObjectWithAutomaticToStringSupport& other)
    {
        return other.GetFirstArgument() == 42;
    }
    bool UseStandardConstructor(const ObjectWithAutomaticToStringSupport& other)
    {
        return other.GetFirstArgument() != 42;
    }

    TEST(ObjectTests, AutoimplementToString_UseDefaultConstructorSelectorAttribute)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Utilities::DefaultRegistration::Reflect(typeLibrary);
        TypeConversions::AddBuiltInConversions(typeLibrary->GetConversionGraph());

        using namespace Classes;
        Class<ObjectWithAutomaticToStringSupport> cls(typeLibrary, "ObjectWithAutomaticToStringSupport");
        cls.Constructor<int>(Explicit)
            .AddSignature("firstArgument")
            .AddAttribute<DefaultConstructorSelectorAttribute>(&UseStandardConstructor);
        
        cls.NamedConstructor("MagicConstructor", [](int arg) {return ObjectWithAutomaticToStringSupport(arg); }, Explicit)
            .AddSignature("firstArgument")
            .AddAttribute<DefaultConstructorSelectorAttribute>(&UseMagicConstructor);
        
        //We need to register setters and getters in order to enable extraction of firstArgument from ObjectWithAutomaticToStringSupport.
        //Otherwise, there is no way for ToString to know how get firstArgument from an object.
        cls.SetGet("FirstArgument", &ObjectWithAutomaticToStringSupport::SetFirstArgument, &ObjectWithAutomaticToStringSupport::GetFirstArgument);

        EXPECT_EQ("ObjectWithAutomaticToStringSupport(41)", ToString(Object(typeLibrary, ObjectWithAutomaticToStringSupport(41)), CreateScriptFormattingService()));
        EXPECT_EQ("MagicConstructor(42)", ToString(Object(typeLibrary, ObjectWithAutomaticToStringSupport(42)), CreateScriptFormattingService()));
    }

    TEST(ObjectTests, AutoimplementToString_UseGenerateConstructorArgumentsCallback)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Utilities::DefaultRegistration::Reflect(typeLibrary);
        TypeConversions::AddBuiltInConversions(typeLibrary->GetConversionGraph());

        using namespace Classes;
        Class<ObjectWithAutomaticToStringSupport> cls(typeLibrary, "ObjectWithAutomaticToStringSupport");
        cls.Constructor<int>(Explicit).AddSignature("firstArgument");
        //Add a special constructor taking tens and ones.
        cls.Constructor<int, int>()
            .AddSignature("tens", "ones")
            .AddAttribute<GenerateConstructorArgumentsAttribute>([](const ObjectWithAutomaticToStringSupport& other, int& first, int& second) 
        { 
            //This method tries to take an object of type ObjectWithAutomaticToStringSupport and extract first and second from it.
            //Returns true on success.
            if (other.GetFirstArgument() >= 0 && other.GetFirstArgument() < 100)
            {
                first = other.GetFirstArgument() / 10;
                second = other.GetFirstArgument() - first * 10;
                return true;
            }
            return false;
        });
        cls.SetGet("FirstArgument", &ObjectWithAutomaticToStringSupport::SetFirstArgument, &ObjectWithAutomaticToStringSupport::GetFirstArgument);

        EXPECT_EQ("ObjectWithAutomaticToStringSupport(4, 1)", ToString(Object(typeLibrary, ObjectWithAutomaticToStringSupport(41)), CreateScriptFormattingService()));
        EXPECT_EQ("ObjectWithAutomaticToStringSupport(1042)", ToString(Object(typeLibrary, ObjectWithAutomaticToStringSupport(1042)), CreateScriptFormattingService()));
    }

    class Plane3d {
    public:
        Plane3d() {}
        Plane3d(double a, double b, double c, double d) : m_abcd{ a,b,c,d } {}
        static Plane3d ZPlane3d(double z)
        {
            return Plane3d(0, 0, 1, z);
        }
        Plane3d operator-() const { return Plane3d(-m_abcd[0], -m_abcd[1], -m_abcd[2], m_abcd[3]); }
        double GetA() const { return m_abcd[0]; }
        double GetB() const { return m_abcd[1]; }
        double GetC() const { return m_abcd[2]; }
        double GetD() const { return m_abcd[3]; }
        static bool GenerateZPlaneArguments(const Plane3d& plane, double& z)
        {
            if (plane.GetA() == 0 && plane.GetB() == 0 && plane.GetC() == 1)
            {
                z = plane.GetD();
                return true;
            }
            return false;
        }
        static bool GenerateNegativePlaneArguments(const Plane3d& plane, Plane3d& result)
        {
            result = -plane;
            return true;
        }
    private:
        double m_abcd[4];
    };
    
    TEST(ObjectTests, AutoimplementToStringFromConstructor_PreventInfiniteRecursion)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Utilities::DefaultRegistration::Reflect(typeLibrary);
        TypeConversions::AddBuiltInConversions(typeLibrary->GetConversionGraph());

        using namespace Classes;
        Class<Plane3d> cls(typeLibrary, "Plane3d");
        cls.Constructor<double,double,double,double>().AddSignature("a","b","c","d");
        cls.Get("a",&Plane3d::GetA);
        cls.Get("b",&Plane3d::GetB);
        cls.Get("c",&Plane3d::GetC);
        cls.Get("d",&Plane3d::GetD);
        /*
        We want to treat operator- as a constructor. This may sound strange, but the reason is:
        1. We want to enable, in the GUI, to select negative planes from a drop down list.
        2. We want to enable, in text formatting, -ZPlane3d to be the result of text formatting.
        */
        cls.Operator(-This.Const)
            .AddAttribute<ListAsConstructorAttribute>()
            .AddAttribute<GenerateConstructorArgumentsAttribute>(Plane3d::GenerateNegativePlaneArguments);
        
        cls.NamedConstructor("ZPlane3d", &Plane3d::ZPlane3d, Explicit)
            .AddSignature("Plane")
            .AddAttribute<GenerateConstructorArgumentsAttribute>(Plane3d::GenerateZPlaneArguments);

        EXPECT_EQ("ZPlane3d(5)", ToString(Object(typeLibrary, Plane3d::ZPlane3d(5)), CreateScriptFormattingService()));
        EXPECT_EQ("-ZPlane3d(5)", ToString(Object(typeLibrary, -Plane3d::ZPlane3d(5)), CreateScriptFormattingService()));
        EXPECT_EQ("Plane3d(-1, -0, -0, 4)", ToString(Object(typeLibrary, -Plane3d(1,0,0,4)), CreateScriptFormattingService()));
    }

    TEST(ObjectTests, ToStringFromNamedAttribute)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Utilities::DefaultRegistration::Reflect(typeLibrary);
        TypeConversions::AddBuiltInConversions(typeLibrary->GetConversionGraph());

        using namespace Classes;

        Class<ObjectWithAutomaticToStringSupport> cls(typeLibrary, "ObjectWithAutomaticToStringSupport");
        cls.AddAttribute<ObjectNameAttribute>([](const ObjectWithAutomaticToStringSupport& obj) {return "Bm1"; });
        cls.Constructor<int>(Explicit)
            .AddSignature("firstArgument");

        cls.SetGet("FirstArgument", &ObjectWithAutomaticToStringSupport::SetFirstArgument, &ObjectWithAutomaticToStringSupport::GetFirstArgument);

        EXPECT_EQ("Bm1", ToString(Object(typeLibrary, ObjectWithAutomaticToStringSupport(5)), CreateScriptFormattingService(false)));
        EXPECT_EQ("ObjectWithAutomaticToStringSupport(5)", ToString(Object(typeLibrary, ObjectWithAutomaticToStringSupport(5)), CreateScriptFormattingService(true)));
    }

    TEST(ObjectTests, ToStringFromNullPointer)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Utilities::DefaultRegistration::Reflect(typeLibrary);
        TypeConversions::AddBuiltInConversions(typeLibrary->GetConversionGraph());

        using namespace Classes;

        Class<ObjectWithAutomaticToStringSupport> cls(typeLibrary, "ObjectWithAutomaticToStringSupport");
        cls.Constructor<int>()
            .AddSignature("firstArgument");

        cls.SetGet("FirstArgument", &ObjectWithAutomaticToStringSupport::SetFirstArgument, &ObjectWithAutomaticToStringSupport::GetFirstArgument);

        EXPECT_EQ("null", ToString(Object(typeLibrary, Variants::Variant(nullptr, Types::TypeId<const ObjectWithAutomaticToStringSupport*>())), CreateScriptFormattingService()));
        cls.AddAttribute<AlternativeNullPointerNameAttribute>("DummyFunction()");
        EXPECT_EQ("DummyFunction()", ToString(Object(typeLibrary, Variants::Variant(nullptr, Types::TypeId<const ObjectWithAutomaticToStringSupport*>())), CreateScriptFormattingService()));
    }

    TEST(ObjectTests, ToStringShortSyntax)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Utilities::DefaultRegistration::Reflect(typeLibrary);
        TypeConversions::AddBuiltInConversions(typeLibrary->GetConversionGraph());

        using namespace Classes;

        Class<ObjectWithAutomaticToStringSupport> cls(typeLibrary, "ObjectWithAutomaticToStringSupport");
        cls.Constructor<int>(Explicit)
            .AddSignature("firstArgument")
            .AddAttribute<ShortSyntaxAttribute>(ShortSyntaxArgument::ShortSyntax);

        cls.SetGet("FirstArgument", &ObjectWithAutomaticToStringSupport::SetFirstArgument, &ObjectWithAutomaticToStringSupport::GetFirstArgument);

        EXPECT_EQ("ObjectWithAutomaticToStringSupport(43)", ToString(Object(typeLibrary, ObjectWithAutomaticToStringSupport(43)), CreateScriptFormattingService()));
        EXPECT_EQ("43", ToString(Object(typeLibrary, ObjectWithAutomaticToStringSupport(43)), CreateScriptFormattingService(true, true)));
    }

    class ConstantCurveOffset {
    public:
        ConstantCurveOffset(const Vector3d& constantOffset, bool useLocalSystem) 
            : m_constantOffset(constantOffset)
            , m_useLocalSystem(useLocalSystem)
        {}
        Vector3d GetConstantOffset() const { return m_constantOffset; }
        bool UseLocalSystem() const { return m_useLocalSystem; }
    private:
        Vector3d m_constantOffset;;
        bool m_useLocalSystem;
    };

    TEST(ObjectTests, ToStringNestedShortSyntax)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Utilities::DefaultRegistration::Reflect(typeLibrary);
        TypeConversions::AddBuiltInConversions(typeLibrary->GetConversionGraph());
        Reflect<Vector3d>(typeLibrary);
        using namespace Classes;

        Class<ConstantCurveOffset> cls(typeLibrary, "ConstantCurveOffset");
        auto& constructor = cls.Constructor<const Vector3d&, bool>()
            .AddSignature("constantOffset", "useLocalSystem");

        cls.Get("ConstantOffset", &ConstantCurveOffset::GetConstantOffset);
        cls.Get("UseLocalSystem", &ConstantCurveOffset::UseLocalSystem);

        EXPECT_EQ("ConstantCurveOffset(Vector3d(1, 2, 3), true)", ToString(Object(typeLibrary, ConstantCurveOffset(Vector3d(1, 2, 3), true)), CreateScriptFormattingService(true, true)));
        constructor.AddAttribute<ShortSyntaxAttribute>(ShortSyntaxArgument::ShortSyntax, ShortSyntaxArgument::Implicit);
        EXPECT_EQ("1 2 3", ToString(Object(typeLibrary, ConstantCurveOffset(Vector3d(1, 2, 3), true)), CreateScriptFormattingService(true, true)));
        constructor.AddAttribute<ShortSyntaxAttribute>(ShortSyntaxArgument::ScriptSyntax, ShortSyntaxArgument::Implicit);
        EXPECT_EQ("Vector3d(1, 2, 3)", ToString(Object(typeLibrary, ConstantCurveOffset(Vector3d(1, 2, 3), true)), CreateScriptFormattingService(true, true)));

        constructor.AddAttribute<ShortSyntaxAttribute>(ShortSyntaxArgument::ScriptSyntax, ShortSyntaxArgument::ScriptSyntax);
        EXPECT_EQ("Vector3d(1, 2, 3) true", ToString(Object(typeLibrary, ConstantCurveOffset(Vector3d(1, 2, 3), true)), CreateScriptFormattingService(true, true)));
    }

    TEST(ObjectTests, ParseShortSyntax)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Utilities::DefaultRegistration::Reflect(typeLibrary);
        TypeConversions::AddBuiltInConversions(typeLibrary->GetConversionGraph());
        Reflect<Vector3d>(typeLibrary);
        using namespace Classes;

        Class<ConstantCurveOffset> cls(typeLibrary, "ConstantCurveOffset");
        auto& constructor = cls.Constructor<const Vector3d&, bool>(Explicit)
            .AddSignature("constantOffset", Arg("useLocalSystem")=true)
            .AddAttribute<ShortSyntaxAttribute>(ShortSyntaxArgument::ShortSyntax, ShortSyntaxArgument::Implicit);

        cls.Get("ConstantOffset", &ConstantCurveOffset::GetConstantOffset);
        cls.Get("UseLocalSystem", &ConstantCurveOffset::UseLocalSystem);

        auto expressionResult = Attributes::ExpandExpression("10 0 1", DNVS::MoFa::Reflection::Types::TypeId<ConstantCurveOffset>(), FakeExpressionEvaluator(typeLibrary));
        EXPECT_TRUE(expressionResult.IsValid());
        EXPECT_EQ("ConstantCurveOffset(Vector3d(10, 0, 1), true)", expressionResult.GetString());
    }

    TEST(ObjectTests, ParseShortSyntax_ProperErrorMessage)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Utilities::DefaultRegistration::Reflect(typeLibrary);
        TypeConversions::AddBuiltInConversions(typeLibrary->GetConversionGraph());
        Reflect<Vector3d>(typeLibrary);
        using namespace Classes;

        Class<ConstantCurveOffset> cls(typeLibrary, "ConstantCurveOffset");
        auto& constructor = cls.Constructor<const Vector3d&, bool>(Explicit)
            .AddSignature("constantOffset", Arg("useLocalSystem") = true)
            .AddAttribute<ShortSyntaxAttribute>(ShortSyntaxArgument::ShortSyntax, ShortSyntaxArgument::Implicit,"3 4 5");

        cls.Get("ConstantOffset", &ConstantCurveOffset::GetConstantOffset);
        cls.Get("UseLocalSystem", &ConstantCurveOffset::UseLocalSystem);

        try {
            auto expressionResult = Attributes::ExpandExpression("10 0", DNVS::MoFa::Reflection::Types::TypeId<ConstantCurveOffset>(), FakeExpressionEvaluator(typeLibrary));
        }
        catch (std::runtime_error& e)
        {
            EXPECT_EQ("Invalid short syntax for type ConstantCurveOffset *. Expected short syntax of the form '3 4 5'.",std::string(e.what()));
            return;
        }
        EXPECT_FALSE(true) << "Expected exception to be thrown";
    }

    TEST(ObjectTests, ConvertObjectToObject)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        using namespace Classes;
        Class<int>(typeLibrary, "int").Operator(This.Const == This.Const);
        Reflect<Object>(typeLibrary);
        EXPECT_EQ(Object(typeLibrary, 42), Object(typeLibrary, 42).As<Object>());
    }

    class Base {
    public:
        virtual ~Base() {}
    };

    class Derived : public Base{

    };
    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, Derived**)
    {
        using namespace Classes;
        Class<Derived> cls(typeLibrary, "Derived");
    }
    TEST(ObjectTests, reflection_cast_fundamental)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        using namespace Classes;
        Class<int>(typeLibrary, "int").Operator(This.Const == This.Const);
        TypeConversions::AddBuiltInConversions(typeLibrary->GetConversionGraph());
        EXPECT_DOUBLE_EQ(5,reflection_cast<double>(5, typeLibrary));
        int a = 6;
        EXPECT_DOUBLE_EQ(a, reflection_cast<double>(a, typeLibrary));

    }

    TEST(ObjectTests, reflection_cast_smart_pointer)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Reflect<Derived>(typeLibrary);
        std::shared_ptr<Base> base(new Derived);
        EXPECT_NO_THROW(reflection_cast<Derived*>(base, typeLibrary));

    }

    TEST(ObjectTests, reflection_cast_const_ref_to_pointer)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Reflect<Derived>(typeLibrary);
        std::shared_ptr<Base> base(new Derived);
        std::map<int, Base*> map = { {1, base.get()} };
        const std::map<int, Base*>& cmap = map;
        EXPECT_NO_THROW(reflection_cast<Derived*>(cmap.begin()->second, typeLibrary));

    }

    TEST(ObjectTests, is_convertible_fundamental)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        TypeConversions::AddBuiltInConversions(typeLibrary->GetConversionGraph());
        EXPECT_TRUE(reflection_is_convertible<double>(5, typeLibrary));
    }

    TEST(ObjectTests, is_convertible_derived_is_registered)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Reflect<Derived>(typeLibrary);
        std::shared_ptr<Base> base = std::make_shared<Derived>();
        EXPECT_TRUE(reflection_is_convertible<Derived*>(base, typeLibrary));
    }

    class Base1 {
    public:
        virtual ~Base1() {}
        double bigData1[100];
    };

    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, Base1**)
    {
        using namespace Classes;
        Class<Base1> cls(typeLibrary, "Base1");
    }

    class Base2 {
    public:
        virtual ~Base2() {}
        double bigData2[100];
    };
    
    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, Base2**)
    {
        using namespace Classes;
        Class<Base2> cls(typeLibrary, "Base2");
    }
    class PolymorphicDerived : public Base1, public Base2 {};
    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, PolymorphicDerived**)
    {
        using namespace Classes;
        Class<PolymorphicDerived, Public<Base1,Base2>> cls(typeLibrary, "PolymorphicDerived");
    }
    TEST(ObjectTests, ConvertToBaseOfMultipleInheritance_SmartPointer)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Reflect<PolymorphicDerived>(typeLibrary);
        std::shared_ptr<Base2> base = std::make_shared<PolymorphicDerived>();
        PolymorphicDerived* dReflected = Objects::Object(typeLibrary, base).As<PolymorphicDerived*>();
        std::shared_ptr<PolymorphicDerived> dCast = std::dynamic_pointer_cast<PolymorphicDerived>(base);
        EXPECT_EQ(dCast.get(), dReflected);
    }

    TEST(ObjectTests, ConvertToBaseOfMultipleInheritance_Pointer)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Reflect<PolymorphicDerived>(typeLibrary);
        std::shared_ptr<Base2> base = std::make_shared<PolymorphicDerived>();
        PolymorphicDerived* dReflected = Objects::Object(typeLibrary, base.get()).As<PolymorphicDerived*>();
        std::shared_ptr<PolymorphicDerived> dCast = std::dynamic_pointer_cast<PolymorphicDerived>(base);
        EXPECT_EQ(dCast.get(), dReflected);
    }

    TEST(ObjectTests, ConvertToBaseOfMultipleInheritance_Pointer_Dynamic)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Reflect<PolymorphicDerived>(typeLibrary);
        std::shared_ptr<Base2> base = std::make_shared<PolymorphicDerived>();
        PolymorphicDerived* dReflected = Objects::Object(typeLibrary, Variants::VariantService::ReflectDynamic(base.get())).As<PolymorphicDerived*>();
        std::shared_ptr<PolymorphicDerived> dCast = std::dynamic_pointer_cast<PolymorphicDerived>(base);
        EXPECT_EQ(dCast.get(), dReflected);
    }
}}}}}
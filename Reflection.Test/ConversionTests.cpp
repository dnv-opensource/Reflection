//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"
#include "Reflection/Classes/Class.h"
#include "Reflection/Objects/Object.h"
#include "Reflection/TypeConversions/BuiltInConversions.h"
#include "Reflection/TypeLibraries/TypeLibraryFactory.h"
#include "Reflection/Enums/Enum.h"

using namespace DNVS::MoFa::Reflection;
using namespace DNVS::MoFa::Reflection::TypeLibraries;
using namespace DNVS::MoFa::Reflection::TypeConversions;
using namespace DNVS::MoFa::Reflection::Objects;
using namespace DNVS::MoFa::Reflection::Classes;
using namespace DNVS::MoFa::Reflection::Enums;

TEST(ConversionTests, POD_Conversion_Fail)
{
    TypeLibraryPointer lib = TypeLibraryFactory::CreateDefaultTypeLibrary();
    AddBuiltInConversions(lib->GetConversionGraph());
    Object v(lib, "42");
    EXPECT_THROW(v.As<int>(), std::runtime_error);
}

TEST(ConversionTests, POD_Conversion_Success)
{
    TypeLibraryPointer lib = TypeLibraryFactory::CreateDefaultTypeLibrary();
    AddBuiltInConversions(lib->GetConversionGraph());
    Object v(lib, 5.6);
    EXPECT_NO_THROW(v.As<int>());	
}

TEST(ConversionTests, Pointer_Conversion_Fail)
{
    TypeLibraryPointer lib = TypeLibraryFactory::CreateDefaultTypeLibrary();
    AddBuiltInConversions(lib->GetConversionGraph());
    Object v(lib, 5.4);
    EXPECT_THROW(v.As<const double*>(), std::runtime_error);
}

TEST(ConversionTests, Value_Conversion_Fail)
{
    TypeLibraryPointer lib = TypeLibraryFactory::CreateDefaultTypeLibrary();
    AddBuiltInConversions(lib->GetConversionGraph());
    Object v(lib, "4.5");
    EXPECT_THROW(v.As<double>(), std::runtime_error);
}

TEST(ConversionTests, Conversion_short_to_int)
{
    TypeLibraryPointer lib = TypeLibraryFactory::CreateDefaultTypeLibrary();
    AddBuiltInConversions(lib->GetConversionGraph());
    Object v(lib, short(345));
    EXPECT_EQ(345, v.As<int>());
}

TEST(ConversionTests, Conversion_Int_Is_Double)
{
    TypeLibraryPointer lib = TypeLibraryFactory::CreateDefaultTypeLibrary();
    AddBuiltInConversions(lib->GetConversionGraph());
    Object v(lib, 43);
    EXPECT_TRUE(v.IsConvertibleTo<double>());
}

TEST(ConversionTests, Conversion_Double_Is_Int)
{
    TypeLibraryPointer lib = TypeLibraryFactory::CreateDefaultTypeLibrary();
    AddBuiltInConversions(lib->GetConversionGraph());
    Object v(lib, 42.4);
    EXPECT_TRUE(v.IsConvertibleTo<int>());
}

TEST(ConversionTests, Conversion_Double_Is_RefDouble_Fail)
{
    TypeLibraryPointer lib = TypeLibraryFactory::CreateDefaultTypeLibrary();
    AddBuiltInConversions(lib->GetConversionGraph());
    Object v(lib, 42.4);
    EXPECT_FALSE(v.IsConvertibleTo<double&>());
}

TEST(ConversionTests, Conversion_Double_Is_CRefDouble)
{
    TypeLibraryPointer lib = TypeLibraryFactory::CreateDefaultTypeLibrary();
    AddBuiltInConversions(lib->GetConversionGraph());
    Object v(lib, 42.4);
    EXPECT_TRUE(v.IsConvertibleTo<const double&>());
}

class ClassWithDoubleAndIntConstructors 
{
public:
    ClassWithDoubleAndIntConstructors(double arg) :m_arg(arg) {}
    ClassWithDoubleAndIntConstructors(int arg) : m_arg(arg) {}
    ~ClassWithDoubleAndIntConstructors() {m_arg = 0.0; }
public:
    double m_arg;
};

void DoReflect(TypeLibraryPointer library, ClassWithDoubleAndIntConstructors**)
{
    Class<ClassWithDoubleAndIntConstructors, std::shared_ptr<ClassWithDoubleAndIntConstructors> > cls(library, "ClassWithDoubleAndIntConstructors");
    cls.Constructor<double>();
    cls.Constructor<int>();
}

TEST(ConversionTests, UserConversion_doubleToClass)
{
    TypeLibraryPointer lib = TypeLibraryFactory::CreateDefaultTypeLibrary();
    AddBuiltInConversions(lib->GetConversionGraph());
    Reflect<ClassWithDoubleAndIntConstructors>(lib);
    Object v(lib, 5.3);
    ClassWithDoubleAndIntConstructors cls = v.As<ClassWithDoubleAndIntConstructors>();
    EXPECT_DOUBLE_EQ(5.3, cls.m_arg);
}

TEST(ConversionTests, UserConversion_intToClass)
{
    TypeLibraryPointer lib = TypeLibraryFactory::CreateDefaultTypeLibrary();
    AddBuiltInConversions(lib->GetConversionGraph());
    Reflect<ClassWithDoubleAndIntConstructors>(lib);
    Object v(lib, 42);
    Object converted = v.ConvertTo<const ClassWithDoubleAndIntConstructors&>();
    const ClassWithDoubleAndIntConstructors& cls = converted.As<const ClassWithDoubleAndIntConstructors&>();
    EXPECT_EQ(42, cls.m_arg);    
}

class TestClass 
{
public:
    explicit TestClass(int data) {m_data = data; }
    static TestClass* Create(int data) {return new TestClass(data); }
    int m_data;
};

void DoReflect(TypeLibraryPointer typeLibrary, TestClass**)
{
    Class<TestClass> cls(typeLibrary, "TestClass");
    cls.Constructor(&TestClass::Create);
}

TEST(ConversionTests, ConversionByConstructorFunction)
{
    TypeLibraryPointer lib = TypeLibraryFactory::CreateDefaultTypeLibrary();
    AddBuiltInConversions(lib->GetConversionGraph());
    Reflect<TestClass>(lib);
    Object v(lib, 42);
    Object testClass = v.ConvertTo<const TestClass&>();
    EXPECT_NO_THROW(EXPECT_EQ(42, v.As<const TestClass&>().m_data));
}

TEST(ConversionTests, WrapConstRefDouble_ChangeOrigin_CopyNotChanged)
{
    TypeLibraryPointer lib = TypeLibraryFactory::CreateDefaultTypeLibrary();
    AddBuiltInConversions(lib->GetConversionGraph());
    double c = 42.0;
    Object v = Object::Create<const double&>(lib, c);
    v = v.ToLValue();
    c = 41;
    EXPECT_EQ(42, v.As<int>());
}

TEST(ConversionTests, WrapDouble_ConvertToLValue_NoThrow)
{
    TypeLibraryPointer lib = TypeLibraryFactory::CreateDefaultTypeLibrary();
    AddBuiltInConversions(lib->GetConversionGraph());
    double c = 42.0;
    Object v = Object::Create<double>(lib, c);
    EXPECT_NO_THROW(v = v.ToLValue());
}

TEST(ConversionTests, WrapConstRefString_ChangeOrigin_CopyNotChanged)
{
    TypeLibraryPointer lib = TypeLibraryFactory::CreateDefaultTypeLibrary();
    Class<std::string, std::string> cls(lib, "String");
    AddBuiltInConversions(lib->GetConversionGraph());
    std::string c = "Hello";
    Object v = Object::Create<const std::string&>(lib, c);
    v = v.ToLValue();
    c = "Bye";
    EXPECT_EQ("Hello", v.As<std::string>());
}

enum class ConstRefEnum {
    val1,
    val2
};

TEST(ConversionTests, WrapConstRefEnum_ChangeOrigin_CopyNotChanged)
{
    TypeLibraryPointer lib = TypeLibraryFactory::CreateDefaultTypeLibrary();
    Enum<ConstRefEnum> enm(lib, "ConstRefEnum");
    enm.EnumValue("val1", ConstRefEnum::val1);
    enm.EnumValue("val2", ConstRefEnum::val2);

    AddBuiltInConversions(lib->GetConversionGraph());
    ConstRefEnum c = ConstRefEnum::val1;
    Object v = Object::Create<const ConstRefEnum&>(lib, c);
    v = v.ToLValue();
    c = ConstRefEnum::val2;
    EXPECT_EQ(ConstRefEnum::val1, v.As<ConstRefEnum>());
}
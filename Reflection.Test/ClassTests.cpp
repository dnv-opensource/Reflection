//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection\TypeLibraries\TypeLibraryFactory.h"
#include "Reflection\Classes\Class.h"
#include "Reflection\Objects\Object.h"
#include "Reflection\Objects\Delegate.h"
#include "Reflection\Members\MemberWithArguments.h"
#include "Reflection\TypeConversions\BuiltInConversions.h"
#include "Reflection\Properties\PropertyStaticGet.h"
#include "gtest\gtest.h"
#include "Reflection\DoReflect.h"
#include "Services\Allocators\ForwardingAllocatorSelector.h"
//#include "Utility\SmartPointer.h"
#include "boost\optional\optional.hpp"
#include "Reflection/Utilities/ReflectOptional.h"

using DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryFactory;
using DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer;
using DNVS::MoFa::Reflection::TypeLibraries::TypePointer;
using DNVS::MoFa::Reflection::TypeConversions::ConversionGraphPointer;
using DNVS::MoFa::Reflection::Objects::Object;
using DNVS::MoFa::Reflection::Variants::Variant;
using namespace DNVS::MoFa::Reflection;
//using namespace DNVS::MoFa::Utility;

using namespace DNVS::MoFa::Reflection::Classes;

TEST(ClassTests,ClassOfString)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    Class<std::string> cls(typeLibrary,"string");
}

TEST(ClassTests,ClassOfString_DefineConstructor)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    Class<std::string, std::shared_ptr<std::string>> cls(typeLibrary, "string");
    cls.Constructor<const char*>();
    Object text(typeLibrary,"Hello world");
    EXPECT_EQ("Hello world",text.As<std::string>());
}

TEST(ClassTests,ClassOfString_DefineOperator)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    Class<std::string, std::shared_ptr<std::string>> cls(typeLibrary, "string");
    cls.Operator(This.Const+This.Const);
    
    Object a(typeLibrary,std::string("Hello "));
    Object b(typeLibrary,std::string("world"));
    Object c=a+b;
    EXPECT_EQ("Hello world",c.As<std::string>());
}

TEST(ClassTests,ClassOfString_DefineOperatorPlusEqual)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    Class<std::string, std::shared_ptr<std::string>> cls(typeLibrary, "string");
    cls.Operator(This+=Other<const std::string&>());

    std::string txt("Hello ");
    Object a(typeLibrary,DNVS::MoFa::Reflection::Variants::VariantService().ReflectType<std::string&>(txt));
    Object b(typeLibrary,std::string("world"));
    a+=b;
    EXPECT_EQ("Hello world",a.As<std::string>());
    EXPECT_EQ("Hello world",txt);
}

TEST(ClassTests,ClassOfString_DefineFunction)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    Class<std::string, std::shared_ptr<std::string>> cls(typeLibrary, "string");
    cls.Function("clear",&std::string::clear);

    std::string txt("Hello ");
    Object a(typeLibrary,DNVS::MoFa::Reflection::Variants::VariantService().ReflectType<std::string&>(txt));
    std::vector<Object> args;
    a.Invoke("clear",args);
    EXPECT_EQ("",a.As<std::string>());
    EXPECT_EQ("",txt);
}

TEST(ClassTests,ClassOfString_DefineConstructor_Construct)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    Class<std::string, std::shared_ptr<std::string>> cls(typeLibrary, "string");
    cls.Constructor<const char*>();
    Object text(typeLibrary,"Hello world");
    TypePointer type=typeLibrary->LookupType(typeid(std::string));
    std::vector<Variant> variables(1,text.GetVariant());
    Object result(typeLibrary,type->PrepareInvoke(Members::ConstructorString,variables)->Invoke());
    EXPECT_EQ("Hello world",*result.As<std::shared_ptr<std::string> >());
}

TEST(ClassTests,Vector_PushBack)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    DNVS::MoFa::Reflection::TypeConversions::AddBuiltInConversions(typeLibrary->GetConversionGraph());

    Class<std::vector<double>,std::shared_ptr<std::vector<double>>> cls(typeLibrary,"VectorOfDouble");
    cls.Function<void(std::vector<double>::*)(const double&)>("push_back",&std::vector<double>::push_back);
    std::vector<double> myVec;
    Object a(typeLibrary,&myVec);
    std::vector<Object> args(1,Object(typeLibrary,8.3));
    a.Invoke("push_back",args);
    EXPECT_EQ(1,myVec.size());
    EXPECT_NEAR(8.3,myVec[0],1e-6);
}

class BaseClass2 {
public:
    int TestCall(int a) {return a;}
};

void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, BaseClass2**)
{
    Class<BaseClass2> cls(typeLibrary, "BaseClass2");
}

class DerivedClass : public BaseClass2 {

};

TEST(ClassTests,DefineClassWithBase_RegisterClass_CallBaseMethod)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    DNVS::MoFa::Reflection::TypeConversions::AddBuiltInConversions(typeLibrary->GetConversionGraph());
    Class<BaseClass2,std::shared_ptr<BaseClass2>> clsBase(typeLibrary,"BaseClass2");
    clsBase.Function("TestCall",&BaseClass2::TestCall);

    Class<DerivedClass,Public<BaseClass2>,std::shared_ptr<DerivedClass> > clsDerived(typeLibrary,"DerivedClass");
    DerivedClass derived;
    Object obj(typeLibrary,&derived);
    std::vector<Object> args(1,Object(typeLibrary,4));
    ASSERT_NO_THROW(obj.Invoke("TestCall",args));
    EXPECT_EQ(4,obj.Invoke("TestCall",args).As<int>());
}

TEST(ClassTests,DefinePropertySetGetSize_Invoke)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    DNVS::MoFa::Reflection::TypeConversions::AddBuiltInConversions(typeLibrary->GetConversionGraph());

    Class<std::vector<double>,std::shared_ptr<std::vector<double>>> cls(typeLibrary,"VectorOfDouble");
    cls.Set("size",&std::vector<double>::resize);
    cls.Get("size",&std::vector<double>::size);

    std::vector<double> myVec;
    Object a(typeLibrary,&myVec);

    std::vector<Object> args(1,Object(typeLibrary,5));
    a.Invoke("size",args);
    EXPECT_EQ(5,myVec.size());
}

class TestClassWithAlternativeLookup {
public:
    ///Customized lookup function, used by reflection to delegate the lookup method to the class.
    Members::MemberPointer Lookup(const std::string& name,std::shared_ptr<TypeLibraries::ITypeLibrary> typeLibrary) const
    {
        if(name=="Hello") return new Properties::PropertyStaticGet<int>(name,typeLibrary->GetConversionGraph(),5);
        else return Members::MemberPointer();
    }
private:
    TypeLibraries::TypeLibraryPointer m_typeLibrary;
};

TEST(ClassTests,DefineAlternativeLookupFunction)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    DNVS::MoFa::Reflection::TypeConversions::AddBuiltInConversions(typeLibrary->GetConversionGraph());

    Classes::Class<TestClassWithAlternativeLookup> cls(typeLibrary,"TestClassWithAlternativeLookup");
    cls.Function("__lookup__",&TestClassWithAlternativeLookup::Lookup);
    Objects::Object objA(typeLibrary,TestClassWithAlternativeLookup());
    EXPECT_EQ(5,objA.Invoke("Hello",std::vector<Objects::Object>()).As<int>());
}

class NestedClass {
public:
    int Hello() const { return 5; }
};

void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, NestedClass**)
{
    Class<NestedClass> cls(typeLibrary, "NestedClass");
    cls.Function("Hello", &NestedClass::Hello);
}

class TestClass2WithAlternativeLookup {
public:
    ///Customized lookup function, used by reflection to delegate the lookup method to the class.
    Members::MemberPointer Lookup(const std::string& name, std::shared_ptr<TypeLibraries::ITypeLibrary> typeLibrary) const
    {
        return Objects::Object(typeLibrary, m_nested).Lookup(name).AsMember();
    }
private:
    NestedClass m_nested;
};

void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, TestClass2WithAlternativeLookup**)
{
    Class<TestClass2WithAlternativeLookup> cls(typeLibrary, "TestClass2WithAlternativeLookup");
    cls.Function("__lookup__", &TestClass2WithAlternativeLookup::Lookup);
}

TEST(ClassTests, DefineAlternativeLookupFunction_WithForwarding)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    DNVS::MoFa::Reflection::TypeConversions::AddBuiltInConversions(typeLibrary->GetConversionGraph());
    DNVS::MoFa::Reflection::Reflect<NestedClass>(typeLibrary);
    DNVS::MoFa::Reflection::Reflect<TestClass2WithAlternativeLookup>(typeLibrary);
    Objects::Object objA(typeLibrary, TestClass2WithAlternativeLookup());
    EXPECT_EQ(5, objA.Invoke("Hello", std::vector<Objects::Object>()).As<int>());
}
TEST(ClassTests,ClassOfString_DefineConstructor_CallConstructor)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    Class<std::string, std::shared_ptr<std::string>> cls(typeLibrary, "string");
    cls.Constructor<const char*>();
    Object text(typeLibrary,std::string("Hello world"));
    std::vector<Object> objects;
    objects.push_back(Object(typeLibrary,"Bye world"));
    Object constructed = text.Invoke(Members::ConstructorString,objects);
    EXPECT_EQ("Bye world",constructed.As<std::string>());
}

TEST(ClassTests,ClassOfString_DefineConstructor_CallConstructorUsingLookup)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    Class<std::string, std::shared_ptr<std::string>> cls(typeLibrary, "string");
    cls.Constructor<const char*>();
    Object text(typeLibrary,std::string("Hello world"));
    std::vector<Object> objects;
    Object constructed = text.Lookup(Members::ConstructorString)("Bye world");
    EXPECT_EQ("Bye world",constructed.As<std::string>());
}

TEST(ClassTests,ClassOfString_DefineConstructor_CallConstructorFromGlobalObjectUsingLookup)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    Class<std::string, std::shared_ptr<std::string>> cls(typeLibrary, "string");
    cls.Constructor<const char*>();
    Object globalObject(typeLibrary,Members::GlobalType());
    std::vector<Object> objects;
    Object constructed = globalObject.Lookup("string")("Bye world");
    EXPECT_EQ("Bye world",constructed.As<std::string>());
}

using DNVS::MoFa::Reflection::Classes::ArgumentPack;


TEST(ClassTests, NewArgumentPackImplementation)
{
    static_assert(std::is_same_v<std::allocator<int>, ArgumentPack<int, void, std::allocator<int>>::AllocatorType>,"Allocator not detected correctly");
    static_assert(std::is_same_v<std::shared_ptr<int>, ArgumentPack<int, void, std::allocator<int>, std::shared_ptr<int>>::HolderType>, "Pointer not detected correctly");
//    static_assert(std::is_same_v<SmartPointer<int>, ArgumentPack<int, void, SmartPointer<int>>::HolderType>, "Pointer not detected correctly");
    static_assert(std::is_same_v<Public<double>, ArgumentPack<int, void, std::allocator<int>, Public<double>>::PublicType>, "PublicType not detected correctly");

    static_assert(std::is_same_v<std::allocator<void>, ArgumentPack<int, void>::AllocatorType>, "Allocator not detected correctly");
    static_assert(std::is_same_v<DNVS::MoFa::Services::Allocators::ForwardingAllocatorSelector, ArgumentPack<int, void, DNVS::MoFa::Services::Allocators::ForwardingAllocatorSelector>::AllocatorType>, "Allocator not detected correctly");
}

class MyTest {
public:
    MyTest(boost::optional<double> a) {}
};
TEST(ClassTests, VerifySmartPointerOfBoostOptionalValidStorage)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    Class<MyTest>(typeLibrary, "MyTest").Constructor<boost::optional<double>>();
    EXPECT_FALSE(Types::IsPointerWrapper<boost::optional<double>>::value);
    static_assert(std::is_same_v<boost::optional<double>, Types::ReturnType<boost::optional<double>>>,"Not same");
    EXPECT_TRUE(Types::IsPointerWrapper<std::unique_ptr<boost::optional<double>>>::value);
    EXPECT_EQ(std::string(typeid(std::unique_ptr<boost::optional<double>>).name()), std::string(typeid(ClassArgumentPack<boost::optional<double>, std::unique_ptr<boost::optional<double>>>::HolderType).name()));
    static_assert(std::is_same_v<std::unique_ptr<boost::optional<double>>,ClassArgumentPack<boost::optional<double>, std::unique_ptr<boost::optional<double>>>::HolderType>, "Invalid holder type");
}

//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection\Constructors\DefineConstructor.h"

#include "Reflection\TypeLibraries\TypeLibraryFactory.h"
#include "Reflection\Members\Arg.h"
#include <list>
#include "gtest/gtest.h"

using namespace DNVS::MoFa::Reflection::TypeConversions;
using namespace DNVS::MoFa::Reflection::TypeLibraries;
using namespace DNVS::MoFa::Reflection::Constructors;
using namespace DNVS::MoFa::Reflection::Types;
using namespace DNVS::MoFa::Reflection::Members;

class TestClassWithSimpleConstructor {
public:
    TestClassWithSimpleConstructor() {}
    TestClassWithSimpleConstructor(int a) {}
    TestClassWithSimpleConstructor(int a,double b) {}
    TestClassWithSimpleConstructor(int a,double b,std::string text,short arg,float last) {}
    TestClassWithSimpleConstructor(const std::vector<double>& args) {}
};

TEST(ConstructorTests,DefineSimpleConstructor)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    DefineConstructor<TestClassWithSimpleConstructor> def(typeLibrary, typeLibrary->CreateType(typeid(TestClassWithSimpleConstructor),"TestClassWithSimpleConstructor"));
    def.Constructor<int>();
}

TEST(ConstructorTests,DefineConstructorWithNoArgument)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    DefineConstructor<TestClassWithSimpleConstructor> def(typeLibrary, typeLibrary->CreateType(typeid(TestClassWithSimpleConstructor), "TestClassWithSimpleConstructor"));
    def.Constructor();
}
TEST(ConstructorTests,DefineConstructorWithOneArgument_ConversionRegistered)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    DefineConstructor<TestClassWithSimpleConstructor> def(typeLibrary, typeLibrary->CreateType(typeid(TestClassWithSimpleConstructor), "TestClassWithSimpleConstructor"));
    def.Constructor<int>();
    ConversionSequencePointer conversionSequence = typeLibrary->GetConversionGraph()->GetConversionSequence(TypeId<int>(), TypeId<TestClassWithSimpleConstructor*>());
    ASSERT_TRUE(conversionSequence);
    EXPECT_TRUE(conversionSequence->IsValid());
}

TEST(ConstructorTests,DefineConstructorWithTwoArguments_NoConversionRegistered)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    DefineConstructor<TestClassWithSimpleConstructor> def(typeLibrary, typeLibrary->CreateType(typeid(TestClassWithSimpleConstructor),"TestClassWithSimpleConstructor"));
    def.Constructor<int,double>();
    ConversionSequencePointer conversionSequence = typeLibrary->GetConversionGraph()->GetConversionSequence(TypeId<int>(), TypeId<TestClassWithSimpleConstructor*>());
    ASSERT_TRUE(conversionSequence);
    EXPECT_FALSE(conversionSequence->IsValid());
}

TEST(ConstructorTests,DefineConstructorWithTwoArguments_OneDefaultArgument_ConversionRegistered)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    DefineConstructor<TestClassWithSimpleConstructor> def(typeLibrary, typeLibrary->CreateType(typeid(TestClassWithSimpleConstructor),"TestClassWithSimpleConstructor"));
    def.Constructor<int,double>()
       .AddSignature("first", Arg("second")=5.4);
    ConversionSequencePointer conversionSequence = typeLibrary->GetConversionGraph()->GetConversionSequence(TypeId<int>(), TypeId<TestClassWithSimpleConstructor*>());
    ASSERT_TRUE(conversionSequence);
    EXPECT_TRUE(conversionSequence->IsValid());
}

TEST(ConstructorTests,DefineConstructorVararg)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();

    DefineConstructor<TestClassWithSimpleConstructor> def(typeLibrary, typeLibrary->CreateType(typeid(TestClassWithSimpleConstructor),"TestClassWithSimpleConstructor"));
    def.Constructor<const std::vector<double>>(Vararg);
    ConversionSequencePointer conversionSequence = typeLibrary->GetConversionGraph()->GetConversionSequence(TypeId<double>(), TypeId<TestClassWithSimpleConstructor*>());
    ASSERT_TRUE(conversionSequence);
    EXPECT_TRUE(conversionSequence->IsValid());
}

TEST(ConstructorTests,DefineConstructorWithFiveArguments)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    DefineConstructor<TestClassWithSimpleConstructor> def(typeLibrary, typeLibrary->CreateType(typeid(TestClassWithSimpleConstructor),"TestClassWithSimpleConstructor"));
    def.Constructor<int,double,std::string,short,float>();
}
std::string* CreateString(int a) {
    return new std::string(boost::lexical_cast<std::string>(a));
}
TEST(ConstructorTests,UseConstructorFunction)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    DefineConstructor<std::string> def(typeLibrary, typeLibrary->CreateType(typeid(std::string), "string"));
    def.Constructor(&CreateString);
}

std::string* CreateString2(const std::list<int>& args) {return new std::string(boost::lexical_cast<std::string>(args.size()));}

TEST(ConstructorTests,UseConstructorFunctionVararg)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    DefineConstructor<std::string> def(typeLibrary, typeLibrary->CreateType(typeid(std::string), "string"));
    def.Constructor(&CreateString2,Vararg);
}

TEST(ConstructorTests,DefineSameConstructorConversionTwice_Ok)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    DefineConstructor<std::string> def(typeLibrary, typeLibrary->CreateType(typeid(std::string), "string"));
    def.Constructor<const char*>();
    def.Constructor<const char*>();
}
class DummyClassWithAllocator {
public:
    DummyClassWithAllocator(std::allocator<DummyClassWithAllocator> alloc) {}
};
TEST(ConstructorTests,DefineConstructorTakingCustomAllocatorAsLastArgument)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    DefineConstructor<DummyClassWithAllocator, std::allocator<DummyClassWithAllocator> > def(typeLibrary, typeLibrary->CreateType(typeid(DummyClassWithAllocator), "DummyClassWithAllocator"));
    auto& c = def.Constructor(AllocatorAsLastArgument);
    c.Invoke(std::vector<DNVS::MoFa::Reflection::Variants::Variant>());
}

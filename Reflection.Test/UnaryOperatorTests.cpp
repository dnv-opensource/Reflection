//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection\TypeLibraries\TypeLibraryFactory.h"
#include "Reflection\Operators\DefineOperator.h"
#include "Reflection\Objects\Object.h"
#include "Reflection\TypeConversions\BuiltInConversions.h"
#include "gtest\gtest.h"


using namespace DNVS::MoFa::Reflection::TypeConversions;
using namespace DNVS::MoFa::Reflection::TypeLibraries;
using namespace DNVS::MoFa::Reflection::Operators;
using namespace DNVS::MoFa::Reflection::Variants;
using namespace DNVS::MoFa::Reflection::Objects;

using namespace DNVS::MoFa::Operators;

TEST(UnaryOperatorTests,UnaryMinusDouble)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    AddBuiltInConversions(typeLibrary->GetConversionGraph());
    TypePointer type=typeLibrary->CreateType(typeid(int),"int");
    ///Definition of operators
    DefineOperator<int> intClass(type);
    intClass.Operator(-This.Const);
    ///Testing operator overloads
    Object lhs(typeLibrary,45);
    Object result=-lhs;
    EXPECT_EQ(-45,result.As<int>());

}

struct MyClass {
public:
    MyClass(int a) : m_a(a) {}
    int operator*() const {return m_a;}
private:
    int m_a;
};

TEST(UnaryOperatorTests,UnaryDereference)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    AddBuiltInConversions(typeLibrary->GetConversionGraph());
    TypePointer type = typeLibrary->CreateType(typeid(MyClass), "MyClass");
    ///Definition of operators
    DefineOperator<MyClass> myClass(type);
    myClass.Operator(*This.Const);
    ///Testing operator overloads
    MyClass a(4);
    Object lhs(typeLibrary,a);
    Object result=*lhs;
    EXPECT_EQ(4,result.As<int>());
}

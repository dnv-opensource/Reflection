//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection\Operators\This.h"
#include "Reflection\Operators\BinaryOperator.h"
#include "Reflection\Operators\DefineOperator.h"

#include "Reflection\TypeLibraries\TypeLibraryFactory.h"

#include "Reflection\TypeConversions\BuiltInConversions.h"

#include "Reflection\Objects\Object.h"
#include "Reflection\Objects\InvokeBinaryOperator.h"
#include <typeindex>
#include "gtest\gtest.h"


using namespace DNVS::MoFa::Reflection::TypeConversions;
using namespace DNVS::MoFa::Reflection::TypeLibraries;
using namespace DNVS::MoFa::Reflection::Operators;
using namespace DNVS::MoFa::Reflection::Variants;
using namespace DNVS::MoFa::Reflection::Objects;
using namespace DNVS::MoFa::Reflection::Members;

using namespace DNVS::MoFa::Operators;

TEST(BinaryOperatorTests,RegisterShiftOperatorForInt)
{
    typedef decltype(This << int()) type;
    typedef type::Rebind<int,int>::Type MyType;
    EXPECT_EQ(std::type_index(typeid(BinaryOperator<int&,int,Tags::ShiftLeft>)),std::type_index(typeid(MyType)));
}

TEST(BinaryOperatorTests,RegisterPlusOperatorIntInt)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    TypePointer type = typeLibrary->CreateType(typeid(int), "int");
    DefineOperator<int> intClass(type);
    intClass.Operator(This.Const+This.Const);
}

TEST(BinaryOperatorTests,RegisterPlusOperatorIntInt_PrepareInvoke)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();

    AddBuiltInConversions(typeLibrary->GetConversionGraph());
    TypePointer type = typeLibrary->CreateType(typeid(int),"int");
    DefineOperator<int> intClass(type);
    intClass.Operator(This.Const+This.Const);
    Variant lhs=VariantService().ReflectType<int>(5);
    Variant rhs=VariantService().ReflectType<int>(49);
    MemberWithArgumentsPointer delegate=PrepareInvokeBinaryOperator<Tags::Plus>(typeLibrary, lhs, rhs);
    EXPECT_TRUE(delegate);
    EXPECT_TRUE(delegate->IsOk());
}

TEST(BinaryOperatorTests,RegisterPlusOperatorIntInt_Invoke)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();

    AddBuiltInConversions(typeLibrary->GetConversionGraph());
    TypePointer type=typeLibrary->CreateType(typeid(int),"int");
    ///Definition of operators
    DefineOperator<int> intClass(type);
    intClass.Operator(This.Const+This.Const);
    ///Testing operator overloads
    Variant lhs=VariantService().ReflectType<int>(5);
    Variant rhs=VariantService().ReflectType<int>(49);
    Variant result=PrepareInvokeBinaryOperator<Tags::Plus>(typeLibrary,lhs,rhs)->Invoke();
    EXPECT_EQ(54, TypeConversionService(typeLibrary->GetConversionGraph()).As<int>(result));
}

TEST(BinaryOperatorTests,RegisterPlusOperatorIntInt_AndIntDouble_Invoke_IntDouble)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    AddBuiltInConversions(typeLibrary->GetConversionGraph());
    TypePointer type=typeLibrary->CreateType(typeid(int),"int");
    ///Definition of operators
    DefineOperator<int> intClass(type);
    intClass.Operator(This.Const+This.Const);
    intClass.Operator(This.Const+double());
    ///Testing operator overloads
    Variant lhs=VariantService().ReflectType<int>(5);
    Variant rhs=VariantService().ReflectType<double>(4.23);
    Variant result=PrepareInvokeBinaryOperator<Tags::Plus>(typeLibrary,lhs,rhs)->Invoke();
    EXPECT_EQ(9.23, TypeConversionService(typeLibrary->GetConversionGraph()).As<double>(result));
}

TEST(BinaryOperatorTests,RegisterPlusOperatorIntInt_AndDoubleInt_Invoke_DoubleInt)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    AddBuiltInConversions(typeLibrary->GetConversionGraph());

    TypePointer type=typeLibrary->CreateType(typeid(int),"int");
    ///Definition of operators
    DefineOperator<int> intClass(type);
    intClass.Operator(This.Const+This.Const);
    intClass.Operator(double()+This.Const);
    ///Testing operator overloads
    Variant lhs=VariantService().ReflectType<double>(4.23);
    Variant rhs=VariantService().ReflectType<int>(5);
    Variant result=PrepareInvokeBinaryOperator<Tags::Plus>(typeLibrary,lhs,rhs)->Invoke();
    EXPECT_EQ(9.23, TypeConversionService(typeLibrary->GetConversionGraph()).As<double>(result));
}

TEST(BinaryOperatorTests,RegisterPlusOperatorIntInt_AndDoubleInt_Invoke_DoubleInt_UseObjects)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    AddBuiltInConversions(typeLibrary->GetConversionGraph());
    TypePointer type=typeLibrary->CreateType(typeid(int),"int");
    ///Definition of operators
    DefineOperator<int> intClass(type);
    intClass.Operator(This.Const+This.Const);
    intClass.Operator(double()+This.Const);
    ///Testing operator overloads
    Object lhs(typeLibrary,5.23);
    Object rhs(typeLibrary,45);
    Object result=lhs+rhs;
    EXPECT_NEAR(50.23,result.As<double>(),1e-6);

}

class ClassWithBinaryOperatorReturningNothing
{
public:
    void operator+(int) {}
};
TEST(BinaryOperatorTests, RegisterPlusOperator_ResultReturnsNothing)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    AddBuiltInConversions(typeLibrary->GetConversionGraph());
    TypePointer type = typeLibrary->CreateType(typeid(ClassWithBinaryOperatorReturningNothing), "ClassWithBinaryOperatorReturningNothing");
    DefineOperator<ClassWithBinaryOperatorReturningNothing> myClass(type);
    auto& method = myClass.Operator(This.Const + int());
}

TEST(BinaryOperatorTests, RegisterPlusOperatorIntInt_ToStringOnMethod)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    AddBuiltInConversions(typeLibrary->GetConversionGraph());
    TypePointer type = typeLibrary->CreateType(typeid(int), "int");
    ///Definition of operators
    DefineOperator<int> intClass(type);
    auto& method = intClass.Operator(This.Const + This.Const);
    EXPECT_EQ( "15 + 42" , method.ToString({ "15", "42" }));
}
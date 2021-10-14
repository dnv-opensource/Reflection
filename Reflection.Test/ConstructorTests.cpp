//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"
#include <Reflection/Classes/Class.h>

#include "Reflection/Members/MemberWithArguments.h"

#include <Reflection/Objects/Object.h>
#include <Reflection/Objects/Delegate.h>
#include <Reflection/Containers/ReflectList.h>

#include "Reflection/TypeLibraries/TypeLibraryFactory.h"

#include <list>

using namespace DNVS::MoFa::Reflection;
using namespace DNVS::MoFa::Reflection::TypeLibraries;
using namespace DNVS::MoFa::Reflection::Classes;
using namespace DNVS::MoFa::Reflection::TypeConversions;
using namespace DNVS::MoFa::Reflection::Objects;

class ClassWithConstructors 
{
public:
    ClassWithConstructors() {overload = 0; }
    ClassWithConstructors(int){overload = 1; }
    ClassWithConstructors(int, int){overload = 2; }
    ClassWithConstructors(int, int, int){overload = 3; }
    ClassWithConstructors(int, int, int, int){overload = 4; }
    ClassWithConstructors(int, int, int, int, int){overload = 5; }
    ClassWithConstructors(int, int, int, int, int, int){overload = 6; }
    ClassWithConstructors(int, int, int, int, int, int, int){overload = 7; }
    ClassWithConstructors(int, int, int, int, int, int, int, int){overload = 8; }
    ClassWithConstructors(int, int, int, int, int, int, int, int, int){overload = 9; }
    ClassWithConstructors(int, int, int, int, int, int, int, int, int, int){overload = 10; }
    int overload;
};

void DoReflect(TypeLibraryPointer lib, ClassWithConstructors**)
{
    Class<ClassWithConstructors> cls(lib, "ClassWithConstructors");
    cls.Constructor().AddDocumentation("Function0");
    cls.Constructor<int>().AddSignature("a0").AddDocumentation("Function1");
    cls.Constructor<int, int>().AddSignature("a0", "a1").AddDocumentation("Function2");
    cls.Constructor<int, int, int>().AddSignature("a0", "a1", "a2").AddDocumentation("Function3");
    cls.Constructor<int, int, int, int>().AddSignature("a0", "a1", "a2", "a3").AddDocumentation("Function4");
    cls.Constructor<int, int, int, int, int>().AddSignature("a0", "a1", "a2", "a3", "a4").AddDocumentation("Function5");
    cls.Constructor<int, int, int, int, int, int>().AddSignature("a0", "a1", "a2", "a3", "a4", "a5").AddDocumentation("Function6");
    cls.Constructor<int, int, int, int, int, int, int>().AddSignature("a0", "a1", "a2", "a3", "a4", "a5", "a6").AddDocumentation("Function7");
    cls.Constructor<int, int, int, int, int, int, int, int>().AddSignature("a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7").AddDocumentation("Function8");
    cls.Constructor<int, int, int, int, int, int, int, int, int>().AddSignature("a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "a8").AddDocumentation("Function9");
    cls.Constructor<int, int, int, int, int, int, int, int, int, int>().AddSignature("a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "a8", "a9").AddDocumentation("Function10");
}

TEST(ConstructorTests, Construct)
{
    TypeLibraryPointer lib = TypeLibraryFactory::CreateDefaultTypeLibrary();
    Reflect<ClassWithConstructors>(lib);
    Object arg(lib, 1);
    Object wrapper(lib, Members::GlobalType());
    EXPECT_EQ(0, wrapper.Lookup("ClassWithConstructors")().As<ClassWithConstructors>().overload);
    EXPECT_EQ(1, wrapper.Lookup("ClassWithConstructors")(arg).As<ClassWithConstructors>().overload);
    EXPECT_EQ(2, wrapper.Lookup("ClassWithConstructors")(arg, arg).As<ClassWithConstructors>().overload);
    EXPECT_EQ(3, wrapper.Lookup("ClassWithConstructors")(arg, arg, arg).As<ClassWithConstructors>().overload);
    EXPECT_EQ(4, wrapper.Lookup("ClassWithConstructors")(arg, arg, arg, arg).As<ClassWithConstructors>().overload);
    EXPECT_EQ(5, wrapper.Lookup("ClassWithConstructors")(arg, arg, arg, arg, arg).As<ClassWithConstructors>().overload);
    EXPECT_EQ(6, wrapper.Lookup("ClassWithConstructors")(arg, arg, arg, arg, arg, arg).As<ClassWithConstructors>().overload);
    EXPECT_EQ(7, wrapper.Lookup("ClassWithConstructors")(arg, arg, arg, arg, arg, arg, arg).As<ClassWithConstructors>().overload);
    EXPECT_EQ(8, wrapper.Lookup("ClassWithConstructors")(arg, arg, arg, arg, arg, arg, arg, arg).As<ClassWithConstructors>().overload);
    EXPECT_EQ(9, wrapper.Lookup("ClassWithConstructors")(arg, arg, arg, arg, arg, arg, arg, arg, arg).As<ClassWithConstructors>().overload);
    EXPECT_EQ(10, wrapper.Lookup("ClassWithConstructors")(arg, arg, arg, arg, arg, arg, arg, arg, arg, arg).As<ClassWithConstructors>().overload);
}

TEST(ConstructorTests, Construct_Check_Argument_Names)
{
    TypeLibraryPointer lib = TypeLibraryFactory::CreateDefaultTypeLibrary();
    Reflect<ClassWithConstructors>(lib);
    Object wrapper(lib, Members::GlobalType());
    Object arg(lib, 1);
    std::vector<Object> args;
    for(size_t i = 0; i <= 10; ++i) {
        Members::MemberWithArgumentsPointer overload = wrapper.PrepareInvoke("ClassWithConstructors", args, Members::MemberType::TypeConstructor);
        ASSERT_TRUE(overload);
        EXPECT_TRUE(overload->IsOk());
        for(int j = 0; j<overload->GetMember()->GetArity(); ++j)
        {
            char dst[20];
            _itoa_s(j, dst, 19, 10);
            std::string name = "a" + std::string(dst);
            EXPECT_EQ(name, overload->GetMember()->GetArgumentInfo(j)->GetName());
        }
        args.push_back(arg);
    }    
}

class ClassWithConstructorsValidation 
{
public:
    ClassWithConstructorsValidation() {overload = 0; }
    ClassWithConstructorsValidation(int){overload = 1; }
    ClassWithConstructorsValidation(int, int){overload = 2; }
    ClassWithConstructorsValidation(int, int, int){overload = 3; }
    ClassWithConstructorsValidation(int, int, int, int){overload = 4; }
    ClassWithConstructorsValidation(int, int, int, int, int){overload = 5; }
    ClassWithConstructorsValidation(int, int, int, int, int, int){overload = 6; }
    ClassWithConstructorsValidation(int, int, int, int, int, int, int){overload = 7; }
    ClassWithConstructorsValidation(int, int, int, int, int, int, int, int){overload = 8; }
    ClassWithConstructorsValidation(int, int, int, int, int, int, int, int, int){overload = 9; }
    ClassWithConstructorsValidation(int, int, int, int, int, int, int, int, int, int){overload = 10; }
    static void Validate0() {}
    static void Validate1(int a0) 
    
    {if(a0<0) 
        throw std::runtime_error("Error"); }
    static void Validate2(int a0, int) 
    
    {if(a0<0) 
        throw std::runtime_error("Error"); }
    static void Validate3(int a0, int, int) 
    
    {if(a0<0) 
        throw std::runtime_error("Error"); }
    static void Validate4(int a0, int, int, int) 
    
    {if(a0<0) 
        throw std::runtime_error("Error"); }
    static void Validate5(int a0, int, int, int, int) 
    
    {if(a0<0) 
        throw std::runtime_error("Error"); }
    static void Validate6(int a0, int, int, int, int, int) 
    
    {if(a0<0) 
        throw std::runtime_error("Error"); }
    static void Validate7(int a0, int, int, int, int, int, int) 
    
    {if(a0<0) 
        throw std::runtime_error("Error"); }
    static void Validate8(int a0, int, int, int, int, int, int, int) 
    
    {if(a0<0) 
        throw std::runtime_error("Error"); }
    static void Validate9(int a0, int, int, int, int, int, int, int, int) 
    
    {if(a0<0) 
        throw std::runtime_error("Error"); }
    static void Validate10(int a0, int, int, int, int, int, int, int, int, int) 
    
    {if(a0<0) 
        throw std::runtime_error("Error"); }
    int overload;
};

void DoReflect(TypeLibraryPointer lib, ClassWithConstructorsValidation**)
{
    Class<ClassWithConstructorsValidation> cls(lib, "ClassWithConstructorsValidation");
    cls.Constructor().AddValidation(&ClassWithConstructorsValidation::Validate0);
    cls.Constructor<int>().AddValidation(&ClassWithConstructorsValidation::Validate1);
    cls.Constructor<int, int>().AddValidation(&ClassWithConstructorsValidation::Validate2);
    cls.Constructor<int, int, int>().AddValidation(&ClassWithConstructorsValidation::Validate3);
    cls.Constructor<int, int, int, int>().AddValidation(&ClassWithConstructorsValidation::Validate4);
    cls.Constructor<int, int, int, int, int>().AddValidation(&ClassWithConstructorsValidation::Validate5);
    cls.Constructor<int, int, int, int, int, int>().AddValidation(&ClassWithConstructorsValidation::Validate6);
    cls.Constructor<int, int, int, int, int, int, int>().AddValidation(&ClassWithConstructorsValidation::Validate7);
    cls.Constructor<int, int, int, int, int, int, int, int>().AddValidation(&ClassWithConstructorsValidation::Validate8);
    cls.Constructor<int, int, int, int, int, int, int, int, int>().AddValidation(&ClassWithConstructorsValidation::Validate9);
    cls.Constructor<int, int, int, int, int, int, int, int, int, int>().AddValidation(&ClassWithConstructorsValidation::Validate10);
}

TEST(ConstructorTests, Construct_Validation)
{
    TypeLibraryPointer lib = TypeLibraryFactory::CreateDefaultTypeLibrary();
    Object wrapper(lib, Members::GlobalType());
    Reflect<ClassWithConstructorsValidation>(lib);
    {
        Object arg(lib, 1);
        EXPECT_EQ(0, wrapper.Lookup("ClassWithConstructorsValidation")().As<ClassWithConstructorsValidation>().overload);
        EXPECT_EQ(1, wrapper.Lookup("ClassWithConstructorsValidation")(arg).As<ClassWithConstructorsValidation>().overload);
        EXPECT_EQ(2, wrapper.Lookup("ClassWithConstructorsValidation")(arg, arg).As<ClassWithConstructorsValidation>().overload);
        EXPECT_EQ(3, wrapper.Lookup("ClassWithConstructorsValidation")(arg, arg, arg).As<ClassWithConstructorsValidation>().overload);
        EXPECT_EQ(4, wrapper.Lookup("ClassWithConstructorsValidation")(arg, arg, arg, arg).As<ClassWithConstructorsValidation>().overload);
        EXPECT_EQ(5, wrapper.Lookup("ClassWithConstructorsValidation")(arg, arg, arg, arg, arg).As<ClassWithConstructorsValidation>().overload);
        EXPECT_EQ(6, wrapper.Lookup("ClassWithConstructorsValidation")(arg, arg, arg, arg, arg, arg).As<ClassWithConstructorsValidation>().overload);
        EXPECT_EQ(7, wrapper.Lookup("ClassWithConstructorsValidation")(arg, arg, arg, arg, arg, arg, arg).As<ClassWithConstructorsValidation>().overload);
        EXPECT_EQ(8, wrapper.Lookup("ClassWithConstructorsValidation")(arg, arg, arg, arg, arg, arg, arg, arg).As<ClassWithConstructorsValidation>().overload);
        EXPECT_EQ(9, wrapper.Lookup("ClassWithConstructorsValidation")(arg, arg, arg, arg, arg, arg, arg, arg, arg).As<ClassWithConstructorsValidation>().overload);
        EXPECT_EQ(10, wrapper.Lookup("ClassWithConstructorsValidation")(arg, arg, arg, arg, arg, arg, arg, arg, arg, arg).As<ClassWithConstructorsValidation>().overload);
    }
    {
        Object arg(lib, -1);
        EXPECT_THROW(wrapper.Lookup("ClassWithConstructorsValidation")(arg), std::runtime_error);
        EXPECT_THROW(wrapper.Lookup("ClassWithConstructorsValidation")(arg, arg), std::runtime_error);
        EXPECT_THROW(wrapper.Lookup("ClassWithConstructorsValidation")(arg, arg, arg), std::runtime_error);
        EXPECT_THROW(wrapper.Lookup("ClassWithConstructorsValidation")(arg, arg, arg, arg), std::runtime_error);
        EXPECT_THROW(wrapper.Lookup("ClassWithConstructorsValidation")(arg, arg, arg, arg, arg), std::runtime_error);
        EXPECT_THROW(wrapper.Lookup("ClassWithConstructorsValidation")(arg, arg, arg, arg, arg, arg), std::runtime_error);
        EXPECT_THROW(wrapper.Lookup("ClassWithConstructorsValidation")(arg, arg, arg, arg, arg, arg, arg), std::runtime_error);
        EXPECT_THROW(wrapper.Lookup("ClassWithConstructorsValidation")(arg, arg, arg, arg, arg, arg, arg, arg), std::runtime_error);
        EXPECT_THROW(wrapper.Lookup("ClassWithConstructorsValidation")(arg, arg, arg, arg, arg, arg, arg, arg, arg), std::runtime_error);
        EXPECT_THROW(wrapper.Lookup("ClassWithConstructorsValidation")(arg, arg, arg, arg, arg, arg, arg, arg, arg, arg), std::runtime_error);
    }
}
TEST(ConstructorTests, Construct_Validation_Call_Validate)
{
    TypeLibraryPointer lib = TypeLibraryFactory::CreateDefaultTypeLibrary();
    Object wrapper(lib, Members::GlobalType());
    Reflect<ClassWithConstructorsValidation>(lib);
    Object arg(lib, -1);
    std::vector<Object> args;
    for(size_t i = 0; i <= 10; ++i) {
        Members::MemberWithArgumentsPointer overload = wrapper.PrepareInvoke("ClassWithConstructorsValidation", args, Members::MemberType::TypeConstructor);
        ASSERT_TRUE(overload);
        if(i>0) EXPECT_THROW(overload->Validate(), std::runtime_error);
        else EXPECT_NO_THROW(overload->Validate());
        args.push_back(arg);
    }
}

class ClassWithConstructorFunctions 
{
public:
    static int count;
    ClassWithConstructorFunctions(int overload_in) {overload = overload_in; ++count; }
    ~ClassWithConstructorFunctions() {--count; }
    static ClassWithConstructorFunctions* Create(){ return new ClassWithConstructorFunctions(0); }
    static ClassWithConstructorFunctions* Create1(int){ return new ClassWithConstructorFunctions(1); }
    static ClassWithConstructorFunctions* Create(int, int){ return new ClassWithConstructorFunctions(2); }
    static ClassWithConstructorFunctions* Create(int, int, int){ return new ClassWithConstructorFunctions(3); }
    static ClassWithConstructorFunctions* Create(int, int, int, int){ return new ClassWithConstructorFunctions(4); }
    static ClassWithConstructorFunctions* Create(int, int, int, int, int){ return new ClassWithConstructorFunctions(5); }
    static ClassWithConstructorFunctions* Create(int, int, int, int, int, int){ return new ClassWithConstructorFunctions(6); }
    static ClassWithConstructorFunctions* Create(int, int, int, int, int, int, int){ return new ClassWithConstructorFunctions(7); }
    static ClassWithConstructorFunctions* Create(int, int, int, int, int, int, int, int){ return new ClassWithConstructorFunctions(8); }
    static ClassWithConstructorFunctions* Create(int, int, int, int, int, int, int, int, int){ return new ClassWithConstructorFunctions(9); }
    static ClassWithConstructorFunctions* Create(int, int, int, int, int, int, int, int, int, int){ return new ClassWithConstructorFunctions(10); }
    int overload;
};

void DoReflect(TypeLibraryPointer lib, ClassWithConstructorFunctions**)
{
    Class<ClassWithConstructorFunctions, std::shared_ptr<ClassWithConstructorFunctions> > cls(lib, "ClassWithConstructorFunctions");
    cls.Constructor<ClassWithConstructorFunctions*()>(ClassWithConstructorFunctions::Create);
    cls.Constructor(ClassWithConstructorFunctions::Create1);
    cls.Constructor<ClassWithConstructorFunctions*(int, int)>(ClassWithConstructorFunctions::Create);
    cls.Constructor<ClassWithConstructorFunctions*(int, int, int)>(ClassWithConstructorFunctions::Create);
    cls.Constructor<ClassWithConstructorFunctions*(int, int, int, int)>(ClassWithConstructorFunctions::Create);
    cls.Constructor<ClassWithConstructorFunctions*(int, int, int, int, int)>(ClassWithConstructorFunctions::Create);
    cls.Constructor<ClassWithConstructorFunctions*(int, int, int, int, int, int)>(ClassWithConstructorFunctions::Create);
    cls.Constructor<ClassWithConstructorFunctions*(int, int, int, int, int, int, int)>(ClassWithConstructorFunctions::Create);
    cls.Constructor<ClassWithConstructorFunctions*(int, int, int, int, int, int, int, int)>(ClassWithConstructorFunctions::Create);
    cls.Constructor<ClassWithConstructorFunctions*(int, int, int, int, int, int, int, int, int)>(ClassWithConstructorFunctions::Create);
    cls.Constructor<ClassWithConstructorFunctions*(int, int, int, int, int, int, int, int, int, int)>(ClassWithConstructorFunctions::Create);
}

int ClassWithConstructorFunctions::count = 0;
TEST(ConstructorTests, Constructor_Function)
{
    TypeLibraryPointer lib = TypeLibraryFactory::CreateDefaultTypeLibrary();
    Object wrapper(lib, Members::GlobalType());
    Reflect<ClassWithConstructorFunctions>(lib);
    Object arg(lib, -1);
    std::vector<Object> args;
    for(size_t i = 0; i <= 10; ++i) 
    {
        Members::MemberWithArgumentsPointer overload = wrapper.PrepareInvoke("ClassWithConstructorFunctions", args, Members::MemberType::TypeConstructor);
        ASSERT_TRUE(overload);
        EXPECT_EQ((int)i, Object(lib, overload->Invoke()).As<const ClassWithConstructorFunctions&>().overload);
        args.push_back(arg);
    }
    EXPECT_EQ(0, ClassWithConstructorFunctions::count);
}

class ClassWithConstructorFunctionsVararg 
{
public:
    static int count;
    ClassWithConstructorFunctionsVararg(int overload_in, const std::list<std::string>& strings_in) {overload = overload_in; strings = strings_in; ++count; }
    ~ClassWithConstructorFunctionsVararg() {--count; }
    static ClassWithConstructorFunctionsVararg* Create(const std::list<std::string>& strings){ return new ClassWithConstructorFunctionsVararg(0, strings); }
    static ClassWithConstructorFunctionsVararg* Create1(int, const std::list<std::string>& strings){ return new ClassWithConstructorFunctionsVararg(1, strings); }
    static ClassWithConstructorFunctionsVararg* Create(int, int, const std::list<std::string>& strings){ return new ClassWithConstructorFunctionsVararg(2, strings); }
    static ClassWithConstructorFunctionsVararg* Create(int, int, int, const std::list<std::string>& strings){ return new ClassWithConstructorFunctionsVararg(3, strings); }
    static ClassWithConstructorFunctionsVararg* Create(int, int, int, int, const std::list<std::string>& strings){ return new ClassWithConstructorFunctionsVararg(4, strings); }
    static ClassWithConstructorFunctionsVararg* Create(int, int, int, int, int, const std::list<std::string>& strings){ return new ClassWithConstructorFunctionsVararg(5, strings); }
    static ClassWithConstructorFunctionsVararg* Create(int, int, int, int, int, int, const std::list<std::string>& strings){ return new ClassWithConstructorFunctionsVararg(6, strings); }
    static ClassWithConstructorFunctionsVararg* Create(int, int, int, int, int, int, int, const std::list<std::string>& strings){ return new ClassWithConstructorFunctionsVararg(7, strings); }
    static ClassWithConstructorFunctionsVararg* Create(int, int, int, int, int, int, int, int, const std::list<std::string>& strings){ return new ClassWithConstructorFunctionsVararg(8, strings); }
    static ClassWithConstructorFunctionsVararg* Create(int, int, int, int, int, int, int, int, int, const std::list<std::string>& strings){ return new ClassWithConstructorFunctionsVararg(9, strings); }
    static ClassWithConstructorFunctionsVararg* Create(int, int, int, int, int, int, int, int, int, int, const std::list<std::string>& strings){ return new ClassWithConstructorFunctionsVararg(10, strings); }
    int overload;
    std::list<std::string> strings;
};
int ClassWithConstructorFunctionsVararg::count = 0;

void DoReflect(TypeLibraryPointer lib, ClassWithConstructorFunctionsVararg**)
{
    Class<ClassWithConstructorFunctionsVararg, std::shared_ptr<ClassWithConstructorFunctionsVararg> > cls(lib, "ClassWithConstructorFunctionsVararg");
    cls.Constructor<ClassWithConstructorFunctionsVararg*(const std::list<std::string>&)>(ClassWithConstructorFunctionsVararg::Create, Vararg);
    cls.Constructor(ClassWithConstructorFunctionsVararg::Create1, Vararg);
    cls.Constructor<ClassWithConstructorFunctionsVararg*(int, int, const std::list<std::string>&)>(ClassWithConstructorFunctionsVararg::Create, Vararg);
    cls.Constructor<ClassWithConstructorFunctionsVararg*(int, int, int, const std::list<std::string>&)>(ClassWithConstructorFunctionsVararg::Create, Vararg);
    cls.Constructor<ClassWithConstructorFunctionsVararg*(int, int, int, int, const std::list<std::string>&)>(ClassWithConstructorFunctionsVararg::Create, Vararg);
    cls.Constructor<ClassWithConstructorFunctionsVararg*(int, int, int, int, int, const std::list<std::string>&)>(ClassWithConstructorFunctionsVararg::Create, Vararg);
    cls.Constructor<ClassWithConstructorFunctionsVararg*(int, int, int, int, int, int, const std::list<std::string>&)>(ClassWithConstructorFunctionsVararg::Create, Vararg);
    cls.Constructor<ClassWithConstructorFunctionsVararg*(int, int, int, int, int, int, int, const std::list<std::string>&)>(ClassWithConstructorFunctionsVararg::Create, Vararg);
    cls.Constructor<ClassWithConstructorFunctionsVararg*(int, int, int, int, int, int, int, int, const std::list<std::string>&)>(ClassWithConstructorFunctionsVararg::Create, Vararg);
    cls.Constructor<ClassWithConstructorFunctionsVararg*(int, int, int, int, int, int, int, int, int, const std::list<std::string>&)>(ClassWithConstructorFunctionsVararg::Create, Vararg);
    cls.Constructor<ClassWithConstructorFunctionsVararg*(int, int, int, int, int, int, int, int, int, int, const std::list<std::string>&)>(ClassWithConstructorFunctionsVararg::Create, Vararg);
}

TEST(ConstructorTests, Constructor_Function_Vararg)
{
    TypeLibraryPointer lib = TypeLibraryFactory::CreateDefaultTypeLibrary();
    Object wrapper(lib, Members::GlobalType());
    Reflect<ClassWithConstructorFunctionsVararg>(lib);
    Object arg(lib, -1);
    std::vector<Object> args;
    args.push_back(Object(lib, std::string("Hello world")));
    for(size_t i = 0; i <= 10; ++i) {
        Members::MemberWithArgumentsPointer overload = wrapper.PrepareInvoke("ClassWithConstructorFunctionsVararg", args, Members::MemberType::TypeConstructor);
        ASSERT_TRUE(overload);
        EXPECT_EQ((int)i, Object(lib, overload->Invoke()).As<const ClassWithConstructorFunctionsVararg&>().overload);
        args.insert(args.begin(), arg);
        args.push_back(Object(lib, std::string("Next argument")));
    }
    EXPECT_EQ(0, ClassWithConstructorFunctionsVararg::count);
}

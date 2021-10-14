//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"

#include "Reflection/Classes/Class.h"

#include "Reflection/Members/GlobalType.h"

#include "Reflection/Objects/Object.h"
#include "Reflection/Objects/Delegate.h"

#include "Reflection/TypeConversions/BuiltInConversions.h"

#include "Reflection/TypeLibraries/TypeLibraryFactory.h"
#include <Reflection/Containers/ReflectList.h>
#include <Reflection/Containers/ReflectSet.h>
#include <numeric>
#include <iostream>
#include <list>

using namespace DNVS::MoFa::Reflection;
using namespace DNVS::MoFa::Reflection::TypeLibraries;
using namespace DNVS::MoFa::Reflection::TypeConversions;
using namespace DNVS::MoFa::Reflection::Classes;
using namespace DNVS::MoFa::Reflection::Objects;
using namespace DNVS::MoFa::Reflection::Types;
using namespace DNVS::MoFa::Reflection::TypeLibraries;


class ClassTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m_lib = TypeLibraryFactory::CreateDefaultTypeLibrary();
    }

    TypeLibraryPointer m_lib;
};

class BaseClass 
{

};

void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, BaseClass**)
{
    Class<BaseClass> cls(typeLibrary, "BaseClass");
}

class ClassTestClass : public BaseClass 
{
public:
    ClassTestClass(int) {}
    ClassTestClass(int, double) {}
    void OverloadedFunction(int) {}
    void OverloadedFunction(double) {}
};

void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, ClassTestClass**)
{
    Class<ClassTestClass, Public<BaseClass>> cls(typeLibrary, "ClassTestClass");
    cls.Constructor<int>();
    cls.Constructor<int, double>();
    cls.Function("OverloadedFunction", (void (ClassTestClass::*)(int))&ClassTestClass::OverloadedFunction);
    cls.Function("OverloadedFunction", (void (ClassTestClass::*)(double))&ClassTestClass::OverloadedFunction);
}

TEST_F(ClassTest, RegisterConstructors)
{
    Reflect<ClassTestClass>(m_lib);
}

TEST_F(ClassTest, RegisterOverloadedFunction)
{
    Reflect<ClassTestClass>(m_lib);
    ClassTestClass test(4);
    Object var(Object::Create<ClassTestClass*>(m_lib, &test));
    EXPECT_NO_THROW(Delegate d = var.Lookup("OverloadedFunction"));
}

class ConstructorBase1 
{
public:
    virtual ~ConstructorBase1() {}
    int a;
};

void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, ConstructorBase1** dummy)
{
    Class<ConstructorBase1> cls(typeLibrary, "ConstructorBase1");
}

class ConstructorBase2 
{
public:
    virtual ~ConstructorBase2() {}
    int b;
};

void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, ConstructorBase2** dummy)
{
    Class<ConstructorBase2> cls(typeLibrary, "ConstructorBase2");
}

class ConstructorClass : public ConstructorBase1, public ConstructorBase2
{
public:
    ConstructorClass() {}
    virtual ~ConstructorClass() {}
};

void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, ConstructorClass** dummy)
{
    Class<ConstructorClass, Public<ConstructorBase1, ConstructorBase2> >(typeLibrary, "ConstructorClass")
        .Constructor();
}
TEST_F(ClassTest, TestConstructor)
{
    Reflect<ConstructorClass>(m_lib);

    Object wrapper(m_lib, Members::GlobalType());
    Object var = wrapper.Lookup("ConstructorClass")();
    ConstructorBase1* base1 = var.As<ConstructorBase1*>();
    ConstructorBase2* base2 = var.As<ConstructorBase2*>();
    EXPECT_NE(base1, nullptr);
    EXPECT_NE(base2, nullptr);    
    EXPECT_NE(static_cast<void*>(base1), static_cast<void*>(base2));
}

class ConstructorClassWithArguments 
{
public:
    ConstructorClassWithArguments(int intValue) : m_intValue(intValue) {}
    int IntValue() const {return m_intValue; }
    void IntValue(int intValue) {m_intValue = intValue; }
private:
    int m_intValue;
};
void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, ConstructorClassWithArguments**)
{
    Class<ConstructorClassWithArguments, std::shared_ptr<ConstructorClassWithArguments> > cls(typeLibrary, "ConstructorClassWithArguments");
    cls.Constructor<int>();
    cls.Get("IntValue", &ConstructorClassWithArguments::IntValue);
    cls.Set("IntValue", &ConstructorClassWithArguments::IntValue);
}

TEST_F(ClassTest, TestConstructorWithArguments)
{
    Reflect<ConstructorClassWithArguments>(m_lib);
    Object wrapper(m_lib, Members::GlobalType());
    Object var = wrapper.Lookup("ConstructorClassWithArguments")(42);
    EXPECT_EQ(42, var.Lookup("IntValue").As<int>());
    var.Lookup("IntValue") = Object(m_lib, 43);
    EXPECT_EQ(43, var.Lookup("IntValue").As<int>());
}

class ConstructorWithOverloads 
{
public:
    ConstructorWithOverloads(double a0) : m_intValue(0), m_doubleValue(a0) {}
    ConstructorWithOverloads(int a0) :m_intValue(a0), m_doubleValue(0) {}
    int IntValue() const {return m_intValue; }
    void IntValue(int value) {m_intValue = value; }
    const double& DoubleValue() const {return m_doubleValue; }
    void DoubleValue(const double& value) {m_doubleValue = value; }
    void Execute(int arg) {}
private:
    int m_intValue;
    double m_doubleValue;
};

void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, ConstructorWithOverloads**)
{
    Class<ConstructorWithOverloads> cls(typeLibrary, "ConstructorWithOverloads");
    cls.Constructor<int>();
    cls.Constructor<double>();
    cls.Get("IntValue", &ConstructorWithOverloads::IntValue);
    cls.Set("IntValue", &ConstructorWithOverloads::IntValue);
    cls.Get("DoubleValue", &ConstructorWithOverloads::DoubleValue);
    cls.Set("DoubleValue", &ConstructorWithOverloads::DoubleValue);
    cls.Function("Execute", &ConstructorWithOverloads::Execute);
}

TEST_F(ClassTest, TestConstructorWithOverloads)
{
    AddBuiltInConversions(m_lib->GetConversionGraph());
    Reflect<ConstructorWithOverloads>(m_lib);
    Object wrapper(m_lib, Members::GlobalType());
    Object intClass = wrapper.Lookup("ConstructorWithOverloads")(42);
    EXPECT_EQ(42, intClass.Lookup("IntValue").As<int>());
    EXPECT_DOUBLE_EQ(0.0, intClass.Lookup("DoubleValue").As<double>());
    Object doubleClass = wrapper.Lookup("ConstructorWithOverloads")(98.4);
    EXPECT_EQ(0, doubleClass.Lookup("IntValue").As<int>());
    EXPECT_DOUBLE_EQ(98.4, doubleClass.Lookup("DoubleValue").As<double>());
    ConstructorWithOverloads* cls = doubleClass.As<ConstructorWithOverloads*>();
}

class ClassWithVarargFunctions 
{
public:
    ClassWithVarargFunctions() {}
    void AddValues(const std::list<double>& values) 
    {
        m_result = 0;
        m_result = std::accumulate(values.begin(), values.end(), m_result);
    }
    double Result() const {return m_result; }
private:
    double m_result;
};

void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, ClassWithVarargFunctions**)
{
    Class<ClassWithVarargFunctions> cls(typeLibrary, "ClassWithVarargFunctions");
    cls.Constructor();
    cls.Function("AddValues", &ClassWithVarargFunctions::AddValues, Vararg);
    cls.Get("Result", &ClassWithVarargFunctions::Result);
}

TEST_F(ClassTest, TestMemberFunctionVararg)
{
    AddBuiltInConversions(m_lib->GetConversionGraph());
    Reflect<ClassWithVarargFunctions>(m_lib);
    Object wrapper(m_lib, Members::GlobalType());
    Object cls = wrapper.Lookup("ClassWithVarargFunctions")();
    cls.Lookup("AddValues")(1.3, 4.5, 6.9);
    EXPECT_DOUBLE_EQ(1.3 + 4.5 + 6.9, cls.Lookup("Result").As<double>());
}

class ClassWithVarargOverloadedFunctions 
{
public:
    int TestFunction(int a, const std::set<double>& args) 
    {
        return 1;
    }
    int TestFunction(double a, int b) 
    {
        return 2;
    }
private:
};

void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, ClassWithVarargOverloadedFunctions**)
{
    Class<ClassWithVarargOverloadedFunctions> cls(typeLibrary, "ClassWithVarargOverloadedFunctions");
    cls.Function("TestFunction", (int (ClassWithVarargOverloadedFunctions::*)(int, const std::set<double>&))&ClassWithVarargOverloadedFunctions::TestFunction, Vararg);
    cls.Function("TestFunction", (int (ClassWithVarargOverloadedFunctions::*)(double, int))&ClassWithVarargOverloadedFunctions::TestFunction);
}

TEST_F(ClassTest, TestOverloadedMemberFunctionVararg)
{
    AddBuiltInConversions(m_lib->GetConversionGraph());
    Reflect<ClassWithVarargOverloadedFunctions>(m_lib);
    Object wrapper(m_lib, Members::GlobalType());

    ClassWithVarargOverloadedFunctions cls;
    Object var = Object::Create<ClassWithVarargOverloadedFunctions&>(m_lib, cls);
    EXPECT_EQ(2, var.Lookup("TestFunction")(1, 4.5).As<int>());
    EXPECT_EQ(1, var.Lookup("TestFunction")(1.2, 1, 5.6).As<int>());
}

TEST_F(ClassTest, TestCast)
{
    Class<BaseClass>(m_lib, "BaseClass");
    Class<ClassTestClass, Public<BaseClass> >(m_lib, "ClassTestClass");
    ConversionSequencePointer conversionSequence = m_lib->GetConversionGraph()->GetConversionSequence(TypeId<ClassTestClass*>(), TypeId<BaseClass*>());
    ASSERT_NE(conversionSequence, nullptr);
    EXPECT_TRUE(conversionSequence->IsValid());
}

class ClassWithOperators 
{
public:
    ClassWithOperators(const double& value) : m_value(value) {}
    const double& Value() const {return m_value; }
    ClassWithOperators operator+(const ClassWithOperators& other) const 
    {
        ClassWithOperators cls(m_value + other.m_value);
        return cls;
    }
    friend double operator+(const double& lhs, const ClassWithOperators& rhs) 
    {
        return lhs + rhs.Value();
    }
private:
    double m_value;
};

void DoReflect(TypeLibraryPointer typeLibrary, ClassWithOperators**)
{
    Class<ClassWithOperators> cls(typeLibrary, "ClassWithOperators");
    cls.Constructor<const double&>();
    cls.Operator(This.Const + This.Const);
    cls.Operator(double() + This.Const);
    cls.Get("Value", &ClassWithOperators::Value);
}

TEST_F(ClassTest, TestOperatorOverload)
{
    AddBuiltInConversions(m_lib->GetConversionGraph());
    Reflect<ClassWithOperators>(m_lib);
    Object wrapper(m_lib, Members::GlobalType());
    Object a = wrapper.Lookup("ClassWithOperators")(5.6);
    Object b = wrapper.Lookup("ClassWithOperators")(7.8);
    EXPECT_EQ(5.6 + 7.8, (a + b).Lookup("Value").As<double>());
    EXPECT_EQ(9.1 + 5.6, (Object(m_lib, 9.1) + a).As<double>());
}

class ClassWithArguments 
{
public:
    int Function(int test = 3) const {return test; }
};

void DoReflect(TypeLibraryPointer typeLibrary, ClassWithArguments**)
{
    Class<ClassWithArguments> cls(typeLibrary, "ClassWithArguments");
    cls.Function("Function", &ClassWithArguments::Function)
        .AddSignature(Arg("test") = 3);
}

TEST_F(ClassTest, TestDefaultArguments)
{
    Reflect<ClassWithArguments>(m_lib);
    Object wrapper(m_lib, Members::GlobalType());
    ClassWithArguments args;
    Object wrap = Object::Create<const ClassWithArguments&>(m_lib, args);
    EXPECT_EQ(args.Function(), wrap.Lookup("Function")().As<int>());
    EXPECT_EQ(args.Function(5), wrap.Lookup("Function")(5).As<int>());
    TypePointer type = m_lib->LookupType(wrap.GetVariant().GetDecoratedTypeInfo().GetTypeInfo());
    ASSERT_NE(type, nullptr);
    EXPECT_TRUE(type->Lookup("Function")->GetArgumentInfo(1)->HasDefault());
    EXPECT_EQ(3, Object(m_lib, type->Lookup("Function")->GetArgumentInfo(1)->GetDefault()).As<int>());
}

class ClassWithConversion 
{
public:
    ClassWithConversion(const double& var) : m_data(var) {}
    operator const double&() const {return m_data; }
private:
    double m_data;
};

void DoReflect(TypeLibraryPointer typeLibrary, ClassWithConversion**)
{
    Class<ClassWithConversion>(typeLibrary, "ClassWithConversion")
        .Constructor<const double&>()
        ;
    AddUserConversion<const ClassWithConversion&, const double&>(typeLibrary->GetConversionGraph());
}
TEST_F(ClassTest, TestImplicitConversion)
{
    AddBuiltInConversions(m_lib->GetConversionGraph());
    Reflect<ClassWithConversion>(m_lib);
    Object wrapper(m_lib, Members::GlobalType());
    Object var = wrapper.Lookup("ClassWithConversion")(5.6);
    EXPECT_EQ(5.6, var.As<const double&>());
    EXPECT_EQ(5.6, var.As<double>());
}

class MyFirstClass 
{
public:
    MyFirstClass(const double& arg) : m_arg(arg) {}
    const double& Value() const {return m_arg; }
private:
    double m_arg;
};

void DoReflect(TypeLibraryPointer typeLibrary, MyFirstClass**)
{
    Class<MyFirstClass, std::shared_ptr<MyFirstClass> > cls(typeLibrary, "MyFirstClass");
    cls.Constructor<const double&>();
    cls.Get("Value", &MyFirstClass::Value);
}

class MyOtherClass 
{
public:
    MyOtherClass() {}
    double Test(const MyFirstClass& cls) const 
    {
        return cls.Value();
    }
};

void DoReflect(TypeLibraryPointer typeLibrary, MyOtherClass**)
{
    Class<MyOtherClass, std::shared_ptr<MyOtherClass> > cls(typeLibrary, "MyOtherClass");
    cls.Constructor();
    cls.Function("Test", &MyOtherClass::Test);
}

TEST_F(ClassTest, TestConversionByConstructor)
{
    AddBuiltInConversions(m_lib->GetConversionGraph());
    Reflect<MyFirstClass>(m_lib);
    Reflect<MyOtherClass>(m_lib);
    Object wrapper(m_lib, Members::GlobalType());

    MyOtherClass cls;
    Object var(m_lib, cls);
    EXPECT_EQ(9.67, var.Lookup("Test")(9.67).As<double>());
}

class ClassWithMultiArgumentFunction 
{
public:
    ClassWithMultiArgumentFunction() {}
    double Add(int a0, int a1, const double& a2, const double& a3 = 7.7, float a4 = 6.7f) const 
    {
        return a0 + a1 + a2 + a3 + a4;
    }
};

void DoReflect(TypeLibraryPointer typeLibrary, ClassWithMultiArgumentFunction**)
{
    Class<ClassWithMultiArgumentFunction> cls(typeLibrary, "ClassWithMultiArgumentFunction");
    cls.Constructor();
    cls.Function("Add", &ClassWithMultiArgumentFunction::Add)
        .AddSignature("a0", "a1", "a2", Arg("a3") = 7.7, Arg("a4") = 6.7f)
        ;
}

TEST_F(ClassTest, MemberFunctionWithMultipleArguments)
{
    AddBuiltInConversions(m_lib->GetConversionGraph());
    Reflect<ClassWithMultiArgumentFunction>(m_lib);
    Object wrapper(m_lib, Members::GlobalType());

    ClassWithMultiArgumentFunction cls;
    Object var(m_lib, cls);
    EXPECT_EQ(cls.Add(10, 23, 4.5, 63.21, 4.44f), var.Lookup("Add")(10, 23, 4.5, 63.21, 4.44f).As<double>());
    EXPECT_EQ(cls.Add(10, 23, 4.5), var.Lookup("Add")(10, 23, 4.5).As<double>());
}

class LoopClassTestClass 
{
public:
    double Multiply(const double& a, const double& b) const 
    {
        return a*b;
    }
};

void DoReflect(TypeLibraryPointer typeLibrary, LoopClassTestClass**)
{
    Class<LoopClassTestClass>(typeLibrary, "LoopClassTestClass")
        .Function("Multiply", &LoopClassTestClass::Multiply)
        ;
}
TEST_F(ClassTest, TestLoopPerformance)
{
    AddBuiltInConversions(m_lib->GetConversionGraph());
    Object wrapper(m_lib, Members::GlobalType());
    Reflect<LoopClassTestClass>(m_lib);

#ifdef _DEBUG
    size_t MaxIterations = 100;
#else
    size_t MaxIterations = 100000;
#endif
    ::clock_t start = clock();
    LoopClassTestClass cls;
    double result = 1.0e-10;
    for(size_t i = 0; i<MaxIterations; ++i) {
        result = cls.Multiply(1, result);
    }
    ::clock_t end = clock();
    double result2 = 1.0e-10;
    std::cout << "Native time: " << double(end-start) / CLOCKS_PER_SEC << std::endl;
    {
        start = clock();
        Object var(m_lib, cls);
        for(size_t i = 0; i<MaxIterations; ++i) {
            result2 = var.Lookup("Multiply")(1, result2).As<double>();
        }
        end = clock();
        std::cout << "Reflected time: " << double(end-start) / CLOCKS_PER_SEC << std::endl;

    }
    EXPECT_DOUBLE_EQ(result, result2);
}
namespace std {
    static void DoReflect(TypeLibraryPointer typeLibrary, std::vector<double>**)
    {
        Class<std::vector<double> > cls(typeLibrary, "VectorOfDouble");
        cls.Constructor();
        cls.Function<void(size_t)>("resize", &std::vector<double>::resize);
        cls.Operator(This[size_t()]);
        cls.Operator(This.Const[size_t()]);
        cls.Function<void(const double&)>("push_back", &std::vector<double>::push_back);
    }
}
TEST_F(ClassTest, TestVector)
{
    AddBuiltInConversions(m_lib->GetConversionGraph());
    Reflect<std::vector<double>>(m_lib);
    Class<double>(m_lib, "Double")
        .Operator(This = This.Const)
        ;

    Object wrapper(m_lib, Members::GlobalType());
    Object v = wrapper.Lookup("VectorOfDouble")();
    v.Lookup("resize")(100);
    for(size_t i = 0; i<100; ++i) {
        v[Object(m_lib, i)].Assign(Object(m_lib, i*0.001));
    }
    for(size_t i = 0; i<100; ++i) {
        EXPECT_DOUBLE_EQ(i*0.001, v[Object(m_lib, i)].As<double>());
    }
}


class Shape 
{
public:
    virtual ~Shape() {}
    virtual double Area() const = 0;
    double Extrude(const double& height) const
    {
        return Area()*height;
    }
};

void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, Shape**)
{
    Class<Shape, std::shared_ptr<Shape> > cls(typeLibrary, "Shape");
    cls.Function("Area", &Shape::Area);
    cls.Function("Extrude", &Shape::Extrude);
}

class Circle : public Shape 
{
public:
    Circle(const double& radius) : m_radius(radius)
    {}
    virtual double Area() const 
    {
        return m_radius*m_radius*3.14159;
    }
private:
    double m_radius;
};

void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, Circle**)
{
    Class<Circle, Public<Shape>, std::shared_ptr<Circle>> cls(typeLibrary, "Circle");
    cls.Constructor<const double&>();
}

TEST_F(ClassTest, CirclePerformance)
{
    AddBuiltInConversions(m_lib->GetConversionGraph());
    Reflect<Circle>(m_lib);
#ifdef _DEBUG
    size_t MaxIterations = 100;
#else
    size_t MaxIterations = 100000;
#endif
    Object wrapper(m_lib, Members::GlobalType());
    ::clock_t start = clock();
    Object p = wrapper.Lookup("Circle")(6.87);
    for(size_t i = 0; i<MaxIterations; ++i) {
        p.Lookup("Extrude")(i);
    }
    ::clock_t end = clock();
    std::cout << "Reflected time: " << double(end-start) / CLOCKS_PER_SEC << std::endl;
}

class FunctionWithSimpleOverloads 
{
public:
    void Test(int) {}
    int Test() {return 1; }
    int Test() const {return 2; }
    void Test(double) const {}
};

void DoReflect(TypeLibraryPointer typeLibrary, FunctionWithSimpleOverloads**)
{
    Class<FunctionWithSimpleOverloads> cls(typeLibrary, "FunctionWithSimpleOverloads");
    cls.Function<void(int)>("Test", &FunctionWithSimpleOverloads::Test);
    cls.Function<int()>("Test", &FunctionWithSimpleOverloads::Test);
    cls.Function<int(), Const>("Test", &FunctionWithSimpleOverloads::Test);
    cls.Function<void(double), Const>("Test", &FunctionWithSimpleOverloads::Test);
}

TEST_F(ClassTest, TestElegantFunctionOverload)
{
    Reflect<FunctionWithSimpleOverloads>(m_lib);
}

class ClassTestClassA 
{
public:
    virtual ~ClassTestClassA() {}
    virtual ClassTestClassA* GetThis() = 0;
};

void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, ClassTestClassA**)
{
    Class<ClassTestClassA, std::shared_ptr<ClassTestClassA>>(typeLibrary, "ClassTestClassA")
        .Function("GetThis", &ClassTestClassA::GetThis)
        ;
}

class ClassTestClassB : public ClassTestClassA 
{
public:
    virtual ~ClassTestClassB() {}
    virtual ClassTestClassA* GetThis() {return this; }
};

void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, ClassTestClassB**)
{
    Class<ClassTestClassB, Public<ClassTestClassA>, std::shared_ptr<ClassTestClassB>>(typeLibrary, "ClassTestClassB")
        .Constructor()
        ;
}

TEST_F(ClassTest, TestVirtualConstructor)
{
    DNVS::MoFa::Reflection::Reflect<ClassTestClassB>(m_lib);
    ClassTestClassB classTestClass;
    Object v = Object::Create<ClassTestClassB&>(m_lib, classTestClass);
    EXPECT_TRUE(v.IsConvertibleTo<ClassTestClassB*>());
    Object result = v.Lookup("GetThis")();
    EXPECT_TRUE(result.IsConvertibleTo<ClassTestClassB*>());    
}

class ClassWithVarargConstructor 
{
public:
    static void ClassWithVarargConstructorConstraints(int a, int b, const double& c, size_t number, std::list<bool>& args)
    {
        if(args.size() != number) 
            throw std::runtime_error("number must be the same as the number of vararg arguments");
    }
    ClassWithVarargConstructor(int a, int b, const double& c, size_t number, std::list<bool>& args) {EXPECT_EQ(number, args.size()); }
};

void DoReflect(TypeLibraryPointer lib, ClassWithVarargConstructor**)
{
    Class<ClassWithVarargConstructor, std::shared_ptr<ClassWithVarargConstructor> > cls(lib, "ClassWithVarargConstructor");
    cls.Constructor<int, int, const double&, size_t, std::list<bool> >(Vararg)
        .AddSignature("a", "b", Arg("c") = 3.4, Arg("number") = 0)
        .AddValidation(&ClassWithVarargConstructor::ClassWithVarargConstructorConstraints);

}


TEST_F(ClassTest, VarargConstructor)
{
    Reflect<ClassWithVarargConstructor>(m_lib);
    AddBuiltInConversions(m_lib->GetConversionGraph());
    Object wrapper(m_lib, Members::GlobalType());
    EXPECT_NO_THROW(Object v = wrapper.Lookup("ClassWithVarargConstructor")(1, 5));
    EXPECT_NO_THROW(Object u = wrapper.Lookup("ClassWithVarargConstructor")(1, 5, 3.1, 4, true, false, true, true));
}

TEST_F(ClassTest, VarargConstructorWithConstraints)
{
    Reflect<ClassWithVarargConstructor>(m_lib);
    AddBuiltInConversions(m_lib->GetConversionGraph());
    Object wrapper(m_lib, Members::GlobalType());
    EXPECT_THROW(Object v = wrapper.Lookup("ClassWithVarargConstructor")(1, 5, 3.1, 2, false), std::runtime_error);
}

class InClassRegistrationTester 
{
public:
    InClassRegistrationTester(int a);
    void TestFunction(int, int, std::set<double>&);
    int TestFunction(double) const;
    InClassRegistrationTester operator+(const double&);
};

void DoReflect(TypeLibraryPointer lib, InClassRegistrationTester**)
{
    Class<InClassRegistrationTester, std::shared_ptr<InClassRegistrationTester> > cls(lib, "InClassRegistrationTester");
    cls.Constructor<int>();
    cls.Function<void(int, int, std::set<double>&)>("TestFunction", &InClassRegistrationTester::TestFunction, Vararg);
    cls.Function<int(double), Const>("TestFunction", &InClassRegistrationTester::TestFunction);
    cls.Operator(This + double());
}


InClassRegistrationTester::InClassRegistrationTester(int a)
{

}

void InClassRegistrationTester::TestFunction(int, int, std::set<double>&)
{

}

int InClassRegistrationTester::TestFunction(double) const
{
    return 0;
}


InClassRegistrationTester InClassRegistrationTester::operator+(const double& a)
{
    return InClassRegistrationTester((int)a);
}

TEST_F(ClassTest, InClassRegistration)
{
    Reflect<InClassRegistrationTester>(m_lib);
    AddBuiltInConversions(m_lib->GetConversionGraph());
    Object wrapper(m_lib, Members::GlobalType());

    Object v = wrapper.Lookup("InClassRegistrationTester")(5);
    v.Lookup("TestFunction")(1, 2, 5.6, 4.7);
}

class RefCountedClass 
{
public:
    RefCountedClass() {m_count++; }
    ~RefCountedClass() {m_count--; }
    static int m_count;
};

int RefCountedClass::m_count = 0;

TEST_F(ClassTest, MemoryLeakTest)
{
    Reflect<InClassRegistrationTester>(m_lib);
    AddBuiltInConversions(m_lib->GetConversionGraph());

    Object wrapper(m_lib, Members::GlobalType());
    Class<RefCountedClass, std::shared_ptr<RefCountedClass> >(m_lib, "RefCountedClass")
        .Constructor()
        ;

    EXPECT_EQ(0, RefCountedClass::m_count);
    RefCountedClass cls;
    EXPECT_EQ(1, RefCountedClass::m_count);
    Object v = wrapper.Lookup("RefCountedClass")();
    EXPECT_EQ(2, RefCountedClass::m_count);
    v.Reset();
    EXPECT_EQ(1, RefCountedClass::m_count);
}

class MyClassTestClass 
{
public:
    MyClassTestClass(double a, double b) {}
};

void DoReflect(TypeLibraryPointer lib, MyClassTestClass**)
{
    Class<MyClassTestClass> cls(lib, "MyClassTestClass");
    cls.Constructor<double, double>()
        .AddValidation([](double a, double b) {
            if (fabs(a - b) < 1e-6)
                throw std::runtime_error("a cannot equal b");
        });
}

TEST_F(ClassTest, ConstructorWithConstraints)
{
    Reflect<MyClassTestClass>(m_lib);
    AddBuiltInConversions(m_lib->GetConversionGraph());
    Object wrapper(m_lib, Members::GlobalType());
    EXPECT_THROW(Object v = wrapper.Lookup("MyClassTestClass")(4.5, 4.5), std::runtime_error);
}

namespace Sections {
    class PipeSection
    {
    public:
        static void ValidatePipeSection(double radius, double thickness) {
            if (thickness > radius)
                throw std::runtime_error("Thickness cannot be greater than Radius");
        }
        PipeSection(double radius, double thickness) : m_radius(radius), m_thickness(thickness) {}
        void SetThickness(double thickness) { m_thickness = thickness; }
        void ValidateThickness(double thickness)
        {
            if (thickness > m_radius)
                throw std::runtime_error("Thickness cannot be greater than Radius");
        }
    private:
        double m_radius;
        double m_thickness;
    };
    void DoReflect(TypeLibraryPointer lib, PipeSection**)
    {
        Class<PipeSection> cls(lib, "PipeSection");
        cls.Constructor<double, double>()
            .AddValidation(&PipeSection::ValidatePipeSection);
        cls.Function("SetThickness", &PipeSection::SetThickness)
            .AddValidation(&PipeSection::ValidateThickness);
    }
}

TEST_F(ClassTest, FunctionConstraints)
{
    Reflect<Sections::PipeSection>(m_lib);
    AddBuiltInConversions(m_lib->GetConversionGraph());
    Object wrapper(m_lib, Members::GlobalType());

    EXPECT_THROW(wrapper.Lookup("PipeSection")(5.0, 9), std::runtime_error);
    Object v = wrapper.Lookup("PipeSection")(5.0, 1);
    EXPECT_THROW(v.Lookup("SetThickness")(8), std::runtime_error);
}

class LoadCase 
{
public:
    LoadCase() {}
};

void DoReflect(TypeLibraryPointer lib, LoadCase**)
{
    Class<LoadCase> cls(lib, "LoadCase");
    cls.Constructor();
}

class EnvironmentManager 
{
public:
    EnvironmentManager(LoadCase* lc) : m_lc(lc) {}
    void Test(double val) {}
    operator LoadCase*() const {return m_lc; }
    LoadCase* m_lc;
};

void DoReflect(TypeLibraryPointer lib, EnvironmentManager**)
{
    Class<EnvironmentManager, std::shared_ptr<EnvironmentManager>> cls(lib, "EnvironmentManager");
    cls.Unite<LoadCase>();
    cls.Function("Test", &EnvironmentManager::Test);
}

TEST_F(ClassTest, JoinClasses)
{
    Reflect<LoadCase>(m_lib);
    Reflect<EnvironmentManager>(m_lib);
    AddBuiltInConversions(m_lib->GetConversionGraph());
    Object wrapper(m_lib, Members::GlobalType());
    Object v = wrapper.Lookup("LoadCase")();
    EXPECT_NO_THROW(v.Lookup("Test")(5.2));
}

class SomeClass 
{
public:
    SomeClass() {}
    void CRefFunction(const SomeClass& cref) {}
    void RefFunction(SomeClass& cref) {}
    void CPtrFunction(const SomeClass* cref) {}
    void PtrFunction(SomeClass* cref) {}
};

void DoReflect(TypeLibraryPointer lib, SomeClass**)
{
    Class<SomeClass> cls(lib, "SomeClass");
    cls.Constructor();
    cls.Function("CRefFunction", &SomeClass::CRefFunction);
    cls.Function("RefFunction", &SomeClass::RefFunction);
    cls.Function("CPtrFunction", &SomeClass::CPtrFunction);
    cls.Function("PtrFunction", &SomeClass::PtrFunction);
}


TEST_F(ClassTest, PointerToReferenceCast)
{
    Reflect<SomeClass>(m_lib);
    SomeClass someOtherClass;
    SomeClass* someClass = &someOtherClass;
    Object v(m_lib, someClass);
    EXPECT_NO_THROW(v.As<const SomeClass&>());
    EXPECT_NO_THROW(v.As<SomeClass&>());
    EXPECT_NO_THROW(v.Lookup("CRefFunction")(someClass));
    EXPECT_NO_THROW(v.Lookup("RefFunction")(someClass));
}

TEST_F(ClassTest, ReferenceToPointerCast)
{
    Reflect<SomeClass>(m_lib);
    SomeClass someOtherClass;
    Object v = Object::Create<SomeClass&>(m_lib, someOtherClass);
    EXPECT_NO_THROW(v.As<const SomeClass*>());
    EXPECT_NO_THROW(v.As<SomeClass*>());
    EXPECT_NO_THROW(v.Lookup("CPtrFunction")(someOtherClass));
    EXPECT_NO_THROW(v.Lookup("PtrFunction")(someOtherClass));
}

class TestIntClass 
{
public:
    TestIntClass(const int& input) {m_value = input; EXPECT_EQ(5, m_value); }
    const int& GetValue() {return m_value; }
private:
    int m_value;
};

void DoReflect(TypeLibraryPointer lib, TestIntClass**)
{
    Class<TestIntClass> cls(lib, "TestIntClass");
    cls.Constructor<const int&>();
    cls.Get("GetValue", &TestIntClass::GetValue);
}

TEST_F(ClassTest, TestIntToConstIntRef)
{
    AddBuiltInConversions(m_lib->GetConversionGraph());
    Reflect<TestIntClass>(m_lib);
    Object wrapper(m_lib, Members::GlobalType());
    Object a = wrapper.Lookup("TestIntClass")(5);
    EXPECT_EQ(5, a.Lookup("GetValue").As<int>());
}

class ClassWithValueArgument 
{
public:
    ClassWithValueArgument() {}
    void TestFn(Objects::Object value) 
    {
        EXPECT_DOUBLE_EQ(42.0, value.As<double>());
    }
};

void DoReflect(TypeLibraryPointer lib, ClassWithValueArgument**)
{
    Class<ClassWithValueArgument> cls(lib, "ClassWithValueArgument");
    cls.Function("TestFn", &ClassWithValueArgument::TestFn);
}

///TODO Add support for passing variant arguments.
TEST_F(ClassTest, DISABLED_TestGenericConversion_Value)
{
    AddBuiltInConversions(m_lib->GetConversionGraph());
    Reflect<ClassWithValueArgument>(m_lib);
    ClassWithValueArgument val;
    Object reflectedVal = Object::Create<ClassWithValueArgument&>(m_lib, val);
    reflectedVal.Lookup("TestFn")(42);
}
///TODO Add suppor for enums
/*
enum MyEnum {
    Red,
    Green,
    Blue,
};

void ClassTest::TestEnum()
{
    TypeLibrary typeLibrary;
    Enum<MyEnum> myEnum("MyEnum",typeLibrary);
    myEnum.Value("meRed",Red);
    myEnum.Value("meGreen",Green);
    myEnum.Value("meBlue",Blue);
    EXPECT_NO_THROW(CPPUNIT_ASSERT(typeLibrary.GetGlobal()->GetMethod("meGreen")));
    EXPECT_NO_THROW(EXPECT_EQ(Green,Delegate(Value(typeLibrary,Variant::VoidStorage()),"meGreen").As<MyEnum>()));
}
*/

class ClassWithDocumentedArgument 
{
public:
    void TestFunction(int a) {}
};

void DoReflect(TypeLibraryPointer typeLibrary, ClassWithDocumentedArgument**)
{
    Class<ClassWithDocumentedArgument> cls(typeLibrary, "ClassWithDocumentedArgument");
    cls.Function("TestFunction", &ClassWithDocumentedArgument::TestFunction)
        .AddSignature(Arg("a", "Some argument"));
}

TEST_F(ClassTest, DocumentArgument)
{
    Reflect<ClassWithDocumentedArgument>(m_lib);
    Members::MemberPointer member = m_lib->LookupType(typeid(ClassWithDocumentedArgument))->Lookup("TestFunction");
    EXPECT_EQ("Some argument", member->GetArgumentInfo(1)->GetDocumentation());
}

TEST_F(ClassTest, ClassWithSharedPointerStorage)
{
    typedef Class<int, std::shared_ptr<int>> Cls;
    Cls cls(m_lib, "Int");
    static_assert(Types::IsPointerWrapper<std::shared_ptr<int>>::value, "Should be a pointerwrapper");
    static_assert(std::is_same<std::shared_ptr<int>, Cls::HolderType>::value, "Types should be same");
}

class IntrusiveTestClass : public DNVS::MoFa::TypeUtilities::IntrusiveClass<IntrusiveTestClass>
{
public:
    IntrusiveTestClass(bool& alive) : m_alive(alive) { m_alive = true; }
    ~IntrusiveTestClass() { m_alive = false; }
private:
    bool& m_alive;
};

TEST_F(ClassTest, ClassWithIntrusivePointerStorage_CheckCountAfterCast)
{
    Class<IntrusiveTestClass, DNVS::MoFa::TypeUtilities::IntrusivePointer<IntrusiveTestClass>> cls(m_lib, "IntrusiveTestClass");
    bool alive = false;
    DNVS::MoFa::TypeUtilities::IntrusivePointer<IntrusiveTestClass> itc = new IntrusiveTestClass(alive);
    EXPECT_TRUE(alive);
    IntrusiveTestClass* castObject = Object(m_lib, itc).As<IntrusiveTestClass*>();
    itc.reset();
    EXPECT_FALSE(alive);
    
}

using namespace DNVS::MoFa;

class CheckIfAlive : public TypeUtilities::IntrusiveClass<CheckIfAlive> {
public:
    CheckIfAlive(bool& alive) : m_alive(alive) { m_alive = true; }
    ~CheckIfAlive() { m_alive = false; }
private:
    bool& m_alive;
};

namespace DNVS {namespace MoFa { namespace Reflection { namespace Variants {
    template<typename U>
    class IxionRefPtrStorage : public StorageBase
    {
    public:
        IxionRefPtrStorage(const TypeUtilities::IntrusivePointer<U>& smartPointer)
            : m_ptr(smartPointer)
        {
            m_data = static_cast<void*>(smartPointer.get());
        }
        ~IxionRefPtrStorage()
        {
        }
        static bool IsConvertible(Variants::StoragePointer storage, bool strict = false)
        {
            if (!strict && storage == nullptr)
                return true;
            return dynamic_cast<IxionRefPtrStorage*>(storage.get()) != 0;
        }
    private:
        TypeUtilities::IntrusivePointer<U> m_ptr;
    };

    template<typename U>
    struct CopyableSmartPointerSelector<TypeUtilities::IntrusivePointer<U>> : public IxionRefPtrStorage<U>
    {
    public:
        CopyableSmartPointerSelector(const TypeUtilities::IntrusivePointer<U>& smartPointer)
            : IxionRefPtrStorage<U>(smartPointer)
        {}
        ~CopyableSmartPointerSelector()
        {}
    };
}}}}

TEST_F(ClassTest, CheckIfAlive)
{
    bool alive = false;
    TypeUtilities::IntrusivePointer<CheckIfAlive> ptr(new CheckIfAlive(alive));
    Reflection::Variants::StoragePointer s(new Reflection::Variants::CopyableSmartPointerSelector<TypeUtilities::IntrusivePointer<CheckIfAlive>>(ptr));
    s.reset();
    ptr.reset();
    EXPECT_FALSE(alive);
}

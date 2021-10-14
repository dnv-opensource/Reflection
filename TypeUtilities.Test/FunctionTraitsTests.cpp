//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <TypeUtilities/FunctionTraits.h>
#include "boost/mpl/size_t.hpp"
#include "boost/static_assert.hpp"
#include "boost/type_traits/is_same.hpp"
#include "gtest/gtest.h"
#include <boost/function.hpp>
#include "TypeUtilities/IsCallable.h"
#include "GoogleTest/gtest_assert_at.h"
#include "boost/optional/optional.hpp"
#include <vector>

using namespace DNVS::MoFa::TypeUtilities;


template<typename A0,typename A1>
class ArgumentVisitor {
public:
    template<typename T,size_t Index>
    void Visit() {
        VisitImpl<T>(boost::mpl::size_t<Index>());
    }
    template<typename T>
    void VisitImpl(boost::mpl::size_t<0>) {
        BOOST_STATIC_ASSERT((boost::is_same<T,A0>::value));
    }
    template<typename T>
    void VisitImpl(boost::mpl::size_t<1>) {
        BOOST_STATIC_ASSERT((boost::is_same<T,A1>::value));
    }
};

TEST(FunctionTraitsTests,VisitFunction)
{
    typedef int (*Signature)(int,double);
    FunctionTraits<Signature>::Visit(ArgumentVisitor<int,double>());
    EXPECT_EQ((size_t)2,FunctionTraits<Signature>::Arity);
}

TEST(FunctionTraitsTests,VisitBoostFunction)
{
    typedef boost::function<int(void*,double)> Signature;
    FunctionTraits<Signature>::Visit(ArgumentVisitor<void*,double>());
    EXPECT_EQ((size_t)2,FunctionTraits<Signature>::Arity);
}
class FunctionTraitsTestsClass {};
TEST(FunctionTraitsTests,VisitMemberFunction)
{
    typedef void (FunctionTraitsTestsClass::*Signature)(short);
    FunctionTraits<Signature>::Visit(ArgumentVisitor<FunctionTraitsTestsClass*,short>());
    EXPECT_EQ((size_t)2,FunctionTraits<Signature>::Arity);
}

TEST(FunctionTraitsTests,VisitConstMemberFunction)
{
    typedef void (FunctionTraitsTestsClass::*Signature)(short) const;
    FunctionTraits<Signature>::Visit(ArgumentVisitor<const FunctionTraitsTestsClass*,short>());
    EXPECT_EQ((size_t)2,FunctionTraits<Signature>::Arity);
}

TEST(FunctionTraitsTests, AssertClassType)
{
    typedef void (FunctionTraitsTestsClass::*Signature)(short) const;
    static_assert(std::is_same_v<const FunctionTraitsTestsClass*, FunctionTraits<Signature>::Argument0>, "Wrong type");
}

template<typename T>
std::enable_if_t<HasReturnType<T, void>> TestEnableIfFunctionTraits(T) {}

class IBase {
public:
    virtual ~IBase() {}
};

class Derived : public IBase {};
void TestEnableIfFunctionTraits(std::shared_ptr<IBase> ptr) {}

TEST(FunctionTraitsTests, EnableIfFunctionTraits)
{
    TestEnableIfFunctionTraits(std::make_shared<Derived>());
}

void FunctionRef(int) {}

template<typename T>
void TestIsCallable(T arg,const char* file, int line)
{
    EXPECT_TRUE_AT(IsCallable<T>, file,line);
}

class TestIsCallableClass
{
    TestIsCallableClass() {}
public:
    template<typename... Args>
    TestIsCallableClass(Args&&... args)
    {
        InitializeDefaultValue(std::forward<Args>(args)...);
    }

private:
    template<typename Function, typename... Args>
    std::enable_if_t<IsCallable<Function>> InitializeDefaultValue(Function callback, Args&&... args)
    {
    }
};
enum class MyTestEnum {Red};
MyTestEnum GetDefaultBeamConversionOption(boost::optional<std::vector<void*>> beams) { return MyTestEnum::Red; }
TEST(FunctionTraitsTests, IsCallableTest)
{
    auto fn = []() {};
    EXPECT_TRUE(IsCallable<decltype(fn)>);
    EXPECT_TRUE(IsCallable<void()>);
    EXPECT_TRUE(IsCallable<std::function<void()>>);
    EXPECT_FALSE(IsCallable<std::shared_ptr<int>>);
    EXPECT_TRUE(IsCallable<decltype(FunctionRef)>);
    TestIsCallable(FunctionRef,__FILE__, __LINE__);
    TestIsCallable(&FunctionRef, __FILE__, __LINE__);
    TestIsCallableClass cls(GetDefaultBeamConversionOption);
}

TEST(FunctionTraitsTests, HasConvertibleSignatureTests)
{
    EXPECT_TRUE((HasConvertibleSignature<bool(Derived*), bool(const IBase*)>));
    EXPECT_FALSE((HasConvertibleSignature<bool(Derived*), bool(const IBase*, int)>));
    EXPECT_FALSE((HasConvertibleSignature<bool(Derived*), bool(int)>));
    EXPECT_TRUE((HasConvertibleSignature<bool(Derived*, double), bool(const IBase*, double)>));
    EXPECT_FALSE((HasConvertibleSignature<bool(Derived*), IBase(Derived*)>));
}
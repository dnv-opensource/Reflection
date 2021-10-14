#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest\gtest.h"
#include <boost/bind/bind.hpp>
using namespace boost::placeholders;

#include "boost\lexical_cast.hpp"
#define CPPUNIT_NS DNVS::MoFa::GoogleTest
#define CPPUNIT_TEST_SUITE(Name)
#define CPPUNIT_TEST(Name)
#define CPPUNIT_TEST_SUITE_END()
#define CPPUNIT_TEST_SUITE_REGISTRATION(Name)
#define CPPUNIT_ASSERT(condition) ASSERT_TRUE(!!(condition))
#define CPPUNIT_ASSERT_MESSAGE(message,condition) ASSERT_TRUE(!!(condition)) << message
#define CPPUNIT_FAIL( message ) FAIL() << message
#define CPPUNIT_ASSERT_EQUAL(expected,actual) ASSERT_EQ(expected,actual)
#define CPPUNIT_ASSERT_EQUAL_MESSAGE(message,expected,actual) ASSERT_EQ(expected,actual) << message
#define CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,actual,delta) ASSERT_NEAR(expected,actual,delta)
#define CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(message,expected,actual,delta)  ASSERT_NEAR(expected,actual,delta)
#define CPPUNIT_ASSERT_THROW( expression, ExceptionType ) ASSERT_THROW(expression,ExceptionType)
#define CPPUNIT_ASSERT_THROW_MESSAGE( message, expression, ExceptionType )  ASSERT_THROW(expression,ExceptionType) << message
#define CPPUNIT_ASSERT_NO_THROW( expression ) ASSERT_NO_THROW(expression)
#define CPPUNIT_ASSERT_NO_THROW_MESSAGE( message, expression ) ASSERT_NO_THROW(expression) << message
#define MOFA_ASSERT_EQUAL(expected,actual) ASSERT_EQ(expected,actual)
#define MOFA_ASSERT_EQUAL_MESSAGE(message,expected,actual) ASSERT_EQ(expected,actual) << message

#define MOFA_ASSERT_CONTAINERS_EQUAL(expected,actual) \
    {auto expectedEval=expected;\
    auto actualEval=actual;\
    MOFA_ASSERT_EQUAL(expectedEval.size(),actualEval.size());\
    auto it1=expectedEval.begin();\
    auto it2=actualEval.begin();\
    size_t i=0;\
    while(it1!=expectedEval.end()) \
    MOFA_ASSERT_EQUAL_MESSAGE("At index = "+boost::lexical_cast<std::string>(i++),*it1++,*it2++);}

#define MOFA_ASSERT_CONTAINER_DOUBLES_EQUAL(expected,actual,tolerance)\
{auto expectedEval=expected;\
    auto actualEval=actual;\
    MOFA_ASSERT_EQUAL(expectedEval.size(),actualEval.size());\
    auto it1=expectedEval.begin();\
    auto it2=actualEval.begin();\
    size_t i=0;\
    while(it1!=expectedEval.end()) \
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("At index = "+boost::lexical_cast<std::string>(i++),*it1++,*it2++,tolerance);}

namespace DNVS {namespace MoFa {namespace GoogleTest {
    class TestFixture : public testing::Test {
    public:
        virtual void setUp() {}
        virtual void tearDown() {}
    protected:
        virtual void SetUp() {setUp();}
        virtual void TearDown() {tearDown();}
    };
}}}

#if _MSC_VER == 1600
#define MOFA_GTEST_TEST_(test_case_name, test_name, parent_class, parent_id,test_runner)\
class GTEST_TEST_CLASS_NAME_(test_case_name, test_name) : public parent_class {\
public:\
    GTEST_TEST_CLASS_NAME_(test_case_name, test_name)() {}\
private:\
    void Run();\
    virtual void TestBody();\
    static ::testing::TestInfo* const test_info_ GTEST_ATTRIBUTE_UNUSED_;\
    GTEST_DISALLOW_COPY_AND_ASSIGN_(\
    GTEST_TEST_CLASS_NAME_(test_case_name, test_name));\
};\
    \
    ::testing::TestInfo* const GTEST_TEST_CLASS_NAME_(test_case_name, test_name)\
    ::test_info_ =\
    ::testing::internal::MakeAndRegisterTestInfo(\
#test_case_name, #test_name, NULL, NULL, \
    (parent_id), \
    parent_class::SetUpTestCase, \
    parent_class::TearDownTestCase, \
    new ::testing::internal::TestFactoryImpl<\
    GTEST_TEST_CLASS_NAME_(test_case_name, test_name)>);\
    void GTEST_TEST_CLASS_NAME_(test_case_name, test_name)::TestBody()\
    {\
        test_runner.Run(boost::bind(&GTEST_TEST_CLASS_NAME_(test_case_name, test_name)::Run,this));\
    }\
    void GTEST_TEST_CLASS_NAME_(test_case_name, test_name)::Run()

#define MOFA_TEST_F(test_fixture, test_name,test_runner)\
    MOFA_GTEST_TEST_(test_fixture, test_name, test_fixture, \
    ::testing::internal::GetTypeId<test_fixture>(),test_runner)


#define MOFA_TEST(test_case_name, test_name,test_runner) MOFA_GTEST_TEST_(test_case_name, test_name,::testing::Test, ::testing::internal::GetTestTypeId(),test_runner)

#else
#define MOFA_GTEST_TEST_(test_case_name, test_name, parent_class, parent_id,test_runner, file, line)\
class GTEST_TEST_CLASS_NAME_(test_case_name, test_name) : public parent_class {\
public:\
    GTEST_TEST_CLASS_NAME_(test_case_name, test_name)() {}\
private:\
    void Run();\
    virtual void TestBody();\
    static ::testing::TestInfo* const test_info_ GTEST_ATTRIBUTE_UNUSED_;\
    GTEST_DISALLOW_COPY_AND_ASSIGN_(\
    GTEST_TEST_CLASS_NAME_(test_case_name, test_name));\
};\
    \
    ::testing::TestInfo* const GTEST_TEST_CLASS_NAME_(test_case_name, test_name)\
    ::test_info_ =\
    ::testing::internal::MakeAndRegisterTestInfo(\
#test_case_name, #test_name, NULL, NULL,\
    ::testing::internal::CodeLocation(file, line),\
    (parent_id), \
    parent_class::SetUpTestCase, \
    parent_class::TearDownTestCase, \
    new ::testing::internal::TestFactoryImpl<\
    GTEST_TEST_CLASS_NAME_(test_case_name, test_name)>);\
    void GTEST_TEST_CLASS_NAME_(test_case_name, test_name)::TestBody()\
    {\
        test_runner.Run(boost::bind(&GTEST_TEST_CLASS_NAME_(test_case_name, test_name)::Run,this));\
    }\
    void GTEST_TEST_CLASS_NAME_(test_case_name, test_name)::Run()

#define MOFA_TEST_F(test_fixture, test_name,test_runner)\
    MOFA_GTEST_TEST_(test_fixture, test_name, test_fixture, \
    ::testing::internal::GetTypeId<test_fixture>(),test_runner, __FILE__, __LINE__)


#define MOFA_TEST(test_case_name, test_name,test_runner) MOFA_GTEST_TEST_(test_case_name, test_name,::testing::Test, ::testing::internal::GetTestTypeId(),test_runner, __FILE__, __LINE__)
#endif

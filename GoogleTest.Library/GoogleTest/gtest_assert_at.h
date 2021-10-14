#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"

#define GTEST_ASSERT_AT_(expression, file, line, on_failure) \
    GTEST_AMBIGUOUS_ELSE_BLOCKER_ \
    if (const ::testing::AssertionResult gtest_ar = (expression)) \
    ; \
  else \
  on_failure(file,line,gtest_ar.failure_message())

#define GTEST_NONFATAL_FAILURE_AT_(file,line,message) \
    GTEST_MESSAGE_AT_(file,line,message, ::testing::TestPartResult::kNonFatalFailure)

#define GTEST_PRED_FORMAT1_AT_(pred_format, v1, file,line,on_failure)\
    GTEST_ASSERT_AT_(pred_format(#v1, v1),\
    file,line,on_failure)

#define GTEST_PRED_FORMAT2_AT_(pred_format, v1, v2, file,line,on_failure)\
    GTEST_ASSERT_AT_(pred_format(#v1, #v2, v1, v2),\
    file,line,on_failure)

#define GTEST_PRED_FORMAT3_AT_(pred_format, v1, v2, v3, file,line,on_failure)\
    GTEST_ASSERT_AT_(pred_format(#v1, #v2, #v3, v1, v2, v3),\
    file,line,on_failure)

#define GTEST_TEST_NO_THROW_AT_(statement, fail,file,line) \
    GTEST_AMBIGUOUS_ELSE_BLOCKER_ \
    if (::testing::internal::AlwaysTrue()) { \
    try { \
    GTEST_SUPPRESS_UNREACHABLE_CODE_WARNING_BELOW_(statement); \
    } \
    catch (...) { \
    goto GTEST_CONCAT_TOKEN_(gtest_label_testnothrow_, __LINE__); \
    } \
    } else \
    GTEST_CONCAT_TOKEN_(gtest_label_testnothrow_, __LINE__): \
    fail(file, line, "Expected: " #statement " doesn't throw an exception.\n" \
    "  Actual: it throws.")

#define GTEST_TEST_BOOLEAN_AT_(expression, text, actual, expected, fail, file, line) \
    GTEST_AMBIGUOUS_ELSE_BLOCKER_ \
    if (const ::testing::AssertionResult gtest_ar_ = \
    ::testing::AssertionResult(expression)) \
    ; \
  else \
  fail(file, line, ::testing::internal::GetBoolAssertionFailureMessage(\
  gtest_ar_, text, #actual, #expected).c_str())

//Expect at macros

#define EXPECT_PRED_FORMAT2_AT(pred_format, v1, v2, file, line) \
    GTEST_PRED_FORMAT2_AT_(pred_format, v1, v2, file,line, GTEST_NONFATAL_FAILURE_AT_)

#define EXPECT_PRED_FORMAT3_AT(pred_format, v1, v2, v3, file, line) \
    GTEST_PRED_FORMAT3_AT_(pred_format, v1, v2, v3, file,line, GTEST_NONFATAL_FAILURE_AT_)

#define EXPECT_PRED_FORMAT1_AT(pred_format, v1, file, line) \
    GTEST_PRED_FORMAT1_AT_(pred_format, v1, file, line, GTEST_NONFATAL_FAILURE_AT_)

#define EXPECT_DOUBLE_EQ_AT(expected, actual,file,line)\
    EXPECT_PRED_FORMAT2_AT(::testing::internal::CmpHelperFloatingPointEQ<double>, \
    expected, actual,file,line)

#define EXPECT_NEAR_AT(expected, actual,diff,file,line)\
    EXPECT_PRED_FORMAT3_AT(::testing::internal::DoubleNearPredFormat, \
    expected, actual,diff,file,line)

#define EXPECT_EQ_AT(val1, val2,file,line)\
    EXPECT_PRED_FORMAT2_AT(::testing::internal::EqHelper::Compare, val1, val2, file,line)

#define EXPECT_GT_AT(expected, actual,file,line)\
    EXPECT_PRED_FORMAT2_AT(::testing::internal::CmpHelperGT, \
    expected, actual,file,line)

#define EXPECT_LT_AT(expected, actual,file,line)\
    EXPECT_PRED_FORMAT2_AT(::testing::internal::CmpHelperLT, \
    expected, actual,file,line)

#define EXPECT_GE_AT(expected, actual,file,line)\
    EXPECT_PRED_FORMAT2_AT(::testing::internal::CmpHelperGE, \
    expected, actual,file,line)

#define EXPECT_LE_AT(expected, actual,file,line)\
    EXPECT_PRED_FORMAT2_AT(::testing::internal::CmpHelperLE, \
    expected, actual,file,line)

#define EXPECT_NE_AT(val1, val2,file,line)\
    EXPECT_PRED_FORMAT2_AT(::testing::internal::CmpHelperNE, val1, val2,file,line)

#define EXPECT_NO_THROW_AT(statement,file,line) \
    GTEST_TEST_NO_THROW_AT_(statement, GTEST_NONFATAL_FAILURE_AT_,file,line)

#define EXPECT_TRUE_AT(condition, file, line) \
    GTEST_TEST_BOOLEAN_AT_(condition, #condition, false, true, \
    GTEST_NONFATAL_FAILURE_AT_, file, line)

#define EXPECT_FALSE_AT(condition, file, line) \
    GTEST_TEST_BOOLEAN_AT_(!(condition), #condition, true, false, \
    GTEST_NONFATAL_FAILURE_AT_, file, line)


#define GTEST_FATAL_FAILURE_AT_(message, file, line) \
  return GTEST_MESSAGE_AT_(file, line, message, ::testing::TestPartResult::kFatalFailure)

#define FAIL_AT(file, line) GTEST_FAIL_AT(file,line)

inline ::testing::AssertionResult AssertTrueNoMessage(const char* expr,bool conditional)
{
    if(conditional)
        return ::testing::AssertionSuccess();
    else
        return ::testing::AssertionFailure();
}

#define GTEST_TEST_NO_THROW_EXCEPTION_AT_(statement, exceptionType, fail,file,line) \
    GTEST_AMBIGUOUS_ELSE_BLOCKER_ \
    if (::testing::internal::AlwaysTrue()) { \
    try { \
    GTEST_SUPPRESS_UNREACHABLE_CODE_WARNING_BELOW_(statement); \
    } \
    catch(exceptionType e)\
    {\
    std::string error ="Expected: " #statement " doesn't throw an exception. Actual: it throws:." + std::string(e.what());\
    fail(file, line, error.c_str());\
    }\
    catch (...) { \
    goto GTEST_CONCAT_TOKEN_(gtest_label_testnothrow_, __LINE__); \
    } \
    } else \
    GTEST_CONCAT_TOKEN_(gtest_label_testnothrow_, __LINE__): \
    fail(file, line, "Expected: " #statement " doesn't throw an exception.\n" \
    "  Actual: it throws.")


#define EXPECT_NO_THROW_EXCEPTION(statement,exceptionType) \
    GTEST_TEST_NO_THROW_EXCEPTION_AT_(statement, exceptionType, GTEST_NONFATAL_FAILURE_AT_,__FILE__,__LINE__)

#define ASSERT_EXCEPTION( TRY_BLOCK, EXCEPTION_TYPE, MESSAGE )        \
try                                                                   \
{                                                                     \
    TRY_BLOCK;                                                        \
    FAIL() << "exception '" << MESSAGE << "' not thrown at all!";     \
}                                                                     \
catch( const EXCEPTION_TYPE& e )                                      \
{                                                                     \
    EXPECT_EQ( std::string(MESSAGE), std::string(e.what()) )          \
        << " exception message is incorrect.";                        \
}                                                                     \
catch( ... )                                                          \
{                                                                     \
    FAIL() << "exception '" << MESSAGE                                \
           << "' not thrown with expected type '" << #EXCEPTION_TYPE  \
           << "'!";                                                   \
}
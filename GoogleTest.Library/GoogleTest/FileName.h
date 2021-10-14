#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include <algorithm>
#include <gtest/gtest.h>

namespace DNVS { namespace MoFa { namespace GoogleTest {

    class FileName
    {
    public:
        static std::string GetUniqueFileName(const std::string& extension)
        {
            const ::testing::TestInfo* ti = ::testing::UnitTest::GetInstance()->current_test_info();
            if (!ti)
                throw std::runtime_error("No TestInfo object available - this function must be called in the context of a Google test");
            std::string name = std::string(ti->test_case_name()) + "_" + std::string(ti->name());
            // Replace any "/" with "_". This occurs in parameterised tests
            std::replace(name.begin(), name.end(), '/', '_');
            if (!extension.empty())
                name += "." + extension;
            return std::move(name);
        }
    };

}}}

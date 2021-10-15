//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"
#include "FakeSubsystem.h"
#include "boost/assign/list_of.hpp"
#include "Plugins/SortSubsystems.h"
using boost::assign::list_of;
namespace DNVS { namespace MoFa { namespace Plugins { namespace Test {

    TEST(SubsystemSortinTests, SortSubsystemsPriorToRunning)
    {
        std::list<std::shared_ptr<IFakeSubsystem>> fakeSubsystems;
        fakeSubsystems.push_back(std::make_shared<FakeSubsystem>("Structure", list_of("Properties")));
        fakeSubsystems.push_back(std::make_shared<FakeSubsystem>("Properties"));
        fakeSubsystems = SortSubsystems(fakeSubsystems);
        EXPECT_EQ("Properties", fakeSubsystems.front()->GetName());
    }

    TEST(SubsystemSortinTests, DISABLED_SortComplexSubsystems)
    {
        std::list<std::shared_ptr<IFakeSubsystem>> fakeSubsystems;
        auto analysis = std::make_shared<FakeSubsystem>("Analysis", list_of("Sets"));
        auto sets = std::make_shared<FakeSubsystem>("Sets", list_of("Structure")("Guide"));
        auto properties = std::make_shared<FakeSubsystem>("Properties");
        auto guide = std::make_shared<FakeSubsystem>("Guide");
        auto structure = std::make_shared<FakeSubsystem>("Structure", list_of("Properties"));
        fakeSubsystems.push_back(analysis);
        fakeSubsystems.push_back(sets);
        fakeSubsystems.push_back(structure);
        fakeSubsystems.push_back(properties);
        fakeSubsystems.push_back(guide);
        std::list<std::shared_ptr<IFakeSubsystem>> presorted;
        presorted.push_back(guide);
        presorted.push_back(properties);
        presorted.push_back(structure);
        presorted.push_back(sets);
        presorted.push_back(analysis);
        auto sortedSubsystems = SortSubsystems(fakeSubsystems);        
        EXPECT_EQ(presorted, sortedSubsystems);
    }
}}}}

//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection\Members\MemberWithArguments.h"
#include "gtest\gtest.h"

using namespace DNVS::MoFa::Reflection::Variants;
using namespace DNVS::MoFa::Reflection::TypeConversions;
using namespace DNVS::MoFa::Reflection::Members;

TEST(MemberWithArgumentsTests,CreateMemberWithArguments)
{
    ConversionGraphPointer cp;
    MemberPointer mp;
    std::vector<Variant> args;
    ASSERT_NO_THROW(MemberWithArguments mm(cp,mp,args));
}

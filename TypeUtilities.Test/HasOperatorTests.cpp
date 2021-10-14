//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypeUtilities/HasOperator.h"

using namespace DNVS::MoFa::TypeUtilities;
struct Dummy {};
struct DummyWithEqualTo {
    bool operator == (const DummyWithEqualTo&) const { return true; }
};
static_assert(HasEqualTo<int>, "int == int failed");
static_assert(HasEqualTo<Dummy> == false, "Dummy == Dummy should have failed");
static_assert(HasEqualTo<DummyWithEqualTo>, "DummyWithEqualTo == DummyWithEqualTo failed");
static_assert(HasNotEqualTo<int>, "int != int failed");
static_assert(HasNotEqualTo<Dummy> == false, "Dummy != Dummy should have failed");
static_assert(HasNotEqualTo<DummyWithEqualTo> == false, "DummyWithEqualTo != DummyWithEqualTo should have failed");

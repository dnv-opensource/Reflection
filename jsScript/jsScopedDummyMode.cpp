//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "jsScopedDummyMode.h"
#include "jsStack.h"

jsScopedDummyMode::jsScopedDummyMode()
{
    if (jsStack::HasStack())
    {
        m_oldValue = jsStack::stack()->dummyMode();
        jsStack::stack()->setDummyMode(true);
    }
}

jsScopedDummyMode::~jsScopedDummyMode()
{
    if (jsStack::HasStack())
        jsStack::stack()->setDummyMode(m_oldValue);
}

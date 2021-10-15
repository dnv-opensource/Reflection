#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "jsScript_config.h"
class JSSCRIPT_IMPORT_EXPORT jsScopedDummyMode {
public:
    jsScopedDummyMode();
    ~jsScopedDummyMode();
private:
    bool m_oldValue;
};
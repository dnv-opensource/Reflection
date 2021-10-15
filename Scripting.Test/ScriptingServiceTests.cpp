//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"
#include "Scripting/As.h"
#include "Scripting/IScriptingService.h"

namespace DNVS  {namespace MoFa {namespace Scripting {
    TEST(ScriptingServiceTests,DISABLED_CheckSyntaxTest)
    {
        ScriptingServicePointer service;
        auto result = service->Execute("");
        result.As<int>();
    }
}}}

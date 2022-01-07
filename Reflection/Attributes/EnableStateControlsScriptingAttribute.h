#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IAttribute.h"


namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {
    /*
    If an item with this attribute shall be scripted, ignore scripting if it is disabled.
    */

    class EnabledStateControlsScriptingAttribute 
        : public IAttribute
    {
    public:
    };
}}}}
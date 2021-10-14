#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlNodeHandler.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    /*
    If a class has no constructors, try to add all members of that class.
    */
    class ControlNoConstructorsHandler : public IControlNodeHandler
    {
    public:
        virtual bool OnInitialize(ControlNode& node, bool priorState = false);
    private:
        bool ShouldAddMembers(ControlNode& node) const;
    };
}}}}
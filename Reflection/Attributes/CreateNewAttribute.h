#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IAttribute.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    /**
    Set this attribute on the top node when you want a dialog for creating new objects.
    */
    class CreateNewAttribute : public IAttribute
    {};
}}}}
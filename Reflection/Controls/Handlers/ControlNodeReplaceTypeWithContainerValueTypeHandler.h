#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlNodeAttributeHandler.h"
#include "Reflection/Attributes/RecordAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    /*
    For rows in grids etc. We want to disable containers. We instead replace the represented type with the containers value type.
    */
    class ControlNodeReplaceTypeWithContainerValueTypeHandler : public IControlNodeHandler
    {
    public:
        void OnInitializeType(ControlNode& node, const Types::DecoratedTypeInfo& type) override;
    };
}}}}
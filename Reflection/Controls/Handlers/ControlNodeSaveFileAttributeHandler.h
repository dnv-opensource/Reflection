#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlNodeAttributeHandler.h"
#include "Reflection/Attributes/SaveFileAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    /*
    Handles the SaveFileAttribute. Adds new nodes to handle the file aspect of the dialog
    */
    class ControlNodeSaveFileAttributeHandler : public IControlNodeAttributeHandler<Attributes::SaveFileAttribute>
    {
    public:
        bool OnInitialize(ControlNode& node, const Attributes::SaveFileAttribute& saveFileAttribute, bool priorState = false) override;
    };
}}}}
#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlNodeAttributeHandler.h"
#include "Reflection/Attributes/FileAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    /*
    Handles the FileAttribute. Adds new nodes to handle the file aspect of the dialog
    */
    class ControlNodeFileAttributeHandler : public IControlNodeAttributeHandler<Attributes::FileAttribute>
    {
    public:
        bool OnInitialize(ControlNode& node, const Attributes::FileAttribute& fileAttribute, bool priorState = false) override;
    };
}}}}
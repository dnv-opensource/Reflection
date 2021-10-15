#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IPlugin.h"

namespace DNVS {namespace MoFa { namespace Plugins {
    class IPluginLoader {
    public:
        virtual ~IPluginLoader() {}
        virtual std::shared_ptr<IPlugin> LoadPlugin(const std::string& name) = 0;
    };
}}}

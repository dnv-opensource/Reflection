#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IPluginManager.h"
#include "IPluginLoader.h"
#include <map>

namespace DNVS { namespace MoFa { namespace Plugins {
    class IPlugin;
    class PLUGINS_IMPORT_EXPORT PluginManager : public IPluginManager
    {
    public:
        PluginManager(std::shared_ptr<IPluginLoader> pluginLoader);
        virtual void AddPlugin(const std::string& name) override;
        virtual std::list<std::shared_ptr<ISubsystem>> GetSubsystems() override;
    private:
        std::map<std::string, std::shared_ptr<IPlugin>> m_plugins;
        std::shared_ptr<IPluginLoader> m_pluginLoader;
    };
}}}
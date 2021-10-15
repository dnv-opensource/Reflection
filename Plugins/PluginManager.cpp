//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "PluginManager.h"
#include <algorithm>

namespace DNVS { namespace MoFa { namespace Plugins {

    PluginManager::PluginManager(std::shared_ptr<IPluginLoader> pluginLoader)
        :   m_pluginLoader(pluginLoader)
    {
    }

    void PluginManager::AddPlugin(const std::string& name)
    {
        if(m_plugins.find(name) == m_plugins.end())
        {
            auto plugin = m_pluginLoader->LoadPlugin(name);
            m_plugins.insert(std::make_pair(name,plugin));
            auto dependencies = plugin->GetDependencies();
            std::for_each(dependencies.begin(), dependencies.end(), [this](const std::string& dependency) 
            {
                AddPlugin(dependency);
            });
        }
    }

    std::list<std::shared_ptr<ISubsystem>> PluginManager::GetSubsystems()
    {
        std::list<std::shared_ptr<ISubsystem>> subsystems;
        for(auto it = m_plugins.begin(); it != m_plugins.end(); ++it)
        {
            auto localSubsystem = it->second->GetSubsystems();
            subsystems.insert(subsystems.end(), localSubsystem.begin(), localSubsystem.end());
        }
        return subsystems;
    }
}}}
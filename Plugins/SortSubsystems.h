#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <type_traits>
#include "IOrdered.h"
#include <map>
#include <memory>
#include <string>
#include <list>

namespace DNVS { namespace MoFa { namespace Plugins {
    template<typename T>
    class SubsystemPrerequisiteMap 
    {
    public:
        template<typename GetNamedSubsystem>
        SubsystemPrerequisiteMap(const std::list<std::shared_ptr<T>>& subsystems, GetNamedSubsystem getNamedSubsystem)
        {
            for(auto it = subsystems.begin(); it != subsystems.end(); ++it)
            {
                const auto& subsystem = *it;
                auto prerequisites = subsystem->GetPrerequisites();
                for(auto ipre = prerequisites.begin(); ipre != prerequisites.end(); ++ipre)
                {
                    auto prerequisite = getNamedSubsystem(*ipre);
                    if(prerequisite != nullptr)
                    {
                        m_prerequisiteMap[subsystem].insert(prerequisite);
                    }
                }
            }
        }
        std::set<std::shared_ptr<T>> GetMissingPrerequisites(const std::shared_ptr<T>& subsystem, const std::set<std::shared_ptr<T>>& includedPrerequisites) const
        {
            std::set<std::shared_ptr<T>> missingPrerequisites;
            auto it = m_prerequisiteMap.find(subsystem);
            if(it == m_prerequisiteMap.end())
                return missingPrerequisites;
            else
            {
                std::set_difference(it->second.begin(), it->second.end(), includedPrerequisites.begin(), includedPrerequisites.end(), std::inserter(missingPrerequisites, missingPrerequisites.begin()));
                return missingPrerequisites;
            }
        }
    private:
        std::map<std::shared_ptr<T>, std::set<std::shared_ptr<T>>> m_prerequisiteMap;
    };

    template<typename T>
    class NameSortedSubsystems 
    {
    public:
        NameSortedSubsystems(const std::list<std::shared_ptr<T>>& subsystems)
        {
            for(auto it = subsystems.begin(); it != subsystems.end(); ++it)
            {
                const auto& subsystem = *it;
                m_sortedByName[subsystem->GetName()] = subsystem;
            }
        }
        std::shared_ptr<T> GetNamedSubsystem(const std::string& name) const
        {
            auto it = m_sortedByName.find(name);
            if(it != m_sortedByName.end())
                return it->second;
            else
                return nullptr;
        }
        std::list<std::shared_ptr<T>> GetSortedSubsystems() const
        {
            std::list<std::shared_ptr<T>> sortedSubsystems;
            for(auto it = m_sortedByName.begin(); it!= m_sortedByName.end(); ++it)
            {
                sortedSubsystems.push_back(it->second);
            }
            return sortedSubsystems;
        }
    private:
        std::map<std::string,std::shared_ptr<T> > m_sortedByName;
    };
    template<typename T>
    class DependencySortedSubsystems 
    {
    public:
        template<typename GetMissingPrerequisites>
        DependencySortedSubsystems(const std::list<std::shared_ptr<T>>& subsystems, const GetMissingPrerequisites& getMissingPrerequisites)
        {            
            for(auto it = subsystems.begin(); it != subsystems.end(); ++it)
            {
                AddSubsystem(*it, getMissingPrerequisites);
            }
        }

        template<typename GetMissingPrerequisites>
        void AddSubsystem(const std::shared_ptr<T>& subsystem, const GetMissingPrerequisites& getMissingPrerequisites)
        {
            if(m_includedSubsystems.count(subsystem)>0)
                return;
            auto missingPrerequisites = getMissingPrerequisites(subsystem, m_includedSubsystems);
            for(auto it = missingPrerequisites.begin(); it != missingPrerequisites.end(); ++it)
            {
                AddSubsystem(*it, getMissingPrerequisites);
            }
            m_sortedSubsystems.push_back(subsystem);
            m_includedSubsystems.insert(subsystem);
        }
        const std::list<std::shared_ptr<T>>& GetSortedSubsystems() const
        {
            return m_sortedSubsystems;
        }
    private:
        std::set<std::shared_ptr<T>> m_includedSubsystems;
        std::list<std::shared_ptr<T>> m_sortedSubsystems;
    };

    template<typename T>
    typename std::enable_if<std::is_base_of<IOrdered, T>::value, std::list<std::shared_ptr<T>>>::type SortSubsystems(const std::list<std::shared_ptr<T>>& subsystems)
    {
        NameSortedSubsystems<T> sortedByName(subsystems);
        SubsystemPrerequisiteMap<T> subsystemPrerequisites(subsystems,[&sortedByName](const std::string& name) {return sortedByName.GetNamedSubsystem(name);});
        DependencySortedSubsystems<T> sortedByDependency(sortedByName.GetSortedSubsystems(), [&subsystemPrerequisites](const std::shared_ptr<T>& subsystem, const std::set<std::shared_ptr<T>>& includedPrerequisites) 
        {
            return subsystemPrerequisites.GetMissingPrerequisites(subsystem, includedPrerequisites);
        });
        return sortedByDependency.GetSortedSubsystems();
    }

    template<typename T>
    typename std::enable_if<!std::is_base_of<IOrdered, T>::value, std::list<std::shared_ptr<T>>>::type SortSubsystems(const std::list<std::shared_ptr<T>>& subsystems)
    {
        return subsystems;
    }
}}}
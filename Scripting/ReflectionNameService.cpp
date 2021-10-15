//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ReflectionNameService.h"
#include "Reflection\Types\DynamicTypeTraits.h"
#include "Reflection\Attributes\AttributeCollection.h"
#include "ReflectionNewObjectScope.h"

namespace DNVS { namespace MoFa { namespace Scripting {

    Object ReflectionNameService::GetObject(const std::string& name) const
    {
        auto it = m_namedObjects.find(name);
        if (it != m_namedObjects.end())
            return it->second;
        else
            return Object();
    }

    bool ReflectionNameService::RemoveObject(const Object& object)
    {
        Object dynamicObject = object.ConvertToDynamicType();
        auto it = m_objectsWithNames.find(dynamicObject);
        if (it != m_objectsWithNames.end())
        {
            m_namedObjects.erase(it->second);
            m_objectsWithNames.erase(dynamicObject);
            return true;
        }
        return m_unnamedObjects.erase(dynamicObject) > 0;
    }

    bool ReflectionNameService::HasName(const Object& object) const
    {
        Object dynamicObject = object.ConvertToDynamicType();
        if (!GetNameAttribute(dynamicObject))
            return false;
        if (m_objectsWithNames.count(dynamicObject) > 0)
            return true;
        if (m_unnamedObjects.count(dynamicObject) > 0)
            return true;
        return false;
    }

    bool ReflectionNameService::IsNameInUse(const std::string& name) const
    {
        return (m_namedObjects.find(name) != m_namedObjects.end());
    }

    std::string ReflectionNameService::GetStoredName(const Object& object) const
    {
        auto it = m_objectsWithNames.find(object);
        if (it != m_objectsWithNames.end())
            return it->second;
        else
            return "";
    }

    bool ReflectionNameService::RenameObject(const Object& object, const std::string& name)
    {
        Object dynamicObject = object.ConvertToDynamicType();
        GetNameAttribute(dynamicObject);
        auto nameAttribute = GetNameAttribute(dynamicObject);
        if (nameAttribute)
        {
            if (name.empty())
            {
                //m_unnamedObjects.insert(dynamicObject);
                auto it = m_objectsWithNames.find(dynamicObject);
                if (it != m_objectsWithNames.end())
                {
                    m_namedObjects.erase(it->second);
                    m_objectsWithNames.erase(it);
                }
            }
            else
            {
                m_namedObjects[name] = dynamicObject;
                m_objectsWithNames[dynamicObject] = name;
            }
            if(nameAttribute->GetName(dynamicObject) != name)
                nameAttribute->SetName(dynamicObject, name);
            return true;
        }
        return false;
    }

    void ReflectionNameService::Clear()
    {
        std::map<Object, std::string, LessObjectPointer> backupObjectsWithName;
        std::map<std::string, Object> backupNamedObjects;
        std::swap(backupNamedObjects, m_namedObjects);
        std::swap(backupObjectsWithName, m_objectsWithNames);
        m_unnamedObjects.clear();
    }

    std::unique_ptr<DNVS::MoFa::Scripting::INewObjectScope> ReflectionNameService::CreateNewObjectScope() const
    {
        return std::make_unique<ReflectionNewObjectScope>();
    }

    std::set<std::string> ReflectionNameService::GetAllNames() const
    {
        std::set<std::string> allNames;
        for (const auto& namedObjectPair : m_namedObjects)
        {
            allNames.insert(namedObjectPair.first);
        }
        return allNames;
    }

    std::string ReflectionNameService::GetName(const Object& object) const
    {
        Object dynamicObject = object.ConvertToDynamicType();
        auto nameAttribute = GetNameAttribute(dynamicObject);
        if (nameAttribute)
            return nameAttribute->GetName(dynamicObject);
        else
            return "";
    }

    const Reflection::Attributes::NameAttribute* ReflectionNameService::GetNameAttribute(const Object& object) const
    {
        if (!Reflection::Types::IsPointer(object.GetDecoratedTypeInfo()))
            return nullptr;
        auto type = object.GetType();
        if (!type)
            return nullptr;
        if (!type->GetAttributeCollection().HasAttribute<Reflection::Attributes::NameAttribute>())
            return nullptr;
        else
            return &type->GetAttributeCollection().GetAttribute<Reflection::Attributes::NameAttribute>();
    }

}}}



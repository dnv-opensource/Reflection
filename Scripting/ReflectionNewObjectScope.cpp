//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ReflectionNewObjectScope.h"

namespace DNVS { namespace MoFa { namespace Scripting {

    bool ReflectionNewObjectScope::IsNewObject(const Reflection::Objects::Object& object) const
    {
        for (const auto& arg : m_newObjects)
            if (arg == object)
                return true;
        return false;
    }

    void ReflectionNewObjectScope::AddNewObject(const Object& object)
    {
        m_newObjects.emplace_back(object);
    }

    void ReflectionNewObjectScope::RenameObject(const Object& object, bool added)
    {
    }

}}}


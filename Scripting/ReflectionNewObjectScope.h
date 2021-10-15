#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "INameService.h"
#include <list>
namespace DNVS { namespace MoFa { namespace Scripting {
    class SCRIPTING_IMPORT_EXPORT ReflectionNewObjectScope 
        : public INewObjectScope
    {
    public:
        bool IsNewObject(const Reflection::Objects::Object& object) const override;
        void AddNewObject(const Object& object) override;
        void RenameObject(const Object& object, bool added) override;
    private:
        std::list<Reflection::Objects::Object> m_newObjects;
    };
}}}
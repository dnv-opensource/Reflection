#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection\Objects\Object.h"
#include "Config.h"

namespace DNVS { namespace MoFa { namespace Scripting {
    using Reflection::Objects::Object;

    class INewObjectScope {
    public:
        virtual bool IsNewObject(const Object& object) const = 0;
        virtual void AddNewObject(const Object& object) = 0;
        virtual void RenameObject(const Object& object, bool added) = 0;
        virtual ~INewObjectScope() {}
    };

    class INameService
    {
    public:
        virtual ~INameService() {}
        virtual std::unique_ptr<INewObjectScope> CreateNewObjectScope() const = 0;
        virtual Object GetObject(const std::string& name) const = 0;
        virtual bool IsNameInUse(const std::string& name) const = 0;
        virtual bool RemoveObject(const Object& object) = 0;
        virtual bool HasName(const Object& object) const = 0;
        virtual std::string GetStoredName(const Object& object) const = 0;
        virtual std::string GetName(const Object& object) const = 0;
        virtual bool RenameObject(const Object& object, const std::string& name) = 0;
        virtual std::set<std::string> GetAllNames() const = 0;
        virtual void Clear() = 0;
    };
}}}
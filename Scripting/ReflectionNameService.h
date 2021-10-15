#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "INameService.h"

#include "Reflection\Objects\LessObjectPointer.h"
#include "Reflection\Attributes\NameAttribute.h"
#include <map>
#include <set>
#include "Config.h"

namespace DNVS { namespace MoFa { namespace Scripting {
    using Reflection::Objects::LessObjectPointer;
#pragma warning(push)
#pragma warning(disable: 4275)
    class SCRIPTING_IMPORT_EXPORT ReflectionNameService : public INameService
    {
    public:
        virtual Object GetObject(const std::string& name) const override;
        virtual bool RemoveObject(const Object& object) override;
        virtual bool HasName(const Object& object) const override;
        virtual bool IsNameInUse(const std::string& name) const override;
        virtual std::string GetStoredName(const Object& object) const override;
        virtual std::string GetName(const Object& object) const override;
        virtual bool RenameObject(const Object& object, const std::string& name) override;
        virtual void Clear() override;
        virtual std::unique_ptr<INewObjectScope> CreateNewObjectScope() const override;
        virtual std::set<std::string> GetAllNames() const override;
    private:
        const Reflection::Attributes::NameAttribute* GetNameAttribute(const Object& object) const;
        std::map<std::string, Object> m_namedObjects;
        std::map<Object, std::string, LessObjectPointer> m_objectsWithNames;
        std::set<Object, LessObjectPointer> m_unnamedObjects;
    };
#pragma warning(pop)
}}}

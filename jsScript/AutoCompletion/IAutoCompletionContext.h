#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <memory>
#include "Reflection\Objects\Object.h"
#include "jsAutoCompletion.h"

namespace DNVS { namespace MoFa {namespace Scripting {
    class JSSCRIPT_IMPORT_EXPORT IAutoCompletionContext
    {
    public:
        virtual ~IAutoCompletionContext() {}
        virtual bool IsGlobalContext() const = 0;
        virtual void CollectMembers(std::shared_ptr<jsAutoCompletion> autoComplete) const = 0;
        virtual void CollectFunctions(const Reflection::Objects::Object& function, std::shared_ptr<jsAutoCompletion> autoComplete) const = 0;
        virtual bool HasContext() const = 0;
        virtual Reflection::Objects::Object lookup(const std::string& function) = 0;
        virtual void SetNestedContext(const Reflection::Objects::Object& nestedContext) = 0;
        virtual void SetGlobalContext() = 0;
    };
}}}
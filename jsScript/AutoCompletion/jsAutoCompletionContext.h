#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IAutoCompletionContext.h"

#include "jsScript\jsValue.h"

namespace DNVS { namespace MoFa { namespace Scripting {
    class JSSCRIPT_IMPORT_EXPORT jsAutoCompletionContext : public IAutoCompletionContext
    {
    public:
        jsAutoCompletionContext();
        virtual bool IsGlobalContext() const override;
        virtual void CollectMembers(std::shared_ptr<jsAutoCompletion> autoComplete) const override;
        virtual void CollectFunctions(const Reflection::Objects::Object& function, std::shared_ptr<jsAutoCompletion> autoComplete) const override;
        virtual bool HasContext() const override;
        virtual Reflection::Objects::Object lookup(const std::string& function) override;
        virtual void SetNestedContext(const Reflection::Objects::Object& nestedContext) override;
        virtual void SetGlobalContext() override;
    private:
        mofa::ref<jsValue> m_context;
    };
}}}
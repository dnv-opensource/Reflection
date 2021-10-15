#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IAutoCompletionContext.h"
#include <list>
namespace DNVS { namespace MoFa {namespace Scripting {
    class JSSCRIPT_IMPORT_EXPORT CompositeAutoCompletionContext : public IAutoCompletionContext
    {
    public:
        template<typename... Args>
        CompositeAutoCompletionContext(const Args&... contexts)
        {
            Push(contexts...);
        }
        virtual bool IsGlobalContext() const override;
        virtual void CollectMembers(std::shared_ptr<jsAutoCompletion> autoComplete) const override;
        virtual void CollectFunctions(const Reflection::Objects::Object& function, std::shared_ptr<jsAutoCompletion> autoComplete) const override;
        virtual bool HasContext() const override;
        virtual Reflection::Objects::Object lookup(const std::string& function) override;
        virtual void SetNestedContext(const Reflection::Objects::Object& nestedContext) override;
        virtual void SetGlobalContext() override;
    private:
        void Push() {}
        template<typename... Args>
        void Push(const std::shared_ptr<IAutoCompletionContext>& context, const Args&... args)
        {
            m_contexts.push_back(context);
            Push(args...);
        }
        std::list<std::shared_ptr<IAutoCompletionContext>> m_contexts;
    };
}}}
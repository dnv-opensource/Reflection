//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "CompositeAutoCompletionContext.h"
#include "Scripting\Scripting.h"

namespace DNVS { namespace MoFa {namespace Scripting {

    bool CompositeAutoCompletionContext::IsGlobalContext() const
    {
        for (const auto& context : m_contexts)
        {
            if (context->IsGlobalContext())
                return true;
        }
        return false;
    }

    void CompositeAutoCompletionContext::CollectMembers(std::shared_ptr<jsAutoCompletion> autoComplete) const
    {
        for (const auto& context : m_contexts)
        {
            context->CollectMembers(autoComplete);
        }
    }

    void CompositeAutoCompletionContext::CollectFunctions(const Reflection::Objects::Object& function, std::shared_ptr<jsAutoCompletion> autoComplete) const
    {
        for (const auto& context : m_contexts)
        {
            context->CollectFunctions(function, autoComplete);
        }
    }

    bool CompositeAutoCompletionContext::HasContext() const
    {
        for (const auto& context : m_contexts)
        {
            if (context->HasContext())
                return true;
        }
        return false;
    }

    Reflection::Objects::Object CompositeAutoCompletionContext::lookup(const std::string& function)
    {
        for (auto& context : m_contexts)
        {
            auto object = context->lookup(function);
            if(object.IsValid())
                return object;
        }
        return Reflection::Objects::Object();
    }

    void CompositeAutoCompletionContext::SetNestedContext(const Reflection::Objects::Object& nestedContext)
    {
        for (auto& context : m_contexts)
        {
            context->SetNestedContext(nestedContext);
        }
    }

    void CompositeAutoCompletionContext::SetGlobalContext()
    {
        for (auto& context : m_contexts)
        {
            context->SetGlobalContext();
        }
    }

}}}


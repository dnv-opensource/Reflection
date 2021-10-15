//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "jsAutoCompletionContext.h"
#include "..\jsModelObject.h"
#include "..\jsStack.h"
#include "..\jsTypeInfo.h"

namespace DNVS { namespace MoFa { namespace Scripting {

    jsAutoCompletionContext::jsAutoCompletionContext()
    {
        SetGlobalContext();
    }

    bool jsAutoCompletionContext::IsGlobalContext() const
    {
        return m_context != nullptr && typeid(*m_context) == typeid(jsModelObject);
    }

    void jsAutoCompletionContext::CollectMembers(std::shared_ptr<jsAutoCompletion> autoComplete) const
    {
        jsTypeInfo::jsValueType type = IsGlobalContext() ? jsTypeInfo::jsAll : jsTypeInfo::jsValueType(~(jsTypeInfo::jsConstructor | jsTypeInfo::jsObject));
        jsValue::TypeInfoSet members;
        jsValue* prototype = m_context;
        while (prototype)
        {
            prototype->getTypeInfo(members, type);
            prototype = prototype->prototype();
        }
        for (const auto& typeInfo : members)
            autoComplete->addVariable(typeInfo->name());
    }

    void jsAutoCompletionContext::CollectFunctions(const Reflection::Objects::Object& function, std::shared_ptr<jsAutoCompletion> autoComplete) const
    {
        mofa::ref<jsValue> fn = function.As<mofa::ref<jsValue>>();
        if (!fn)
            return;
        jsValue::TypeInfoSet typeInfo;
        fn->getTypeInfo(typeInfo, jsTypeInfo::jsFunction | jsTypeInfo::jsConstructor |
            jsTypeInfo::jsPropertyGet | jsTypeInfo::jsPropertyPut, m_context);

        for (jsValue::TypeInfoSet::iterator ifun = typeInfo.begin(); ifun != typeInfo.end(); ifun++)
        {
            jsTypeInfo* info = (*ifun);

            if (info->type() & jsTypeInfo::jsPropertyGet)
            {
                std::string value = info->returned()->name() + " " + info->name() + "()";
                autoComplete->addFunction(value);
            }

            if (info->type() & jsTypeInfo::jsPropertyPut)
            {
                std::string value = info->name() + "(" + info->returned()->name() + ")";
                autoComplete->addFunction(value);
            }

            std::string value = info->format(false);

            if (value.size() > 0)
                autoComplete->addFunction(value);
        }
    }

    bool jsAutoCompletionContext::HasContext() const
    {
        return m_context != nullptr;
    }

    Reflection::Objects::Object jsAutoCompletionContext::lookup(const std::string& function)
    {
        if (m_context)
        {
            mofa::ref<jsValue> value = m_context->lookup(function);
            if (value)
                return Reflection::Objects::Object(jsStack::stack()->GetTypeLibrary(), value);
        }
        return Reflection::Objects::Object();
    }

    void jsAutoCompletionContext::SetNestedContext(const Reflection::Objects::Object& nestedContext)
    {
        m_context = nestedContext.As<mofa::ref<jsValue>>();
    }

    void jsAutoCompletionContext::SetGlobalContext()
    {
        m_context = mofa::ref<jsValue>(new jsModelObject);
    }

}}}


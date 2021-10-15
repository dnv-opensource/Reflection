//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ScriptExpressionEvaluator.h"
#include "Scripting.h"

namespace DNVS {namespace MoFa {namespace Scripting {

    ScriptExpressionEvaluator::ScriptExpressionEvaluator(const std::shared_ptr<IScriptingService>& scriptingService)
        : m_scriptingService(scriptingService)
    {

    }

    Reflection::Objects::Object ScriptExpressionEvaluator::EvaluateExpression(const std::string& text) const noexcept
    {
        try {
            return m_scriptingService->Test(ValidateAndFixScript(text));
        }
        catch (...)
        {
            return Reflection::Objects::Object();
        }
    }

    Reflection::TypeLibraries::TypeLibraryPointer ScriptExpressionEvaluator::GetTypeLibrary() const
    {
        return m_scriptingService->GetTypeLibrary();
    }

}}}


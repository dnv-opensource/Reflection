#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IScriptingService.h"
#include "Reflection\IExpressionEvaluator.h"
#include "Config.h"

namespace DNVS {namespace MoFa {namespace Scripting {
    class SCRIPTING_IMPORT_EXPORT ScriptExpressionEvaluator : public Reflection::IExpressionEvaluator
    {
    public:
        ScriptExpressionEvaluator(const std::shared_ptr<IScriptingService>& scriptingService);
        virtual Reflection::Objects::Object EvaluateExpression(const std::string& text) const noexcept override;
        virtual Reflection::TypeLibraries::TypeLibraryPointer GetTypeLibrary() const override;
    private:
        std::shared_ptr<IScriptingService> m_scriptingService;
    };
}}}
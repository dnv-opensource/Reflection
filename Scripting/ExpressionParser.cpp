//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ExpressionParser.h"
#include "IScriptingService.h"
#include "Services/ServiceProvider.h"
#include "ScriptExpressionEvaluator.h"
#include "Reflection/Attributes/ExpressionExpanderAttribute.h"

namespace DNVS {namespace MoFa {namespace Scripting {
    ExpressionParser::ExpressionParser(const Reflection::Types::DecoratedTypeInfo& info)
        : m_info(info)
    {

    }

    std::string ExpressionParser::ParseExpression(const std::string& expression, const Reflection::Objects::Object& existingValue, bool throwOnError)
    {
        auto scriptingService = Services::ServiceProvider::Instance().TryGetService<Scripting::IScriptingService>();
        if (scriptingService)
        {
            try {
                Scripting::ScriptExpressionEvaluator expressionEvaluator(scriptingService);
                m_object = expressionEvaluator.EvaluateExpression(expression);
                if (m_info.GetTypeInfoPointer() == nullptr || m_object.ConvertTo(m_info).IsValid())
                    return expression;
                else
                {
                    Reflection::ExpressionResult result = Reflection::Attributes::ExpandExpression(expression, m_info, expressionEvaluator, existingValue);
                    if (result.IsValid())
                    {
                        m_object = result.GetValue();
                        if (m_object.IsConvertibleTo(m_info))
                            return result.GetString();
                    }
                }
            }
            catch (const std::exception& e)
            {
                SetErrorMessage(e.what(), throwOnError);
            }
            catch (...)
            {
                SetErrorMessage("Unknown error", throwOnError);
            }
            SetErrorMessage("Unable to convert '" + expression + "' to " + scriptingService->GetTypeLibrary()->GetTypeFormatter()->FormatType(m_info), throwOnError);
        }
        else
            SetErrorMessage("Scripting engine not initialized", throwOnError);
        return "";
    }

    Reflection::Objects::Object ExpressionParser::GetParsedObject() const
    {
        return m_object;
    }

    const std::string& ExpressionParser::GetErrorMessage() const
    {
        return m_errorMessage;
    }

    void ExpressionParser::SetErrorMessage(const std::string& errorMessage, bool throwOnError)
    {
        if (throwOnError)
            throw std::runtime_error(errorMessage);
        else
            m_errorMessage = errorMessage;
    }

}}}
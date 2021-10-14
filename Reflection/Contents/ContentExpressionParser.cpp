//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ContentExpressionParser.h"
#include "IReflectionContentsService.h"
#include "ContentExpressionEvaluator.h"
#include "Reflection/Attributes/ExpressionExpanderAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection { namespace Contents {

    ContentExpressionParser::ContentExpressionParser(const Reflection::Types::DecoratedTypeInfo& info)
        : m_info(info)
    {

    }

    std::string ContentExpressionParser::ParseExpression(const std::string& expression, const Reflection::Objects::Object& existingValue, bool throwOnError /*= false*/)
    {
        auto scriptingService = Services::ServiceProvider::Instance().TryGetService<IReflectionContentsService>();
        if (scriptingService)
        {
            try {
                ContentExpressionEvaluator expressionEvaluator(scriptingService);
                m_object = expressionEvaluator.EvaluateExpression(expression);
                if (m_info.GetTypeInfoPointer() == nullptr || m_object.ConvertTo(m_info).IsValid())
                    return expression;
                else
                {
                    Reflection::ExpressionResult result = Attributes::ExpandExpression(expression, m_info, expressionEvaluator, existingValue);
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

    Objects::Object ContentExpressionParser::GetParsedObject() const
    {
        return m_object;
    }

    const std::string& ContentExpressionParser::GetErrorMessage() const
    {
        return m_errorMessage;
    }

    void ContentExpressionParser::SetErrorMessage(const std::string& errorMessage, bool throwOnError)
    {
        if (throwOnError)
            throw std::runtime_error(errorMessage);
        else
            m_errorMessage = errorMessage;
    }

}}}}


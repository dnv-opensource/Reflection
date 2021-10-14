//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ContentExpressionResult.h"
#include "Reflection/Contents/TypedTextContent.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Contents { 

    size_t FindNextNonSpace(const std::string& text, size_t index)
    {
        while (index < text.size() && isspace(text.at(index)))
            ++index;
        return index;
    }

    ContentExpressionResult TryGenerateNextExpression(const std::string& text, const Types::DecoratedTypeInfo& info, const IExpressionEvaluator& expressionEvaluator, size_t startIndex, size_t index, const ContentExpressionResult& result)
    {
        //If info is "Length":
        //and the full expression to evaluate is "22 m", the first time we enter this function will be when index points to the end of "22".
        //We then evaluate to see if this expression is valid. If this is the case, return it.
        ContentExpressionResult resultOfEntireExpression(text, info, expressionEvaluator, startIndex, index);
        //Check if the expression is convertible to info.
        if (resultOfEntireExpression.IsValid())
        {
            //If this is the first time we are inside this function, or if the previous time we entered produced an invalid result, return the valid result.            
            if (!result.IsValid())
                return resultOfEntireExpression;
            //If we have a more complex expression, such as "22 -44" we must ensure that we return 22, and not 22-44, which is -22.
            //This we do by evaluating the second part of the expression in isolation, that is, we evaluate "-44" and see if it is convertible to our example type, "Length".
            //If it is, then we terminate the search at this point. Caveat: If we have a variable called m: "m = 55 m", then the short syntax evaluation will fail, since "m" can be evaluated in isolation.
            //So: We want to support the expression "22 m", but not the expression "22 -41". Both have a space in it, and we must handle this in a good way.
            if (!ContentExpressionResult(text, info, expressionEvaluator, result.GetEndOfExpression(), index).IsValid())
                return resultOfEntireExpression;            
        }
        return result;
    }

    bool IsRemainderEmpty(const std::string& text, size_t startIndex)
    {
        return FindNextNonSpace(text, startIndex) == text.size();
    }

    ContentExpressionResult::ContentExpressionResult()
        : m_startOfExpression(0)
        , m_endOfExpression(0)
    {

    }

    ContentExpressionResult::ContentExpressionResult(const std::string& text, const Types::DecoratedTypeInfo& info, const IExpressionEvaluator& expressionEvaluator, size_t startIndex, size_t endIndex)
        : m_startOfExpression(startIndex)
        , m_endOfExpression(endIndex)
    {
        m_expandedExpression = text.substr(startIndex, endIndex - startIndex);
        auto object = expressionEvaluator.EvaluateExpression(m_expandedExpression);
        if (object.IsConvertibleTo(info))
        {
            m_value = std::make_shared<TypedTextContent>(m_expandedExpression, info, object);
        }
    }

    ContentExpressionResult::ContentExpressionResult(const std::string& text)
        : m_expandedExpression(text)
    {

    }

    ContentExpressionResult::ContentExpressionResult(const std::shared_ptr<IContent>& value, size_t startAndEnd)
        : m_startOfExpression(startAndEnd)
        , m_endOfExpression(startAndEnd)
        , m_value(value)
        , m_expandedExpression(value->GetScriptText())
    {
    }

    ContentExpressionResult::ContentExpressionResult(const std::string& text, const std::shared_ptr<IContent>& value, size_t start, size_t end)
        : m_startOfExpression(start)
        , m_endOfExpression(end)
        , m_value(value)
        , m_expandedExpression(text)
    {

    }

    bool ContentExpressionResult::IsValid() const
    {
        return m_value && m_value->IsValid();
    }

    std::string ContentExpressionResult::GetString() const
    {
        return m_expandedExpression;
    }

    size_t ContentExpressionResult::GetEndOfExpression() const
    {
        return m_endOfExpression;
    }

    size_t ContentExpressionResult::GetStartOfExpression() const
    {
        return m_startOfExpression;
    }

    std::shared_ptr<IContent> ContentExpressionResult::GetContent() const
    {
        return m_value;
    }

    void ContentExpressionResult::SetExpandedExpression(const std::string& expandedExpression)
    {
        m_expandedExpression = expandedExpression;
    }

    void ContentExpressionResult::AddOffset(size_t startOffset)
    {
        m_startOfExpression += startOffset;
        m_endOfExpression += startOffset;
    }

}}}}



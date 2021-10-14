//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IExpressionEvaluator.h"

namespace DNVS {namespace MoFa { namespace Reflection { 

    size_t FindNextNonSpace(const std::string& text, size_t index = 0)
    {
        while (index < text.size() && isspace(text.at(index)))
            ++index;
        return index;
    }

    ExpressionResult TryGenerateNextExpression(const std::string& text, const Types::DecoratedTypeInfo& info, const IExpressionEvaluator& expressionEvaluator, size_t startIndex, size_t index, const ExpressionResult& result)
    {
        //If info is "Length":
        //and the full expression to evaluate is "22 m", the first time we enter this function will be when index points to the end of "22".
        //We then evaluate to see if this expression is valid. If this is the case, return it.
        ExpressionResult resultOfEntireExpression(text, info, expressionEvaluator, startIndex, index);
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
            if (!ExpressionResult(text, info, expressionEvaluator, result.GetEndOfExpression(), index).IsValid())
                return resultOfEntireExpression;            
        }
        return result;
    }


    /*
    This method iterates through an expression such as:

    "-22 m 18 m"
    It will split the expression list at each space, so the above expression will become "-22", "m", "18", "m".
    Each of these expressions are evaluated using the expressionEvaluator. In addition, we evaluate the combined expression "-22 m"
    The resulting object must be convertible to info.
    */
    ExpressionResult EvaluateNextExpression(const std::string& text, const Types::DecoratedTypeInfo& info, const IExpressionEvaluator& expressionEvaluator, size_t startIndex)
    {
        startIndex = FindNextNonSpace(text, startIndex);
        
        ExpressionResult result;
        bool expressionIsEvaluated = false;
        for (size_t index = startIndex; index < text.size(); ++index)
        {
            if (isspace(text.at(index)))
            {
                if (!expressionIsEvaluated)
                {
                    //Evaluate the expression from startIndex to index (which is a space in the text).
                    result = TryGenerateNextExpression(text, info, expressionEvaluator, startIndex, index, result);
                }
                expressionIsEvaluated = true;
            }
            else
                expressionIsEvaluated = false;
        }
        if (!expressionIsEvaluated)
        {
            result = TryGenerateNextExpression(text, info, expressionEvaluator, startIndex, text.size(), result);
        }
        return result;
    }

    bool IsRemainderEmpty(const std::string& text, size_t startIndex)
    {
        return FindNextNonSpace(text, startIndex) == text.size();
    }

    ExpressionResult::ExpressionResult() 
        : m_startOfExpression(0)
        , m_endOfExpression(0)
    {

    }

    ExpressionResult::ExpressionResult(const std::string& text, const Types::DecoratedTypeInfo& info, const IExpressionEvaluator& expressionEvaluator, size_t startIndex, size_t endIndex)
        : m_startOfExpression(startIndex)
        , m_endOfExpression(endIndex)
    {
        m_expandedExpression = text.substr(startIndex, endIndex - startIndex);
        auto object = expressionEvaluator.EvaluateExpression(m_expandedExpression);
        if (object.IsConvertibleTo(info))
        {
            m_value = object;
        }
    }

    ExpressionResult::ExpressionResult(const std::string& text)
        : m_expandedExpression(text)
    {

    }

    ExpressionResult::ExpressionResult(const Objects::Object& value, size_t startAndEnd)
        : m_startOfExpression(startAndEnd)
        , m_endOfExpression(startAndEnd)
        , m_value(value)
        , m_expandedExpression(ToString(value, Formatting::FormattingService()))
    {

    }

    ExpressionResult::ExpressionResult(const std::string& text, const Objects::Object& value, size_t start, size_t end)
        : m_startOfExpression(start)
        , m_endOfExpression(end)
        , m_value(value)
        , m_expandedExpression(text)
    {

    }

    bool ExpressionResult::IsValid() const
    {
        return m_value.IsValid();
    }

    std::string ExpressionResult::GetString() const
    {
        return m_expandedExpression;
    }

    size_t ExpressionResult::GetEndOfExpression() const
    {
        return m_endOfExpression;
    }

    size_t ExpressionResult::GetStartOfExpression() const
    {
        return m_startOfExpression;
    }

    Objects::Object ExpressionResult::GetValue() const
    {
        return m_value;
    }

    void ExpressionResult::SetExpandedExpression(const std::string& expandedExpression)
    {
        m_expandedExpression = expandedExpression;
    }

    void ExpressionResult::AddOffset(size_t startOffset)
    {
        m_startOfExpression += startOffset;
        m_endOfExpression += startOffset;
    }

}}}



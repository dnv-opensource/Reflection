//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ContentShortSyntaxExpander.h"
#include "Reflection/Contents/MemberContent.h"
#include "Reflection/Contents/TypeContent.h"
#include "Reflection/Attributes/AttributeCollection.h"
#include "Reflection/Utilities/ConstructorsFromTypeService.h"

namespace DNVS {namespace MoFa {namespace Reflection { namespace Contents { 

    ContentShortSyntaxExpression::ContentShortSyntaxExpression(const std::string& expression, const Members::MemberPointer& member, const IExpressionEvaluator& expressionEvaluator, const std::shared_ptr<IContent>& oldContent, bool allowRemainder) 
        : m_member(member)
        , m_expressionEvaluator(expressionEvaluator)
        , m_allowRemainder(allowRemainder)
        , m_expression(expression)
        , m_oldContent(oldContent)
        , m_shortSyntaxAttribute(member->GetAttributeCollection().GetAttribute<Attributes::ShortSyntaxAttribute>())
    {

    }

    bool ContentShortSyntaxExpression::TryExpand()
    {
        if (m_member->GetArity() != m_shortSyntaxAttribute.GetArguments().size())
            return false;
        for (size_t i = 0; i < m_member->GetArity(); ++i)
        {
            size_t currentStart = 0;
            if (!m_results.empty())
                currentStart = m_results.back().GetEndOfExpression();
            m_results.push_back(EvaluateArgument(i, currentStart));
        }
        return Validate();
    }

    ContentExpressionResult ContentShortSyntaxExpression::GetExpressionResult() const
    {
        size_t startIndex = 0;
        size_t endIndex = 0;
        std::vector<std::shared_ptr<IContent>> args;
        for (const auto& result : m_results)
        {
            args.push_back(result.GetContent());
            endIndex = std::max(result.GetEndOfExpression(), endIndex);
        }
        std::shared_ptr<IContent> content;
        if(endIndex == 0 || !m_allowRemainder)
            content = std::make_shared<MemberContent>(m_member, args, m_expression);
        else
            content = std::make_shared<MemberContent>(m_member, args, m_expression.substr(startIndex, endIndex-startIndex));
        ContentExpressionResult result(content->GetScriptText(), content, startIndex, endIndex);
        return result;
    }

    ContentExpressionResult ContentShortSyntaxExpression::GetErrorExpressionOrThrow() const
    {
        if (!m_allowRemainder)
        {
            std::string error = "Invalid short syntax for type " + m_expressionEvaluator.GetTypeLibrary()->GetTypeFormatter()->FormatType(m_member->GetReturnType()) + ".";
            if (!m_shortSyntaxAttribute.GetSampleShortSyntax().empty())
            {
                error += " Expected short syntax of the form '" + m_shortSyntaxAttribute.GetSampleShortSyntax() + "'.";
            }
            throw std::runtime_error(error);
        }
        else
            return GetExpressionResult();
    }

    ContentExpressionResult ContentShortSyntaxExpression::EvaluateNextExpression(const std::string& text, const Types::DecoratedTypeInfo& info, size_t startIndex) const
    {
        startIndex = FindNextNonSpace(text, startIndex);

        ContentExpressionResult result;
        bool expressionIsEvaluated = false;
        for (size_t index = startIndex; index < text.size(); ++index)
        {
            if (isspace(text.at(index)))
            {
                if (!expressionIsEvaluated)
                {
                    //Evaluate the expression from startIndex to index (which is a space in the text).
                    result = TryGenerateNextExpression(text, info, m_expressionEvaluator, startIndex, index, result);
                }
                expressionIsEvaluated = true;
            }
            else
                expressionIsEvaluated = false;
        }
        if (!expressionIsEvaluated)
        {
            result = TryGenerateNextExpression(text, info, m_expressionEvaluator, startIndex, text.size(), result);
        }
        return result;
    }

    std::shared_ptr<IContent> ContentShortSyntaxExpression::TryGetArgumentAtIndex(size_t index) const
    {
        std::shared_ptr<IContent> arg;
        if (m_oldContent)
            arg = m_oldContent->TryGetArgument(m_member, index);
        if (!arg)
            arg = TypeContent(m_member->GetReturnType()).TryGetArgument(m_member, index);
        return arg;
    }

    ContentExpressionResult ContentShortSyntaxExpression::EvaluateArgument(size_t index, size_t currentStart) const
    {
        switch (m_shortSyntaxAttribute.GetArguments()[index])
        {
            case Attributes::ShortSyntaxArgument::Implicit:
            {
                return ContentExpressionResult(TryGetArgumentAtIndex(index), currentStart);
            }
            case Attributes::ShortSyntaxArgument::ShortSyntax:
            {
                ContentExpressionResult result = ExpandExpression(
                    m_expression.substr(currentStart),
                    m_member->GetArgumentInfo(index)->GetTypeInfo(),
                    m_expressionEvaluator,
                    TryGetArgumentAtIndex(index),
                    true
                );
                result.AddOffset(currentStart);
                return result;
            }
            case Attributes::ShortSyntaxArgument::ScriptSyntax:
            default:
            {
                return EvaluateNextExpression(m_expression, m_member->GetArgumentInfo(index)->GetTypeInfo(), currentStart);
            }
        }
    }

    bool ContentShortSyntaxExpression::Validate() const
    {
        if (m_results.size() != m_member->GetArity())
            return false;

        for (const auto& result : m_results)
        {
            if (!result.IsValid())
                return false;
        }
        size_t endOfExpression = 0;
        if (!m_results.empty())
            endOfExpression = m_results.back().GetEndOfExpression();
        if (!m_allowRemainder)
            return IsRemainderEmpty(m_expression, endOfExpression);
        else
            return true;
    }

    ContentExpressionResult ExpandExpression(const std::string& expression, const Types::DecoratedTypeInfo& info, const IExpressionEvaluator& expressionEvaluator, const std::shared_ptr<IContent>& existingValue /*= nullptr*/, bool allowRemainder /*= false */)
    {
        ContentExpressionResult result(expression, info, expressionEvaluator, 0, expression.size());
        if (result.IsValid())
            return result;
        std::exception_ptr ptr;
        ContentExpressionResult bestFit;
        for (auto member : Utilities::ConstructorsFromTypeService::GetService(expressionEvaluator.GetTypeLibrary())->CollectAllConstructorMembersOfThisAndDerivedClasses(info, true))
        {
            if (member->GetAttributeCollection().HasAttribute<Attributes::ShortSyntaxAttribute>())
            {
                try {
                    result = ExpandShortSyntaxExpression(expression, member, expressionEvaluator, existingValue, allowRemainder);
                    if (result.IsValid())
                        return result;
                    else
                        bestFit = result;
                }
                catch (std::exception)
                {
                    ptr = std::current_exception();
                }
            }
        }
        if (ptr)
            std::rethrow_exception(ptr);
        if (bestFit.GetContent())
            return bestFit;
        return ContentExpressionResult(expression);
    }

    ContentExpressionResult ExpandShortSyntaxExpression(const std::string& expression, const Members::MemberPointer& member, const IExpressionEvaluator& expressionEvaluator, const std::shared_ptr<IContent>& existingValue, bool allowRemainder)
    {
        ContentShortSyntaxExpression expander(expression, member, expressionEvaluator, existingValue, allowRemainder);
        expander.TryExpand();
        return expander.GetExpressionResult();
    }

}}}}


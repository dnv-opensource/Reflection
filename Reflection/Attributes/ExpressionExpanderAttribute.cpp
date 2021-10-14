//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ExpressionExpanderAttribute.h"
#include "ShortSyntaxAttribute.h"
#include "AttributeCollection.h"
#include "Reflection/Members/IMember.h"
#include "Reflection/TypeLibraries/IType.h"
#include "Reflection/Utilities/ConstructorsFromTypeService.h"
#include "Reflection/Utilities/ArgumentFromObjectExtracter.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {

    class ShortSyntaxExpressionExpander
    {
    public:
        ShortSyntaxExpressionExpander(
            const std::string& expression, 
            const Members::MemberPointer& member, 
            const IExpressionEvaluator& expressionEvaluator, 
            const Objects::Object& existingValue,
            bool allowRemainder)
            : m_member(member)
            , m_expressionEvaluator(expressionEvaluator)
            , m_allowRemainder(allowRemainder)
            , m_expression(expression)
            , m_existingValue(existingValue)
            , m_shortSyntaxAttribute(member->GetAttributeCollection().GetAttribute<ShortSyntaxAttribute>())
        {}
    public:
        bool TryExpand()
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
        ExpressionResult GetExpressionResult() const
        {
            size_t startIndex = 0;
            size_t endIndex = 0;
            std::vector<std::string> stringArgs;
            std::vector<Variants::Variant> args;
            for (const auto& result : m_results)
            {
                stringArgs.push_back(result.GetString());
                args.push_back(result.GetValue().GetVariant());
                endIndex = result.GetEndOfExpression();
            }
            ExpressionResult result(m_member->ToString(stringArgs), Objects::Object(m_expressionEvaluator.GetTypeLibrary(), m_member->Invoke(args)), startIndex, endIndex);
            return result;
        }
        ExpressionResult GetErrorExpressionOrThrow() const
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
                return ExpressionResult(m_expression);
        }
    private:
        ExpressionResult EvaluateArgument(size_t index, size_t currentStart) const
        {
            switch (m_shortSyntaxAttribute.GetArguments()[index])
            {
                case ShortSyntaxArgument::Implicit:
                {
                    auto object = GetObjectAtIndex(index);
                    return ExpressionResult(object, currentStart);
                }
                case ShortSyntaxArgument::ShortSyntax:
                {
                    ExpressionResult result = ExpandExpression(
                        m_expression.substr(currentStart),
                        m_member->GetArgumentInfo(index)->GetTypeInfo(),
                        m_expressionEvaluator,
                        GetObjectAtIndex(index),
                        true
                    );
                    result.AddOffset(currentStart);
                    return result;
                }
                case ShortSyntaxArgument::ScriptSyntax:
                default:
                {
                    return EvaluateNextExpression(m_expression, m_member->GetArgumentInfo(index)->GetTypeInfo(), m_expressionEvaluator, currentStart);
                }
            }
        }
        Objects::Object GetObjectAtIndex(size_t index) const
        {
            auto result = Utilities::ArgumentFromObjectExtracter(m_expressionEvaluator.GetTypeLibrary(), m_member, index)(m_existingValue.GetVariant());
            return Objects::Object(m_expressionEvaluator.GetTypeLibrary(), result);
        }
        bool Validate() const
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
        std::vector<ExpressionResult> m_results;
        Members::MemberPointer m_member;
        const IExpressionEvaluator& m_expressionEvaluator;
        bool m_allowRemainder;
        std::string m_expression;
        Objects::Object m_existingValue;
        const Attributes::ShortSyntaxAttribute& m_shortSyntaxAttribute;
    };

    ExpressionResult ExpandShortSyntaxExpression(const std::string& expression, const Members::MemberPointer& member, const IExpressionEvaluator& expressionEvaluator, const Objects::Object& existingValue, bool allowRemainder)
    {
        ShortSyntaxExpressionExpander expander(expression, member, expressionEvaluator, existingValue, allowRemainder);
        if (expander.TryExpand())
        {
            return expander.GetExpressionResult();
        }
        return expander.GetErrorExpressionOrThrow();
    }

    bool SupportsShortSyntax(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Types::DecoratedTypeInfo& info)
    {
        for (auto member : Utilities::ConstructorsFromTypeService::GetService(typeLibrary)->CollectAllConstructorMembersOfThisAndDerivedClasses(info, true))
        {
            if (member->GetAttributeCollection().HasAttribute<ShortSyntaxAttribute>())
                return true;
        }
        return false;
    }

    ExpressionResult ExpandExpression(const std::string& expression, const Types::DecoratedTypeInfo& info, const IExpressionEvaluator& expressionEvaluator, const Objects::Object& existingValue, bool allowRemainder)
    {
        ExpressionResult result(expression, info, expressionEvaluator, 0, expression.size());
        if (result.IsValid())
            return result;
        std::exception_ptr ptr;
        for (auto member : Utilities::ConstructorsFromTypeService::GetService(expressionEvaluator.GetTypeLibrary())->CollectAllConstructorMembersOfThisAndDerivedClasses(info, true))
        {
            if (member->GetAttributeCollection().HasAttribute<ShortSyntaxAttribute>())
            {
                try {
                    result = ExpandShortSyntaxExpression(expression, member, expressionEvaluator, existingValue, allowRemainder);
                    if (result.IsValid())
                        return result;
                }
                catch(std::exception)
                {
                    ptr = std::current_exception();
                }
            }
        }
        if (ptr)
            std::rethrow_exception(ptr);
        return ExpressionResult(expression);
    }

    std::string TryGetShortSyntax(const Members::MemberPointer& member, const std::vector<std::string>& args)
    {
        if (!member)
            throw std::runtime_error("member is null");
        if (!member->GetAttributeCollection().HasAttribute<ShortSyntaxAttribute>())
            return member->ToString(args);
        else
        {
            std::string result;
            for (const std::string& str : args)
            {
                if (!result.empty())
                    result += " ";
                result += str;
            }
            return result;
        }
    }

}}}}

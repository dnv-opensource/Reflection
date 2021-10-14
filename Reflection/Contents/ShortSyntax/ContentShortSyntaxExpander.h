#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Attributes/ShortSyntaxAttribute.h"
#include "ContentExpressionResult.h"
namespace DNVS {namespace MoFa {namespace Reflection { namespace Contents { 
    class ContentShortSyntaxExpression
    {
    public:
        ContentShortSyntaxExpression(
            const std::string& expression,
            const Members::MemberPointer& member,
            const IExpressionEvaluator& expressionEvaluator,
            const std::shared_ptr<IContent>& oldContent,
            bool allowRemainder);
        bool TryExpand();
        ContentExpressionResult GetExpressionResult() const;
        ContentExpressionResult GetErrorExpressionOrThrow() const;
        /*
        Evaluates an expression, starting at a given position inside the expression, to find out if it represents an expression of type info.
        Example:
        We want to support the short syntax "0m 1 m 4m" in place of "Vector3d(0m, 1m, 4m)".
        To achieve this, we can write:
        //Captures the first part of the expression, "0m", and validates its type as Units::Length.
        auto x = EvaluateNextExpression(expression, Types::TypeId<Units::Length>(), evaluator, 0);
        //Captures the first part of the expression, "1 m", and validates its type as Units::Length.
        auto y = EvaluateNextExpression(expression, Types::TypeId<Units::Length>(), evaluator, x.GetEndOfExpression());
        //Captures the first part of the expression, "4m", and validates its type as Units::Length.
        auto z = EvaluateNextExpression(expression, Types::TypeId<Units::Length>(), evaluator, y.GetEndOfExpression());
        //Checks that x,y and z are all valid expressions (of the correct type) and that the rest of the expression is empty (save spaces)
        if (x.IsValid() && y.IsValid() && z.IsValid() && IsRemainderEmpty(expression, z.GetEndOfExpression()))
        */
        /*
This method iterates through an expression such as:

"-22 m 18 m"
It will split the expression list at each space, so the above expression will become "-22", "m", "18", "m".
Each of these expressions are evaluated using the expressionEvaluator. In addition, we evaluate the combined expression "-22 m"
The resulting object must be convertible to info.
*/
        ContentExpressionResult EvaluateNextExpression(const std::string& text, const Types::DecoratedTypeInfo& info, size_t startIndex) const;
    private:
        std::shared_ptr<IContent> TryGetArgumentAtIndex(size_t index) const;
        ContentExpressionResult EvaluateArgument(size_t index, size_t currentStart) const;
        bool Validate() const;
        std::vector<ContentExpressionResult> m_results;
        Members::MemberPointer m_member;
        const IExpressionEvaluator& m_expressionEvaluator;
        bool m_allowRemainder;
        std::string m_expression;
        std::shared_ptr<IContent> m_oldContent;
        const Attributes::ShortSyntaxAttribute& m_shortSyntaxAttribute;
    };

    //Evaluates whether expression is convertible to info. If not, it tries to expand the expression, using the ExpressionExpanderAttribute for info, if found.
    ContentExpressionResult ExpandExpression(
        const std::string& expression,
        const Types::DecoratedTypeInfo& info,
        const IExpressionEvaluator& expressionEvaluator,
        const std::shared_ptr<IContent>& existingValue = nullptr,
        bool allowRemainder = false
    );

    ContentExpressionResult ExpandShortSyntaxExpression(
        const std::string& expression,
        const Members::MemberPointer& member,
        const IExpressionEvaluator& expressionEvaluator,
        const std::shared_ptr<IContent>& existingValue = nullptr,
        bool allowRemainder = false
    );

}}}}
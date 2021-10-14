#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Objects/Object.h"

namespace DNVS {namespace MoFa { namespace Reflection { 
    //The expression evaluator will evaluate any javascript expression to check whether it is valid or not.
    //If it is valid, it will return an Object representing this expression. If it is not valid, it will return an empty Object.
    class IExpressionEvaluator
    {
    public:
        virtual ~IExpressionEvaluator() {}
        //Evaluates an expression. Returns a valid object if expression is valid, otherwise returns an invalid expression.
        virtual Objects::Object EvaluateExpression(const std::string& expression) const noexcept = 0;
        virtual TypeLibraries::TypeLibraryPointer GetTypeLibrary() const = 0;
    };

    /*
    ExpressionResult represents a subset of an expression, generated by using the EvaluateNextExpression command.
    Example:
    If we want to evaluate the expression "0 m 8m 5m", ExpressionResult for the first expression would hold:
    The start position in the string for this expression: 0,
    The end position in the string for this expression: 3,
    The object evaluated by this expression: Length(0) - represented by an Object.
    */
    struct REFLECTION_IMPORT_EXPORT ExpressionResult
    {
        ExpressionResult(const std::string& text,const Objects::Object& value, size_t start, size_t end);
        ExpressionResult(const Objects::Object& value, size_t startAndEnd);
        ExpressionResult();
        ExpressionResult(const std::string& text);
        ExpressionResult(const std::string& text, const Types::DecoratedTypeInfo& info, const IExpressionEvaluator& expressionEvaluator, size_t startIndex, size_t endIndex);
        bool IsValid() const;
        std::string GetString() const;
        size_t GetEndOfExpression() const;
        size_t GetStartOfExpression() const;
        Objects::Object GetValue() const;
        void SetExpandedExpression(const std::string& expandedExpression);
        void AddOffset(size_t startOffset);
    private:
        size_t m_startOfExpression;
        size_t m_endOfExpression;
        Objects::Object m_value;
        std::string m_expandedExpression;
    };
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
    ExpressionResult REFLECTION_IMPORT_EXPORT EvaluateNextExpression(const std::string& text, const Types::DecoratedTypeInfo& info, const IExpressionEvaluator& expressionEvaluator, size_t startIndex = 0);
    /*
    Returns true if the rest of the characters after startIndex are blank. Otherwise, returns false.
    */
    bool REFLECTION_IMPORT_EXPORT IsRemainderEmpty(const std::string& text, size_t startIndex);
}}}

#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include "Reflection/IExpressionEvaluator.h"
#include "boost\none.hpp"
#include "boost\optional\optional.hpp"
#include "TypeUtilities/FunctionTraits.h"
#include "Reflection/Config.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    //Wrapper callback that translates Objects::Object to boost::optional<T>.
    template<typename T>
    class ShortScriptSyntaxCallback 
    {
    public:
        typedef std::function<std::string(const std::string&, const IExpressionEvaluator&, boost::optional<T>)> InputCallback;
        ShortScriptSyntaxCallback(const InputCallback& callback) : m_inputCallback(callback) {}
        std::string operator()(const std::string& shortScript, const IExpressionEvaluator& evaluator, const Objects::Object& existingValue)
        {
            if (existingValue.IsConvertibleTo<T>())
                return m_inputCallback(shortScript, evaluator, existingValue.As<T>());
            else
                return m_inputCallback(shortScript, evaluator, boost::none);
        }
    private:
        InputCallback m_inputCallback;
    };
    /*
    Use the ExpressionExpanderAttribute when you want to allow short syntax in any dialog field.
    What this means, is that if you want to allow the user to write
    "0m 0m 1m" in place of "Vector3d(0m, 0m, 1m)", you need to add to your reflected class:
    cls.AddAttribute<ExpressionExpanderAttribute>(EvaluateShortSyntax);
    Where EvaluateShortSyntax is a function with the following signature:
    std::string EvaluateShortSyntax(const std::string&, const IExpressionEvaluator&, boost::optional<T> existingValue);
    and T is the type you want to reflect.
    The method should evaluate whether the syntax is valid and return a valid script expression.
    If the syntax is not valid, it should just return the input unmodified.
    In the above example:
    EvaluateShortSyntax("0 0 ", ...); should return "0 0 "
    whereas
    EvaluateShortSyntax(" 0 0 1", ...); should return "Vector3d(0, 0, 1)".
    */
    class ExpressionExpanderAttribute {
    public:
        typedef std::function<std::string(const std::string&, const IExpressionEvaluator&, const Objects::Object&)> Callback;
        template<typename CallbackT>
        ExpressionExpanderAttribute(const CallbackT& callback)
        {
            #define SHORT_SCRIPT_SYNTAX_ERROR_MESSAGE "The function signature must look like this: (const std::string&, const IExpressionEvaluator&, boost::optional<T>), where T is the type you want to evaluate short syntax for."
            static_assert(TypeUtilities::FunctionTraits<CallbackT>::Arity == 3, SHORT_SCRIPT_SYNTAX_ERROR_MESSAGE);
            static_assert(std::is_convertible_v<TypeUtilities::FunctionTraits<CallbackT>::Argument0, const std::string&>, SHORT_SCRIPT_SYNTAX_ERROR_MESSAGE);
            static_assert(std::is_convertible_v<TypeUtilities::FunctionTraits<CallbackT>::Argument1, const IExpressionEvaluator&>, SHORT_SCRIPT_SYNTAX_ERROR_MESSAGE);
            typedef typename TypeUtilities::FunctionTraits<CallbackT>::Argument2::value_type value_type;
            #undef SHORT_SCRIPT_SYNTAX_ERROR_MESSAGE
            m_callback = ShortScriptSyntaxCallback<value_type>(callback);
        }
        std::string ExpandScript(const std::string& shortScript, const IExpressionEvaluator& evaluator, const Objects::Object& existingValue) const
        {
            return m_callback(shortScript, evaluator, existingValue);
        }
    private:
        Callback m_callback;
    };
    //Evaluates whether expression is convertible to info. If not, it tries to expand the expression, using the ExpressionExpanderAttribute for info, if found.
    ExpressionResult REFLECTION_IMPORT_EXPORT ExpandExpression(
        const std::string& expression,
        const Types::DecoratedTypeInfo& info,
        const IExpressionEvaluator& expressionEvaluator,
        const Objects::Object& existingValue = Objects::Object(),
        bool allowRemainder = false
    );
    ExpressionResult REFLECTION_IMPORT_EXPORT ExpandShortSyntaxExpression(
        const std::string& expression,
        const Members::MemberPointer& member,
        const IExpressionEvaluator& expressionEvaluator,
        const Objects::Object& existingValue,
        bool allowRemainder
    );
    bool REFLECTION_IMPORT_EXPORT SupportsShortSyntax(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Types::DecoratedTypeInfo& info);
    std::string REFLECTION_IMPORT_EXPORT TryGetShortSyntax(const Members::MemberPointer& member, const std::vector<std::string>& args);
}}}}

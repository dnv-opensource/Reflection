#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <functional>
#include "Reflection/Variants/Variant.h"
#include "Formatting/FormattingService.h"
#include "TypeUtilities/FunctionTraits.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    
    template<typename LambdaFunctionT>
    class ParserAttributeCallback
    {
    public:
#define PARSER_ATTRIBUTE_CALLBACK_ERROR_MESSAGE "The function signature must look like this: boost::optional<T>(const std::string&, const Formatting::FormattingService& service), where T is the type you want to parse for."
        static_assert(TypeUtilities::FunctionTraits<LambdaFunctionT>::Arity == 2, PARSER_ATTRIBUTE_CALLBACK_ERROR_MESSAGE);
        static_assert(std::is_convertible_v<typename TypeUtilities::FunctionTraits<LambdaFunctionT>::Argument0, const std::string&>, PARSER_ATTRIBUTE_CALLBACK_ERROR_MESSAGE);
        static_assert(std::is_convertible_v<typename TypeUtilities::FunctionTraits<LambdaFunctionT>::Argument1, const DNVS::MoFa::Formatting::FormattingService&>, PARSER_ATTRIBUTE_CALLBACK_ERROR_MESSAGE);
        typedef typename TypeUtilities::FunctionTraits<LambdaFunctionT>::ReturnType::value_type value_type;
#undef PARSER_ATTRIBUTE_CALLBACK_ERROR_MESSAGE

        ParserAttributeCallback(const LambdaFunctionT& callback) : m_inputCallback(callback) {}
        Variants::Variant operator()(const std::string& shortScript, const DNVS::MoFa::Formatting::FormattingService& service)
        {
            boost::optional<value_type> value = m_inputCallback(shortScript, service);
            if (value)
                return Variants::VariantService::ReflectType<value_type>(*value);
            else
                return Variants::Variant();
        }
    private:
        LambdaFunctionT m_inputCallback;
    };

    template<typename T>
    struct LambdaFunctionGetter 
    {
        template<typename LambdaFunctionT>
        static ParserAttributeCallback<LambdaFunctionT> Get(const LambdaFunctionT& lambdaFunction)
        {
            return ParserAttributeCallback<LambdaFunctionT>(lambdaFunction);
        }
    };

    template<>
    struct LambdaFunctionGetter<Variants::Variant>
    {
        template<typename LambdaFunctionT>
        static const LambdaFunctionT& Get(const LambdaFunctionT& lambdaFunction)
        {
#define PARSER_ATTRIBUTE_CALLBACK_ERROR_MESSAGE "The function signature must look like this: Variants::Variant(const std::string&, const DNVS::MoFa::Formatting::FormattingService& service), where T is the type you want to parse for."
            static_assert(TypeUtilities::FunctionTraits<LambdaFunctionT>::Arity == 2, PARSER_ATTRIBUTE_CALLBACK_ERROR_MESSAGE);
            static_assert(std::is_convertible_v<TypeUtilities::FunctionTraits<LambdaFunctionT>::Argument0, const std::string&>, PARSER_ATTRIBUTE_CALLBACK_ERROR_MESSAGE);
            static_assert(std::is_convertible_v<TypeUtilities::FunctionTraits<LambdaFunctionT>::Argument1, const DNVS::MoFa::Formatting::FormattingService&>, PARSER_ATTRIBUTE_CALLBACK_ERROR_MESSAGE);
#undef PARSER_ATTRIBUTE_CALLBACK_ERROR_MESSAGE
            return lambdaFunction;
        }
    };

    /*
    The parser attribute gives a way of taking a text string and converting it back to a variant. It is a very simplified version of the javascript parser.
    The reason we need this is for performance during XML import.
    You need to pass in a lambda function that does the actual conversion.
    */
    class ParserAttribute 
    {
    public:
        template<typename LambdaFunctionT>
        ParserAttribute(LambdaFunctionT lambdaFunction)
            : m_callback(LambdaFunctionGetter<TypeUtilities::FunctionTraits<LambdaFunctionT>::ReturnType>::Get(lambdaFunction))
        {
        }
        Variants::Variant Parse(const std::string& text, const DNVS::MoFa::Formatting::FormattingService& service) const
        {
            return m_callback(text, service);
        }
    private:
        std::function<Variants::Variant(const std::string&, const DNVS::MoFa::Formatting::FormattingService&)> m_callback;
    };

}}}}
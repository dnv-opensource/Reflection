#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Objects/Object.h"
#include "Reflection/Types/DecoratedTypeInfo.h"

namespace DNVS {namespace MoFa {namespace Reflection { namespace Contents {
    //The expression parser will use the scripting engine and the short syntax parser in the reflection library to parse an expression and return a scriptable version of that string + the parsed object.
    class ContentExpressionParser
    {
    public:
        ContentExpressionParser(const Reflection::Types::DecoratedTypeInfo& info);
        std::string ParseExpression(const std::string& expression, const Reflection::Objects::Object& existingValue, bool throwOnError = false);
        Reflection::Objects::Object GetParsedObject() const;
        const std::string& GetErrorMessage() const;
    private:
        void SetErrorMessage(const std::string& errorMessage, bool throwOnError);
        Reflection::Types::DecoratedTypeInfo m_info;
        Reflection::Objects::Object m_object;
        std::string m_errorMessage;
    };
}}}}
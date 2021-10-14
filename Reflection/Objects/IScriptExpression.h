#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>

namespace DNVS {namespace MoFa {namespace Reflection {namespace Objects {
    class IScriptExpression {
    public:
        virtual ~IScriptExpression() {}
        virtual void SetScriptExpression(const std::string& text) = 0;
        virtual std::string GetScriptExpression() const = 0;
        virtual Objects::Object GetObject() const = 0;
    };
}}}}
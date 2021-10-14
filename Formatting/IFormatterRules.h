#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IFormatter.h"
#include <string>
#include <memory>

namespace DNVS { namespace MoFa { namespace Formatting {
    class FORMATTING_IMPORT_EXPORT IFormatterRules : public IFormatter
    {
    public:
        //If true, some expressions can be enclosed in parenthesis, such as "(AISC)" if AISC is deduced from structure.
        virtual bool ShouldEncloseExpression() const = 0;

        //If true, "Bm1.CurveOffset" is expanded to "LinearVaryingCurveOffset(...)"
        virtual bool ExpandExpression() const = 0;
        enum ExpressionRepresentation
        {
            Serialization, //Use SerializationAttribute value if available
            SimplifiedGui, //Allows "Vector3d(1m, 2m, 4m)" to be formatted as "1m 2m 4m"
            SimplifiedReport, //Allows "Vector3d(1m, 2m, 4m)" to be formatted as "<1m, 2m, 4m>"
            Exact //"Vector3d(1m, 2m, 4m)" is represented as "Vector3d(1m, 2m, 4m)".
        };
        //Returns the formatting rules for expressions.
        virtual ExpressionRepresentation GetExpressionRepresentation() const = 0;

        //If false, allows enums such as "ssFromStructure" to be dispayed as "From Structure"
        virtual bool RequireValidScript() const = 0;
        static std::string ConditionallyEncloseExpression(const std::string& expression, const std::shared_ptr<IFormatterRules>& rules)
        {
            if (rules->ShouldEncloseExpression())
                return "(" + expression + ")";
            else
                return expression;
        }
    }; 
}}}

#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IFormatterRules.h"

namespace DNVS { namespace MoFa { namespace Formatting {
    class FORMATTING_IMPORT_EXPORT DefaultFormatterRules : public IFormatterRules
    {
    public:
        DefaultFormatterRules(bool shouldEncloseExpression = false, ExpressionRepresentation representation = Exact, bool requireValidSctipt = true, bool expandExpression = false);
        virtual bool ShouldEncloseExpression() const override;
        //Returns the formatting rules for expressions.
        virtual ExpressionRepresentation GetExpressionRepresentation() const override;

        //If false, allows enums such as "ssFromStructure" to be displayed as "From Structure"
        virtual bool RequireValidScript() const override;
        virtual bool ExpandExpression() const override;
    private:
        ExpressionRepresentation m_representation;
        bool m_shouldEncloseExpression;
        bool m_requireValidSctipt;
        bool m_expandExpression;
    };
}}}

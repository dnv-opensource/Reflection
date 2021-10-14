//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DefaultFormatterRules.h"

namespace DNVS { namespace MoFa { namespace Formatting {

    DefaultFormatterRules::DefaultFormatterRules(bool shouldEncloseExpression, ExpressionRepresentation representation, bool requireValidSctipt, bool expandExpression)
        : m_shouldEncloseExpression(shouldEncloseExpression)
        , m_representation(representation)
        , m_requireValidSctipt(requireValidSctipt)
        , m_expandExpression(expandExpression)
    {        
    }

    bool DefaultFormatterRules::ShouldEncloseExpression() const
    {
        return m_shouldEncloseExpression;
    }

    DefaultFormatterRules::ExpressionRepresentation DefaultFormatterRules::GetExpressionRepresentation() const
    {
        return m_representation;
    }

    bool DefaultFormatterRules::RequireValidScript() const
    {
        return m_requireValidSctipt;
    }

    bool DefaultFormatterRules::ExpandExpression() const
    {
        return m_expandExpression;
    }

}}}



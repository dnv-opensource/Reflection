#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Scripting\Scripting.h"
#include "Scripting\ExpressionParser.h"
#include "jsType.h"

template<class T>
std::string evaluateSyntax(const std::string& str, jsType<T>)
{
    DNVS::MoFa::Scripting::ExpressionParser parser(DNVS::MoFa::Reflection::Types::TypeId<T>());
    return parser.ParseExpression(str, DNVS::MoFa::Reflection::Objects::Object(), true);
}

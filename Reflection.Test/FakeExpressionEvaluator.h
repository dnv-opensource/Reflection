#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection\IExpressionEvaluator.h"
#include "boost\lexical_cast.hpp"
#include "Reflection\TypeLibraries\TypeLibraryPointer.h"


namespace DNVS { namespace MoFa { namespace Reflection {
    class FakeExpressionEvaluator : public IExpressionEvaluator
    {
    public:
        FakeExpressionEvaluator(TypeLibraries::TypeLibraryPointer lib) : m_lib(lib) {}
        virtual Objects::Object EvaluateExpression(const std::string& expression) const noexcept
        {
            try {
                return Objects::Object(m_lib, boost::lexical_cast<double>(expression));
            }
            catch (...)
            {
                return Objects::Object();
            }
        }
        virtual TypeLibraries::TypeLibraryPointer GetTypeLibrary() const
        {
            return m_lib;
        }
    private:
        TypeLibraries::TypeLibraryPointer m_lib;
    };
}}}
#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include "IAttribute.h"
namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    /**
    Allows a programmer to display an alternative text when formatting an expression with value nullptr.
    Usage:
    Class<ICurveOffset,CurveOffsetPointer> cls(...);
    cls.AddAttribute<AlternativeNullPointerNameAttribute>("NoCurveOffset()");
    */
    class AlternativeNullPointerNameAttribute : public IAttribute {
    public:
        AlternativeNullPointerNameAttribute(const std::string& nullPointerName) : m_nullPointerName(nullPointerName) {}
        const std::string& GetNullPointerName() const { return m_nullPointerName; }
    private:
        std::string m_nullPointerName;
    };
}}}}
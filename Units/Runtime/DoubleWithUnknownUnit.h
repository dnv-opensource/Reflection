#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "../Config.h"
#include <ostream>
namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {
    class DynamicQuantity;
    class UNITS_IMPORT_EXPORT DoubleWithUnknownUnit {
    public:
        DoubleWithUnknownUnit(double value);
        DynamicQuantity operator+(const DynamicQuantity& other) const;
        friend UNITS_IMPORT_EXPORT DynamicQuantity operator+(const DynamicQuantity& lhs, const DoubleWithUnknownUnit& rhs);
        DynamicQuantity operator-(const DynamicQuantity& other) const;
        friend UNITS_IMPORT_EXPORT DynamicQuantity operator-(const DynamicQuantity& lhs, const DoubleWithUnknownUnit& rhs);
        double GetValue() const { return m_value; }
    private:
        double m_value;
    };
    template<class _Elem, class _Traits> inline
    std::basic_ostream<_Elem, _Traits>& operator<<(std::basic_ostream<_Elem, _Traits>& stream, const DoubleWithUnknownUnit& doubleWithUnknownUnit)
    {
        return stream << doubleWithUnknownUnit.GetValue();
    }
}}}}

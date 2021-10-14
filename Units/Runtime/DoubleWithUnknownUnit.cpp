//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DoubleWithUnknownUnit.h"
#include "DatabaseUnitSystem.h"
#include "DynamicQuantity.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {

    DoubleWithUnknownUnit::DoubleWithUnknownUnit(double value) : m_value(value)
    {

    }

    DynamicQuantity DoubleWithUnknownUnit::operator+(const DynamicQuantity& other) const
    {
        return DynamicQuantity(m_value, GetInputUnitSystem().GetUnit(other.GetSimplifiedUnit().GetDimension())) + other;
    }

    DynamicQuantity operator+(const DynamicQuantity& lhs, const DoubleWithUnknownUnit& rhs)
    {
        return lhs + DynamicQuantity(rhs.m_value, GetInputUnitSystem().GetUnit(lhs.GetSimplifiedUnit().GetDimension()));
    }

    DynamicQuantity DoubleWithUnknownUnit::operator-(const DynamicQuantity& other) const
    {
        return DynamicQuantity(m_value, GetInputUnitSystem().GetUnit(other.GetSimplifiedUnit().GetDimension())) - other;
    }

    DynamicQuantity operator-(const DynamicQuantity& lhs, const DoubleWithUnknownUnit& rhs)
    {
        return lhs - DynamicQuantity(rhs.m_value, GetInputUnitSystem().GetUnit(lhs.GetSimplifiedUnit().GetDimension()));
    }

}}}}


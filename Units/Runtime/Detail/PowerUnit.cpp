//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "PowerUnit.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime { namespace Detail {

    PowerUnit::PowerUnit( UnitPointer lhs, int exponent )
        :   m_lhs(lhs)
        ,   m_exponent(exponent)
    {
    }

    double PowerUnit::GetConversionFactor() const
    {
        return pow(m_lhs->GetConversionFactor(), m_exponent);
    }

    std::string PowerUnit::GetUnitName( UnitNameModifier modifier ) const
    {
        return m_lhs->GetUnitName(Pow(modifier, m_exponent));
    }

    DynamicDimension PowerUnit::GetDimension() const
    {
        return m_lhs->GetDimension()*m_exponent;
    }

    std::list<UnitPointer> PowerUnit::GetChildren() const
    {
        std::list<UnitPointer> children;
        children.push_back(m_lhs);
        return std::move(children);
    }

    PowerUnit* PowerUnit::CreateCopy( const std::list<UnitPointer>& children ) const
    {
        if(children.size() != 1) 
            throw std::runtime_error("children should have size 1");
        return new PowerUnit(children.front(), m_exponent);
    }

    void PowerUnit::GetUnitComponents( ListOfFundamentalUnitsWithExponents& listOfFundamentalUnits, UnitNameModifier modifier) const
    {
        m_lhs->GetUnitComponents(listOfFundamentalUnits, Pow(modifier, m_exponent));
    }

    UnitPointer PowerUnit::CreatePowerUnit( UnitPointer lhs, int exponent )
    {
        if(exponent == 1) 
            return lhs;
        else 
            return std::make_shared<PowerUnit>(lhs, exponent);
    }

}}}}}
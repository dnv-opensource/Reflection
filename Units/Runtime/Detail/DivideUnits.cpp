//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DivideUnits.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime { namespace Detail {
    DivideUnits::DivideUnits( UnitPointer lhs, UnitPointer rhs )
        :   m_lhs(lhs)
        ,   m_rhs(rhs)
    {
    }

    double DivideUnits::GetConversionFactor() const
    {
        return m_lhs->GetConversionFactor() / m_rhs->GetConversionFactor();
    }

    std::string DivideUnits::GetUnitName(UnitNameModifier modifier) const
    {
        return m_lhs->GetUnitName(modifier) + Simplify(modifier).GetDivideSymbol() + m_rhs->GetUnitName(Simplify(!modifier));
    }

    DynamicDimension DivideUnits::GetDimension() const
    {
        return m_lhs->GetDimension() - m_rhs->GetDimension();
    }

    std::list<UnitPointer> DivideUnits::GetChildren() const
    {
        std::list<UnitPointer> children;
        children.push_back(m_lhs);
        children.push_back(m_rhs);
        return std::move(children);        
    }

    DivideUnits* DivideUnits::CreateCopy( const std::list<UnitPointer>& children ) const
    {
        if(children.size() != 2) 
            throw std::runtime_error("children should have size 2");
        return new DivideUnits(children.front(), children.back());
    }

    void DivideUnits::GetUnitComponents( ListOfFundamentalUnitsWithExponents& listOfFundamentalUnits, UnitNameModifier modifier) const
    {
        m_lhs->GetUnitComponents(listOfFundamentalUnits , modifier);
        m_rhs->GetUnitComponents(listOfFundamentalUnits , !modifier);
    }

}}}}}
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "MultiplyUnits.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime { namespace Detail {
    MultiplyUnits::MultiplyUnits( UnitPointer lhs, UnitPointer rhs )
        :   m_lhs(lhs)
        ,   m_rhs(rhs)
    {
    }

    double MultiplyUnits::GetConversionFactor() const
    {
        return m_lhs->GetConversionFactor() * m_rhs->GetConversionFactor();
    }

    std::string MultiplyUnits::GetUnitName(UnitNameModifier modifier) const
    {
        return m_lhs->GetUnitName(modifier) + Simplify(modifier).GetMultiplySymbol() + m_rhs->GetUnitName(Simplify(modifier));
    }

    DynamicDimension MultiplyUnits::GetDimension() const
    {
        return m_lhs->GetDimension() + m_rhs->GetDimension();
    }

    std::list<UnitPointer> MultiplyUnits::GetChildren() const
    {
        std::list<UnitPointer> children;
        children.push_back(m_lhs);
        children.push_back(m_rhs);
        return std::move(children);
    }

    MultiplyUnits* MultiplyUnits::CreateCopy( const std::list<UnitPointer>& children ) const
    {
        if(children.size() != 2) 
            throw std::runtime_error("children should have size 2");
        return new MultiplyUnits(children.front(), children.back());        
    }

    void MultiplyUnits::GetUnitComponents( ListOfFundamentalUnitsWithExponents& listOfFundamentalUnits, UnitNameModifier modifier) const
    {
        m_lhs->GetUnitComponents(listOfFundamentalUnits, modifier);
        m_rhs->GetUnitComponents(listOfFundamentalUnits, modifier);
    }

}}}}}
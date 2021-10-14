//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "FundamentalUnit.h"
#include "ListOfFundamentalUnitsWithExponents.h"
namespace DNVS {namespace MoFa {namespace Units { namespace Runtime { namespace Detail {

    FundamemtalUnit::FundamemtalUnit( const std::string& name, double conversionFactor, const DynamicDimension& dimension ) 
        :   m_name(name)
        ,   m_conversionFactor(conversionFactor)
        ,   m_dimension(dimension)
    {
    }

    double FundamemtalUnit::GetConversionFactor() const
    {
        return m_conversionFactor;
    }

    DynamicDimension FundamemtalUnit::GetDimension() const
    {
        return m_dimension;
    }

    std::string FundamemtalUnit::GetUnitName(UnitNameModifier modifier) const
    {
        return m_name + modifier.GetExponentSymbol();
    }

    std::list<UnitPointer> FundamemtalUnit::GetChildren() const
    {
        return std::list<UnitPointer>();
    }

    FundamemtalUnit* FundamemtalUnit::CreateCopy( const std::list<UnitPointer>& children ) const
    {
        if(children.size() != 0) 
            throw std::runtime_error("children should have size 0");
        return new FundamemtalUnit(m_name, m_conversionFactor, m_dimension);
    }


    void FundamemtalUnit::GetUnitComponents( ListOfFundamentalUnitsWithExponents& listOfFundamentalUnits, UnitNameModifier modifier /*= UnitNameModifier( ) */ ) const
    {
        listOfFundamentalUnits.AddFundamentalUnit(UnitPointer(new FundamemtalUnit(m_name, m_conversionFactor, m_dimension)), modifier.GetExponentIncludingInversion());
    }

}}}}}
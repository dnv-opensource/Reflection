#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IUnit.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime { namespace Detail {
    class MultiplyUnits : public IUnit
    {
    public:        
        MultiplyUnits(UnitPointer lhs, UnitPointer rhs);
        virtual double GetConversionFactor() const;
        virtual std::string GetUnitName(UnitNameModifier modifier) const;
        virtual DynamicDimension GetDimension() const;
        virtual std::list<UnitPointer> GetChildren() const;
        virtual MultiplyUnits* CreateCopy(const std::list<UnitPointer>& children) const;
        virtual void GetUnitComponents( ListOfFundamentalUnitsWithExponents& listOfFundamentalUnits, UnitNameModifier modifier = UnitNameModifier( ) ) const;
    private:
        UnitPointer m_lhs;
        UnitPointer m_rhs;
    };

}}}}}

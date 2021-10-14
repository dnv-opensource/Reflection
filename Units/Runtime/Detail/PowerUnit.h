#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IUnit.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime { namespace Detail {
    class PowerUnit : public IUnit
    {
    public:        
        PowerUnit(UnitPointer lhs, int exponent);
        virtual double GetConversionFactor() const;
        virtual std::string GetUnitName(UnitNameModifier modifier) const;
        virtual DynamicDimension GetDimension() const;
        virtual std::list<UnitPointer> GetChildren() const;
        virtual PowerUnit* CreateCopy(const std::list<UnitPointer>& children) const;
        static UnitPointer CreatePowerUnit(UnitPointer lhs, int exponent);
        virtual void GetUnitComponents( ListOfFundamentalUnitsWithExponents& listOfFundamentalUnits, UnitNameModifier modifier = UnitNameModifier( ) ) const;
    private:
        UnitPointer m_lhs;
        int m_exponent;
    };
}}}}}

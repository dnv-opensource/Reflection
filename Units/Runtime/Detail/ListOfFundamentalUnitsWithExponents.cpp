//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ListOfFundamentalUnitsWithExponents.h"
#include "PowerUnit.h"
#include "DivideUnits.h"
#include "MultiplyUnits.h"
#include "FundamentalUnit.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime { namespace Detail {

    void ListOfFundamentalUnitsWithExponents::AddFundamentalUnit( UnitPointer unit, int exponent )
    {
        for(auto it = m_listOfUnitsWithExponent.begin(); it != m_listOfUnitsWithExponent.end(); ++it)
        {
            if(it->first->GetUnitName() == unit->GetUnitName()) 
            {
                it->second += exponent;
                return;
            }
        }
        m_listOfUnitsWithExponent.push_back(UnitPointerExponentPair(unit, exponent));
    }

    UnitPointer ListOfFundamentalUnitsWithExponents::BuildCompositeUnit() const
    {
        UnitPointer unit;
        for(auto it = m_listOfUnitsWithExponent.begin(); it != m_listOfUnitsWithExponent.end(); ++it)
        {
            int exponent = it->second;
            if(exponent == 0) 
                continue;
            else if(!unit) 
            {
                unit = PowerUnit::CreatePowerUnit(it->first, exponent);
            }
            else if(exponent<1) 
            {
                UnitPointer rhs = PowerUnit::CreatePowerUnit(it->first, -exponent);
                unit = std::make_shared<DivideUnits>(unit, rhs);
            }
            else 
            {
                UnitPointer rhs = PowerUnit::CreatePowerUnit(it->first, exponent);
                unit = std::make_shared<MultiplyUnits>(unit, rhs);
            }
        }
        if(!unit) 
            unit = UnitPointer(new FundamemtalUnit("", 1.0, DynamicDimension(0, 0, 0, 0, 0)));
        return unit;
    }

    void ListOfFundamentalUnitsWithExponents::TryMoveUnitWithPositiveExponentFirst()
    {
        if (m_listOfUnitsWithExponent.empty())
            return;
        if (m_listOfUnitsWithExponent.front().second > 0)
            return;
        for (auto it = m_listOfUnitsWithExponent.begin(); it != m_listOfUnitsWithExponent.end(); ++it)
        {
            if (it->second > 0)
                std::swap(*it, m_listOfUnitsWithExponent.front());
        }
    }

}}}}}


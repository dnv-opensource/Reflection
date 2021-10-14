//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "UnitManager.h"
#include "Units/Length.h"
#include "Units/ForcePerUnitArea.h"
#include "Units/Energy.h"
#include "Units/Volume.h"
#include "Units/Mass.h"
#include "Units/TempDiff.h"
#include "Units/Force.h"
#include "Units/Time.h"
#include "Units/Frequency.h"
#include "Units/Acceleration.h"
#include "Units/Angle.h"
#include "Units/Velocity.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {


    UnitManager::UnitManager()
    {

    }

    UnitManager::UnitManager( UnitManager&& other )
        :   m_units(std::move(other.m_units))
    {
    }

    void UnitManager::AddSIUnit( const Unit& unit )
    {
        AddUnit(unit);
        AddUnitWithPrefix(unit,"Y",1e+24);
        AddUnitWithPrefix(unit,"Z",1e+21);
        AddUnitWithPrefix(unit,"E",1e+18);
        AddUnitWithPrefix(unit,"P",1e+15);
        AddUnitWithPrefix(unit,"T",1e+12);
        AddUnitWithPrefix(unit,"G",1e+9);
        AddUnitWithPrefix(unit,"M",1e+6);
        AddUnitWithPrefix(unit,"k",1e+3);
        AddUnitWithPrefix(unit,"K",1e+3);
        AddUnitWithPrefix(unit,"h",1e+2);
        AddUnitWithPrefix(unit,"H",1e+2);
        AddUnitWithPrefix(unit,"D",1e+1);
        AddUnitWithPrefix(unit,"d",1e-1);
        AddUnitWithPrefix(unit,"c",1e-2);
        AddUnitWithPrefix(unit,"m",1e-3);
        AddUnitWithPrefix(unit,"u",1e-6);   // really greek letter µ
        AddUnitWithPrefix(unit,"n",1e-9);
        AddUnitWithPrefix(unit,"p",1e-12);
        AddUnitWithPrefix(unit,"f",1e-15);
        AddUnitWithPrefix(unit,"a",1e-18);
        AddUnitWithPrefix(unit,"z",1e-21);
        AddUnitWithPrefix(unit,"y",1e-24);
    }

    void UnitManager::AddUnit( const Unit& unit )
    {
        m_units[unit.GetUnitName()]=unit;
    }

    UnitManager::Units::const_iterator UnitManager::lower_bound( const std::string& unitName) const
    {
        return m_units.lower_bound(unitName);
    }

    UnitManager::Units::const_iterator UnitManager::upper_bound( const std::string& unitName) const
    {
        return m_units.upper_bound(unitName);
    }

    UnitManager::Units::const_iterator UnitManager::begin() const
    {
        return m_units.begin();
    }

    UnitManager::Units::const_iterator UnitManager::end() const
    {
        return m_units.end();
    }

    UnitManager::Units::const_iterator UnitManager::find( const std::string& unitName) const
    {
        return m_units.find(unitName);
    }

    void UnitManager::AddUnitWithPrefix( const Unit& unit, const std::string& prefix, double prefixFactor )
    {
        AddUnit(Unit(prefix+unit.GetUnitName(),prefixFactor*unit.GetConversionFactor(),unit.GetDimension()));
    }

    void UnitManager::AddAllSIUnits()
    {
        AddSIUnit(_m);
        AddSIUnit(_Pa);
        AddSIUnit(_J);
        AddSIUnit(_l);
        AddSIUnit(_g);
        AddSIUnit(_delK);
        AddSIUnit(_N);
        AddSIUnit(_s);
        AddSIUnit(_min);
        AddSIUnit(_hr);
        AddSIUnit(_Hz);
    }

    void UnitManager::AddImperialUnits()
    {
        AddUnit(_G);
        AddUnit(_Hz);
        AddUnit(_radps);
        AddUnit(_Knot);
        AddUnit(_psi);
        AddUnit(_ksi);
        AddUnit(_bar);
        AddUnit(_atm);
        AddUnit(_Btu);
        AddUnit(_lb);
        AddUnit(_tonne);
        AddUnit(_kilotonne);
        AddUnit(_ton);
        AddUnit(_kips);
        AddUnit(_slug);
        AddUnit(_delC);
        AddUnit(_delF);
        AddUnit(_rad);
        AddUnit(_deg);
        AddUnit(Unit("°",_deg.GetValue(), AngleDimension()));
        AddUnit(_rev);
        AddUnit(_lbf);
        AddUnit(_kipf);
        AddUnit(_kgf);
        AddUnit(_in);
        AddUnit(_ft);
    }

    void UnitManager::AddAllUnits()
    {
        AddAllSIUnits();
        AddImperialUnits();
    }

}}}}
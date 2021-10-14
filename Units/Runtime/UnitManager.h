#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Unit.h"
#include <map>
#include "..\Config.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {
    ///Keep track of all valid units
    class UNITS_IMPORT_EXPORT UnitManager
    {    
    public:
        UnitManager();
        UnitManager(UnitManager&& other);
        void AddAllUnits();
        ///Adds imperial and other non-SI units.
        void AddImperialUnits();
        ///Adds all SI units and their prefixed variants.
        void AddAllSIUnits();
        ///Adds an SI unit and all its prefixed variants.
        void AddSIUnit(const Unit& unit);
        void AddUnit(const Unit& unit);
        void AddUnitWithPrefix(const Unit& unit, const std::string& prefix, double prefixFactor );
        typedef std::map<std::string, Unit> Units;

        Units::const_iterator lower_bound(const std::string& unitName) const;
        Units::const_iterator upper_bound(const std::string& unitName) const;
        Units::const_iterator begin() const;
        Units::const_iterator end() const;
        Units::const_iterator find(const std::string& unitName) const;
    private:
        UnitManager(const UnitManager& other);
        Units m_units;
    };
}}}}
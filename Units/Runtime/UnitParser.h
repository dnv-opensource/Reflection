#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "../Config.h"
#include "UnitManager.h"
#include "Detail/UnitStringIterator.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {
    class UNITS_IMPORT_EXPORT UnitParser
    {    
    public:
        UnitParser(bool throwOnError = false);
        UnitParser(UnitManager&& unitManager, bool throwOnError = false);
        ///Parses a unit string and returns a valid unit expression.
        Unit GetUnitFromConversionFactor(const DynamicDimension& dimension, double unitConversionFactor, double tolerance = 1e-6) const;
        Unit Parse(const std::string& unitName) const;
        std::pair<bool,int> ParseExponent( Detail::UnitStringIterator& iUnitName ) const;
        Unit ParseUnitString( Detail::UnitStringIterator& iUnitName ) const;
        Unit ParseUnitWithExponent( Detail::UnitStringIterator& iUnitName ) const;
        static const UnitParser& GetUnitParser(bool throwOnError = false);
    private:
        UnitManager m_unitManager;
        bool m_throwOnError;
    };
}}}}
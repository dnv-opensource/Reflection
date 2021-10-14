#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "..\DynamicDimension.h"
namespace DNVS {namespace MoFa {namespace Units { namespace Runtime { namespace Detail {
    class UnitMatcher {
    public:
        UnitMatcher(const DynamicDimension& dimension, double unitConversionFactor, double tolerance = 1e-6)
            :   m_dimension(dimension)
            ,   m_unitConversionFactor(unitConversionFactor)
            ,   m_tolerance(tolerance)
        {}
        void MatchUnit(const Unit& unit)
        {
            if(unit.GetDimension() != m_dimension)
                return;
            if(GetConversionFactorDifference(unit) <= m_tolerance)
            {
                if(!m_closestUnit.IsValid() || GetConversionFactorDifference(unit) < GetConversionFactorDifference(m_closestUnit) || FormatScore(unit) < FormatScore(m_closestUnit))
                    m_closestUnit = unit;
            }
        }
        const Unit& GetBestUnit() const
        {
            return m_closestUnit;
        }
    private:
        double GetConversionFactorDifference(const Unit& unit)
        {
            return fabs(unit.GetConversionFactor() - m_unitConversionFactor);
        }
        // Aims to give low (good) score to units which are correctly formatted according to standard and infinite score to others
        double FormatScore(const Unit& unit)
        {
            if (unit.GetUnitName().front() == 'K')
                return std::numeric_limits<double>::max();
            else
                return 1.0;
        }
        DynamicDimension m_dimension;
        double m_unitConversionFactor;
        double m_tolerance;
        Unit m_closestUnit;


    };
}}}}}
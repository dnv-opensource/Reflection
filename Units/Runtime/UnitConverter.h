#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "UnitSystem.h"
namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {
    class UnitConverter {
    public:
        UnitConverter(const UnitSystem& from, const UnitSystem& to)
            :   m_from(from)
            ,   m_to(to)
        {
        }
        template<typename QuantityT>
        double Convert(double value, std::enable_if_t<IsQuantityWithPhenomenon<QuantityT>::value,void*> = nullptr)
        {
            typedef typename QuantityT::PhenomenonType PhenomenonT;
            Unit fromUnit = m_from.GetUnit(PhenomenonT());
            Unit toUnit = m_to.GetUnit(PhenomenonT());
            return value*fromUnit.GetConversionFactor()/toUnit.GetConversionFactor();
        }
        template<typename QuantityT>
        double Convert(double value, std::enable_if_t<!IsQuantityWithPhenomenon<QuantityT>::value, void*> = nullptr)
        {
            typedef typename QuantityT::DimensionType DimensionT;
            Unit fromUnit = m_from.GetUnit(DimensionT());
            Unit toUnit = m_to.GetUnit(DimensionT());
            return value*fromUnit.GetConversionFactor() / toUnit.GetConversionFactor();
        }
        void SetFromSystem(const UnitSystem& from)
        {
            m_from = from;
        }
        void SetToSystem(const UnitSystem& to)
        {
            m_to = to;
        }
        UnitConverter CreateReversedConverter()
        {
            return UnitConverter(m_to, m_from);
        }
    private:
        UnitSystem m_from;
        UnitSystem m_to;
    };
}}}}
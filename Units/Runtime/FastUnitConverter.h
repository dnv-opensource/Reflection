#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "UnitSystem.h"
#include "..\Config.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {
    class UNITS_IMPORT_EXPORT FastUnitConverter {
    public:
        FastUnitConverter(const UnitSystem& from, const UnitSystem& to);
        double GetConversionFactor(const DynamicDimension& dimension) const;

        template<typename QuantityT, typename T>
        T Convert(T value) const
        {
            typedef typename QuantityT::DimensionType DimensionT;
            if (!m_isConversionNecessary)
                return value;
            else
                return value * GetConversionFactor(DimensionT());
        }

        template<typename QuantityT, typename T>
        void ConvertInline(T& value) const
        {
            typedef typename QuantityT::DimensionType DimensionT;
            if(m_isConversionNecessary)
                value*=GetConversionFactor(DimensionT());
        }
        FastUnitConverter CreateReversedConverter() const;
        const UnitSystem& GetFrom() const;
        const UnitSystem& GetTo() const;
    private:
        double m_lengthConversionFactor;
        double m_forceConversionFactor;
        double m_tempDiffConversionFactor;
        double m_timeConversionFactor;
        double m_angleConversionFactor;
        bool m_isConversionNecessary;
        UnitSystem m_from;
        UnitSystem m_to;
    };
}}}}
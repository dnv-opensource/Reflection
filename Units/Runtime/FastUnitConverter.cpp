//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "FastUnitConverter.h"
#include "UnitConverter.h"
#include "..\Length.h"
#include "..\Force.h"
#include "..\TempDiff.h"
#include "..\Time.h"
#include "..\Angle.h"
#include "Math\CompareAlmostEqual.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {

    FastUnitConverter::FastUnitConverter(const UnitSystem& from, const UnitSystem& to) 
        : m_from(from)
        , m_to(to)
        , m_isConversionNecessary(true)
    {
        UnitConverter converter(from, to);
        m_lengthConversionFactor = converter.Convert<Length>(1.0);
        m_forceConversionFactor = converter.Convert<Force>(1.0);
        m_tempDiffConversionFactor = converter.Convert<TempDiff>(1.0);
        m_timeConversionFactor = converter.Convert<Time>(1.0);
        m_angleConversionFactor = converter.Convert<Angle>(1.0);
        if (
            Math::CompareAlmostEqual(m_lengthConversionFactor, 1.0) &&
            Math::CompareAlmostEqual(m_forceConversionFactor, 1.0) &&
            Math::CompareAlmostEqual(m_tempDiffConversionFactor, 1.0) &&
            Math::CompareAlmostEqual(m_timeConversionFactor, 1.0) &&
            Math::CompareAlmostEqual(m_angleConversionFactor, 1.0)
            )
            m_isConversionNecessary = false;
    }

    double FastUnitConverter::GetConversionFactor(const DynamicDimension& dimension) const
    {
        double lengthConversionFactor = pow(m_lengthConversionFactor, dimension.GetLength());
        double forceConversionFactor = pow(m_forceConversionFactor, dimension.GetForce());
        double timeConversionFactor = pow(m_timeConversionFactor, dimension.GetTime());
        double temperatureConversionFactor = pow(m_tempDiffConversionFactor, dimension.GetTemperatureDifference());
        double angleConversionFactor = pow(m_angleConversionFactor, dimension.GetAngle());
        return lengthConversionFactor*forceConversionFactor*timeConversionFactor*temperatureConversionFactor*angleConversionFactor;
    }

    FastUnitConverter FastUnitConverter::CreateReversedConverter() const
    {
        return FastUnitConverter(m_to, m_from);
    }

    const UnitSystem& FastUnitConverter::GetFrom() const
    {
        return m_from;
    }

    const UnitSystem& FastUnitConverter::GetTo() const
    {
        return m_to;
    }

}}}}


#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "UnitSystem.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {
    template<typename AllocatorT>
    class UnitSystemGenerator
    {
    public:
        UnitSystemGenerator(const UnitSystem& unitSystem)
        {
            m_lengthUnit = unitSystem.GetLengthUnitString().c_str();
            m_forceUnit = unitSystem.GetForceUnitString().c_str();
            m_timeUnit = unitSystem.GetTimeUnitString().c_str();
            m_tempDiffUnit = unitSystem.GetTempDiffUnitString().c_str();
            m_angleUnit = unitSystem.GetAngleUnitString().c_str();
        }
        UnitSystem GetUnitSystem() const {
            return UnitSystem(m_lengthUnit.c_str(), m_forceUnit.c_str(), m_tempDiffUnit.c_str(), m_timeUnit.c_str(), m_angleUnit.c_str());
        }
    private:
        typedef std::basic_string<char, std::char_traits<char>, AllocatorT> UnitString;
        UnitString m_lengthUnit;
        UnitString m_forceUnit;
        UnitString m_timeUnit;
        UnitString m_tempDiffUnit;
        UnitString m_angleUnit;
    };
}}}}
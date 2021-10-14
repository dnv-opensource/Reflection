//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "UnitHasInvalidDimension.h"
#include "sstream"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {

    UnitHasInvalidDimension::UnitHasInvalidDimension(const Unit& unit, const DynamicDimension& dimension)
    {
        std::stringstream stream;
        stream << "Unit '" << unit.GetUnitName() << "' has dimensions: " << unit.GetDimension() << " expected " << dimension;
        m_errorMessage = stream.str();
    }

    UnitHasInvalidDimension::UnitHasInvalidDimension(const Unit& unit, const DynamicPhenomenon& phenomenon)
    {
        std::stringstream stream;
        stream << "Unit '" << unit.GetUnitName() << "' has dimensions: " << unit.GetDimension() << " expected " << phenomenon.GetName() << " with dimension " << phenomenon.GetDimension();
        m_errorMessage = stream.str();
    }

    char const* UnitHasInvalidDimension::what() const
    {
        return m_errorMessage.c_str();
    }

}}}}



#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <stdexcept>
#include "..\Config.h"
#include "Unit.h"
#include "DynamicDimension.h"
#include "DynamicPhenomenon.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {
#pragma warning(push)
#pragma warning(disable:4275)
    class UNITS_IMPORT_EXPORT UnitHasInvalidDimension : public std::exception
    {
    public:
        UnitHasInvalidDimension(const Unit& unit, const DynamicDimension& dimension);
        UnitHasInvalidDimension(const Unit& unit, const DynamicPhenomenon& phenomenon);
        virtual char const* what() const;
    private:
        std::string m_errorMessage;
    };
#pragma warning(pop)
}}}}

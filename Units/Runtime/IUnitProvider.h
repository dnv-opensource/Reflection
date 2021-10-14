#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DynamicPhenomenon.h"
#include "DynamicDimension.h"
#include "Unit.h"


namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {
    /**
    This interface provides information about the units of dimensions or phenomenons.
    It is the base class for the database unit provider and the input unit provider.
    */
    class IUnitProvider
    {
    public:
        virtual Unit GetDimensionUnit(const DynamicDimension& dimension) = 0;
        virtual Unit GetPhenomenonUnit(const DynamicPhenomenon& phenomenon) = 0;
        virtual void SetDimensionUnit(const DynamicDimension& dimension, const Unit& unit) = 0;
        virtual void SetPhenomenonUnit(const DynamicPhenomenon& phenomenon, const Unit& unit) = 0;
        virtual std::map<DynamicDimension, Unit> GetDimensionUnits() const = 0;
        virtual std::map<DynamicPhenomenon, Unit> GetPhenomenonUnits() const = 0;
        virtual void Clear() = 0;
        virtual std::shared_ptr<IUnitProvider> GetCache() const = 0;
        virtual void ClearCache() = 0;
    };
}}}}
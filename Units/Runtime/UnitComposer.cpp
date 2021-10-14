//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "UnitComposer.h"
#include "..\Force.h"
#include "..\Length.h"
#include "..\Time.h"
#include "..\TempDiff.h"
#include "..\Angle.h"
#include "..\ForAllQuantities.h"
#include "..\ForAllDimensions.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {

    UnitComposer::UnitComposer(const std::shared_ptr<IUnitProvider>& provider, double tolerance) 
        : m_provider(provider)
        , m_tolerance(tolerance)
    {
    }

    Unit UnitComposer::GetUnit(const DynamicPhenomenon& dynamicPhenomenon) const
    {
        Unit unit = m_provider->GetPhenomenonUnit(dynamicPhenomenon);
        if (!unit.IsValid())
        {
            ForAllQuantitiesLambda([&](auto quantity) 
            {
                typedef decltype(quantity)::PhenomenonType PhenomenonType;
                if (PhenomenonType::GetName() == dynamicPhenomenon.GetName())
                    unit = this->GetUnit(PhenomenonType());
            });
        }
        return unit;
    }

    Unit UnitComposer::GetUnit(const DynamicDimension& dynamicDimension) const
    {
        Unit unit = m_provider->GetDimensionUnit(dynamicDimension);
        if (!unit.IsValid())
        {
            ForAllUsedDimensionsLambda([&](auto dimension)
            {
                if (DynamicDimension(dimension) == dynamicDimension)
                    unit = this->GetUnit(dimension);
            });
            if (!unit.IsValid())
            {
                unit = ComposeDefaultUnit(dynamicDimension);
                if (unit.IsValid() && m_provider->GetCache())
                    m_provider->GetCache()->SetDimensionUnit(dynamicDimension, unit);
            }
        }
        return unit;
    }

    Unit UnitComposer::GetDefaultUnit(const DynamicPhenomenon& phenomenon, const Unit& fallbackUnit) const
    {
        Unit unit = m_provider->GetDimensionUnit(phenomenon.GetDimension());
        if (!unit.IsValid())
            unit = m_provider->GetPhenomenonUnit(phenomenon);
        if(!unit.IsValid())
            unit = fallbackUnit;
        return unit;
    }

    Unit UnitComposer::ComposeDefaultUnit(const DynamicDimension& dim) const
    {
        Unit unit;
        AppendUnit(unit, dim.GetLength(), GetDefaultUnit(LengthPhenomenon(), _m));
        AppendUnit(unit, dim.GetTime(), GetDefaultUnit(TimePhenomenon(), _s));
        AppendUnit(unit, dim.GetForce(), GetDefaultUnit(ForcePhenomenon(), _N));
        AppendUnit(unit, dim.GetTemperatureDifference(), GetDefaultUnit(TempDiffPhenomenon(), _delC));
        AppendUnit(unit, dim.GetAngle(), GetDefaultUnit(AnglePhenomenon(), _rad));
        unit.SimplifyUnit();
        if (unit.IsValid())
            return unit;
        else
            return Unit::GetEmptyUnit();
    }

    bool UnitComposer::CompareUnits(const Unit& lhs, const Unit& rhs) const
    {
        return fabs(lhs.GetConversionFactor() - rhs.GetConversionFactor()) < m_tolerance;
    }

    void UnitComposer::AppendUnit(Unit& unit, int pow, const Unit& baseUnit) const
    {
        if (pow != 0)
        {
            Unit newUnit = Pow(baseUnit, pow);
            if (!unit.IsValid())
                unit = newUnit;
            else
                unit *= newUnit;
        }
    }

}}}}


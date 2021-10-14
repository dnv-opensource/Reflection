//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "UnitSystem.h"
#include "DefaultUnitProvider.h"
#include "..\Length.h"
#include "..\Time.h"
#include "..\Force.h"
#include "..\TempDiff.h"
#include "..\Angle.h"
#include "DynamicQuantity.h"
#include "UnitParser.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {

    UnitSystem::UnitSystem(std::shared_ptr<IUnitProvider> provider)
        : m_provider(provider)
        , m_tolerance(1e-6)
    {
    }

    UnitSystem::UnitSystem( const Unit& length, const Unit& force, const Unit& tempDiff, const Unit& time, const Unit& angle )
        : UnitSystem(std::make_shared<DefaultUnitProvider>(length, force, tempDiff, time, angle))
    {
    }

    UnitSystem::UnitSystem( const Unit& length, const Unit& force, const Unit& tempDiff, const Unit& time)
        : UnitSystem(length, force, tempDiff, time, _rad)
    {
    }

    UnitSystem::UnitSystem( const Unit& length, const Unit& force, const Unit& tempDiff)
        : UnitSystem(length, force, tempDiff, _s) 
    {

    }

    UnitSystem::UnitSystem(const Unit& length, const Unit& force)
        : UnitSystem(length, force, _delC)
    {

    }

    UnitSystem::UnitSystem()
        : UnitSystem(_m, _N)
    {
    }

    UnitSystem::UnitSystem(const std::string& lengthUnit, const std::string& forceUnit, const std::string& tempDiffUnit, const std::string& timeUnit, const std::string& angleUnit)
        : UnitSystem(nullptr)
    {
        UnitParser parser;
        Unit length = parser.Parse(lengthUnit);
        Unit force = parser.Parse(forceUnit);
        Unit tempDiff = parser.Parse(tempDiffUnit);
        Unit time = parser.Parse(timeUnit);
        Unit angle = parser.Parse(angleUnit);
        m_provider = std::make_shared<DefaultUnitProvider>(length, force, tempDiff, time, angle);
    }

    UnitSystem::UnitSystem(const std::string& lengthUnit, const std::string& forceUnit, const std::string& tempDiffUnit)
        : UnitSystem(lengthUnit, forceUnit, tempDiffUnit, "s","rad")
    {
    }

    UnitSystem::UnitSystem(const std::string& lengthUnit, const std::string& forceUnit)
        : UnitSystem(lengthUnit, forceUnit, "delC")
    {
    }

    bool UnitSystem::operator==(const UnitSystem& other) const
    {
        if (GetLengthUnitString() != other.GetLengthUnitString())
            return false;
        if (GetForceUnitString() != other.GetForceUnitString())
            return false;
        if (GetTempDiffUnitString() != other.GetTempDiffUnitString())
            return false;
        if (GetTimeUnitString() != other.GetTimeUnitString())
            return false;
        if (GetAngleUnitString() != other.GetAngleUnitString())
            return false;
        return true;
    }

    bool UnitSystem::operator!=(const UnitSystem& other) const
    {
        return !operator==(other);
    }

    double UnitSystem::Convert(const DynamicQuantity& q) const
    {
        return q.GetNeutralValue() / GetUnit(q.GetUnit().GetDimension()).GetConversionFactor();
    }

    double UnitSystem::Convert(const DynamicPhenomenon& phenomenon, const DynamicQuantity& q) const
    {
        if (!q.IsCompatible(phenomenon))
            throw std::runtime_error("Invalid unit: '" + q.GetUnit().GetUnitName() + "' is not a valid " + phenomenon.GetName() + " unit");
        return q.GetNeutralValue() / GetUnit(phenomenon).GetConversionFactor();
    }

    void UnitSystem::Reset(const UnitSystem& other)
    {
        m_provider->Clear();
        for (const auto& dimensionPair : other.m_provider->GetDimensionUnits())
        {
            m_provider->SetDimensionUnit(dimensionPair.first, dimensionPair.second);
        }
        for (const auto& phenomenonPair : other.m_provider->GetPhenomenonUnits())
        {
            m_provider->SetPhenomenonUnit(phenomenonPair.first, phenomenonPair.second);
        }
    }

    std::string UnitSystem::GetLengthUnitString() const
    {
        return GetUnit<Length>().GetUnitName();
    }

    std::string UnitSystem::GetForceUnitString() const
    {
        return GetUnit<Force>().GetUnitName();
    }

    std::string UnitSystem::GetTempDiffUnitString() const
    {
        return GetUnit<TempDiff>().GetUnitName();
    }

    std::string UnitSystem::GetTimeUnitString() const
    {
        return GetUnit<Time>().GetUnitName();
    }

    std::string UnitSystem::GetAngleUnitString() const
    {
        return GetUnit<Angle>().GetUnitName();
    }

    Unit UnitSystem::ParseUnit(const std::string& unitString) const
    {
        return UnitParser(true).Parse(unitString);
    }

}}}}
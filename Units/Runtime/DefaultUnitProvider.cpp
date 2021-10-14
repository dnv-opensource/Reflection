//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DefaultUnitProvider.h"
#include "..\Length.h"
#include "..\Angle.h"
#include "..\TempDiff.h"
#include "..\Force.h"
#include "..\Time.h"
#include "UnitHasInvalidDimension.h"
namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {

    DefaultUnitProvider::DefaultUnitProvider(const Unit& length, const Unit& force, const Unit& tempDiff, const Unit& time, const Unit& angle)
        : DefaultUnitProvider(length, force, tempDiff, time)
    {
        SetPhenomenonUnit(AnglePhenomenon(), angle);
    }

    DefaultUnitProvider::DefaultUnitProvider(const Unit& length, const Unit& force, const Unit& tempDiff, const Unit& time)
        : DefaultUnitProvider(length, force, tempDiff)
    {
        SetPhenomenonUnit(TimePhenomenon(), time);
    }

    DefaultUnitProvider::DefaultUnitProvider(const Unit& length, const Unit& force, const Unit& tempDiff)
        : DefaultUnitProvider(length, force)
    {
        SetPhenomenonUnit(TempDiffPhenomenon(), tempDiff);
    }

    DefaultUnitProvider::DefaultUnitProvider(const Unit& length, const Unit& force)
        : DefaultUnitProvider()
    {
        SetPhenomenonUnit(LengthPhenomenon(), length);
        SetPhenomenonUnit(ForcePhenomenon(), force);
    }

    DefaultUnitProvider::DefaultUnitProvider()
    {
        SetDimensionUnit(AngleDimension(), _rad);
    }

    Unit DefaultUnitProvider::GetDimensionUnit(const DynamicDimension& dimension)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_dimensions.find(dimension);
        if(it != m_dimensions.end())
            return it->second;
        else if (m_cache)
            return m_cache->GetDimensionUnit(dimension);
        else
            return Unit();
    }

    Unit DefaultUnitProvider::GetPhenomenonUnit( const DynamicPhenomenon& phenomenon )
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_phenomenons.find(phenomenon);
        if (it != m_phenomenons.end())
            return it->second;
        else if (m_cache)
            return m_cache->GetPhenomenonUnit(phenomenon);
        else
            return Unit();
    }
    void DefaultUnitProvider::SetDimensionUnit( const DynamicDimension& dimension, const Unit& unit )
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (unit.GetDimension() != dimension)
            throw UnitHasInvalidDimension(unit, dimension);
        m_dimensions[dimension] = unit;
        ClearCache();
    }

    void DefaultUnitProvider::SetPhenomenonUnit( const DynamicPhenomenon& phenomenon, const Unit& unit )
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (unit.GetDimension() != phenomenon.GetDimension())
            throw UnitHasInvalidDimension(unit, phenomenon);
        m_phenomenons[phenomenon] = unit;
        ClearCache();
    }

    void DefaultUnitProvider::Clear()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_phenomenons.clear();
    }

    std::map<DynamicDimension, Unit> DefaultUnitProvider::GetDimensionUnits() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_dimensions;
    }

    std::map<DynamicPhenomenon, Unit> DefaultUnitProvider::GetPhenomenonUnits() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_phenomenons;
    }

    std::shared_ptr<DNVS::MoFa::Units::Runtime::IUnitProvider> DefaultUnitProvider::GetCache() const
    {
        if (!m_cache)
            m_cache = std::make_shared<DefaultUnitProvider>();
        return m_cache;
    }

    void DefaultUnitProvider::ClearCache()
    {
        m_cache.reset();
    }

}}}}
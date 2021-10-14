//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "SimplifiedUnit.h"
#include "UnitParser.h"
#include "UnitSystem.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {

    SimplifiedUnit::SimplifiedUnit(Unit other)
        : m_phenomenon(other.GetDimension())
    {
        operator=(other);
    }

    SimplifiedUnit::SimplifiedUnit(const std::string& unitName, double conversionFactor, const DynamicDimension& dimension)
        : m_conversionFactor(conversionFactor)
        , m_phenomenon(dimension)
    {
        if (unitName.size() > 31)
            TrySimplifyUnit(unitName);
        else
            m_string = unitName;
    }

    SimplifiedUnit::SimplifiedUnit(double conversionFactor, const DynamicDimension& dimension)
        : m_conversionFactor(conversionFactor)
        , m_phenomenon(dimension)
    {}

    SimplifiedUnit::SimplifiedUnit(double conversionFactor, const DynamicPhenomenon& phenomenon)
        : m_conversionFactor(conversionFactor)
        , m_phenomenon(phenomenon)
    {}

    bool SimplifiedUnit::TrySimplifyUnit()
    {
        if (!m_string.empty())
            return TrySimplifyUnit(m_string);
        return false;
    }

    bool SimplifiedUnit::TrySimplifyUnit(const std::string& unitName)
    {
        Unit unit = UnitParser::GetUnitParser().Parse(unitName);
        if (unit.IsValid())
        {
            unit.SimplifyUnit();
            std::string newUnitName = unit.GetUnitName();
            if (newUnitName.size() <= 31)
            {
                m_string = newUnitName;
                return true;
            }
        }
        return false;
    }

    bool SimplifiedUnit::IsDimensionless() const
    {
        return m_phenomenon.GetDimension() == NoDimension();
    }

    double SimplifiedUnit::GetConversionFactor() const
    {
        return m_conversionFactor;
    }

    std::string SimplifiedUnit::GetUnitName() const
    {
        if (m_string.empty() && m_conversionFactor == 1.0 && !IsDimensionless())
        {
            if (m_phenomenon.IsValid())
                return UnitSystem().GetUnit(m_phenomenon).GetUnitName();
            else
                return UnitSystem().GetUnit(m_phenomenon.GetDimension()).GetUnitName();
        }
        return m_string;
    }

    bool SimplifiedUnit::HasUnitName() const
    {
        return !m_string.empty();
    }

    const DynamicDimension& SimplifiedUnit::GetDimension() const
    {
        return m_phenomenon.GetDimension();
    }

    const DynamicPhenomenon& SimplifiedUnit::GetPhenomenon() const
    {
        return m_phenomenon;
    }

    Unit SimplifiedUnit::GetUnit() const
    {
        if (m_string.empty())
            return UnitSystem().GetUnit(GetDimension());
        else 
            return UnitParser::GetUnitParser().Parse(m_string);
    }

    SimplifiedUnit& SimplifiedUnit::operator=(Unit other)
    {
        if (!other.IsValid())
            throw std::runtime_error("While creating SimplifiedUnit: unit is not valid");
        m_phenomenon = other.GetDimension();
        m_conversionFactor = other.GetConversionFactor();
        std::string unitName = other.GetUnitName();
        if (unitName.length() > 31)
        {
            other.SimplifyUnit();
            unitName = other.GetUnitName();
        }
        if (unitName.length() <= 31)
            m_string = unitName;
        return *this;
    }

    SimplifiedUnit SimplifiedUnit::operator*(const SimplifiedUnit& other) const
    {
        double conversionFactor = m_conversionFactor * other.GetConversionFactor();
        DynamicDimension dim = GetDimension() + other.GetDimension();
        if (m_string.empty() || other.m_string.empty())
            return SimplifiedUnit(conversionFactor, dim);
        else
            return SimplifiedUnit(GetUnitName() + "*" + other.GetUnitName(), conversionFactor, dim);
    }

    SimplifiedUnit& SimplifiedUnit::operator*=(const SimplifiedUnit& other)
    {
        return *this = *this * other;        
    }

    SimplifiedUnit SimplifiedUnit::operator/(const SimplifiedUnit& other) const
    {
        double conversionFactor = m_conversionFactor / other.GetConversionFactor();
        DynamicDimension dim = GetDimension() - other.GetDimension();
        if (m_string.empty() || other.m_string.empty())
            return SimplifiedUnit(conversionFactor, dim);
        else
        {
            Unit result = GetUnit() / other.GetUnit();
            result.SimplifyUnit();
            return SimplifiedUnit(result);
        }            
    }

    SimplifiedUnit& SimplifiedUnit::operator/=(const SimplifiedUnit& other)
    {
        return *this = *this / other;
    }

    SimplifiedUnit Pow(const SimplifiedUnit& other, int exponent)
    {
        return SimplifiedUnit(Pow(other.GetUnit(), exponent));
    }

    bool SimplifiedUnit::operator!=(const SimplifiedUnit& other) const
    {
        return !operator==(other);
    }

    bool SimplifiedUnit::operator==(const SimplifiedUnit& other) const
    {
        return GetPhenomenon() == other.GetPhenomenon() && m_conversionFactor == other.m_conversionFactor;
    }

}}}}


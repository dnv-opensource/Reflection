//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DynamicQuantity.h"
#include "Math/CompareAlmostEqual.h"
#include <intrin.h>
#include <sstream>
#include "UnitParser.h"
#include "UnitSystem.h"
#include "../Formatting/DefaultUnitFormatter.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {

    DynamicQuantity::DynamicQuantity( double value, const Unit& unit )
        :   m_value(value)
        ,   m_unit(unit)
    {
    }

    DynamicQuantity::DynamicQuantity(double value, const std::string& unitName)
        :   m_value(value)
        ,   m_unit(UnitParser::GetUnitParser(true).Parse(unitName))
    {}
    
    DynamicQuantity::DynamicQuantity(const DynamicQuantity& other)
        :   m_value(other.m_value)
        ,   m_unit(other.m_unit)
    {}

    DynamicQuantity::DynamicQuantity( double value )
        :   m_value(value)
        ,   m_unit(Unit::GetEmptyUnit())
    {

    }


    DynamicQuantity::DynamicQuantity()
        : DynamicQuantity(0.0)
    {
    }

    DynamicQuantity::DynamicQuantity(double value, const DynamicPhenomenon& phenomenon)
        : DynamicQuantity(value, UnitSystem().GetUnit(phenomenon))
    {        
    }

    DynamicQuantity::DynamicQuantity(double value, const SimplifiedUnit& unit)
        : m_value(value)
        , m_unit(unit)
    {}

    DynamicQuantity& DynamicQuantity::operator=(const DynamicQuantity& other)
    {
        if(this != &other) 
        {
            m_value = other.m_value;
            m_unit = other.m_unit;
        }
        return *this;
    }


    double DynamicQuantity::GetValue() const
    {
        return m_value;
    }

    Unit DynamicQuantity::GetUnit() const
    {
        return m_unit.GetUnit();
    }

    const SimplifiedUnit& DynamicQuantity::GetSimplifiedUnit() const
    {
        return m_unit;
    }

    bool DynamicQuantity::TrySimplifyUnit()
    {
        return m_unit.TrySimplifyUnit();
    }

    DynamicQuantity DynamicQuantity::operator-() const
    {
        return DynamicQuantity(-GetValue(), GetSimplifiedUnit());
    }

    ///NOOP returns a copy of this.
    DynamicQuantity DynamicQuantity::operator+() const
    {
        return DynamicQuantity(*this);
    }

    DynamicQuantity operator*(double lhs, const DynamicQuantity& other)
    {
        return DynamicQuantity(lhs * other.GetValue(), other.GetSimplifiedUnit());
    }

    DynamicQuantity DynamicQuantity::operator*(double other) const
    {
        return DynamicQuantity(GetValue() * other, GetSimplifiedUnit());
    }

    DynamicQuantity DynamicQuantity::operator*(const DynamicQuantity& other) const
    {
        return DynamicQuantity(*this) *= other;
    }

    DynamicQuantity& DynamicQuantity::operator*=( const DynamicQuantity& other )
    {
        m_unit = GetSimplifiedUnit() * other.GetSimplifiedUnit();
        m_value *= other.GetValue();
        RemoveUnitIfDimensionLess();
        return *this;
    }

    DynamicQuantity operator/(double lhs, const DynamicQuantity& other)
    {
        return DynamicQuantity(lhs / other.GetValue(), Pow(other.GetSimplifiedUnit(), -1));
    }

    DynamicQuantity DynamicQuantity::operator/(double other) const
    {
        return DynamicQuantity(GetValue() / other, GetSimplifiedUnit());
    }

    DynamicQuantity DynamicQuantity::operator/(const DynamicQuantity& other) const
    {
        return DynamicQuantity(*this) /= other;
    }

    DynamicQuantity& DynamicQuantity::operator/=( const DynamicQuantity& other )
    
    {
        m_unit = GetSimplifiedUnit() / other.GetSimplifiedUnit();
        m_value /= other.GetValue();
        RemoveUnitIfDimensionLess();
        return *this;
    }

    DynamicQuantity DynamicQuantity::operator+(DynamicQuantity other ) const
    {
        try {
            other.ChangeUnit(GetSimplifiedUnit());
        }
        catch (std::exception e)
        {
            throw std::runtime_error("When trying to compute '" + GetString() + " + " + other.GetString() + "' : " + e.what());
        }
        DynamicQuantity result(*this);
        result.m_value += other.GetValue();
        return result;
    }

    DynamicQuantity& DynamicQuantity::operator+=(DynamicQuantity other)
    {
        other.ChangeUnit(GetSimplifiedUnit());
        m_value += other.GetValue();
        return *this;
    }

    DynamicQuantity DynamicQuantity::operator-(DynamicQuantity other ) const
    {
        try {
            other.ChangeUnit(GetSimplifiedUnit());
        }
        catch (std::exception e)
        {
            throw std::runtime_error("When trying to compute '" + GetString() + " - " + other.GetString() + "' : " + e.what());
        }
        DynamicQuantity result(*this);
        result.m_value -= other.GetValue();
        return result;
    }

    DynamicQuantity& DynamicQuantity::operator-=(DynamicQuantity other)
    {
        other.ChangeUnit(GetSimplifiedUnit());
        m_value -= other.GetValue();
        return *this;
    }

    bool DynamicQuantity::TryChangeUnit( const SimplifiedUnit& newUnit )
    {
        if (GetSimplifiedUnit().GetDimension() != newUnit.GetDimension())
            return false;
        double scaledValue = m_value * m_unit.GetConversionFactor();
        m_unit = newUnit;
        m_value = scaledValue / m_unit.GetConversionFactor();
        return true;
    }

    void DynamicQuantity::ChangeUnit(const SimplifiedUnit& newUnit)
    {
        if(!TryChangeUnit(newUnit))
            throw std::runtime_error("Incompatible units.");
    }

    void DynamicQuantity::ChangeUnit(const std::string& newUnitName)
    {
        ChangeUnit(UnitParser::GetUnitParser(true).Parse(newUnitName));
    }

    bool DynamicQuantity::operator==(const DynamicQuantity& other) const
    {
        if(GetSimplifiedUnit().GetDimension() != other.GetSimplifiedUnit().GetDimension()) 
            return false;
        return Math::CompareAlmostEqual(GetNeutralValue(), other.GetNeutralValue());
    }

    bool DynamicQuantity::operator!=( const DynamicQuantity& other ) const
    {
        return !(*this == other);
    }

    double DynamicQuantity::GetNeutralValue() const
    {
        return GetValue() * GetSimplifiedUnit().GetConversionFactor();
    }

    bool DynamicQuantity::operator<( const DynamicQuantity& other ) const
    {
        if(GetSimplifiedUnit().GetDimension() != other.GetSimplifiedUnit().GetDimension()) 
            throw std::runtime_error("Unable to compare. Invalid dimensions.");
        return GetNeutralValue() < other.GetNeutralValue();
    }

    bool DynamicQuantity::operator>( const DynamicQuantity& other ) const
    {
        return other < *this;
    }

    bool DynamicQuantity::operator<=( const DynamicQuantity& other ) const
    {
        return !(other < *this);
    }

    bool DynamicQuantity::operator>=( const DynamicQuantity& other ) const
    {
        return !(*this < other);
    }

    std::string DynamicQuantity::GetString() const
    {
        if(_isnan(m_value)) 
            return "NaN";
        if(!_finite(m_value)) 
        {
            if(m_value > 0) 
                return "+Infinity";
            else 
                return "-Infinity";
        }
        std::stringstream stream;
        stream.precision(10);
        std::string unitName = m_unit.GetUnitName();
        if (unitName.empty())
            stream << m_value * m_unit.GetConversionFactor();
        else
            stream << m_value << " " << unitName;
        return stream.str();
    }

    bool DynamicQuantity::RemoveUnitIfDimensionLess()
    {
        if (m_unit.IsDimensionless())
        {
            m_value *= m_unit.GetConversionFactor();
            m_unit = SimplifiedUnit(1.0, NoDimension());
            return true;
        }
        return false;
    }

    bool DynamicQuantity::RemoveConversionFactorIfNoUnitName()
    {
        if (!m_unit.HasUnitName())
        {
            m_value *= m_unit.GetConversionFactor();
            m_unit = SimplifiedUnit(1.0, m_unit.GetDimension());
            return true;
        }
        return false;
    }

    bool DynamicQuantity::IsCompatible(const DynamicQuantity& other) const
    {
        return GetSimplifiedUnit().GetDimension() == other.GetSimplifiedUnit().GetDimension();
    }

    bool DynamicQuantity::IsCompatible(const DynamicPhenomenon& phenomenon) const
    {
        return GetSimplifiedUnit().GetDimension() == phenomenon.GetDimension();
    }

    bool DynamicQuantity::IsCompatible(const DynamicDimension& dimension) const
    {
        return GetSimplifiedUnit().GetDimension() == dimension;
    }

    DynamicQuantity Pow(const DynamicQuantity& quantity, int exponent)
    {
        return DynamicQuantity(pow(quantity.GetValue(), exponent), Pow(quantity.GetSimplifiedUnit(), exponent));
    }

    std::string UNITS_IMPORT_EXPORT ToString(const DynamicQuantity& quantity, const DNVS::MoFa::Formatting::FormattingService& service)
    {
        if (quantity.GetSimplifiedUnit().HasUnitName())
            return quantity.GetString();
        else if (quantity.GetSimplifiedUnit().GetPhenomenon().IsValid())
        {
            auto formatter = service.GetFormatterOrDefault<Formatting::IUnitFormatter, Formatting::DefaultUnitFormatter>();
            return formatter->FormatQuantity(quantity.GetNeutralValue(), quantity.GetSimplifiedUnit().GetPhenomenon(), service);
        }
        else 
        {
            auto formatter = service.GetFormatterOrDefault<Formatting::IUnitFormatter, Formatting::DefaultUnitFormatter>();
            return formatter->FormatQuantity(quantity.GetNeutralValue(), quantity.GetSimplifiedUnit().GetDimension(), service);
        }
    }

}}}}
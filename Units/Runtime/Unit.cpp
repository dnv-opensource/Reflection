//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Unit.h"
#include <algorithm>
#include "Detail/MultiplyUnits.h"
#include "Detail/DivideUnits.h"
#include "Detail/PowerUnit.h"
#include "Detail/ListOfFundamentalUnitsWithExponents.h"
#include "Detail/AddExponentToLastTerm.h"
#include "Detail/EmptyUnit.h"

#include "Math/CompareAlmostEqual.h"
#include "../ForAllQuantities.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {

    Unit::Unit( Detail::UnitPointer unit )
        :   m_impl(unit)
    {
    }

    Unit::Unit( const Unit& other )
        :   m_impl(other.m_impl)
    {
    }

    Unit::Unit()
    {

    }

    double Unit::GetConversionFactor() const
    {
        if(!IsValid()) 
            return 0.0;
        return m_impl->GetConversionFactor();
    }

    std::string Unit::GetUnitName() const
    {
        if(!IsValid()) 
            return "";
        return m_impl->GetUnitName();
    }

    DynamicDimension Unit::GetDimension() const
    {
        if(!IsValid()) 
            return DynamicDimension(0, 0, 0, 0, 0);
        return m_impl->GetDimension();
    }

    Unit Unit::operator*( const Unit& other ) const
    {
        return Unit(*this) *= other;
    }

    Unit& Unit::operator*=(const Unit& other)
    {
        if(!IsValid() || !other.IsValid()) 
            m_impl.reset();
        else 
            m_impl = std::make_shared<Detail::MultiplyUnits>(m_impl, other.m_impl);
        return *this;
    }

    Unit Unit::operator/( const Unit& other ) const
    {
        return Unit(*this) /= other;
    }

    Unit& Unit::operator/=( const Unit& other )
    {
        if(!IsValid() || !other.IsValid()) 
            m_impl.reset();
        else 
            m_impl = std::make_shared<Detail::DivideUnits>(m_impl, other.m_impl);
        return *this;
    }

    bool Unit::operator==( const Unit& other ) const
    {
        if(GetDimension() != other.GetDimension()) 
            return false;
        return Math::CompareAlmostEqual(GetConversionFactor(), other.GetConversionFactor()); 
    }

    bool Unit::operator!=( const Unit& other ) const
    {
        return !operator==(other);
    }

    Unit& Unit::operator=( const Unit& other )
    {
        m_impl = other.m_impl;
        return *this;
    }

    bool Unit::IsValid() const
    {
        return m_impl != nullptr;
    }

    void Unit::SimplifyUnit()
    {
        if(!m_impl) 
            return;
        Detail::ListOfFundamentalUnitsWithExponents fundamentalUnitsWithExponents;
        m_impl->GetUnitComponents(fundamentalUnitsWithExponents);
        fundamentalUnitsWithExponents.TryMoveUnitWithPositiveExponentFirst();
        m_impl = fundamentalUnitsWithExponents.BuildCompositeUnit();
    }

    bool Unit::IsDimensionLess() const
    {
        return GetDimension() == DynamicDimension(0, 0, 0, 0, 0);
    }

    Unit Unit::GetEmptyUnit()
    {
        Unit u;
        u.m_impl = Detail::EmptyUnit::GetEmptyUnit();
        return u;
    }

    Unit Pow(const Unit& other, int exponent)
    {
        if(!other.IsValid()) 
            return Unit();
        return Unit(std::make_shared<Detail::PowerUnit>(other.m_impl, exponent));
    }
    
    Unit AddExponentToLastTerm(const Unit& other, int exponent)
    {
        if(!other.IsValid()) 
            return Unit();
        return Unit(Detail::AddExponentToLastTerm(other.m_impl, exponent));
    }
}}}}



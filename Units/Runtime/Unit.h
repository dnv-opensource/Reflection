#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Detail/IUnit.h"
#include "Detail/FundamentalUnit.h"
#include "../NamedQuantity.h"
#include "../Config.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {
    class DynamicPhenomenon;

    class UNITS_IMPORT_EXPORT Unit 
    {    
    public:
        ///Needed for usage in map.
        Unit();
        ///IsValid  checks whether m_impl is NULL or not.
        bool IsValid() const;
        Unit& operator=(const Unit& other);
        template<typename DimensionT>
        Unit(const std::string& name, QuantityBase<DimensionT> quantity)
            :   m_impl(new Detail::FundamemtalUnit(name, quantity))
        {}
        template<typename DimensionT>
        Unit(const NamedQuantity<DimensionT>& namedQuantity)
            :   m_impl(new Detail::FundamemtalUnit(namedQuantity.GetName(), namedQuantity))
        {}
        Unit(const std::string& unitName, double conversionFactor, const DynamicDimension& dimension)
            :   m_impl(new Detail::FundamemtalUnit(unitName, conversionFactor, dimension))
        {}
        Unit(const Unit& other);
        void SimplifyUnit();
        bool IsDimensionLess() const;
        double GetConversionFactor() const;
        std::string GetUnitName() const;
        DynamicDimension GetDimension() const;
        Unit operator*(const Unit& other) const;
        Unit& operator*=(const Unit& other);
        Unit operator/(const Unit& other) const;
        Unit& operator/=(const Unit& other);
        bool operator==(const Unit& other) const;
        bool operator!=(const Unit& other) const;
        friend Unit UNITS_IMPORT_EXPORT Pow(const Unit& other, int exponent);
        ///Adds exponent to the last term. For the expression kg*m, AddExponentToLastTerm(unit,2) would result in kg*m^2.
        friend Unit UNITS_IMPORT_EXPORT AddExponentToLastTerm(const Unit& other, int exponent);
        static Unit GetEmptyUnit();
        template<typename DimensionT>
        friend bool operator==(const NamedQuantity<DimensionT>& lhs, const Unit& rhs)        
        {
            return Unit(lhs) == rhs;
        }
        template<typename DimensionT>
        friend bool operator==(const Unit& lhs, const NamedQuantity<DimensionT>& rhs)        
        {
            return Unit(lhs) == rhs;
        }
        template<typename DimensionT>
        friend bool operator!=(const NamedQuantity<DimensionT>& lhs, const Unit& rhs)        
        {
            return Unit(lhs) != rhs;
        }
        template<typename DimensionT>
        friend bool operator!=(const Unit& lhs, const NamedQuantity<DimensionT>& rhs)        
        {
            return Unit(lhs) != rhs;
        }
    private:
        Unit(Detail::UnitPointer unit);
        Detail::UnitPointer m_impl;
    };

    template<class _Elem, class _Traits> inline
        std::basic_ostream<_Elem, _Traits>& operator<<(std::basic_ostream<_Elem, _Traits>& stream, const Unit& unit)
    {	
        return stream << "Unit(" 
            << unit.GetUnitName() << ", " 
            << unit.GetConversionFactor() << ", " 
            << unit.GetDimension() << ")";
    }
}}}}
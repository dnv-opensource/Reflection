#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypeUtilities/FixedSizeString.h"
#include "Unit.h"
#include "DynamicPhenomenon.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {

    class UNITS_IMPORT_EXPORT SimplifiedUnit 
    {    
    public:
        ///Needed for usage in map.
        SimplifiedUnit(const SimplifiedUnit& unit) = default;
        SimplifiedUnit(double conversionFactor, const DynamicDimension& dimension);
        SimplifiedUnit(double conversionFactor, const DynamicPhenomenon& phenomenon);
        SimplifiedUnit(Unit other);
        template<typename DimensionT>
        SimplifiedUnit(const QuantityBase<DimensionT>& quantity)
            : m_phenomenon(DimensionT())
            , m_conversionFactor(1)
        {}

        template<typename DimensionT>
        SimplifiedUnit(const NamedQuantity<DimensionT>& namedQuantity)
            : m_phenomenon(DimensionT())
            , m_conversionFactor(namedQuantity.GetValue())
            , m_string(namedQuantity.GetName())
        {}

        SimplifiedUnit& operator=(Unit other);
        SimplifiedUnit& operator=(const SimplifiedUnit& other) = default;
        bool TrySimplifyUnit();
        bool IsDimensionless() const;
        double GetConversionFactor() const;
        std::string GetUnitName() const;
        bool HasUnitName() const;
        const DynamicDimension& GetDimension() const;
        const DynamicPhenomenon& GetPhenomenon() const;
        Unit GetUnit() const;
        SimplifiedUnit operator*(const SimplifiedUnit& other) const;
        SimplifiedUnit& operator*=(const SimplifiedUnit& other);
        SimplifiedUnit operator/(const SimplifiedUnit& other) const;
        SimplifiedUnit& operator/=(const SimplifiedUnit& other);
        bool operator==(const SimplifiedUnit& other) const;
        bool operator!=(const SimplifiedUnit& other) const;
        friend SimplifiedUnit UNITS_IMPORT_EXPORT Pow(const SimplifiedUnit& other, int exponent);
        template<typename DimensionT>
        friend bool operator==(const NamedQuantity<DimensionT>& lhs, const SimplifiedUnit& rhs)        
        {
            return SimplifiedUnit(lhs) == rhs;
        }
        template<typename DimensionT>
        friend bool operator==(const SimplifiedUnit& lhs, const NamedQuantity<DimensionT>& rhs)
        {
            return SimplifiedUnit(lhs) == rhs;
        }
        template<typename DimensionT>
        friend bool operator!=(const NamedQuantity<DimensionT>& lhs, const SimplifiedUnit& rhs)
        {
            return SimplifiedUnit(lhs) != rhs;
        }
        template<typename DimensionT>
        friend bool operator!=(const SimplifiedUnit& lhs, const NamedQuantity<DimensionT>& rhs)
        {
            return SimplifiedUnit(lhs) != rhs;
        }
    private:
        bool TrySimplifyUnit(const std::string& unitName);
        SimplifiedUnit(const std::string& unitName, double conversionFactor, const DynamicDimension& dimension);
        TypeUtilities::FixedSizeString<31> m_string;
        double m_conversionFactor;
        DynamicPhenomenon m_phenomenon;
    };

//     template<class _Elem, class _Traits> inline
//         std::basic_ostream<_Elem, _Traits>& operator<<(std::basic_ostream<_Elem, _Traits>& stream, const Unit& unit)
//     {	
//         return stream << "Unit(" 
//             << unit.GetUnitName() << ", " 
//             << unit.GetConversionFactor() << ", " 
//             << unit.GetDimension() << ")";
//     }
}}}}
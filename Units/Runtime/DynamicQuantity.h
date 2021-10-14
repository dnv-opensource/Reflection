#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Unit.h"
#include "UnitSystem.h"
#include "../Config.h"
#include "DatabaseUnitSystem.h"
#include "SimplifiedUnit.h"
#include "sstream"
#include "../IPhenomenon.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {
    template<typename QuantityT>
    std::enable_if_t<IsQuantityWithPhenomenon<QuantityT>::value, DynamicPhenomenon> TryGetPhenomenon(const QuantityT& quantity)
    {
        return DynamicPhenomenon(typename QuantityT::PhenomenonType());
    }

    template<typename QuantityT>
    std::enable_if_t<IsQuantityWithDimension<QuantityT>::value, DynamicPhenomenon> TryGetPhenomenon(const QuantityT& quantity)
    {
        return DynamicPhenomenon(typename QuantityT::DimensionType());
    }

    class UNITS_IMPORT_EXPORT DynamicQuantity
    {
    public:
        template<typename T>
        DynamicQuantity(const Quantity<T>& quantity,UnitSystem& unitSystem)
            : m_unit(unitSystem.GetUnit(T()))
        {
            m_value = quantity.GetValue()/m_unit.GetConversionFactor();
            RemoveUnitIfDimensionLess();
            RemoveConversionFactorIfNoUnitName();
        }
        template<typename T>
        DynamicQuantity(const QuantityBase<T>& quantity)
            : m_unit(1, typename QuantityBase<T>::DimensionType())
            , m_value(quantity.GetValue())
        {
        }
        template<typename T>
        DynamicQuantity(const Quantity<T>& quantity)
            : m_unit(1, TryGetPhenomenon(quantity))
            , m_value(quantity.GetValue())
        {
        }
        template<typename T>
        DynamicQuantity(const NamedQuantity<T>& quantity)
            : m_unit(quantity)
            , m_value(1)
        {}
        template<typename T>
        DynamicQuantity(double value, const NamedQuantity<T>& quantity)
            : m_unit(quantity)
            , m_value(value)
        {}
        DynamicQuantity();
        explicit DynamicQuantity(double value);
        DynamicQuantity(double value, const Unit& unit);
        DynamicQuantity(double value, const SimplifiedUnit& unit);
        DynamicQuantity(double value, const std::string& unitName);
        DynamicQuantity(const DynamicQuantity& other);
        DynamicQuantity(double value, const DynamicPhenomenon& phenomenon);
        DynamicQuantity& operator=(const DynamicQuantity& other);
        ///Gets the value relative to the current unit
        double GetValue() const;
        std::string GetString() const;
        ///Gets the value multiplied with the unit conversion factor, in the SI units.
        double GetNeutralValue() const;
        ///Returns the unit of this quantity.
        Unit GetUnit() const;
        const SimplifiedUnit& GetSimplifiedUnit() const;
        bool TrySimplifyUnit();
        DynamicQuantity operator-() const;
        DynamicQuantity operator+() const;

        friend UNITS_IMPORT_EXPORT DynamicQuantity operator*(double lhs,const DynamicQuantity& other);
        DynamicQuantity operator*(double other) const;
        DynamicQuantity operator*(const DynamicQuantity& other) const;
        template<typename T>
        DynamicQuantity operator*(const Quantity<T>& other) const
        {
            return *this * DynamicQuantity(other);
        }
        template<typename T>
        friend DynamicQuantity operator*(const Quantity<T>& lhs, const DynamicQuantity& rhs)
        {
            return DynamicQuantity(lhs) * rhs;
        }
        DynamicQuantity& operator*=(const DynamicQuantity& other);

        friend UNITS_IMPORT_EXPORT DynamicQuantity operator/(double lhs,const DynamicQuantity& other);
        DynamicQuantity operator/(double other) const;
        DynamicQuantity operator/(const DynamicQuantity& other) const;
        template<typename T>
        DynamicQuantity operator/(const Quantity<T>& other) const
        {
            return *this / DynamicQuantity(other);
        }
        template<typename T>
        friend DynamicQuantity operator/(const Quantity<T>& lhs, const DynamicQuantity& rhs)
        {
            return DynamicQuantity(lhs) / rhs;
        }
        DynamicQuantity& operator/=(const DynamicQuantity& other);

        DynamicQuantity operator+(DynamicQuantity other) const;
        template<typename T> 
        DynamicQuantity operator+(const Quantity<T>& other) const
        {
            return *this + DynamicQuantity(other);
        }
        template<typename T>
        friend DynamicQuantity operator+(const Quantity<T>& lhs, const DynamicQuantity& rhs)
        {
            return DynamicQuantity(lhs) + rhs;
        }

        DynamicQuantity& operator+=(DynamicQuantity other);

        DynamicQuantity operator-(DynamicQuantity other) const;
        template<typename T>
        DynamicQuantity operator-(const Quantity<T>& other) const
        {
            return *this - DynamicQuantity(other);
        }
        template<typename T>
        friend DynamicQuantity operator-(const Quantity<T>& lhs, const DynamicQuantity& rhs)
        {
            return DynamicQuantity(lhs) - rhs;
        }

        DynamicQuantity& operator-=(DynamicQuantity other);

        bool operator==(const DynamicQuantity& other) const;
        bool operator!=(const DynamicQuantity& other) const;
        bool operator<(const DynamicQuantity& other) const;
        bool operator>(const DynamicQuantity& other) const;
        bool operator<=(const DynamicQuantity& other) const;
        bool operator>=(const DynamicQuantity& other) const;

        friend DynamicQuantity UNITS_IMPORT_EXPORT Pow(const DynamicQuantity& quantity, int exponent);
        //Returns false on error
        bool TryChangeUnit( const SimplifiedUnit& newUnit );
        //Throws on error
        void ChangeUnit(const SimplifiedUnit& newUnit);
        void ChangeUnit(const std::string& newUnitName);
        template<typename QuantityT>
        std::enable_if_t<IsQuantityWithPhenomenon<QuantityT>::value, QuantityT> As() const
        {
            using DimensionType = typename QuantityT::DimensionType;
            using PhenomenonType = typename QuantityT::PhenomenonType;
            if (GetSimplifiedUnit().GetDimension() != DynamicDimension(DimensionType()))
            {               
                if (GetSimplifiedUnit().GetDimension() == NoDimension())
                    return FromInput<QuantityT>(GetNeutralValue());
                throw std::runtime_error("Unable to convert '" + GetString() + "' to " + PhenomenonType::GetName());
            }
                
            return QuantityT(GetNeutralValue());
        }
        template<typename QuantityT>
        std::enable_if_t<!IsQuantityWithPhenomenon<QuantityT>::value, QuantityT> As() const
        {
            using DimensionType = typename QuantityT::DimensionType;
            if (GetSimplifiedUnit().GetDimension() != DynamicDimension(DimensionType()))
            {
                std::stringstream ss;
                ss << DynamicDimension(DimensionType());
                throw std::runtime_error("Unable to convert '" + GetString() + "' to " + ss.str());
            }

            return QuantityT(GetNeutralValue());
        }
        template<typename T>
        operator Quantity<T>() const {
            return As<Quantity<T>>();
        }
        bool RemoveUnitIfDimensionLess();
        bool RemoveConversionFactorIfNoUnitName();
        bool IsCompatible(const DynamicQuantity& other) const;
        bool IsCompatible(const DynamicPhenomenon& phenomenon) const;
        bool IsCompatible(const DynamicDimension& dimension) const;
    private:
        double m_value;
        SimplifiedUnit m_unit;
    };

    template<class _Elem, class _Traits> inline
    std::basic_ostream<_Elem, _Traits>& operator<<(std::basic_ostream<_Elem, _Traits>& stream, const DynamicQuantity& quantity)
    {	
        return stream << quantity.GetString();
    }

    std::string UNITS_IMPORT_EXPORT ToString(const DynamicQuantity& quantity, const DNVS::MoFa::Formatting::FormattingService& service);

}}}}

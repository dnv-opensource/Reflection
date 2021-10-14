#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Unit.h"
#include "IUnitProvider.h"

#include "..\Dimension.h"
#include "..\IPhenomenon.h"
#include <assert.h>
#include <map>
#include "UnitComposer.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {
    class DynamicQuantity;
    class UNITS_IMPORT_EXPORT UnitSystem
    {
    public:
        UnitSystem(const Unit& length, const Unit& force, const Unit& tempDiff, const Unit& time, const Unit& angle);
        UnitSystem(const Unit& length, const Unit& force, const Unit& tempDiff, const Unit& time);
        UnitSystem(const Unit& length, const Unit& force, const Unit& tempDiff);
        UnitSystem(const Unit& length, const Unit& force);
        UnitSystem(const std::string& lengthUnit, const std::string& forceUnit, const std::string& tempDiffUnit, const std::string& timeUnit, const std::string& angleUnit);
        UnitSystem(const std::string& lengthUnit, const std::string& forceUnit, const std::string& tempDiffUnit);
        UnitSystem(const std::string& lengthUnit, const std::string& forceUnit);
        UnitSystem();
        UnitSystem(std::shared_ptr<IUnitProvider> provider);
        
        std::string GetLengthUnitString() const;
        std::string GetForceUnitString() const;
        std::string GetTempDiffUnitString() const;
        std::string GetTimeUnitString() const;
        std::string GetAngleUnitString() const;

        bool operator==(const UnitSystem& other) const;
        bool operator!=(const UnitSystem& other) const;

        template<typename T>
        Unit GetUnit() const {
            return UnitComposer(m_provider, m_tolerance).GetUnit<T>();
        }
        template<typename T>
        Unit GetUnit(const T& arg) const
        {
            return UnitComposer(m_provider, m_tolerance).GetUnit(arg);
        }
        template<typename T>
        void SetUnit(const Unit& unit)
        {
            UnitComposer(m_provider, m_tolerance).SetUnit<T>(unit);
        }
        template<typename T>
        void SetUnit(const std::string& unitString)
        {
            UnitComposer(m_provider, m_tolerance).SetUnit<T>(ParseUnit(unitString));
        }
        template<typename QuantityT, typename ActualQuantityT, typename = std::enable_if_t<std::is_same_v<typename QuantityT::DimensionType, typename ActualQuantityT::DimensionType>>>
        void SetUnit(ActualQuantityT conversionFactor)
        {
            using DimensionType = QuantityT::DimensionType;
            UnitComposer(m_provider, m_tolerance).SetUnit<QuantityT>(Unit("", conversionFactor.GetValue(), DimensionType()));
        }
        template<typename T>
        void SetUnit(const T& arg, const Unit& unit)
        {
            UnitComposer(m_provider, m_tolerance).SetUnit(arg, unit);
        }      
        template<typename T>
        void SetUnit(const T& arg, const std::string& unitString)
        {
            UnitComposer(m_provider, m_tolerance).SetUnit(arg, ParseUnit(unitString));
        }
        template<typename T>
        double Convert(const QuantityBase<T>& q) const
        {
            return q.GetValue()/GetUnit(T()).GetConversionFactor();
        }
        template<typename QuantityT>
        double Convert(const QuantityT& q, std::enable_if_t<IsQuantityWithPhenomenon<QuantityT>::value, void*> = nullptr) const
        {
            return q.GetValue() / GetUnit(typename QuantityT::PhenomenonType()).GetConversionFactor();
        }
        template<typename QuantityT>
        double Convert(const QuantityT& q, std::enable_if_t<IsQuantityWithDimension<QuantityT>::value, void*> = nullptr) const
        {
            return q.GetValue() / GetUnit(typename QuantityT::DimensionType()).GetConversionFactor();
        }
        double Convert(const DynamicQuantity& q) const;
        double Convert(const DynamicPhenomenon& phenomenon, const DynamicQuantity& q) const;
        //Converts from a value in this unit system to the units specified in expectedUnit
        //ConvertToUnit(100,_cm) will convert to 10 if length units is mm or 0.1 if length unit is m.
        template<typename T>
        double ConvertToUnit(double valueInUnitSystem, const QuantityBase<T>& expectedUnit) const
        {
            return valueInUnitSystem * GetUnit(T()).GetConversionFactor() / expectedUnit.GetValue();
        }
        template<typename QuantityT, typename = std::enable_if_t<IsQuantityWithPhenomenon<QuantityT>::value>>
        QuantityT ConvertFromUnitSystem(double valueInUnitSystem) const
        {
            return QuantityT(valueInUnitSystem * GetUnit(typename QuantityT::PhenomenonType()).GetConversionFactor());
        }
        void Reset(const UnitSystem& other);
    private:
        std::shared_ptr<IUnitProvider> m_provider;
        double m_tolerance;
        Unit ParseUnit(const std::string& unitString) const;
    };
}}}}
#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Units/IPhenomenon.h"
#include "IUnitProvider.h"
#include "Unit.h"
#include <type_traits>

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {
    class UNITS_IMPORT_EXPORT UnitComposer {
    public:
        UnitComposer(const std::shared_ptr<IUnitProvider>& provider, double tolerance = 1e-6);


        ///Get unit from a quantity type. Example: GetUnit<Length>();
        template<typename QuantityT>
        std::enable_if_t<IsQuantityWithPhenomenon<QuantityT>::value, Unit> GetUnit() const 
        {
            typedef typename QuantityT::PhenomenonType PhenomenonT;
            return GetUnit(PhenomenonT());
        }
        ///Get unit from a deduced quantity type. Example: GetUnit<decltype(-1.4_m)>();
        template<typename QuantityT>
        std::enable_if_t<IsQuantityWithDimension<QuantityT>::value, Unit> GetUnit() const
        {
            typedef typename QuantityT::DimensionType DimensionT;
            return GetUnit(DimensionT());
        }
        template<typename QuantityT>
        std::enable_if_t<IsQuantityWithPhenomenon<QuantityT>::value> SetUnit(const Unit& unit)
        {
            typedef typename QuantityT::PhenomenonType PhenomenonT;
            SetUnit(PhenomenonT(), unit);
        }
        template<typename QuantityT>
        std::enable_if_t<IsQuantityWithDimension<QuantityT>::value> SetUnit(const Unit& unit)
        {
            typedef typename QuantityT::DimensionType DimensionT;
            SetUnit(DimensionT(), unit);
        }

        template<typename DimensionT>
        std::enable_if_t<!std::is_base_of_v<IPhenomenon,DimensionT>> SetUnit(const DimensionT& dim, const Unit& unit)
        {
            m_provider->SetDimensionUnit(dim, unit);
        }
        template<typename PhenomenonT>
        std::enable_if_t<std::is_base_of_v<IPhenomenon, PhenomenonT>> SetUnit(const PhenomenonT& phenomenon, const Unit& unit)
        {
            m_provider->SetPhenomenonUnit(phenomenon, unit);
        }

        ///Get unit from a phenomenon. Example: GetUnit(LengthPhenomenon());
        ///Tries to compose unit using the ComposeUnit defined on the phenomenon.
        ///For Energy, the SI units will be composed as J, for ForceMoment, the SI units will be composed as N*m.
        template<typename PhenomenonT>
        std::enable_if_t<std::is_base_of_v<IPhenomenon, PhenomenonT>, Unit> GetUnit(const PhenomenonT& phenomenon) const
        {
            Unit unit = m_provider->GetPhenomenonUnit(PhenomenonT());
            if (!unit.IsValid())
            {
                unit = PhenomenonT::ComposeUnit<Unit>([this](auto dimension)
                {
                    return this->GetUnit(dimension);
                });
                if (unit.IsValid() && m_provider->GetCache())
                    m_provider->GetCache()->SetPhenomenonUnit(phenomenon, unit);
                    
            }
            return unit;
        }

        ///Get unit from a dimension. Example: GetUnit(LengthDimension());
        template<typename DimensionT>
        std::enable_if_t<!std::is_base_of_v<IPhenomenon, DimensionT>, Unit> GetUnit(const DimensionT& dim) const
        {
            Unit unit = m_provider->GetDimensionUnit(dim);
            if (!unit.IsValid())
            {
                unit = ComposeDefaultUnit(dim);
                bool found = false;
                Quantity<DimensionT>::ForEachNamedUnit([&, this](const Unit& namedUnit)
                {
                    if (found)
                        return;
                    if (CompareUnits(unit, namedUnit))
                    {
                        unit = namedUnit;
                        found = true;
                    }
                });
                if(unit.IsValid() && m_provider->GetCache())
                    m_provider->GetCache()->SetDimensionUnit(dim, unit);
            }
            return unit;
        }
        ///Find the corresponding phenomenon type and call the GetUnit with this phenomenon.
        Unit GetUnit(const DynamicPhenomenon& phenomenon) const;
        Unit GetUnit(const DynamicDimension& dimension) const;
    private:
        Unit GetDefaultUnit(const DynamicPhenomenon& phenomenon, const Unit& fallbackUnit) const;
        Unit ComposeDefaultUnit(const DynamicDimension& dim) const;
        bool CompareUnits(const Unit& lhs, const Unit& rhs) const;
        void AppendUnit(Unit& unit, int pow, const Unit& baseUnit) const;
        std::shared_ptr<IUnitProvider> m_provider;
        double m_tolerance;
    };
}}}}
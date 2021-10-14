#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IUnit.h"
#include "Units/QuantityBase.h"
#include "Units/Config.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime { namespace Detail {
#pragma warning(push)
#pragma warning(disable:4275) //non dll-interface class 'Base' used as base for dll-interface class 'Class'
    class UNITS_IMPORT_EXPORT FundamemtalUnit : public IUnit
    {
    public:        
        FundamemtalUnit(const std::string& name, double conversionFactor, const DynamicDimension& dimension);
        template<typename DimensionT>
        FundamemtalUnit(const std::string& name, QuantityBase<DimensionT> quantity)
            :   m_name(name)
            ,   m_conversionFactor(quantity.GetValue())
            ,   m_dimension(DimensionT())
        {
        }
        virtual double GetConversionFactor() const;
        virtual std::string GetUnitName(UnitNameModifier modifier) const;
        virtual DynamicDimension GetDimension() const;
        virtual std::list<UnitPointer> GetChildren() const;
        virtual FundamemtalUnit* CreateCopy(const std::list<UnitPointer>& children) const;
        virtual void GetUnitComponents( ListOfFundamentalUnitsWithExponents& listOfFundamentalUnits, UnitNameModifier modifier = UnitNameModifier( ) ) const;
    private:
        std::string m_name;
        double m_conversionFactor;
        DynamicDimension m_dimension;
    };
#pragma warning(pop)
}}}}}

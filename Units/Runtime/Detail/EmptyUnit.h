#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IUnit.h"
#include "Units/Quantity.h"
#include <memory>

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime { namespace Detail {
    class EmptyUnit : public IUnit
    {
    public:        
        EmptyUnit() {}
        static const std::shared_ptr<IUnit>& GetEmptyUnit();
        virtual double GetConversionFactor() const {return 1.0;}
        virtual std::string GetUnitName(UnitNameModifier modifier) const {return "";}
        virtual DynamicDimension GetDimension() const {return DynamicDimension(0,0,0,0,0);}
        virtual std::list<UnitPointer> GetChildren() const { return std::list<UnitPointer>();}
        virtual EmptyUnit* CreateCopy(const std::list<UnitPointer>& children) const {return new EmptyUnit;}
        virtual void GetUnitComponents( ListOfFundamentalUnitsWithExponents& listOfFundamentalUnits, UnitNameModifier modifier = UnitNameModifier( ) ) const {}
    private:
        static std::shared_ptr<IUnit> m_emptyUnit;
    };
}}}}}

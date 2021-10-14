#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Units/Runtime/DynamicDimension.h"
#include "UnitNameModifier.h"

#include <list>
#include <memory>
#include <string>

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime { namespace Detail {
    class ListOfFundamentalUnitsWithExponents;
    class IUnit;
    typedef std::shared_ptr<IUnit> UnitPointer;

    class IUnit
    {
    public:
        virtual ~IUnit() {}
        virtual double GetConversionFactor() const = 0;
        ///Gets the unit name. Invert is relevant when dividing with a composite unit, you need to swap * and /.
        virtual std::string GetUnitName(UnitNameModifier modifier = UnitNameModifier()) const = 0;
        virtual DynamicDimension GetDimension() const = 0;
        virtual std::list<UnitPointer> GetChildren() const = 0;
        typedef std::pair<UnitPointer, int> UnitPointerExponentPair;        
        virtual void GetUnitComponents(ListOfFundamentalUnitsWithExponents& listOfFundamentalUnits, UnitNameModifier modifier = UnitNameModifier()) const = 0;
        virtual IUnit* CreateCopy(const std::list<UnitPointer>& children) const = 0;
    };
    
}}}}}

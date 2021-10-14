#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IUnit.h"
#include <list>

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime { namespace Detail {
    class ListOfFundamentalUnitsWithExponents 
    {
    public:
        void AddFundamentalUnit(UnitPointer unit, int exponent);
        UnitPointer BuildCompositeUnit() const;
        void TryMoveUnitWithPositiveExponentFirst();
    private:
        typedef std::pair<UnitPointer, int> UnitPointerExponentPair;
        std::list<UnitPointerExponentPair> m_listOfUnitsWithExponent;
    };
}}}}}
#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <list>
#include <memory>
#include <string>

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime { namespace Detail {
    class UnitNameModifier
    {
    public:
        UnitNameModifier(bool inverted = false, int exponent = 1);
        std::string GetMultiplySymbol() const;
        std::string GetDivideSymbol() const;
        std::string GetExponentSymbol() const;        
        UnitNameModifier operator!() const;
        friend UnitNameModifier Pow(const UnitNameModifier& other, int exponent);
        ///If exponent is negative, flip inverted and drop sign.
        friend UnitNameModifier Simplify(const UnitNameModifier& other);
        bool GetInverted() const {return m_inverted; }
        int GetExponent() const {return m_exponent; }
        ///If inverted, returns the opposite exponent.
        int GetExponentIncludingInversion() const;
    private:
        bool m_inverted;
        int m_exponent;
    };

}}}}}

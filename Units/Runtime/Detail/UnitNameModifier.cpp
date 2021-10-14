//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "UnitNameModifier.h"
#include <sstream>

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime { namespace Detail {

    UnitNameModifier::UnitNameModifier( bool inverted, int exponent) 
        :   m_inverted(inverted) 
        ,   m_exponent(exponent)
    {

    }

    std::string UnitNameModifier::GetMultiplySymbol() const
    {
        if(m_inverted) 
            return "/";
        else 
            return "*";
    }

    std::string UnitNameModifier::GetDivideSymbol() const
    {
        if(m_inverted) 
            return "*";
        else 
            return "/";
    }

    std::string UnitNameModifier::GetExponentSymbol() const
    {
        if(m_exponent == 1) 
            return "";
        else 
        {
            std::stringstream stream;
            stream << m_exponent;
            return "^" + stream.str();
        }
    }

    UnitNameModifier UnitNameModifier::operator!() const
    {
        return UnitNameModifier(!GetInverted(), GetExponent());
    }

    int UnitNameModifier::GetExponentIncludingInversion() const
    {
        if(m_inverted) 
            return -m_exponent;
        else 
            return m_exponent;
    }

    UnitNameModifier Pow( const UnitNameModifier& other, int exponent )
    {
        return UnitNameModifier(other.GetInverted(), other.GetExponent() * exponent);
    }

    UnitNameModifier Simplify(const UnitNameModifier& other)
    {
        if(other.GetExponent()<0) 
        {
            return UnitNameModifier(!other.GetInverted(), -other.GetExponent());
        }
        else 
            return UnitNameModifier(other.GetInverted(), other.GetExponent());
    }

}}}}}
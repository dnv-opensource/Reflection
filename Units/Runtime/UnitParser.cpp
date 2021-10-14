//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "UnitParser.h"
#include <ctype.h>
#include "Detail\UnitMatcher.h"
#include <tuple>
namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {

    UnitParser::UnitParser(UnitManager && unitManager, bool throwOnError)
        : m_unitManager(std::move(unitManager))
        , m_throwOnError(throwOnError)
    {
    }

    UnitParser::UnitParser(bool throwOnError)
        : m_throwOnError(throwOnError)
    {
        m_unitManager.AddAllUnits();
    }

    Unit UnitParser::Parse(const std::string& unitName ) const
    {
        Detail::UnitStringIterator iUnitName(unitName);
        Unit unit = ParseUnitWithExponent(iUnitName);
        while(unit.IsValid() && !iUnitName.IsEmpty())
        {
            if(*iUnitName == '*')
            {
                ++iUnitName;
                unit *= ParseUnitWithExponent(iUnitName);   
            }
            else if(*iUnitName == '/')
            {
                ++iUnitName;
                unit /= ParseUnitWithExponent(iUnitName);   
            }
            else 
                unit *= ParseUnitWithExponent(iUnitName);   
        }
        if (m_throwOnError && !unit.IsValid())
            throw std::runtime_error("Unable to parse unit: '" + unitName + "'");
        return unit;
    }

    std::pair<bool,int> UnitParser::ParseExponent( Detail::UnitStringIterator& iUnitName ) const
    {
        bool success = true;
        bool parsedDigit = false;
        int exponent = 0;
        int sign = 1;
        if(!iUnitName.IsEmpty() && *iUnitName == '^')
        {
            ++iUnitName;
            success = false;
        }
        if(!iUnitName.IsEmpty() && *iUnitName == '-')
        {
            ++iUnitName;
            sign = -1;
            success = false;
        }
        else if(!iUnitName.IsEmpty() && *iUnitName == '+')
        {
            ++iUnitName;
            sign = 1;
            success = false;
        }
        while(!iUnitName.IsEmpty() && isdigit(*iUnitName)) 
        {
            parsedDigit = true;
            success = true;
            exponent = exponent*10 + (*iUnitName-'0');
            ++iUnitName;
        }
        if(success && exponent == 0 && !parsedDigit)
            exponent = 1;
        return std::make_pair(success, sign*exponent);
    }

    Unit UnitParser::ParseUnitString( Detail::UnitStringIterator& iUnitName ) const
    {
        std::string unitName = iUnitName.GetSubstr();
        size_t firstNonText = unitName.find_first_of("^-+0123456789");
        if(firstNonText != std::string::npos)
            unitName = unitName.substr(0, firstNonText);
        auto it = m_unitManager.upper_bound(unitName);
        if(it == m_unitManager.begin()) 
            return Unit();
        --it;
        const std::string text = it->first;
        if(unitName.substr(0, text.size()) != text)
            return Unit();
        Unit unit = it->second;
        iUnitName += text.size();
        return unit;
    }

    Unit UnitParser::ParseUnitWithExponent( Detail::UnitStringIterator& iUnitName ) const
    {
        Unit unit = ParseUnitString(iUnitName);
        if(!unit.IsValid()) 
            return unit;
        bool success;
        int exponent;
        std::tie(success, exponent) = ParseExponent(iUnitName);
        if(exponent == 0) 
        {
            if(!success)
            {
                //Returns invalid unit
                return Unit();
            }
            else
            {
                //Returns dimensionless
                return Unit("",1.0, DynamicDimension(0,0,0,0,0));
            }
        }
        else if(exponent == 1) 
            return unit;
        else 
            return Pow(unit, exponent);
    }

    const UnitParser& UnitParser::GetUnitParser(bool throwOnError)
    {
        if (throwOnError)
        {
            static thread_local UnitParser parser(true);
            return parser;
        }        
        else
        {
            static thread_local UnitParser parser(false);
            return parser;
        }
    }

    Unit UnitParser::GetUnitFromConversionFactor(const DynamicDimension& dimension, double unitConversionFactor, double tolerance /*= 1e-6*/) const
    {
        Detail::UnitMatcher matcher(dimension, unitConversionFactor, tolerance);
        for(auto it = m_unitManager.begin(); it != m_unitManager.end(); ++it)
        {
            matcher.MatchUnit(it->second);
        }
        return matcher.GetBestUnit();
    }

}}}}
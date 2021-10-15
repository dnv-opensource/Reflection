//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsParameter.h>
#include <jsUnits/jsUnitClass.h>
#include <limits>

jsParameter::jsParameter(const jsUnitValue& value)
{
}

jsParameter::jsParameter(double value,const std::string& unitName)
{
}

jsParameter::operator const double&() const
{
   return m_value;   
}

jsParameter::operator std::string() const
{
   if(_isnan(m_value)) return "NaN";
   if(!_finite(m_value)) {
      if(m_value>0) return "+Infinity";
      else return "-Infinity";
   }

   std::stringstream interpreter;
   interpreter.precision(10);
   interpreter << m_value;
   return interpreter.str();
}

std::string jsParameter::toStringWithPrecision(int precision) const
{
    if (_isnan(m_value)) return "NaN";
    if (!_finite(m_value)) {
        if (m_value>0) return "+Infinity";
        else return "-Infinity";
    }

    std::stringstream interpreter;
    interpreter.precision(precision);
    interpreter << m_value;
    return interpreter.str();
}

void jsParameter::init(jsTypeLibrary& typeLibrary)
{
   jsTClass<jsParameter> cls(typeLibrary,"Parameter");
   if(cls.reinit()) return;
   cls.Constructor<double>();
   cls.Constructor<double,const std::string&>();
   cls.Constructor<const jsUnitValue&>();
   //Operators
   cls.Function("toString", &jsParameter::operator std::string);
   cls.Function("toDouble", &jsParameter::operator const double&);
   cls.ImplicitConversion(&jsParameter::operator std::string);
}



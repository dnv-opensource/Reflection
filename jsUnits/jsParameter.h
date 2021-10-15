//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSPARAMETER_H
#define JSPARAMETER_H

#include <jsUnits/jsUnits_config.h>
#include <jsScript/jsScriptable.h>

class jsUnitValue;

class JSUNITS_IMPORT_EXPORT jsParameter : public jsScriptable
{
public: // external
   jsParameter(const jsParameter& value) : jsScriptable(value) , m_value(value) {}
   jsParameter(double value) : m_value(value) {}
   jsParameter(const jsUnitValue& value);
   jsParameter(double value,const std::string& unitName);
   operator const double&() const;
   operator std::string() const;
   std::string toStringWithPrecision(int precision) const;
public: // internal
   virtual ~jsParameter() {}

   static void init(jsTypeLibrary& typeLibrary);
private:
   double m_value;
};

#endif 

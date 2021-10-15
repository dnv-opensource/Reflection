//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsScript/jsClass.h>

#include <jsScript/jsUnit.h>
#include "Units/Runtime/UnitParser.h"

using namespace DNVS::MoFa::Units::Runtime;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


jsUnit::jsUnit(const std::string& unit)
:  m_unit(UnitParser::GetUnitParser().Parse(unit))
{
   if(!m_unit.IsValid()) 
       throw std::exception("invalid unit");
}

jsUnit::jsUnit(const DNVS::MoFa::Units::Runtime::Unit& unit)
:  m_unit(unit)
{
}

jsUnit::~jsUnit()
{
}

void jsUnit::init(jsTypeLibrary& typeLibrary)
{
   jsTClass<jsUnit> cls(typeLibrary,"Unit");
   if(cls.reinit()) return;
   cls.Constructor<const std::string&>();

   //Operators
   cls.Function("toString", &jsUnit::operator std::string);
   cls.ImplicitConversion(&jsUnit::operator std::string);
}

jsUnit::operator std::string() const
{
   return m_unit.GetUnitName();
}

const DNVS::MoFa::Units::Runtime::Unit jsUnit::unit() const
{
   return m_unit;
}

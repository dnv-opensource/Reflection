//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSQUANTITY_H
#define JSQUANTITY_H

#include <jsScript/jsScript_config.h>
#include <jsScript/jsScriptable.h>

#include "Units/Runtime/DynamicPhenomenon.h"
#include "Units/Runtime/DynamicQuantity.h"
#include <mutex>

class jsUnitValue;

class JSSCRIPT_IMPORT_EXPORT jsBoundDouble {
public:
   jsBoundDouble(double value,bool inclusive=false) : m_value(value) , m_inclusive(inclusive) {}
   operator const double&() const {return m_value;}
   bool inclusive() const {return m_inclusive;}
   jsBoundDouble operator-();
   jsBoundDouble& operator+();
   static jsBoundDouble infinity();
private:
   double m_value;
   bool m_inclusive;
};

class JSSCRIPT_IMPORT_EXPORT jsQuantity : public jsScriptable  
{
public: //external
   double toDouble() const;
   std::string signature() const;
public: //internal
   jsQuantity(const jsAutomation& automation) : jsScriptable(automation) {}
   jsQuantity(const DNVS::MoFa::Units::Runtime::DynamicPhenomenon& phenomenon,double value,const std::string& unitName);
   jsQuantity(const DNVS::MoFa::Units::Runtime::DynamicPhenomenon& phenomenon,const jsUnitValue& value);
   jsQuantity(const DNVS::MoFa::Units::Runtime::DynamicPhenomenon& phenomenon,double value,bool databaseUnits);
   jsQuantity(const DNVS::MoFa::Units::Runtime::DynamicPhenomenon& phenomenon);
   jsQuantity(const DNVS::MoFa::Units::Runtime::DynamicPhenomenon& phenomenon,const std::string& quantity);
	jsQuantity(double value=0.0);
   jsQuantity(double value,const std::string& unit);
   jsQuantity(const jsQuantity& quantity);
   jsQuantity(const DNVS::MoFa::Units::Runtime::DynamicPhenomenon& phenomenon, const DNVS::MoFa::Units::Runtime::DynamicQuantity& quantity);

   bool compatible(const jsQuantity& quantity);
   
   jsQuantity& operator=(const jsQuantity& quantity);
	virtual ~jsQuantity();
   static void init(jsTypeLibrary& typeLibrary);
   operator double() const;
   operator std::string() const;
   DNVS::MoFa::Units::Runtime::Unit GetInputUnit() const;
   DNVS::MoFa::Units::Runtime::Unit GetDatabaseUnit() const;
   operator const DNVS::MoFa::Units::Runtime::DynamicQuantity&() const;
   const DNVS::MoFa::Units::Runtime::DynamicQuantity& GetQuantity() const;
   const DNVS::MoFa::Units::Runtime::DynamicPhenomenon& GetPhenomenon() const;
   DNVS::MoFa::Units::Runtime::DynamicDimension GetDimension() const;
   jsValue* operator-();
   virtual jsValue* operator+(jsValue* op2);
   virtual jsValue* operator-(jsValue* op2);
   virtual jsValue* operator*(jsValue* op2);
   virtual jsValue* operator/(jsValue* op2);
   virtual jsValue* operator==(jsValue* op2);
   virtual jsValue* operator!=(jsValue* op2);
   virtual jsValue* operator<=(jsValue* op2);
   virtual jsValue* operator>=(jsValue* op2);
   virtual jsValue* operator<(jsValue* op2);
   virtual jsValue* operator>(jsValue* op2);
   virtual jsValue* InverseDivide(jsValue* op2);

   enum formatFlags {
      excludeUnitString = 0,
      includeUnitString = 1,
   };
   enum unitType {
      databaseUnit = 0,
      inputUnit    = 1,
   };

   std::string format(formatFlags format=excludeUnitString,unitType unitTypeFlag=inputUnit) const;
   /**Valid values for flags
   std::ios_base::floatfield
   std::ios_base::scientific
   std::ios_base::fixed
   */
   std::string format(int precision,std::ios_base::fmtflags flags=std::ios_base::floatfield,formatFlags format=excludeUnitString,unitType unitTypeFlag=inputUnit) const;
   double value(unitType unitTypeFlag=inputUnit) const;
   double convert(const std::string& unitName) const;
   std::string unitString(unitType unitTypeFlag=inputUnit) const;
   
   void checkRange(const jsBoundDouble& lower_bound=-jsBoundDouble::infinity(),const jsBoundDouble& upper_bound=jsBoundDouble::infinity());

   static formatFlags stringOperatorFormat(formatFlags operator_format);

   static int compareTolerant(const double& a1,const double& a2);
protected:
    std::mutex& GetMutex();
private:
   bool getCompatibleNumber(jsValue* op2,double& number);
   void ThrowIfQuantityAndPhenomenonAreIncompatible();
private:
   DNVS::MoFa::Units::Runtime::DynamicPhenomenon m_phenomenon;
   DNVS::MoFa::Units::Runtime::DynamicQuantity m_quantity;
   static formatFlags m_string_operator_format;
   static std::mutex m_mutex;
};

template<class _E, class _Tr> inline
   std::basic_ostream<_E, _Tr>& __cdecl operator<<(
		std::basic_ostream<_E, _Tr>& _O, const jsQuantity& quantity)
{
   return _O << std::string(quantity);
}

#endif
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSTQUANTITY_H
#define JSTQUANTITY_H

#include <jsScript/jsQuantity.h>
#include <jsScript/jsClass.h>
#include <jsScript/jsUnitValue.h>
#include <float.h>
#pragma warning(disable:4275)

template<class Quantity>
class jsTQuantity : public jsQuantity  
{
public: //external
   jsTQuantity(double value) : jsQuantity(Quantity::GetPhenomenon(),value,false) {}
   jsTQuantity(const jsUnitValue& value) : jsQuantity(Quantity::GetPhenomenon(),value) {}
   jsTQuantity(const DNVS::MoFa::Units::Runtime::DynamicQuantity& quantity) : jsQuantity(Quantity::GetPhenomenon(), quantity) {}
   jsTQuantity(double value,const std::string& unitName) : jsQuantity(Quantity::GetPhenomenon(),value,unitName) {}
   jsTQuantity(const std::string& quantity) : jsQuantity(Quantity::GetPhenomenon(),quantity) {}

   static Quantity* construct_1(double value) {return new Quantity(jsTQuantity(value));}
   static Quantity* construct_2(const jsUnitValue& value) {return new Quantity(jsTQuantity(value));}
   static Quantity* construct_3(double value,const std::string& unitName) {return new Quantity(jsTQuantity(value,unitName));}
   static Quantity* construct_4(const std::string& quantity) {return new Quantity(jsTQuantity(quantity));}

   bool operator==(const jsTQuantity& other);
   bool operator!=(const jsTQuantity& other);
   bool operator<(const jsTQuantity& other);
   bool operator>(const jsTQuantity& other);
   bool operator<=(const jsTQuantity& other);
   bool operator>=(const jsTQuantity& other);

   bool operator==(const double& other);
   bool operator!=(const double& other);
   bool operator<(const double& other);
   bool operator>(const double& other);
   bool operator<=(const double& other);
   bool operator>=(const double& other);

   //Prevents these from being called in order to fix code.
   friend bool operator==(const double& lhs,const jsTQuantity& rhs) {
      if(_isnan(lhs) || _isnan(rhs)) return false;
      return jsQuantity::compareTolerant(lhs,rhs)==0;
   }
   friend bool operator!=(const double& lhs,const jsTQuantity& rhs) {
      if(_isnan(lhs) || _isnan(rhs)) return true;
      return jsQuantity::compareTolerant(lhs,rhs)!=0;
   }
   friend bool operator<(const double& lhs,const jsTQuantity& rhs)  {return jsQuantity::compareTolerant(lhs,rhs)<0;}
   friend bool operator>(const double& lhs,const jsTQuantity& rhs)  {return jsQuantity::compareTolerant(lhs,rhs)>0;}
   friend bool operator<=(const double& lhs,const jsTQuantity& rhs) {return jsQuantity::compareTolerant(lhs,rhs)<=0;}
   friend bool operator>=(const double& lhs,const jsTQuantity& rhs) {return jsQuantity::compareTolerant(lhs,rhs)>=0;}

   static std::string unitString(unitType unitTypeFlag=inputUnit);

   static void init(jsTypeLibrary& typeLibrary);
public: //internal
   jsTQuantity(const jsTQuantity& value) : jsQuantity(value) {}
   jsTQuantity(double value,bool databaseUnits) : jsQuantity(Quantity::GetPhenomenon(),value,databaseUnits) {}
   jsTQuantity() : jsQuantity(Quantity::GetPhenomenon()) {}
   virtual ~jsTQuantity() {}
};

template<class Quantity>
void jsTQuantity<Quantity>::init(jsTypeLibrary& typeLibrary)
{
    std::string phenomenonName = Quantity::GetPhenomenon().GetName();
    jsTClass<Quantity, jsQuantity> cls(typeLibrary, phenomenonName);

   if (cls.reinit())
       return;

   std::string argument = phenomenonName;
   argument[0] = tolower(argument[0]);
   jsStack::stack()->addType(typeid(Quantity), phenomenonName);

   cls.AddDocumentation(phenomenonName + " has the signature: " + jsTQuantity<Quantity>().signature());

   using DNVS::MoFa::Reflection::Members::Arg;
   cls.Constructor(jsTQuantity<Quantity>::construct_1)
       .AddDocumentation("Construct a new " + phenomenonName + " from a double value with the default input unit for " +
           phenomenonName)
       .AddSignature((Arg(argument)));

   cls.Constructor(jsTQuantity<Quantity>::construct_2)
       .AddDocumentation("Construct a new " + phenomenonName + " unit from a unit value")
       .AddSignature((Arg(argument)));

   cls.Constructor(jsTQuantity<Quantity>::construct_3)
       .AddDocumentation("Construct a new " + phenomenonName + " unit explicitly")
       .AddSignature((Arg(argument), "unitName"));

   cls.Constructor(jsTQuantity<Quantity>::construct_4)
       .AddDocumentation("Construct a new " + phenomenonName + " based on a unit string")
       .AddSignature((Arg("unitString")));
}

template<class Quantity>
std::string jsTQuantity<Quantity>::unitString(unitType unitTypeFlag)
{
   return jsQuantity(Quantity::GetPhenomenon()).unitString(unitTypeFlag);
}

template<class Quantity>
bool jsTQuantity<Quantity>::operator==(const jsTQuantity& other)
{
   if(_isnan(*this) || _isnan(other)) return false;
   return compareTolerant(*this,other)==0;
}

template<class Quantity>
bool jsTQuantity<Quantity>::operator!=(const jsTQuantity& other)
{
   if(_isnan(*this) || _isnan(other)) return true;
   return compareTolerant(*this,other)!=0;
}

template<class Quantity>
bool jsTQuantity<Quantity>::operator<(const jsTQuantity& other)
{
   return compareTolerant(*this,other)<0;
}

template<class Quantity>
bool jsTQuantity<Quantity>::operator>(const jsTQuantity& other)
{
   return compareTolerant(*this,other)>0;
}

template<class Quantity>
bool jsTQuantity<Quantity>::operator<=(const jsTQuantity& other)
{
   return compareTolerant(*this,other)<=0;
}

template<class Quantity>
bool jsTQuantity<Quantity>::operator>=(const jsTQuantity& other)
{
   return compareTolerant(*this,other)>=0;
}

template<class Quantity>
bool jsTQuantity<Quantity>::operator==(const double& other)
{
   return compareTolerant(*this,other)==0;
}

template<class Quantity>
bool jsTQuantity<Quantity>::operator!=(const double& other)
{
   return compareTolerant(*this,other)!=0;
}

template<class Quantity>
bool jsTQuantity<Quantity>::operator<(const double& other)
{
   return compareTolerant(*this,other)<0;
}

template<class Quantity>
bool jsTQuantity<Quantity>::operator>(const double& other)
{
   return compareTolerant(*this,other)>0;
}

template<class Quantity>
bool jsTQuantity<Quantity>::operator<=(const double& other)
{
   return compareTolerant(*this,other)<=0;
}

template<class Quantity>
bool jsTQuantity<Quantity>::operator>=(const double& other)
{
   return compareTolerant(*this,other)>=0;
}


#endif

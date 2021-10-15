//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "jsUnitConversion.h"
#include <jsScript/jsConversions.h>
#include <jsScript/jsUnitValue.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool getUniqueQuantity(jsValue* value,jsQuantity& result)
{
   try {
      jsQuantity* quantity=fromJScript(value,jsType<jsQuantity*>());
      result=*quantity;
      return true;
   }
   catch(...) {}
   try {
      jsUnitValue* unitValue=fromJScript(value,jsType<jsUnitValue*>());
      unitValue->createUniqueQuantity(result);
      if(!result.GetPhenomenon().IsValid()) return false;
      return true;
   }
   catch(...) {}
   try {
      double double_value=fromJScript(value,jsType<double>());
      result=jsQuantity(double_value);
      return true;
   }
   catch(...) {}
   return false;
}

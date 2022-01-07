// jsTemperature.cpp: implementation of the jsTemperature class.
//
//////////////////////////////////////////////////////////////////////

#include <jsUnits/jsTemperature.h>
#include <jsUnits/jsUnitClass.h>

// Do not use. Use jsTemperatureDifference in stead.

const char* jsTemperature::unitName() {return "Temperature";}

void jsTemperature::init(jsInterpreter* ip)
{
   jsUnitClass<jsTemperature> cls(ip);
}



// jsTemperature.h: interface for the jsTemperature class.
//
//////////////////////////////////////////////////////////////////////

#ifndef JSTEMPERATURE_H
#define JSTEMPERATURE_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
// Do not use. Use jsTemperatureDifference in stead.
class JSUNITS_IMPORT_EXPORT jsTemperature : public jsTQuantity<jsTemperature>
{
public:
	jsTemperature(const jsTQuantity<jsTemperature>& value) : jsTQuantity<jsTemperature>(value) {}
   jsTemperature(double value,unitType unit=databaseUnit) : jsTQuantity<jsTemperature>(value,unit==databaseUnit) {}
   jsTemperature(double value,const std::string& unitName) : jsTQuantity<jsTemperature>(value,unitName) {}
	virtual ~jsTemperature() {}

   static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
   static void init(jsInterpreter* ip);
};

#endif 

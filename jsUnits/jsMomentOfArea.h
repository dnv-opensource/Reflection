// jsMomentOfArea.h: interface for the jsMomentOfArea class.
//
//////////////////////////////////////////////////////////////////////

#ifndef JSMOMENTOFAREA_H
#define JSMOMENTOFAREA_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>

class JSUNITS_IMPORT_EXPORT jsMomentOfArea : public jsTQuantity<jsMomentOfArea>
{
public:
	jsMomentOfArea(const jsTQuantity<jsMomentOfArea>& value) : jsTQuantity<jsMomentOfArea>(value) {}
   jsMomentOfArea(double value) : jsTQuantity<jsMomentOfArea>(value,true) {}
   jsMomentOfArea(double value,const std::string& unitName) : jsTQuantity<jsMomentOfArea>(value,unitName) {}
	virtual ~jsMomentOfArea() {}

   static const char* unitName();
   static void init(jsInterpreter* ip);
};

#endif 

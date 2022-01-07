// jsMomentOfArea.cpp: implementation of the jsMomentOfArea class.
//
//////////////////////////////////////////////////////////////////////

#include <jsUnits/jsMomentOfArea.h>
#include <jsUnits/jsUnitClass.h>

const char* jsMomentOfArea::unitName() {return "MomentOfArea";}

void jsMomentOfArea::init(jsInterpreter* ip)
{
   jsUnitClass<jsMomentOfArea> cls(ip);
}



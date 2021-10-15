//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSUNIT_H
#define JSUNIT_H

#include <jsScript/jsScriptable.h>
#include "Units/Runtime/Unit.h"

class JSSCRIPT_IMPORT_EXPORT jsUnit : public jsScriptable
{
public:
    jsUnit(const jsAutomation& automation) : jsScriptable(automation) {}
    jsUnit(const DNVS::MoFa::Units::Runtime::Unit& unit);
    jsUnit(const std::string& unit);
    virtual ~jsUnit();
    operator std::string() const; //convert to string
    const DNVS::MoFa::Units::Runtime::Unit unit() const;

    static void init(jsTypeLibrary& typeLibrary);
private:
    DNVS::MoFa::Units::Runtime::Unit m_unit;
};

#endif
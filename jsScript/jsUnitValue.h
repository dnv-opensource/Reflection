//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSUNITVALUE_H
#define JSUNITVALUE_H

#include <jsScript/jsScriptable.h>
#include <jsScript/jsUnit.h>
#include "Units/Runtime/DynamicQuantity.h"

class jsUnit;
class jsQuantity;

class JSSCRIPT_IMPORT_EXPORT jsUnitValue : public jsScriptable
{
public: //external
    double toDouble() const;
public:
    jsUnitValue(const jsAutomation& automation) : jsScriptable(automation) {}
    jsUnitValue(double value, jsUnit* unit);
    jsUnitValue(const jsQuantity& quantity);
    jsUnitValue(const DNVS::MoFa::Units::Runtime::DynamicQuantity& quantity);
    virtual ~jsUnitValue();

    bool createUniqueQuantity(jsQuantity& quantity) const;

    operator std::string() const; //convert to string
    operator const DNVS::MoFa::Units::Runtime::DynamicQuantity&() const;
    const DNVS::MoFa::Units::Runtime::DynamicQuantity& GetQuantity() const;
    static void init(jsTypeLibrary& typeLibrary);

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

private:
    bool getCompatibleNumber(jsValue* op2, double& number);
private:
    DNVS::MoFa::Units::Runtime::DynamicQuantity m_quantity;
};

#endif
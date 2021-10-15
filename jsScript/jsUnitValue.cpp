//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsScript/jsUnitValue.h>
#include <jsScript/jsClass.h>
#include <jsScript/jsUnit.h>
#include <jsScript/jsQuantity.h>

#include <mofaTools/lexical_cast.h>

#include "Units/Quantity.h"
#include "Units/ForAllDimensions.h"
#include "Reflection/Classes/Class.h"
#include "Units/Runtime/DynamicQuantity.h"
#include "Units/Runtime/Unit.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

using namespace DNVS::MoFa::Reflection::Classes;
using namespace DNVS::MoFa::Units;
using namespace DNVS::MoFa::Units::Runtime;

template<typename Dimension>
jsValue* operator*(DNVS::MoFa::Units::Quantity<Dimension> dim, const jsUnitValue& unitValue)
{
    jsTypeConverter<DNVS::MoFa::Units::Quantity<Dimension>, jsQuantity*> converter(jsStack::stack());
    return converter.Convert(dim)->operator*(const_cast<jsUnitValue*>(&unitValue));
}

template<typename Dimension>
jsValue* operator/(DNVS::MoFa::Units::Quantity<Dimension> dim, const jsUnitValue& unitValue)
{
    jsTypeConverter<DNVS::MoFa::Units::Quantity<Dimension>, jsQuantity*> converter(jsStack::stack());
    return converter.Convert(dim)->operator/(const_cast<jsUnitValue*>(&unitValue));
}

jsValue* operator*(const jsUnitValue& unitValue, jsValue* rhs)
{
    return const_cast<jsUnitValue&>(unitValue).operator*(rhs);
}

jsValue* operator/(const jsUnitValue& unitValue, jsValue* rhs)
{
    return const_cast<jsUnitValue&>(unitValue).operator/(rhs);
}

jsValue* operator+(const jsUnitValue& unitValue, jsValue* rhs)
{
    return const_cast<jsUnitValue&>(unitValue).operator+(rhs);
}

jsValue* operator-(const jsUnitValue& unitValue, jsValue* rhs)
{
    return const_cast<jsUnitValue&>(unitValue).operator-(rhs);
}

jsUnitValue* operator*(double factor, const jsUnitValue& unitValue) {
    return new jsUnitValue(unitValue.GetQuantity() * factor);
}

jsUnitValue* operator/(double value, const jsUnitValue& unitValue) {
    return new jsUnitValue(value / unitValue.GetQuantity());
}

jsUnitValue::jsUnitValue(double value, jsUnit* unit)
    : m_quantity(value, unit->unit())
{
}

jsUnitValue::jsUnitValue(const jsQuantity& quantity)
    : jsScriptable(quantity)
    , m_quantity(quantity)
{
}

jsUnitValue::jsUnitValue(const DNVS::MoFa::Units::Runtime::DynamicQuantity& quantity)
    : m_quantity(quantity)
{
}

jsUnitValue::~jsUnitValue()
{

}

struct RegisterjsUnitValueOperators {
public:
    RegisterjsUnitValueOperators()
        : m_cls(jsStack::stack()->GetTypeLibrary(), "")
    {
        m_cls.Function("toString", &jsUnitValue::operator std::string);
        m_cls.Function("toDouble", &jsUnitValue::toDouble);
        m_cls.Operator(This.Const / Other<jsValue*>());
        m_cls.Operator(This.Const*Other<jsValue*>());
        m_cls.Operator(This.Const + Other<jsValue*>());
        m_cls.Operator(This.Const - Other<jsValue*>());
        m_cls.Operator(double() / This.Const);
        m_cls.Operator(double()*This.Const);
    }
    template<typename Dimension>
    void Apply() {
        m_cls.Operator(Quantity<Dimension>()*This.Const);
        m_cls.Operator(Quantity<Dimension>() / This.Const);
    }
private:
    Class<jsUnitValue> m_cls;
};

const DNVS::MoFa::Units::Runtime::DynamicQuantity& jsUnitValue::GetQuantity() const
{
    return m_quantity;
}

void jsUnitValue::init(jsTypeLibrary& typeLibrary)
{
    jsTClass<jsUnitValue> cls(typeLibrary, "UnitValue");
    if (cls.reinit()) return;
    cls.Constructor<double, jsUnit*>();
    cls.Constructor<const jsQuantity&>();
    cls.Constructor<const DNVS::MoFa::Units::Runtime::DynamicQuantity&>();

    //Operators
    cls.Function("toString", &jsUnitValue::operator std::string);
    cls.Function("toDouble", &jsUnitValue::toDouble);
    cls.ImplicitConversion(&jsUnitValue::operator std::string);
    cls.ImplicitConversion(&jsUnitValue::operator const DNVS::MoFa::Units::Runtime::DynamicQuantity&);
    if (jsStack::stack()->GetConversionGraph()) {
        ForAllUsedDimensions(RegisterjsUnitValueOperators());
    }
}


jsUnitValue::operator const DNVS::MoFa::Units::Runtime::DynamicQuantity&() const
{
    return m_quantity;
}

jsUnitValue::operator std::string() const
{
    if (_isnan(m_quantity.GetValue())) return "NaN";
    if (!_finite(m_quantity.GetValue())) {
        if (m_quantity.GetValue() > 0) return "+Infinity";
        else return "-Infinity";
    }
    std::stringstream out;
    out.precision(10);
    out << m_quantity.GetString();
    return out.str();
}

jsValue* jsUnitValue::operator-()
{
    return new jsUnitValue(-m_quantity);
}

jsValue* jsUnitValue::operator+(jsValue* op2)
{
    try {
        DynamicQuantity otherQuantity = fromJScript(op2, jsType<DynamicQuantity>());
        return new jsUnitValue(m_quantity + otherQuantity);
    }
    catch (...){}
    return jsValue::operator+(op2);
}

bool jsUnitValue::getCompatibleNumber(jsValue* op2, double& number)
{
    try {
        DynamicQuantity otherQuantity = fromJScript(op2, jsType<DynamicQuantity>());
        DynamicQuantity thisQuantity = *this;
        otherQuantity.ChangeUnit(thisQuantity.GetSimplifiedUnit());
        number = otherQuantity.GetValue();
        return true;
    }
    catch(...)
    {
        return false;
    }
}

jsValue* jsUnitValue::operator==(jsValue* op2)
{
    double compatibleNumber = 0;
    if (!getCompatibleNumber(op2, compatibleNumber)) return jsScriptable::operator==(op2);
    bool result = jsQuantity::compareTolerant(m_quantity.GetValue(), compatibleNumber) == 0;
    return toJScript(result);
}

jsValue* jsUnitValue::operator!=(jsValue* op2)
{
    double compatibleNumber = 0;
    if (!getCompatibleNumber(op2, compatibleNumber)) return jsScriptable::operator==(op2);
    bool result = jsQuantity::compareTolerant(m_quantity.GetValue(), compatibleNumber) != 0;
    return toJScript(result);
}

jsValue* jsUnitValue::operator<=(jsValue* op2)
{
    double compatibleNumber = 0;
    if (!getCompatibleNumber(op2, compatibleNumber)) return jsScriptable::operator==(op2);
    bool result = jsQuantity::compareTolerant(m_quantity.GetValue(), compatibleNumber) <= 0;
    return toJScript(result);
}

jsValue* jsUnitValue::operator>=(jsValue* op2)
{
    double compatibleNumber = 0;
    if (!getCompatibleNumber(op2, compatibleNumber)) return jsScriptable::operator==(op2);
    bool result = jsQuantity::compareTolerant(m_quantity.GetValue(), compatibleNumber) >= 0;
    return toJScript(result);
}

jsValue* jsUnitValue::operator<(jsValue* op2) {
    double compatibleNumber = 0;
    if (!getCompatibleNumber(op2, compatibleNumber)) return jsScriptable::operator==(op2);
    bool result = jsQuantity::compareTolerant(m_quantity.GetValue(), compatibleNumber) < 0;
    return toJScript(result);
}

jsValue* jsUnitValue::operator>(jsValue* op2)
{
    double compatibleNumber = 0;
    if (!getCompatibleNumber(op2, compatibleNumber)) return jsScriptable::operator==(op2);
    bool result = jsQuantity::compareTolerant(m_quantity.GetValue(), compatibleNumber) > 0;
    return toJScript(result);
}

jsValue* jsUnitValue::operator-(jsValue* op2)
{
    try {
        DynamicQuantity otherQuantity = fromJScript(op2, jsType<DynamicQuantity>());
        DynamicQuantity thisQuantity = *this;
        return new jsUnitValue(thisQuantity - otherQuantity);
    }
    catch (...) {}
    return jsValue::operator-(op2);
}

jsValue* jsUnitValue::operator*(jsValue* op2)
{
    try {
        DynamicQuantity otherQuantity = fromJScript(op2, jsType<DynamicQuantity>());
        DynamicQuantity thisQuantity = *this;
        DynamicQuantity result = thisQuantity * otherQuantity;
        if (result.RemoveUnitIfDimensionLess())
            return toJScript(result.GetValue());
        else
            return new jsUnitValue(result);
    }
    catch(...)
    { }
    try {
        double value = fromJScript(op2, jsType<double>());
        DynamicQuantity thisQuantity = *this;
        return new jsUnitValue(thisQuantity * value);
    }
    catch (...) {}
    return jsValue::operator*(op2);
}

jsValue* jsUnitValue::InverseDivide(jsValue* op2)
{
    try {
        double value = fromJScript(op2, jsType<double>());
        DynamicQuantity thisQuantity = *this;
        return new jsUnitValue(value / thisQuantity);
    }
    catch (...) {}
    return jsValue::InverseDivide(op2);
}

jsValue* jsUnitValue::operator/(jsValue* op2)
{
    try {
        DynamicQuantity otherQuantity = fromJScript(op2, jsType<DynamicQuantity>());
        DynamicQuantity thisQuantity = *this;
        DynamicQuantity result = thisQuantity / otherQuantity;
        if (result.RemoveUnitIfDimensionLess())
            return toJScript(result.GetValue());
        else
            return new jsUnitValue(result);
    }
    catch (...) {}
    try {
        double value = fromJScript(op2, jsType<double>());
        DynamicQuantity thisQuantity = *this;
        return new jsUnitValue(thisQuantity / value);
    }
    catch (...) {}
    return jsValue::operator/(op2);
}

double jsUnitValue::toDouble() const
{
    return m_quantity.GetValue();
}

bool jsUnitValue::createUniqueQuantity(jsQuantity& quantity) const
{
    auto phenomenons = DynamicPhenomenon::GetCompatiblePhenomenons(m_quantity.GetSimplifiedUnit().GetDimension());
    if (phenomenons.size() == 1)
    {
        quantity = jsQuantity(phenomenons.front(), m_quantity);
        return true;
    }
    return false;
}

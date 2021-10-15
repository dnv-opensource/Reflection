//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsScript/jsQuantity.h>
#include <jsScript/jsUnitValue.h>
#include <jsScript/jsUnit.h>
#include <jsScript/jsClass.h>

#include <sstream>
#include <limits>
#include "Units/Runtime/DynamicQuantity.h"
#include "Units/Runtime/UnitParser.h"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include "Reflection/Classes/Class.h"
#include "Units/Formatting/IUnitFormatterService.h"
#include "Units/Formatting/DimensionFormatter.h"
#include "Services/ServiceProvider.h"

jsQuantity::formatFlags jsQuantity::m_string_operator_format = includeUnitString;
using namespace DNVS::MoFa::Units::Runtime;
using namespace DNVS::MoFa::Units;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

jsBoundDouble jsBoundDouble::operator-()
{
    return jsBoundDouble(-m_value, m_inclusive);
}

jsBoundDouble& jsBoundDouble::operator+()
{
    return *this;
}

jsBoundDouble jsBoundDouble::infinity()
{
    return jsBoundDouble(std::numeric_limits<double>::infinity());
}

std::mutex jsQuantity::m_mutex;

std::mutex& jsQuantity::GetMutex()
{
    return m_mutex;
}

jsQuantity::jsQuantity(const jsQuantity& quantity)
    : m_quantity(quantity.m_quantity)
    , m_phenomenon(quantity.m_phenomenon)
{
}

jsQuantity::jsQuantity(const DNVS::MoFa::Units::Runtime::DynamicPhenomenon& phenomenon, double value, const std::string& unitName)
    : m_phenomenon(phenomenon)
    , m_quantity(value, unitName)
{
    ThrowIfQuantityAndPhenomenonAreIncompatible();
}

jsQuantity::jsQuantity(const DNVS::MoFa::Units::Runtime::DynamicPhenomenon& phenomenon)
    : m_phenomenon(phenomenon)
{
}

jsQuantity::jsQuantity(const DNVS::MoFa::Units::Runtime::DynamicPhenomenon& phenomenon, const std::string& quantity)
    : m_phenomenon(phenomenon)
{
    std::lock_guard<std::mutex> guard(GetMutex());
    ///We are using boost::spirit for parsing the quantity here for performance reasons. It is faster than std::stringstream.
    ///It also handles NaN and Infinity correctly.
    using boost::spirit::qi::double_;
    using boost::spirit::qi::char_;
    using boost::spirit::qi::phrase_parse;
    using boost::spirit::qi::_1;
    using boost::spirit::qi::space;

    double value;
    std::vector<char> unitVect;
    if (phrase_parse(quantity.begin(), quantity.end(), double_[boost::phoenix::ref(value) = _1] >> (*char_)[boost::phoenix::ref(unitVect) = _1], space))
    {
        if (unitVect.empty())
            m_quantity = DynamicQuantity(value, GetInputUnitSystem().GetUnit(m_phenomenon));
        else
        {
            std::string unit(&unitVect.front(), unitVect.size());
            m_quantity = DynamicQuantity(value, unit);
            ThrowIfQuantityAndPhenomenonAreIncompatible();
        }
    }
    else
        throw bad_type_conversion(quantity, phenomenon.GetName());
}

jsQuantity::jsQuantity(const DynamicPhenomenon& phenomenon, const DynamicQuantity& quantity)
    : m_phenomenon(phenomenon)
    , m_quantity(quantity)
{
    m_quantity.TryChangeUnit(GetDatabaseUnitSystem().GetUnit(phenomenon));
}

jsQuantity& jsQuantity::operator=(const jsQuantity& quantity)
{
    m_phenomenon = quantity.m_phenomenon;
    m_quantity = quantity.m_quantity;
    return *this;
}

jsQuantity::jsQuantity(double value)
    : m_quantity(value)
{
}

jsQuantity::jsQuantity(const DNVS::MoFa::Units::Runtime::DynamicPhenomenon& phenomenon, double value, bool databaseUnits)
    : m_phenomenon(phenomenon)
{    
    if (databaseUnits)
    {
        m_quantity = DynamicQuantity(value, SimplifiedUnit(1.0, phenomenon.GetDimension()));
    }
    else
    {
        UnitSystem unitSystem = GetInputUnitSystem();
        Unit unit = unitSystem.GetUnit(m_phenomenon);
        m_quantity = DynamicQuantity(value, unit);
    }
}

jsQuantity::jsQuantity(double value, const std::string& unit)
    : m_quantity(value, unit)
{
    auto phenomenons = DynamicPhenomenon::GetCompatiblePhenomenons(m_quantity.GetSimplifiedUnit().GetDimension());
    if (phenomenons.size() == 1)
        m_phenomenon = phenomenons.front();
    else
        throw std::runtime_error("Unknown phenomenon for " + unit);
}

jsQuantity::jsQuantity(const DNVS::MoFa::Units::Runtime::DynamicPhenomenon& phenomenon, const jsUnitValue& value)
    : m_phenomenon(phenomenon)
    , m_quantity(value)
{
    ThrowIfQuantityAndPhenomenonAreIncompatible();
}

jsQuantity::~jsQuantity()
{

}

void jsQuantity::init(jsTypeLibrary& typeLibrary)
{
    jsTClass<jsQuantity> cls(typeLibrary, "Quantity");
    if (cls.reinit()) return;
    cls.Function("toString", &jsQuantity::operator std::string);
    cls.Function("signature", &jsQuantity::signature);
    cls.Function("toDouble", &jsQuantity::toDouble);
    cls.ImplicitConversion(&jsQuantity::operator std::string);
    cls.ImplicitConversion(&jsQuantity::operator const DynamicQuantity&);

    using namespace DNVS::MoFa::Reflection::Classes;
    Class<jsQuantity> cls2(jsStack::stack()->GetTypeLibrary(), "");
    cls2.Function("toString", &jsQuantity::operator std::string);
}

jsQuantity::operator const DynamicQuantity&() const
{
    if (!m_quantity.GetSimplifiedUnit().HasUnitName())
    {
        Unit unit = GetDatabaseUnit();
        const_cast<jsQuantity*>(this)->m_quantity = DynamicQuantity(m_quantity.GetNeutralValue(), unit);
    }
    return m_quantity;
}

std::string jsQuantity::signature() const
{
    return Formatting::DimensionFormatter().FormatAsHtml(m_phenomenon.GetDimension());
}

jsQuantity::operator double() const
{
    return m_quantity.GetNeutralValue();
}

Unit jsQuantity::GetInputUnit() const
{
    return GetInputUnitSystem().GetUnit(m_phenomenon);
}

Unit jsQuantity::GetDatabaseUnit() const
{
    return GetDatabaseUnitSystem().GetUnit(m_phenomenon);
}

jsQuantity::operator std::string() const
{
    return format(10, std::ios_base::floatfield, m_string_operator_format);
}

const DynamicQuantity& jsQuantity::GetQuantity() const
{
    return m_quantity;
}

const DynamicPhenomenon& jsQuantity::GetPhenomenon() const
{
    return m_phenomenon;
}

jsValue* jsQuantity::operator-()
{
    return new jsUnitValue(-m_quantity);
}

jsValue* jsQuantity::operator==(jsValue* op2)
{
    double compatibleNumber = 0;
    if (!getCompatibleNumber(op2, compatibleNumber)) return jsScriptable::operator==(op2);
    if (_isnan(*this) || _isnan(compatibleNumber)) return toJScript(false);
    bool result = jsQuantity::compareTolerant(*this, compatibleNumber) == 0;
    return toJScript(result);
}

jsValue* jsQuantity::operator!=(jsValue* op2)
{
    double compatibleNumber = 0;
    if (!getCompatibleNumber(op2, compatibleNumber)) return jsScriptable::operator==(op2);
    if (_isnan(*this) || _isnan(compatibleNumber)) return toJScript(true);
    bool result = jsQuantity::compareTolerant(*this, compatibleNumber) != 0;
    return toJScript(result);
}

jsValue* jsQuantity::operator<=(jsValue* op2)
{
    double compatibleNumber = 0;
    if (!getCompatibleNumber(op2, compatibleNumber)) return jsScriptable::operator==(op2);
    bool result = jsQuantity::compareTolerant(*this, compatibleNumber) <= 0;
    return toJScript(result);
}

jsValue* jsQuantity::operator>=(jsValue* op2)
{
    double compatibleNumber = 0;
    if (!getCompatibleNumber(op2, compatibleNumber)) return jsScriptable::operator==(op2);
    bool result = jsQuantity::compareTolerant(*this, compatibleNumber) >= 0;
    return toJScript(result);
}

jsValue* jsQuantity::operator<(jsValue* op2) {
    double compatibleNumber = 0;
    if (!getCompatibleNumber(op2, compatibleNumber)) return jsScriptable::operator==(op2);
    bool result = jsQuantity::compareTolerant(*this, compatibleNumber) < 0;
    return toJScript(result);
}

jsValue* jsQuantity::operator>(jsValue* op2)
{
    double compatibleNumber = 0;
    if (!getCompatibleNumber(op2, compatibleNumber)) return jsScriptable::operator==(op2);
    bool result = jsQuantity::compareTolerant(*this, compatibleNumber) > 0;
    return toJScript(result);
}

bool jsQuantity::getCompatibleNumber(jsValue* op2, double& number)
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

void jsQuantity::ThrowIfQuantityAndPhenomenonAreIncompatible()
{
    if (!m_quantity.IsCompatible(m_phenomenon))
        throw std::runtime_error("Invalid unit: '" + m_quantity.GetSimplifiedUnit().GetUnitName() + "' is not a valid " + m_phenomenon.GetName() + " unit");
}

jsValue* jsQuantity::operator+(jsValue* op2)
{
    try {
        DynamicQuantity otherQuantity = fromJScript(op2, jsType<DynamicQuantity>());
        DynamicQuantity thisQuantity = *this;
        return new jsUnitValue(thisQuantity + otherQuantity);
    }
    catch(...) {}
    return jsValue::operator+(op2);
}

jsValue* jsQuantity::operator-(jsValue* op2)
{
    try {
        DynamicQuantity otherQuantity = fromJScript(op2, jsType<DynamicQuantity>());
        DynamicQuantity thisQuantity = *this;
        return new jsUnitValue(thisQuantity - otherQuantity);
    }
    catch (...) {}
    return jsValue::operator-(op2);
}

jsValue* jsQuantity::operator*(jsValue* op2)
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
    catch (...) {}
    try {
        double value = fromJScript(op2, jsType<double>());
        DynamicQuantity thisQuantity = *this;
        return new jsUnitValue(thisQuantity * value);
    }
    catch (...) {}
    return jsValue::operator*(op2);
}


jsValue* jsQuantity::InverseDivide(jsValue* op2)
{
    try {
        double value = fromJScript(op2, jsType<double>());
        DynamicQuantity thisQuantity = *this;
        return new jsUnitValue(value / thisQuantity);
    }
    catch (...) {}
    return jsValue::InverseDivide(op2);
}

jsValue* jsQuantity::operator/(jsValue* op2)
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


std::string jsQuantity::format(formatFlags format, unitType unitTypeFlag) const
{
    int precision = 10;
    std::ios::fmtflags floatFlag = std::ios_base::floatfield;
    auto service = DNVS::MoFa::Services::ServiceProvider::Instance().TryGetService<Formatting::IUnitFormatterService>();
    if (service)
    {
        precision = service->GetPrecision(m_phenomenon);
        floatFlag = service->GetFloatFlags(m_phenomenon);
    }
    return this->format(precision, floatFlag, format, unitTypeFlag);
}

std::string FormatUsingStream(std::ios_base::fmtflags flags, size_t precision, double number)
{
    std::stringstream stream;
    stream.precision(precision);
    if ((flags&std::ios_base::floatfield) == std::ios_base::floatfield)
        flags = (flags)&~std::ios_base::floatfield;
    stream.setf(flags);
    stream << number;
    return stream.str();
}

std::string FormatUsingSprintF(std::ios_base::fmtflags flags, size_t precision, double number)
{
    std::string format = "%.*";
    switch (flags&std::ios_base::floatfield)
    {
    case std::ios_base::floatfield:
        format += "g";
        break;
    case std::ios_base::fixed:
        format += "f";
        break;
    case std::ios_base::scientific:
        format += "e";
        break;
    }
    char numberAsString[200];
    sprintf_s(numberAsString, 200, format.c_str(), int(precision), number);
    return numberAsString;
}

std::string jsQuantity::format(int precision, std::ios_base::fmtflags flags, formatFlags format, unitType unitTypeFlag) const
{
    double number = value(unitTypeFlag);
    if (_isnan(number)) return "NaN";
    if (!_finite(number)) {
        if (number > 0) return "+Infinity";
        else return "-Infinity";
    }
    if (fabs(m_quantity.GetNeutralValue()) < 1e-15)
        number = 0;
    std::string formattedNumber = FormatUsingSprintF(flags, precision, number);
    if (format == includeUnitString) {
        formattedNumber += " " + unitString(unitTypeFlag);
    }
    return formattedNumber;
}

double jsQuantity::value(unitType unitTypeFlag) const
{
    if (unitTypeFlag == databaseUnit) return m_quantity.GetNeutralValue();
    else return GetInputUnitSystem().Convert(m_phenomenon, m_quantity);
}

std::string jsQuantity::unitString(unitType unitTypeFlag) const
{
    if (unitTypeFlag == databaseUnit) {
        return GetDatabaseUnitSystem().GetUnit(m_phenomenon).GetUnitName();
    }
    else {
        return GetInputUnitSystem().GetUnit(m_phenomenon).GetUnitName();
    }
}

DynamicDimension jsQuantity::GetDimension() const
{
    return m_phenomenon.GetDimension();
}

double jsQuantity::toDouble() const
{
    return value();
}

void jsQuantity::checkRange(const jsBoundDouble& lower_bound, const jsBoundDouble& upper_bound)
{
    if ((lower_bound.inclusive() && double(*this) < double(lower_bound)) ||
        (double(*this) <= double(lower_bound)) ||
        (upper_bound.inclusive() && double(*this) > double(upper_bound)) ||
        (double(*this) >= double(upper_bound)))
    {
        std::string error = m_phenomenon.GetName() + " value: " + format() + " out of range ";
        if (lower_bound.inclusive()) error += "[";
        else error += "<";
        error += mofa::lexical_cast<std::string>(double(lower_bound));
        error += ",";
        error += mofa::lexical_cast<std::string>(double(upper_bound));
        if (upper_bound.inclusive()) error += "]";
        else error += ">";
        throw std::range_error(error.c_str());
    }
}

double jsQuantity::convert(const std::string& unitName) const
{
    DynamicQuantity q(m_quantity);
    q.ChangeUnit(unitName);
    return q.GetValue();
}

jsQuantity::formatFlags jsQuantity::stringOperatorFormat(formatFlags operator_format)
{
    // Return value is previous operator format
    formatFlags result = m_string_operator_format;
    m_string_operator_format = operator_format;
    return result;
}

bool jsQuantity::compatible(const jsQuantity& quantity)
{
    if (!m_phenomenon.IsValid()) return false;
    if (!quantity.m_phenomenon.IsValid()) return false;
    return m_quantity.IsCompatible(quantity.m_quantity);
}

int jsQuantity::compareTolerant(const double& a1, const double& a2)
{
    if (a1 == 0.0 && a2 == 0.0) return 0;

    if (fabs(a1) == std::numeric_limits<double>::infinity() || fabs(a2) == std::numeric_limits<double>::infinity()) {
        if (a1 == a2) {
            return 0;
        }
        else if (a1 == -std::numeric_limits<double>::infinity() || a2 == std::numeric_limits<double>::infinity()) {
            return -1;
        }
        else {
            return 1;
        }
    }

    double maximum = (std::max)(fabs(a1), fabs(a2));
    double normalised_a1 = a1 / maximum;
    double normalised_a2 = a2 / maximum;

    if (fabs(normalised_a1 - normalised_a2) < 1e-10) return 0;
    else if (normalised_a1 < normalised_a2) return -1;
    else return 1;
}

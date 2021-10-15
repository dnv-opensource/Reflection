//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef AFX_JSCONVERTINVALIDUNIT_H
#define AFX_JSCONVERTINVALIDUNIT_H

#include <jsScript/jsValue.h>
#include <jsScript/jsUnitValue.h>
#include <jsScript/jsConversions.h>

template<typename T>
class jsUnwrap {
public:
    jsUnwrap(jsValue* value) {
        try {
            m_value = new T(fromJScript(value, jsType<T>()));
            m_success = true;
        }
        catch (...) {
            m_success = false;
        }
    }
    ~jsUnwrap() {
        if (m_success) {
            delete m_value;
        }
    }
    bool ok() const { return m_success; }
    T& operator()() { return *m_value; }
private:
    bool m_success;
    T* m_value;
};

template<typename OldUnit, typename NewUnit>
NewUnit jsConvertInvalidUnit(jsValue* value, std::string& warning)
{
    jsUnwrap<NewUnit> newUnit(value);
    if (newUnit.ok()) return newUnit();
    jsUnwrap<OldUnit> oldUnit(value);
    if (oldUnit.ok()) {
        jsUnwrap<jsUnitValue> unitValue(value);
        if (unitValue.ok()) {
            double inputValue = unitValue().GetQuantity().GetValue();
            double inputFactor = unitValue().GetQuantity().GetSimplifiedUnit().GetConversionFactor();
            double unitFactor = NewUnit(0.).GetInputUnit().GetConversionFactor();
            if (inputFactor == unitFactor || inputValue == 0.0) {
                //We can safely convert the number to the new unit.
                warning = std::string("Converting from old invalid unit ")
                    + OldUnit::GetPhenomenon().GetName() + " to " + NewUnit::GetPhenomenon().GetName()
                    + std::string(" may cause unwanted side-effects. Please check input file");
                return NewUnit(inputValue, NewUnit::unitString());
            }
            else
            {
                double scaledUnitValue = inputValue*inputFactor / unitFactor;
                //We can unsafely convert the number to the new unit.
                warning = std::string("Converting from old invalid unit ")
                    + OldUnit::GetPhenomenon().GetName() + " to " + NewUnit::GetPhenomenon().GetName()
                    + std::string(" will most likely cause unwanted side-effects. Please check input file");
                return NewUnit(scaledUnitValue, NewUnit::unitString());
            }
        }
        else throw std::runtime_error("Unable to convert from " + OldUnit::GetPhenomenon().GetName() + " to " +
            NewUnit::GetPhenomenon().GetName() + ".");
    }
    else return fromJScript(value, jsType<NewUnit>());
}


template<typename OldUnit>
double jsConvertInvalidUnitToDouble(jsValue* value, std::string& warning)
{
    jsUnwrap<double> newUnit(value);
    if (newUnit.ok()) return newUnit();
    jsUnwrap<OldUnit> oldUnit(value);
    if (oldUnit.ok()) {
        jsUnwrap<jsUnitValue> unitValue(value);
        if (unitValue.ok()) {
            double inputFactor = unitValue().GetQuantity().GetSimplifiedUnit().GetConversionFactor();
            double unitFactor = 1.0;
            if (inputFactor == unitFactor || unitValue().GetQuantity().GetValue() == 0.0) {
                //We can safely convert the number to the new unit.
                warning = std::string("Converting from old invalid unit ")
                    + OldUnit::GetPhenomenon().GetName() + " to double"
                    + " may cause unwanted side-effects. Please check input file";
                return unitValue().value();
            }
            else throw std::runtime_error("Unable to convert from " + OldUnit::GetPhenomenon().GetName() +
                " to double. Inconsistent input units used.");
        }
        else throw std::runtime_error("Unable to convert from " + OldUnit::GetPhenomenon().GetName() + " to double.");
    }
    else return fromJScript(value, jsType<double>());
}


#endif

#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "jsMath.h"
#include "Reflection/Classes/Class.h"
#include "Reflection/Attributes/ExampleAttribute.h"

template<typename T>
void InitFunctionsForDouble(DNVS::MoFa::Reflection::Classes::Class<jsMath>& cls)
{
    using namespace DNVS::MoFa::Reflection::Classes;
    cls.StaticFunction("acos", [](const T& x) {return acos(x); })
        .AddDocumentation(R"(Returns the arc cosine of x.
The result is expressed in radians and ranges from +0 to +&#960;)")
.AddSignature("x")
.AddAttribute<ExampleAttribute>(
    "Math.acos(1); //Returns 0\n"
    "Math.acos(0.3); //Returns 1.2661...\n");

    cls.StaticFunction("asin", [](const T& x) { return asin(x); })
        .AddDocumentation(R"(Returns the arc sine of x
The result is expressed in radians and ranges from -&#960;/2 to +&#960;/2)")
.AddSignature("x")
.AddAttribute<ExampleAttribute>(
    "Math.asin(1); //Returns 1.570796... = Math.PI/2\n"
    "Math.asin(0.3); //Returns 0.3046926...\n");

    cls.StaticFunction("atan", [](const T& x) {return atan(x); })
        .AddDocumentation(R"(Returns the arc tangent of x
The result is expressed in radians and ranges from -&#960;/2 to +&#960;/2)")
.AddSignature("x")
.AddAttribute<ExampleAttribute>(
    "Math.atan(1); //Returns 0.785398... = Math.PI/4\n"
    "Math.atan(0.3); //Returns 0.2914567...\n");

    cls.StaticFunction("atan2", [](const T& y, const T& x) {return atan2(y, x); })
        .AddDocumentation(R"(Returns the arc tangent of the quotient y/x of the arguments y and x)
The signs of y and x are used to determine the quadrant of the result.
The result is expressed in radians and ranges from -&#960; to +&#960;)")
.AddSignature("y", "x")
.AddAttribute<ExampleAttribute>("Math.atan(2,3); //Approximately the same as Math.atan(2/3)\n");

    cls.StaticFunction("ceil", [](const T& x) {return ceil(x); })
        .AddDocumentation(R"(Returns the smallest (closest to -&#8734;) number value that is not less than x and is equal to a mathematical integer
If x is already an integer, the result is x)")
.AddSignature("x")
.AddAttribute<ExampleAttribute>(
    "Math.ceil(5); //Returns 5\n"
    "Math.ceil(4.99); //Returns 5\n"
    "Math.ceil(4.01); //Returns 5\n");

    cls.StaticFunction("exp", [](const T& arg) {return exp(arg); })
        .AddDocumentation(R"(Returns the exponential function of x
(e raised to the power of x, where e is the base of the natural logarithms)")
.AddSignature("x")
.AddAttribute<ExampleAttribute>("Math.exp(0); //Returns 1\n"
    "Math.exp(5); //Returns 148.413159...\n"
    "Math.exp(-5); //Returns 0.0067379...\n");

    cls.StaticFunction("floor", [](const T& arg) {return floor(arg); })
        .AddDocumentation(R"(Returns the greatest (closest to +&#8734;) number value that is not greater than x and is equal to a mathematical integer
If x is already an integer, the result is x)")
.AddSignature("x")
.AddAttribute<ExampleAttribute>(
    "Math.floor(5); //Returns 5\n"
    "Math.floor(4.99); //Returns 4\n"
    "Math.floor(4.01); //Returns 4\n");

    cls.StaticFunction("log", [](const T& arg) {return log(arg); })
        .AddDocumentation("Returns an implementation-dependent approximation to the natural logarithm of x")
        .AddSignature("x")
        .AddAttribute<ExampleAttribute>(
            "Math.exp(1); //Returns 0\n"
            "Math.exp(148.413159); //Returns 5\n"
            "Math.exp(0.0067379); //Returns -5\n");

    cls.StaticFunction("max", [](const std::vector<T>& args)
    {
        T max_value = -std::numeric_limits<double>::infinity();
        for (const T& arg : args)
            max_value = (max)(arg, max_value);
        return max_value;
    }, Vararg)
        .AddDocumentation(R"(Given zero or more arguments, returns the largest of the resulting values.
If no arguments are given, the result is -&#8734;)")
.AddAttribute<ExampleAttribute>(
    "Math.max(); //Returns -&#8734;\n"
    "Math.max(5,8,100,-1000); //Returns 100\n"
    "Math.max(-1); //Returns -1\n");

    cls.StaticFunction("min", [](const std::vector<T>& args)
    {
        T min_value = std::numeric_limits<double>::infinity();
        for (const T& arg : args)
            min_value = (min)(arg, min_value);
        return min_value;
    }, Vararg)
        .AddDocumentation(R"(Given zero or more arguments, returns the smallest of the resulting values.
If no arguments are given, the result is +&#8734;)")
.AddAttribute<ExampleAttribute>(
    "Math.min(); //Returns &#8734;\n"
    "Math.min(5,8,100,-1000); //Returns -1000\n"
    "Math.min(-1); //Returns -1\n");


    cls.StaticFunction("pow", [](const T& x, const T& y) { return pow(x, y); })
        .AddDocumentation("Returns the result of raising x to the power y")
        .AddSignature("x", "y")
        .AddAttribute<ExampleAttribute>(
            "Math.pow(10,2); //Returns 100\n"
            "Math.pow(5.6,7.8); //Returns 685277.7177...\n");

    cls.StaticFunction("round", [](const T& x) {return round(x); })
        .AddDocumentation(R"(Returns the number value that is closest to x and is equal to a mathematical integer.
If two integer number values are equally close to x, then the result is the number value that is closer to +&#8734;
If x is already an integer, the result is x.)")
.AddSignature("x")
.AddAttribute<ExampleAttribute>(
    "Math.round(5); //Returns 5\n"
    "Math.round(4.99); //Returns 5\n"
    "Math.round(4.01); //Returns 4\n");

    //Hyperbolic functions
    cls.StaticFunction("cosh", [](const T& x) {return cosh(x); })
        .AddDocumentation("returns the hyperbolic cosine of x")
        .AddSignature("x")
        .AddAttribute<ExampleAttribute>(
            "Math.cosh(0); //Returns 1\n"
            "Math.cosh(5); //Returns 74.20995...\n");

    cls.StaticFunction("sinh", [](const T& x) {return sinh(x); })
        .AddDocumentation("returns the hyperbolic sine of x")
        .AddSignature("x")
        .AddAttribute<ExampleAttribute>(
            "Math.sinh(0); //Returns 0\n"
            "Math.sinh(5); //Returns 74,20321...\n");

    cls.StaticFunction("tanh", [](const T& x) {return tanh(x); })
        .AddDocumentation("returns the hyperbolic tangent of x")
        .AddSignature((Arg("x")))
        .AddAttribute<ExampleAttribute>(
            "Math.tanh(0); //Returns 0\n"
            "Math.tanh(5); //Returns 0,999909... = Math.sinh(5)/Math.cosh(5)\n");

    cls.StaticFunction("abs", [](const T& arg) {return abs(arg); })
        .AddSignature("x")
        .AddDocumentation(R"(Returns the absolute value of x
the result has the same magnitude as x but has positive sign)")
.AddAttribute<ExampleAttribute>(
    "Math.abs(-5); //Returns 5\n"
    "Math.abs(8.5); //Returns 8.5\n");

    cls.StaticFunction("cos", [](const T& arg) {return cos(arg); })
        .AddDocumentation("Returns the cosine of x")
        .AddSignature("x")
        .AddAttribute<ExampleAttribute>(
            "Math.cos(0); //Returns 1\n"
            "Math.cos(1.2661); //Returns 0.3\n");

    cls.StaticFunction("sin", [](const T& arg) {return sin(arg); })
        .AddDocumentation("Returns the sine of x")
        .AddSignature("x")
        .AddAttribute<ExampleAttribute>(
            "Math.sin(Math.PI/2); //Returns 1\n"
            "Math.sin(0.3046926); //Returns 0.3\n");

    cls.StaticFunction("tan", [](const T& arg) {return tan(arg); })
        .AddDocumentation("Returns the tangent of x")
        .AddSignature("x")
        .AddAttribute<ExampleAttribute>(
            "Math.tan(Math.PI/4); //Returns 1\n"
            "Math.tan(0.2914567); //Returns 0.3\n");

    cls.StaticFunction("sqrt", [](const T& arg) {return sqrt(arg); })
        .AddDocumentation("Returns the square root of x")
        .AddSignature("x")
        .AddAttribute<ExampleAttribute>(
            "Math.sqrt(2); //Returns 1,414213... = Math.SQRT2\n"
            "Math.sqrt(9); //Returns 3\n");
}

template<typename T>
void InitFunctionsForUnits(DNVS::MoFa::Reflection::Classes::Class<jsMath>& cls)
{
    using namespace DNVS::MoFa::Reflection::Classes;
    cls.StaticFunction("max", [](const T& firstArg, const std::vector<T>& args)
    {
        T max_value = firstArg;
        for (const T& arg : args)
            max_value = (max)(arg, max_value);
        return max_value;
    }, Vararg)
        .AddDocumentation("Given one or more arguments, returns the largest of the resulting values.");

    cls.StaticFunction("min", [](const T& firstArg, const std::vector<T>& args)
    {
        T min_value = firstArg;
        for (const T& arg : args)
            min_value = (min)(arg, min_value);
        return min_value;
    }, Vararg)
        .AddDocumentation("Given one or more arguments, returns the smallest of the resulting values.");

    cls.StaticFunction("abs", [](const T& arg) {return abs(arg); })
        .AddSignature("x")
        .AddDocumentation(R"(Returns the absolute value of x
the result has the same magnitude as x but has positive sign)");

    cls.StaticFunction("cos", [](const T& arg) {return cos(arg); })
        .AddDocumentation("Returns the cosine of x")
        .AddSignature("x");

    cls.StaticFunction("sin", [](const T& arg) {return sin(arg); })
        .AddDocumentation("Returns the sine of x")
        .AddSignature("x");

    cls.StaticFunction("tan", [](const T& arg) {return tan(arg); })
        .AddDocumentation("Returns the tangent of x")
        .AddSignature("x");

    cls.StaticFunction("sqrt", [](const T& arg) {return sqrt(arg); })
        .AddDocumentation("Returns the square root of x")
        .AddSignature("x");
}

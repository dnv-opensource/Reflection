//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsScript/jsMath.h>
#include <jsScript/jsClass.h>
#include <jsScript/jsUnitValue.h>
#include <jsScript/jsQuantity.h>
#include <limits>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include "Units/Angle.h"
#include "Units/Runtime/DynamicQuantity.h"
#include "Units/Math.h"
#include "Reflection/Classes/Class.h"
#include "Reflection/Attributes/ExampleAttribute.h"
#include <Reflection/Containers/ReflectVector.h>
#include "Reflection/Members/GlobalType.h"
#include "jsMathImpl.h"

/*#include <boost/math/special_functions/acosh.hpp>
#include <boost/math/special_functions/asinh.hpp>
#include <boost/math/special_functions/atanh.hpp>*/
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using namespace DNVS::MoFa::Units;
using namespace DNVS::MoFa::Units::Runtime;
using namespace DNVS::MoFa::Reflection::Classes;
using namespace std;

void jsMath::init(jsTypeLibrary& typeLibrary)
{
    srand((unsigned)time(NULL));

    using namespace DNVS::MoFa::Reflection::Classes;
    Class<jsMath> cls(typeLibrary.GetReflectionTypeLibrary(), "Math");
    cls.AddDocumentation("The Math object - methods");

    cls.StaticFunction("timer", [](long t0) { return long(time(nullptr) - t0); })
        .AddDocumentation("for performance test, returns the lapsed time in seconds since t0")
        .AddSignature("t0")
        .AddAttribute<ExampleAttribute>(
            "Math.timer(0); //Returns time in seconds since 1. January 1970\n"
            "t0 = Math.timer(0);  //Initialises the variable a\n"
            "t  = Math.timer(t0); //Computes time difference in seconds since when variable t0 was set\n"
            "print(t);            //Writes lapsed time to the journal file window\n");

    cls.StaticGet("E", 2.7182818284590452354)
        .AddDocumentation("The number value for e, the base of the natural logarithms, which is approximately 2.7182818284590452354")
        .AddAttribute<ExampleAttribute>(
            "Math.ln(E); //Returns 1"
            "Math.pow(E,5); //The same as Math.exp(5)");
    cls.StaticGet("LN10", 2.302585092994046).AddDocumentation("The number value for the natural logarithm of 10, which is approximately 2.302585092994046");
    cls.StaticGet("LN2", 0.6931471805599453).AddDocumentation("The number value for the natural logarithm of 2, which is approximately 0.6931471805599453");
    cls.StaticGet("LOG2E", 1.4426950408889634).AddDocumentation("The number value for the base-2 logarithm of e, the base of the natural logarithms; this value is approximately 1.4426950408889634");
    cls.StaticGet("LOG10E", 0.4342944819032518).AddDocumentation("The number value for the base-10 logarithm of e, the base of the natural logarithms; this value is approximately 0.4342944819032518");
    cls.StaticGet("PI", 3.1415926535897932).AddDocumentation("The number value for &#960;, the ratio of the circumference of a circle to its diameter, which is approximately 3.1415926535897932");
    cls.StaticGet("SQRT1_2", 0.7071067811865476).AddDocumentation("The number value for the square root of 1/2, which is approximately 0.7071067811865476");
    cls.StaticGet("SQRT2", 1.4142135623730951).AddDocumentation("The number value for the square root of 2, which is approximately 1.4142135623730951");

    Class<DNVS::MoFa::Reflection::Members::GlobalType>(typeLibrary.GetReflectionTypeLibrary(), "")
        .StaticGet("Math", jsMath());
    InitFunctionsForDouble<double>(cls);
    InitFunctionsForUnits<DynamicQuantity>(cls);

}


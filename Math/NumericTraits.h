#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "CompareAlmostEqual.h"

namespace DNVS { namespace MoFa { namespace Math {
    template<typename T>
    struct NumericTraits {
        static T GetPriorValue(T value) {return value-1;}
        static T GetNextValue(T value) {return value+1;}
        static T GetAlmostSameValue(T value) {return value;}
    };

    template<>
    struct NumericTraits<bool> {
        static bool GetPriorValue(bool value) {return !value;}
        static bool GetNextValue(bool value) {return !value;}
        static bool GetAlmostSameValue(bool value) {return value;}
    };

    template<>
    struct NumericTraits<double>
    {        
        static double GetPriorValue(double value) {return AddUlps(value,-20);}
        static double GetNextValue(double value) {return AddUlps(value,20);}
        static double GetAlmostSameValue(double value) {return AddUlps(value,2);}
    private:
        static double AddUlps(double value, int ulps)
        {
            FloatingPointAsInteger<double> intRep(value);
            intRep.AddUlps(ulps);
            return intRep.ToFloatingPoint();
        }
    };
    template<>
    struct NumericTraits<float>
    {        
        static float GetPriorValue(float value) {return AddUlps(value,-20);}
        static float GetNextValue(float value) {return AddUlps(value,20);}
        static float GetAlmostSameValue(float value) {return AddUlps(value,2);}
    private:
        static float AddUlps(float value, int ulps)
        {
            FloatingPointAsInteger<float> intRep(value);
            intRep.AddUlps(ulps);
            return intRep.ToFloatingPoint();
        }
    };
}}}
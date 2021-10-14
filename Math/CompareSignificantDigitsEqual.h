#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <type_traits>
#include <cmath>

namespace DNVS { namespace MoFa { namespace Math {
    template<typename T>
    typename std::enable_if<std::is_floating_point<T>::value, bool>::type CompareSignificantDigitsEqual(T lhs, T rhs, unsigned int signdigits = 4)
    {        
        return ( RoundToSignificantDigits(lhs,signdigits) == RoundToSignificantDigits(rhs,signdigits) );
        
    }

    template<typename T>
    typename std::enable_if<std::is_floating_point<T>::value, T>::type RoundToSignificantDigits(T value, unsigned int nsigndigits = 4)
    {   
        if( fabs(value) == 0.0 ) 
            return value;

        // compute the 10-based exponent of the value
        int exponent = int(floor(log10(fabs(value))));

        // normalise the value to range [1,10>
        T norm = pow(T(10),T(exponent));

        // round off the value to specified number of digits
        T scale = pow(T(10),T(nsigndigits-1));

        int ivalue = int((value/norm)*scale + 0.5*value/fabs(value));

        return (T(ivalue)/scale)*norm;

    }

    template<typename T>
    typename std::enable_if<std::is_integral<T>::value, T>::type RoundToSignificantDigits(T value, unsigned int nsigndigits = 4)
    {   
        return value;
    }
}}}
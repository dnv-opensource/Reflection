#pragma once 
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <type_traits>

namespace DNVS {namespace MoFa {namespace Units {
    template<int Length,int Time,int Force,int TemperatureDifference,int Angle>
    struct Dimension {
        static const int L=Length;
        static const int T=Time;
        static const int F=Force;
        static const int O=TemperatureDifference;
        static const int A=Angle;
    };
    typedef Dimension<0,0,0,0,0> NoDimension;

    template<typename Dim1,typename Dim2>
    struct AddDimensions {
        typedef Dimension<Dim1::L+Dim2::L,Dim1::T+Dim2::T,Dim1::F+Dim2::F,Dim1::O+Dim2::O,Dim1::A+Dim2::A> type;
    };

    template<typename Dim1,typename Dim2>
    struct SubDimensions {
        typedef Dimension<Dim1::L-Dim2::L,Dim1::T-Dim2::T,Dim1::F-Dim2::F,Dim1::O-Dim2::O,Dim1::A-Dim2::A> type;
    };

    template<typename Dim1,int Scale>
    struct ScaleDimension {
        typedef Dimension<Dim1::L*Scale,Dim1::T*Scale,Dim1::F*Scale,Dim1::O*Scale,Dim1::A*Scale> type;
    };
    template<typename Dim1, int Divisor>
    struct DivideDimension {
        static_assert(Dim1::L%Divisor == 0, "Length dimension must be a multiple of Divisor");
        static_assert(Dim1::T%Divisor == 0, "Time dimension must be a multiple of Divisor");
        static_assert(Dim1::F%Divisor == 0, "Force dimension must be a multiple of Divisor");
        static_assert(Dim1::O%Divisor == 0, "TempDiff dimension must be a multiple of Divisor");
        static_assert(Dim1::A%Divisor == 0, "Angle dimension must be a multiple of Divisor");
        typedef Dimension<Dim1::L/Divisor, Dim1::T/Divisor, Dim1::F/Divisor, Dim1::O/Divisor, Dim1::A/Divisor> type;
    };

    template<typename T>
    struct IsDimension : std::integral_constant<bool,false> {};

    template<int Length,int Time,int Force,int TemperatureDifference,int Angle>
    struct IsDimension<Dimension<Length,Time,Force,TemperatureDifference,Angle>> : std::integral_constant<bool,true> {};
}}}

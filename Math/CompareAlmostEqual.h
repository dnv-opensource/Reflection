#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <cstdint>

namespace DNVS { namespace MoFa { namespace Math {
    template<size_t size>
    struct IntegerTraits;

    template<>
    struct IntegerTraits<4>
    {
        typedef int32_t Type;
    };
    
    template<>
    struct IntegerTraits<8>
    {
        typedef int64_t Type;
    };

    template<typename T>
    class FloatingPointAsInteger 
    {
    public:
        typedef typename IntegerTraits<sizeof(T)>::Type IntegerType;
        FloatingPointAsInteger(T floatingValue) 
            :   m_integerRep(*(IntegerType*)&floatingValue)
            ,   m_isNegative(false)
        {
            MakeLexicographicAsTwosComplementInt();
        }
        IntegerType GetIntegerRep() const {return m_integerRep; }
        IntegerType GetNegativeBitMask() const 
        {
            return (((IntegerType)1) << (8 * sizeof(T) - 1));
        }
        void MakeLexicographicAsTwosComplementInt() 
        {
            if(m_integerRep<0) 
            {
                m_integerRep = GetNegativeBitMask() - m_integerRep;
                m_isNegative = true;
            }
        }
        void AddUlps(int ulps)
        {
            m_integerRep += ulps;
        }
        T ToFloatingPoint() const 
        {
            IntegerType integerRep=m_integerRep;
            if(m_isNegative) integerRep = GetNegativeBitMask() - m_integerRep;
            return *(T*)&integerRep;
        }
    private:
        IntegerType m_integerRep;
        bool m_isNegative;
    };

    ///This is based on the following article: http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
    template<typename T>
    typename std::enable_if<std::is_floating_point<T>::value, bool>::type CompareAlmostEqual(T lhs, T rhs, unsigned int maxUlps = 4)
    {        
        FloatingPointAsInteger<T> aInt(lhs);
        FloatingPointAsInteger<T> bInt(rhs);
        unsigned long long intDiff = abs(aInt.GetIntegerRep() - bInt.GetIntegerRep());
        if (intDiff <= maxUlps)
            return true;
        return false;
    }
}}}
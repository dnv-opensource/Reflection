#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

namespace DNVS {namespace MoFa { namespace TypeUtilities { namespace Detail {
    class RadixTreeNodeMetaData 
    {
    public:
        RadixTreeNodeMetaData()
            :   m_currentCharacter(0)
            ,   m_startOfArray(0)
            ,   m_arrayLength(0)
            ,   m_hasRemainder(0)
            ,   m_hasValue(0)
        {}
        RadixTreeNodeMetaData(const RadixTreeNodeMetaData& other)
            :   m_currentCharacter(other.m_currentCharacter)
            ,   m_startOfArray(other.m_startOfArray)
            ,   m_arrayLength(other.m_arrayLength)
            ,   m_hasRemainder(other.m_hasRemainder)
            ,   m_hasValue(other.m_hasValue)
        {}
        RadixTreeNodeMetaData(RadixTreeNodeMetaData&& other)
            :   m_currentCharacter(other.m_currentCharacter)
            ,   m_startOfArray(other.m_startOfArray)
            ,   m_arrayLength(other.m_arrayLength)
            ,   m_hasRemainder(other.m_hasRemainder)
            ,   m_hasValue(other.m_hasValue)
        {
            other.Reset();
        }
        RadixTreeNodeMetaData& operator=(const RadixTreeNodeMetaData& other)
        {
            m_currentCharacter = other.m_currentCharacter;
            m_startOfArray = other.m_startOfArray;
            m_arrayLength = other.m_arrayLength;
            m_hasRemainder = other.m_hasRemainder;
            m_hasValue = other.m_hasValue;
            return *this;
        }
        RadixTreeNodeMetaData& operator=(RadixTreeNodeMetaData&& other)
        {
            m_currentCharacter = other.m_currentCharacter;
            m_startOfArray = other.m_startOfArray;
            m_arrayLength = other.m_arrayLength;
            m_hasRemainder = other.m_hasRemainder;
            m_hasValue = other.m_hasValue;
            other.Reset();
            return *this;
        }
        void Reset()
        {
            m_hasRemainder = 0;
            m_hasValue = 0;
            m_startOfArray = 0;
            m_arrayLength = 0;
            m_currentCharacter = 0;
        }
        void SetCurrentCharacter(char currentCharacter)
        {
            m_currentCharacter = currentCharacter;
        }
        char GetCurrentCharacter() const
        {
            return m_currentCharacter;
        }
        bool HasRemainder() const
        {
            return m_hasRemainder!=0;
        }
        bool HasValue() const
        {
            return m_hasValue!=0;
        }
        void SetHasRemainder(bool value)
        {
            m_hasRemainder = value;
        }
        void SetHasValue(bool value)
        {
            m_hasValue = value;
        }
        bool IsEmpty() const
        {
            return !HasValue() && m_arrayLength == 0;
        }
    protected:
        ///These datas are aligned to 64 bit memory address space. 
        char m_currentCharacter;
        char m_startOfArray;
        short m_arrayLength;
        short m_hasRemainder;
        short m_hasValue;
    };
}}}}

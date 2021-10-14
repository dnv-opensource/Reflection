#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string.h>
#include <string>

namespace DNVS {namespace MoFa {namespace TypeUtilities {
    template<int MaxSize>
    struct FixedSizeString 
    {
    public:
        FixedSizeString(const FixedSizeString& other) = default;
        FixedSizeString& operator=(const FixedSizeString& other) = default;
        FixedSizeString(FixedSizeString&& other) = default;
        FixedSizeString& operator=(FixedSizeString&& other) = default;
        FixedSizeString()
        {
            m_text[0] = 0;
        }
        FixedSizeString(const std::string& text)
        {
            strcpy_s(m_text, text.c_str());
        }
        FixedSizeString& operator=(const char* text)
        {
            memset(m_text, 0, sizeof(m_text));
            memcpy_s(m_text, sizeof(m_text), text, sizeof(m_text));
            m_text[MaxSize] = 0;
            return *this;
        }
        operator std::string() const 
        {
            return std::string(m_text);
        }
        std::string operator+(const std::string& other) const
        {
            return std::string(m_text) + other;
        }
        void clear()
        {
            m_text[0] = 0;
        }
        bool empty() const
        {
            return m_text[0] == 0;
        }
        size_t length() const
        {
            return strlen(m_text);
        }
        FixedSizeString substr(size_t offset = 0, size_t count = std::string::npos)
        {
            size_t len = length();
            FixedSizeString str;
            if(count == 0)
                return str;
            if(offset >= len)
                return str;
            if(count > len - offset) 
                count = len - offset;
            str = m_text+offset;
            str.m_text[count] = 0;
            return str;
        }
        bool operator==(const FixedSizeString& other) const
        {
            return strcmp(m_text, other.m_text) == 0;
        }
        char operator[](size_t i) const {return m_text[i];}
        const char* c_str() const { return m_text; }
    private:
        char m_text[MaxSize+1];
    };
}}}
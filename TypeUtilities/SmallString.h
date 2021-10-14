#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>

namespace DNVS {namespace MoFa { namespace TypeUtilities {
    template<typename CharT, typename AllocatorT = std::allocator<CharT> >
    class SmallString
    {
    public:
        SmallString(const std::string& other)
        {
            operator=(other.c_str());
        }
        SmallString(SmallString&& other)
            :   m_ptr(other.m_ptr)
        {
            other.m_ptr = nullptr;
        }
        SmallString(const SmallString& other)
        {
            operator=(other.m_ptr);
        }
        SmallString(const char* text)
        {
            operator=(text);
        }
        SmallString()
            :   m_ptr(nullptr)
        {}
        ~SmallString()
        {
            clear();
        }
        void clear()
        {
            if(m_ptr)
            {
                m_allocator.deallocate(m_ptr, length()+1);
                m_ptr = 0;
            }
        }
        void Assign(const char* text, size_t length)
        {
            clear();
            if(length == 0)
                return;
            m_ptr = m_allocator.allocate(length+1);
            memcpy_s(m_ptr, length+1, text, length+1);
            m_ptr[length] = 0;
        }
        SmallString& operator=(const char* text)
        {
            if(text == 0)
                clear();
            else
                Assign(text, strlen(text));
            return *this;
        }
        SmallString& operator=(SmallString&& other)
        {
            m_ptr = other.m_ptr;
            other.m_ptr = nullptr;
            return *this;
        }
        SmallString& operator=(const SmallString& other)
        {
            return operator=(other.m_ptr);
        }
        bool empty() const {
            return m_ptr == 0 || m_ptr[0] == 0;
        }
        operator std::string() const {
            if(!empty())
                return m_ptr;
            else
                return std::string();
        }
        char operator[](int index) const { return m_ptr[index];}
        const char* c_str() const { 
            if(m_ptr)
                return m_ptr; 
            else {                
                return &m_empty;
            }
        }
        size_t length() const
        {
            if(m_ptr == 0)
                return 0;
            else
                return strlen(m_ptr);
        }
        SmallString substr(size_t offset = 0, size_t count = std::string::npos) const
        {
            size_t len = length();
            SmallString str;
            if(count == 0)
                return str;
            if(offset >= len)
                return str;
            if(count > len - offset) 
                count = len - offset;
            str.Assign(m_ptr+offset, count);
            return str;
        }
        bool operator==(const SmallString& other) const
        {
            if(m_ptr == other.m_ptr)
                return true;
            if(m_ptr == nullptr || other.m_ptr == nullptr)
                return false;
            return strcmp(m_ptr, other.m_ptr) == 0;
        }
    private:
        static char m_empty;
        char* m_ptr;
        typedef typename AllocatorT::rebind<CharT>::other allocator_type;
        allocator_type m_allocator;
    };
    template<typename CharT, typename AllocatorT>
    char SmallString<CharT, AllocatorT>::m_empty = 0;
}}}
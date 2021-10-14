#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <string>

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime { namespace Detail {

    class UnitStringIterator {
    public:
        UnitStringIterator()
            :   m_pos(0)
        {}

        UnitStringIterator(const std::string& unitName) 
            :   m_unitName(unitName)
            ,   m_pos(0)
        {}
        char operator*() const {return m_unitName.at(m_pos);}
        bool operator==(const UnitStringIterator& other) const {
            return GetSubstr()==other.GetSubstr();
        }
        bool IsEmpty() const
        {
            return m_pos>=m_unitName.size();
        }
        std::string GetSubstr() const
        {
            if(m_pos>=m_unitName.size()) 
                return "";
            return m_unitName.substr(m_pos);
        }
        UnitStringIterator& operator+=(std::size_t pos)
        {
            m_pos+=pos;
            return *this;
        }
        UnitStringIterator& operator++()
        {
            m_pos++;
            return *this;
        }
    private:
        std::size_t m_pos;
        std::string m_unitName;
    };
}}}}}
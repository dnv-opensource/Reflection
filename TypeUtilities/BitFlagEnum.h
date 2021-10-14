#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
namespace DNVS {namespace MoFa {namespace TypeUtilities {

    template<typename DerivedT>
    struct BitFlagEnum {
    public:
        typedef BitFlagEnum Base;
        BitFlagEnum(int opt) 
            :   m_enum(opt) 
        {}
        DerivedT operator|(const DerivedT& other) const
        {
            return DerivedT(m_enum|other.m_enum);
        }
        DerivedT operator&(const DerivedT& other) const
        {
            return DerivedT(m_enum&other.m_enum);
        }
        DerivedT& operator|=(const DerivedT& other)
        {
            return static_cast<DerivedT*>(this)->operator=((*this)|other);
        }
        DerivedT operator-(const DerivedT& other) const 
        {
            DerivedT tmp(m_enum);
            tmp.Remove(other);
            return tmp;
        }
        DerivedT& operator&=(const DerivedT& other)
        {
            return static_cast<DerivedT*>(this)->operator=((*this)&other);
        }
        bool operator==(const DerivedT& other) const 
        {
            return m_enum==other.m_enum;
        }
        bool operator!=(const DerivedT& other) const 
        {
            return m_enum!=other.m_enum;
        }
        void Add(const DerivedT& opt) 
        {
            m_enum=m_enum|opt.m_enum;
        }
        void Remove(const DerivedT& opt)
        {
            m_enum=m_enum&~opt.m_enum;
        }

        bool Has(const DerivedT& opt) const
        {
            return (m_enum&opt.m_enum)==opt.m_enum;
        }
        int AsInt() const 
        {
            return m_enum;
        }
    protected:
        int m_enum;
    };
}}}
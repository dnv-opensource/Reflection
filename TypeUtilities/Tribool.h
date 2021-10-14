#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>

namespace DNVS {namespace MoFa { namespace TypeUtilities {
    class Tribool;
    bool IsIndeterminate(const Tribool& other);
    class Tribool 
    {
    public:
        Tribool()
            :   m_value(IndeterminateValue)
        {}
        Tribool(bool value)
            :   m_value(value?True:False)
        {}
        Tribool(const Tribool& other)
            :   m_value(other.m_value)
        {}
        Tribool& operator=(const Tribool& other)
        {
            m_value = other.m_value;
            return *this;
        }
        operator bool() const
        {
            return m_value == True;
        }
        Tribool operator!() const
        {
            switch(m_value)
            {
            case True:
                return false;
            case False:
                return true;
            default:
                return MakeIndeterminate();
            }
        }

        /**
        the result of comparing two tribool values, according to the following table:
        ==              true            false           Indeterminate
        true            true            false           Indeterminate
        false           false           true            Indeterminate
        Indeterminate   Indeterminate   Indeterminate   Indeterminate
        */
        friend Tribool operator==(bool lhs, const Tribool& rhs)
        {
            return Tribool(lhs) == rhs;
        }
        friend Tribool operator==(const Tribool& lhs, bool rhs)
        {
            return lhs == Tribool(rhs);
        }
        Tribool operator==(const Tribool& other) const
        {
            if(IsIndeterminate())
                return MakeIndeterminate();
            if(other.IsIndeterminate())
                return MakeIndeterminate();
            return m_value == other.m_value;
        }
        /**
        the result of comparing two tribool values for inequality, according to the following table:
        !=              true            false           Indeterminate
        true            false           true            Indeterminate
        false           true            false           Indeterminate
        Indeterminate   Indeterminate   Indeterminate   Indeterminate
        */
        friend Tribool operator!=(bool lhs, const Tribool& rhs)
        {
            return Tribool(lhs) != rhs;
        }
        friend Tribool operator!=(const Tribool& lhs, bool rhs)
        {
            return lhs != Tribool(rhs);
        }
        Tribool operator!=(const Tribool& other) const
        {
            return !operator==(other);
        }
        /**
        the result of logically ANDing the two tribool values, according to the following table:
        &&              true            false           Indeterminate
        true            true            false           Indeterminate
        false           false           false           false
        Indeterminate   Indeterminate   false           Indeterminate
        */
#if _BullseyeCoverage
#pragma ("BullseyeCoverage save off")
#endif        
        friend Tribool operator&&(bool lhs, const Tribool& rhs)
        {
            return Tribool(lhs) && rhs;
        }
        friend Tribool operator&&(const Tribool& lhs, bool rhs)
        {
            return lhs && Tribool(rhs);
        }
#if _BullseyeCoverage
#pragma ("BullseyeCoverage restore")
#endif        
        Tribool operator&&(const Tribool& other) const
        {
            if(m_value == False || other.m_value == False)
                return false;
            else if(m_value == IndeterminateValue || other.m_value == IndeterminateValue)
                return MakeIndeterminate();
            else
                return true;
        }

        /**
        the result of logically ORing the two tribool values, according to the following table:
        ||              true            false           Indeterminate
        true            true            true            true
        false           true            false           Indeterminate
        Indeterminate   true            Indeterminate   Indeterminate
        */
#if _BullseyeCoverage
#pragma ("BullseyeCoverage save off")
#endif        
        friend Tribool operator||(bool lhs, const Tribool& rhs)
        {
            return Tribool(lhs) || rhs;
        }
        friend Tribool operator||(const Tribool& lhs, bool rhs)
        {
            return lhs || Tribool(rhs);
        }
#if _BullseyeCoverage
#pragma ("BullseyeCoverage restore")
#endif        
        Tribool operator||(const Tribool& other) const
        {
            if(m_value == True || other.m_value == True)
                return true;
            else if(m_value == IndeterminateValue || other.m_value == IndeterminateValue)
                return MakeIndeterminate();
            else
                return false;
        }
        static Tribool MakeIndeterminate() {
            return Tribool(IndeterminateValue);
        }
        bool IsIndeterminate() const
        {
            return m_value == IndeterminateValue;
        }
    private:
        enum TriBoolValues {
            False,
            True,
            IndeterminateValue
        };
        Tribool(TriBoolValues value) : m_value(value) {}
        TriBoolValues m_value;
    };
    const Tribool Indeterminate(Tribool::MakeIndeterminate());
    inline bool IsIndeterminate(const Tribool& other)
    {
        return other.IsIndeterminate();
    }
}}}

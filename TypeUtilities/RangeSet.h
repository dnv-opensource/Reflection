#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <list>
#include <algorithm>
namespace DNVS {namespace MoFa {namespace TypeUtilities {
    //This set contains an element T once. If it is inserted twice, it is erased from the set.
    template<typename T>
    class RangeSet
    {
    public:
        using value_type = std::pair<T, T>;
        RangeSet(const std::initializer_list<value_type>& values)
        {
            for (const value_type& value : values)
                insert(value);
        }
        RangeSet() {}
        RangeSet(const RangeSet&) = default;
        RangeSet(RangeSet&&) = default;
        RangeSet& operator=(const RangeSet&) = default;
        RangeSet& operator=(RangeSet&&) = default;

        void emplace(T first, T second)
        {
            return insert(std::make_pair(first, second));
        }
        void insert(value_type range)
        {
            if (range.first > range.second)
                std::swap(range.first, range.second);
            for (auto it = m_ranges.begin(); it != m_ranges.end(); ++it)
            {
                if (range.first < it->first)
                {
                    m_ranges.insert(it, range);
                    MergeJoinedRanges();
                    return;
                }
            }
            m_ranges.push_back(range);
            MergeJoinedRanges();
        }
        void erase(value_type range)
        {
            if (range.first > range.second)
                std::swap(range.first, range.second);
            if (m_ranges.empty())
                return;
            auto it = m_ranges.begin();
            while (it != m_ranges.end())
            {
                auto icurrent = it++;
                if (IsValueInRange(icurrent->first, range) && IsValueInRange(icurrent->second, range))
                    m_ranges.erase(icurrent);
                else if (IsValueInRange(icurrent->first, range))
                    icurrent->first = range.second;
                else if (IsValueInRange(icurrent->second, range))
                    icurrent->second = range.first;
                else if (IsValueInRange(range.first, *icurrent))
                {
                    m_ranges.insert(icurrent, std::make_pair(icurrent->first, range.first));
                    icurrent->first = range.second;
                }
            }
        }
        using RangeList = std::list<value_type>;
        using const_iterator = typename RangeList::const_iterator;
        using iterator = typename RangeList::iterator;
        const_iterator begin() const { return m_ranges.begin(); }
        const_iterator end() const { return m_ranges.end(); }
        const_iterator find(const T& value) const {
            for (auto it = begin(); it != end(); ++it)
            {
                if(IsValueInRange(value, *it))
                    return it;
                else if (it->first > value)
                    return end();
            }
            return end();
        }
        const_iterator find(const value_type& value) const {
            for (auto it = begin(); it != end(); ++it)
            {
                bool isFirstValueInRange = IsValueInRange(value.first, *it);
                bool isSecondValueInRange = IsValueInRange(value.second, *it);
                if (isFirstValueInRange != isSecondValueInRange)
                    return end();
                if (isFirstValueInRange)
                    return it;
                else if (it->first > value.first)
                    return end();
            }
            return end();
        }
        iterator begin() { return m_ranges.begin(); }
        iterator end() { return m_ranges.end(); }
        bool empty() const { return m_ranges.empty(); }
        size_t size() const { return m_ranges.size(); }
        const value_type& front() const { return m_ranges.front(); }
        const value_type& back() const { return m_ranges.back(); }
        //Returns true if the ranges of this contains all the ranges of other.
        bool Contains(const RangeSet<T>& other) const
        {
            if (other.empty())
                return true;
            else if (empty())
                return false;
            auto it1 = begin();
            auto it2 = other.begin();
            while (it1 != end() && it2 != other.end())
            {
                bool isFirstInRange = IsValueInRange(it2->first, *it1);
                bool isSecondInRange = IsValueInRange(it2->second, *it1);
                if (isFirstInRange != isSecondInRange)
                    return false;
                if (isFirstInRange)
                    ++it2;
                else {
                    if (it2->first < it1->first)
                        return false;
                    ++it1;
                }
            }
            return true;
        }
        bool Contains(T value) const
        {
            return find(value) != end();
        }
        bool Contains(const value_type& range) const
        {
            return find(range) != end();
        }
    private:
        bool IsValueInRange(T value, const value_type& range) const
        {
            return (value >= range.first && value <= range.second);
        }
        void MergeJoinedRanges()
        {
            if (m_ranges.empty())
                return;
            auto it = m_ranges.begin();
            auto itToModify = it;
            while (it != m_ranges.end())
            {
                auto icurrent = it++;
                if (itToModify != icurrent)
                {
                    if (icurrent->first <= itToModify->second)
                    {
                        itToModify->second = (std::max)(itToModify->second, icurrent->second);
                        m_ranges.erase(icurrent);
                    }
                    else
                        itToModify = icurrent;
                }
            }
        }
        RangeList m_ranges;
    };
}}}
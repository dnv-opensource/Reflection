#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <map>

namespace DNVS {namespace MoFa {namespace TypeUtilities {
    template<typename Key1, typename Key2, typename Less1 = std::less<Key1>, typename Less2 = std::less<Key2>, typename AllocatorT = std::allocator<void>>
    class DualMultimap {
    public:
        typedef std::multimap<Key1, Key2, Less1, AllocatorT> Map1;
        typedef std::multimap<Key2, Key1, Less2, AllocatorT> Map2;
        const Map1& GetMap1() const { return m_map1; }
        const Map2& GetMap2() const { return m_map2; }
        void clear() { 
            m_map1.clear(); 
            m_map2.clear();
        }
        size_t size() const
        {
            return m_map1.size();
        }
        bool empty() const
        {
            return m_map1.empty();
        }
        void insert(const Key1& key1, const Key2& key2)
        {
            m_map1.insert(std::make_pair(key1, key2));
            m_map2.insert(std::make_pair(key2, key1));
        }
        void erase(const Key1& key1, const Key2& key2)
        {
            EraseImpl(m_map1, key1, key2);
            EraseImpl(m_map2, key2, key1);
        }
        void erase1(const Key1& key)
        {
            EraseKey(m_map1, key, m_map2);
        }
        void erase2(const Key2& key)
        {
            EraseKey(m_map2, key, m_map1);
        }
    private:
        template<typename MapT, typename KeyT, typename OtherMapT>
        void EraseKey(MapT& map, const KeyT& key, OtherMapT& otherMap)
        {
            auto range = map.equal_range(key);
            auto it = range.first;
            while (it != range.second)
            {
                auto iCurrent = it++;
                EraseImpl(otherMap, iCurrent->second, iCurrent->first);
                map.erase(iCurrent);
            }
        }

        template<typename MapT,typename KeyT, typename ValueT>
        void EraseImpl(MapT& map, const KeyT& key, const ValueT& value)
        {
            auto range = map.equal_range(key);
            auto it = range.first;
            while (it != range.second)
            {
                auto iCurrent = it++;
                if (iCurrent->second == value)
                    map.erase(iCurrent);
            }
        }
        Map1 m_map1;
        Map2 m_map2;
    };
}}}

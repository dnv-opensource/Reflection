#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

namespace DNVS { namespace MoFa { namespace TypeUtilities {
    template<typename T>
    class ListMaker {
    public:
        ListMaker(T&& arg)
        {
            m_list.push_back(std::forward<T>(arg));
        }
        ListMaker(ListMaker&& other)
            :   m_list(std::move(other.m_list))
        {}
        ListMaker&& operator()(T&& arg) 
        {
            m_list.push_back(std::forward<T>(arg));
            return std::move(*this);
        }
        operator std::list<T>() {return std::move(m_list);}
        template<typename U>
        operator std::list<U>() { return std::list<U>(m_list.begin(), m_list.end());}
    private:
        std::list<T> m_list;
    };
    template<typename T>
    ListMaker<T> MakeList(T&& arg)
    {
        return ListMaker<T>(std::forward<T>(arg));
    }
    template<typename T, typename U>
    class MapMaker {
    public:
        MapMaker(std::pair<T,U>&& arg)
        {
            m_map.insert(std::forward<std::pair<T,U>>(arg));
        }
        MapMaker(MapMaker&& other)
            :   m_map(std::move(other.m_map))
        {}
        MapMaker&& operator()(T&& a0, U&& a1) 
        {
            m_map.insert(std::make_pair(std::forward<T>(a0), std::forward<U>(a1)));
            return std::move(*this);
        }
        operator std::map<T, U>() {return std::move(m_map);}
        template<typename T0, typename U0>
        operator std::map<T0, U0>() {return std::map<T0, U0>(m_map.begin(), m_map.end());}
    private:
        std::map<T, U> m_map;
    };

    template<typename T, typename U>
    MapMaker<T, U> MakeMap(T&& a0, U&& a1)
    {
        return MapMaker<T, U>(std::make_pair(std::forward<T>(a0), std::forward<U>(a1)));
    }
}}}
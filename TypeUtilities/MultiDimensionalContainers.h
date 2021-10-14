#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <vector>
#include <map>
#include <list>

namespace DNVS { namespace MoFa {namespace TypeUtilities {
/*
class DummyMultiDimensionalTraitsT {
public:
  using ComponentType = double;
  using ComponentSorter = std::less<double>;
  static size_t GetComponentSize() {return 3;}
  static double GetComponent(const Point3D& p, size_t index) {return p[index];}
  static Point3D Construct(const std::vector<double>& components) {return Point3D(components.at(0),components.at(1),components.at(2));}
};
*/

template<typename KeyT, typename MultiDimensionalTraitsT, typename AllocatorT>
    class MultiDimensionalComponents
    {
    public:
        using ComponentType = typename MultiDimensionalTraitsT::ComponentType;
        using ComponentSorter = typename MultiDimensionalTraitsT::ComponentSorter;
        using ComponentMap = std::map<ComponentType, size_t, ComponentSorter, AllocatorT>;
        MultiDimensionalComponents()
        {
            m_components.resize(MultiDimensionalTraitsT::GetComponentSize());
        }
        std::pair<typename ComponentMap::const_iterator, typename ComponentMap::const_iterator> EqualRange(const ComponentMap& componentMap, ComponentType value, ComponentType tolerance) const
        {
            if (componentMap.empty())                                                                       
                return std::make_pair(componentMap.end(), componentMap.end());                              
            auto range = componentMap.equal_range(value);                                                   
            if (range.first == componentMap.end())                                                          
                --range.first;                                                                              
            while (value - range.first->first < tolerance && range.first != componentMap.begin())           
                --range.first;                                                                                                                                                                                                                                                                                                                                                                                                                  
            if (value - range.first->first > tolerance)                                                                                                                                             
                ++range.first;                                                                              
            while (range.second != componentMap.end() && range.second->first - value <= tolerance)
            {
                ++range.second;
            }
            return range;
        }
        void Insert(const KeyT& key)
        {
            for (size_t i = 0; i < MultiDimensionalTraitsT::GetComponentSize(); ++i)
                ++m_components[i][MultiDimensionalTraitsT::GetComponent(key, i)];
        }
        void Erase(const KeyT& key, size_t count)
        {
            for (size_t i = 0; i < MultiDimensionalTraitsT::GetComponentSize(); ++i)
            {
                auto it = m_components[i].find(MultiDimensionalTraitsT::GetComponent(key, i));
                if (it != m_components[i].end())
                {
                    if (it->second <= count)
                        m_components[i].erase(it);
                    else
                        it->second -= count;
                }
            }
        }
        void Clear()
        {
            for (auto& component : m_components)
                component.clear();
        }
        template<typename ContainerT>
        std::list<typename ContainerT::const_iterator> FindInBox(const ContainerT& container, const KeyT& key, ComponentType tolerance) const
        {
            std::list<typename ContainerT::const_iterator> found;
            std::vector<ComponentType> candidate(m_components.size());
            FindInBoxRecursive(container, found, key, tolerance, 0, candidate);
            return found;
        }
        template<typename ContainerT>
        void FindInBoxRecursive(const ContainerT& container, std::list<typename ContainerT::const_iterator>& found, const KeyT& key, ComponentType tolerance, size_t index, std::vector<ComponentType>& candidate) const
        {
            if (index == candidate.size())
            {
                auto it = container.find(MultiDimensionalTraitsT::Construct(candidate));
                if (it != container.end())
                    found.emplace_back(it);
            }
            else
            {
                auto range = EqualRange(m_components.at(index), MultiDimensionalTraitsT::GetComponent(key, index), tolerance);
                for (auto it = range.first; it != range.second; ++it)
                {
                    candidate[index] = it->first;
                    FindInBoxRecursive(container, found, key, tolerance, index + 1, candidate);
                }
            }
        }
    private:
        //If KeyT is Point3d, m_components will store a map of all unique X, Y and Z values, and their use count.
        std::vector<ComponentMap, AllocatorT> m_components;
    };

    template<typename T, typename MultiDimensionalTraitsT>
    class MultiDimensionalSorterNoTolerance {
    public:
        using ComponentSorter = typename MultiDimensionalTraitsT::ComponentSorter;
        bool operator()(const T& lhs, const T& rhs) const {
            for (size_t i = 0; i < MultiDimensionalTraitsT::GetComponentSize(); ++i)
            {
                if (MultiDimensionalTraitsT::GetComponent(lhs, i) != MultiDimensionalTraitsT::GetComponent(rhs, i))
                    return m_sorter(MultiDimensionalTraitsT::GetComponent(lhs, i), MultiDimensionalTraitsT::GetComponent(rhs, i));
            }
            return false;
        }
    private:
        ComponentSorter m_sorter;
    };


    template<typename InternalContainer, typename KeyT, typename MultiDimensionalTraitsT, typename AllocatorT>
    class MultiDimensionalTree
    {
    public:
        using iterator = typename InternalContainer::iterator;
        using const_iterator = typename InternalContainer::const_iterator;
        using value_type = typename InternalContainer::value_type;
        using reverse_iterator = typename InternalContainer::reverse_iterator;
        using const_reverse_iterator = typename InternalContainer::const_reverse_iterator;
        using size_type = typename InternalContainer::size_type;
        using allocator_type = typename InternalContainer::allocator_type;
        using key_compare = typename InternalContainer::key_compare;
        using value_compare = typename InternalContainer::value_compare;
        using key_type = typename InternalContainer::key_type;
        iterator begin() _NOEXCEPT { return m_container.begin(); }
        iterator end() _NOEXCEPT { return m_container.end(); }
        const_iterator begin() const _NOEXCEPT { return m_container.begin(); }
        const_iterator end() const _NOEXCEPT { return m_container.end(); }
        reverse_iterator rbegin() _NOEXCEPT { return m_container.rbegin(); }
        const_reverse_iterator rbegin() const _NOEXCEPT { return m_container.rbegin(); }
        reverse_iterator rend() _NOEXCEPT { return m_container.rend(); }
        const_reverse_iterator rend() const _NOEXCEPT { return m_container.rend(); }
        const_iterator cbegin() const _NOEXCEPT { return m_container.begin(); }
        const_iterator cend() const _NOEXCEPT { return m_container.end(); }
        const_reverse_iterator crbegin() const _NOEXCEPT { return m_container.rbegin(); }
        const_reverse_iterator crend() const _NOEXCEPT { return m_container.rend(); }
        size_type size() const _NOEXCEPT { return m_container.size(); }
        size_type max_size() const _NOEXCEPT { return m_container.max_size(); }
        bool empty() const _NOEXCEPT { return m_container.empty(); }
        allocator_type get_allocator() const _NOEXCEPT { return m_container.get_allocator(); }
        key_compare key_comp() const { return m_container.key_comp(); }
        value_compare value_comp() const { return m_container.value_comp(); }
        const_iterator find(const key_type& key) const { return m_container.find(key); }
        iterator find(const key_type& key) { return m_container.find(key); }
        size_type count(const key_type& _Keyval) const { return m_container.count(_Keyval); }
        iterator lower_bound(const key_type& _Keyval) { return m_container.lower_bound(_Keyval); }
        const_iterator lower_bound(const key_type& _Keyval) const { return m_container.lower_bound(_Keyval); }
        iterator upper_bound(const key_type& _Keyval) { return m_container.upper_bound(_Keyval); }
        const_iterator upper_bound(const key_type& _Keyval) const { return m_container.upper_bound(_Keyval); }
        std::pair<iterator, iterator> equal_range(const key_type& _Keyval) { return m_container.equal_range(_Keyval); }
        std::pair<const_iterator, const_iterator> equal_range(const key_type& _Keyval) const { return m_container.equal_range(_Keyval); }

        using ComponentType = typename MultiDimensionalTraitsT::ComponentType;
        std::list<const_iterator> FindInBox(const key_type& key, ComponentType tolerance) const
        {
            return m_components.FindInBox(m_container, key, tolerance);
        }
        size_type erase(const key_type& _Keyval)
        {
            size_type count = m_container.erase(_Keyval);
            m_components.Erase(_Keyval, count);
            return count;
        }
        iterator erase(const_iterator _Where)
        {
            m_components.Erase(*_Where, 1);
            return m_container.erase(_Where);
        }
        void clear() _NOEXCEPT {
            m_container.clear();
            m_components.Clear();
        }
        iterator erase(const_iterator _First, const_iterator _Last)
        {
            if (_First == begin() && _Last == end())
            {	// erase all
                clear();
                return (begin());
            }
            else
            {	// partial erase, one at a time
                while (_First != _Last)
                    erase(_First++);
                return (iterator(_First._Ptr, &this->_Get_data()));
            }
        }
    protected:
        InternalContainer m_container;
        MultiDimensionalComponents<KeyT, MultiDimensionalTraitsT, AllocatorT> m_components;
    };
    
    template<typename T, typename MultiDimensionalTraitsT, typename AllocatorT = std::allocator<T>>
    class MultiDimensionalSet : public MultiDimensionalTree<std::set<T, MultiDimensionalSorterNoTolerance<T, MultiDimensionalTraitsT>, AllocatorT>, T, MultiDimensionalTraitsT, AllocatorT>
    {
    public:
        std::pair<iterator, bool> insert(const value_type& value)
        {
            auto result = m_container.insert(value);
            if (result.second)
                m_components.Insert(value);
            return result;
        }
    };
    template<typename T, typename MultiDimensionalTraitsT, typename AllocatorT = std::allocator<T>>
    class MultiDimensionalMultiSet : public MultiDimensionalTree<std::multiset<T, MultiDimensionalSorterNoTolerance<T, MultiDimensionalTraitsT>, AllocatorT>, T, MultiDimensionalTraitsT, AllocatorT>
    {
    public:
        iterator insert(const value_type& value)
        {
            m_components.Insert(value);
            return m_container.insert(value);
        }
    };

    template<typename KeyT, typename ValueT, typename MultiDimensionalTraitsT, typename AllocatorT = std::allocator<KeyT>>
    class MultiDimensionalMap : public MultiDimensionalTree<std::map<KeyT, ValueT, MultiDimensionalSorterNoTolerance<KeyT, MultiDimensionalTraitsT>, AllocatorT>, KeyT, MultiDimensionalTraitsT, AllocatorT>
    {
    public:
        std::pair<iterator, bool> insert(const value_type& value)
        {
            auto result = m_container.insert(value);
            if (result.second)
                m_components.Insert(value.first);
            return result;
        }
    };

    template<typename KeyT, typename ValueT, typename MultiDimensionalTraitsT, typename AllocatorT = std::allocator<T>>
    class MultiDimensionalMultiMap : public MultiDimensionalTree<std::multimap<KeyT,ValueT, MultiDimensionalSorterNoTolerance<KeyT, MultiDimensionalTraitsT>, AllocatorT>, KeyT, MultiDimensionalTraitsT, AllocatorT>
    {
    public:
        iterator insert(const value_type& value)
        {
            m_components.Insert(value.first);
            return m_container.insert(value);
        }
    };
} } }
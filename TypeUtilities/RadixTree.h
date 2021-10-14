#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Detail\RadixTreeNode.h"
#include "Detail\RadixTreeIterator.h"
#include "Detail\RadixTreeAllocator.h"
#include <string>

namespace DNVS {namespace MoFa { namespace TypeUtilities {
    template<typename KeyT>
    class StringTreeTraits
    {
    public:
        static const bool UseCompressedKey = true;
        typedef KeyT RemainderType;
        bool IsTerminatingCharacter(char c) const { return c==0; }
        template<typename T>
        const char* GetCharactersFromKey(const T& key) const {return key.c_str();}
        const char* GetCharactersFromKey(const char* key) const {return key;}
        template<typename T>
        KeyT GetKey(const T& key) const
        {
            return key;
        }
        const KeyT& GetKey(const KeyT& key) const
        {
            return key;
        }
    };
    template<typename RemainderT>
    class TypeInfoTreeTraits
    {
    public:
        static const bool UseCompressedKey = false;
        typedef RemainderT RemainderType;
        bool IsTerminatingCharacter(char c) const { return c==0; }
        const char* GetCharactersFromKey(const std::type_info* info) const {return info->raw_name();}
        const char* GetCharactersFromKey(const std::type_info& info) const {return info.raw_name();}
        const std::type_info* GetKey(const std::type_info* key) const
        {
            return key;
        }
        const std::type_info* GetKey(const std::type_info& key) const
        {
            return &key;
        }
    };
    template<typename KeyT>
    class UpperCaseAndNumbersTreeTraits : public StringTreeTraits<KeyT>
    {
    public:
        bool IsTerminatingCharacter(char c) const {return c<'0' || c>'Z';}
    };

    template<typename KeyT>
    class DefaultTreeTraits : public StringTreeTraits<KeyT>
    {};

    template<>
    class DefaultTreeTraits<const std::type_info*> : public TypeInfoTreeTraits<std::string>
    {};

    template<typename KeyT, typename ValueT, typename TraitsT = DefaultTreeTraits<KeyT>, typename AllocatorT = std::allocator<char>>
    class RadixTree 
    {
    public:
        typedef Detail::RadixTreeAllocator<KeyT, ValueT, TraitsT, AllocatorT> AllocatorT;
        typedef KeyT key_type;
        typedef ValueT mapped_type;
        typedef std::pair<key_type, mapped_type> value_type;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef AllocatorT allocator_type;        
        typedef Detail::RadixTreeConstIterator<KeyT, ValueT, TraitsT> iterator;
        typedef Detail::RadixTreeConstIterator<KeyT, ValueT, TraitsT> const_iterator;
    private:
        typedef Detail::RadixTreeNode<KeyT, ValueT, TraitsT> NodeType;
        NodeType m_node;
        TraitsT m_traits;
        size_t m_size;
        AllocatorT m_allocator;

        template<typename T>
        const NodeType* FindNodeWithValue(const T& key) const
        {
            const NodeType* node = m_node.FindImpl(m_traits.GetCharactersFromKey(key), m_traits);
            if(node && node->HasValue())
                return node;
            else
                return nullptr;
        }
        template<typename T>
        NodeType* FindNodeWithValue(const T& key)
        {
            NodeType* node = m_node.FindImpl(m_traits.GetCharactersFromKey(key), m_traits);
            if(node && node->HasValue())
                return node;
            else
                return nullptr;
        }
        template<typename T>
        const NodeType* LowerBound(const T& key) const
        {
            const NodeType* node = m_node.LowerOrUpperBound(m_traits.GetCharactersFromKey(key), m_traits, true);
            if(node && node->HasValue())
                return node;
            else
                return nullptr;
        }
        template<typename T>
        const NodeType* UpperBound(const T& key) const
        {
            const NodeType* node = m_node.LowerOrUpperBound(m_traits.GetCharactersFromKey(key), m_traits, false);
            if(node && node->HasValue())
                return node;
            else
                return nullptr;
        }
    public:
        template<typename IteratorT>
        RadixTree(IteratorT first, IteratorT last)
            :   m_size(0)
        {
            insert(first, last);
        }
        RadixTree() 
            :   m_size(0)
        {}
        RadixTree(RadixTree&& other)
            :   m_node(std::move(other.m_node))
            ,   m_size(0)
        {
            std::swap(m_size, other.m_size);
        }
        RadixTree(const RadixTree& other)
            :   m_size(other.m_size)
        {
            m_node.Copy(other.m_node, m_allocator);
        }
        RadixTree(TraitsT traits) 
            :   m_traits(traits)
            ,   m_size(0)
        {}
        ~RadixTree()
        {
            m_node.Destroy(m_allocator);
            m_node.DestroyMembers(m_allocator);
        }
        iterator end()
        {
            return iterator(nullptr, &m_node);
        }
        const_iterator end() const
        {
            return const_iterator(nullptr, &m_node);
        }
        iterator begin()
        {
            return iterator(m_node.GetFirst(), &m_node);
        }
        const_iterator begin() const
        {
            return iterator(m_node.GetFirst(), &m_node);
        }
        template<typename IteratorT>
        void insert(IteratorT first, IteratorT last)
        {
            for(auto it = first; it != last; ++it)
                insert(*it);
        }
        std::pair<iterator, bool> insert(const value_type& value)
        {
            const char* key = m_traits.GetCharactersFromKey(value.first);
            NodeType* node = m_node.GetOrCreateNewNode(key, m_traits, m_allocator);
            bool result = node->HasValue();
            if(!result)
            {
                node->SetValue(value, m_allocator);
                ++m_size;
            }
            return std::make_pair(iterator(node, &m_node), !result);
        }
        template<typename T>
        size_type erase( const T& key )
        {
            NodeType* nodeToDelete = FindNodeWithValue(key);
            if(nodeToDelete)
            {
                nodeToDelete->RemoveValue(m_allocator);
                if(nodeToDelete->GetParent())
                    nodeToDelete->GetParent()->CleanupChildrenIfEmpty(m_allocator);
                --m_size;
                return 1;
            }
            return 0;
        }
        size_type count(const KeyT& key) const
        {
            return FindNodeWithValue(key)==0?0:1;
        }
        template<typename T>
        size_type count(const T& key) const
        {
            return FindNodeWithValue(key)==0?0:1;
        }
        template<typename T>
        iterator lower_bound(const T& Key)
        {
            return iterator(LowerBound(Key), &m_node);
        }
        template<typename T>
        const_iterator lower_bound(const T& Key) const
        {
            return iterator(LowerBound(Key), &m_node);
        }
        template<typename T>
        iterator upper_bound(const T& Key)
        {
            return iterator(UpperBound(Key), &m_node);
        }
        template<typename T>
        const_iterator upper_bound(const T& Key) const
        {
            return iterator(UpperBound(Key), &m_node);
        }
        template<typename T>
        std::pair<iterator,iterator> equal_range( const T& key )
        {
            return std::make_pair(lower_bound(key), upper_bound(key));
        }
        template<typename T>
        std::pair<const_iterator,const_iterator> equal_range( const T& key ) const
        {
            return std::make_pair(lower_bound(key), upper_bound(key));
        }
        template<typename T>
        iterator find(const T& key)
        {
            return iterator(FindNodeWithValue(key), &m_node);
        }
        template<typename T>
        const_iterator find(const T& key) const
        {
            return const_iterator(FindNodeWithValue(key), &m_node);
        }
        template<typename T>
        mapped_type& operator[](const T& key)
        {
            NodeType* node = m_node.GetOrCreateNewNode(m_traits.GetCharactersFromKey(key), m_traits, m_allocator);
            if(!node->HasValue())
            {
                ++m_size;
                node->SetValue(std::make_pair(m_traits.GetKey(key), ValueT()), m_allocator);
            }
            return node->GetMappedValueForEdit();
        }
        void clear()
        {
            m_node.Destroy(m_allocator);
            m_node.DestroyMembers(m_allocator);
            m_node = NodeType();
            m_size = 0;
        }
        size_type size() const
        {
            return m_size;
        }
        bool empty() const
        {
            return m_size == 0;
        }
    };
}}}
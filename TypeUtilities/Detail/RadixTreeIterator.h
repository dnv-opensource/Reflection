#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "RadixTreeNode.h"
#include <utility>

namespace DNVS {namespace MoFa { namespace TypeUtilities { namespace Detail {
    template<typename KeyT, typename ValueT, typename TraitsT>
    class KeyFromRadixTreeNode 
    {
        typedef RadixTreeNode<KeyT, ValueT, TraitsT> NodeType;
    public:
        KeyFromRadixTreeNode(const NodeType* node) : m_node(node) {}
        operator KeyT() const {return m_node->GetKey();}
        KeyFromRadixTreeNode& operator=(const KeyFromRadixTreeNode& other)
        {
            m_node = other.m_node;
            return *this;
        }
        bool operator==(const KeyT& other) const
        {
            return KeyT(*this) == other;
        }
        friend bool operator==(const KeyT& other, const KeyFromRadixTreeNode& self)
        {
            return other == KeyT(self);
        }
    private:
        const NodeType* m_node;
    };

    ///Streaming support for Point
    template<typename _Elem,typename _Traits,typename KeyT, typename ValueT, typename TraitsT>
    std::basic_ostream<_Elem, _Traits>& operator<<(std::basic_ostream<_Elem, _Traits>& _Ostr, const KeyFromRadixTreeNode<KeyT, ValueT, TraitsT>& node)
    {        
        return _Ostr << KeyT(node);
    }

    template<typename KeyT, typename ValueT, typename TraitsT>
    class ValueFromRadixTreeNode 
    {
        typedef RadixTreeNode<KeyT, ValueT, TraitsT> NodeType;
    public:
        ValueFromRadixTreeNode(const NodeType* node) : m_node(node) {}
        operator const ValueT&() const { return m_node->GetMappedValue(); }
        ValueFromRadixTreeNode& operator=(const ValueFromRadixTreeNode& other)
        {
            m_node = other.m_node;
            return *this;
        }
    private:
        const NodeType* m_node;
    };

    template<typename KeyT, typename ValueT, typename TraitsT>
    class PairFromRadixTreeNode 
    {
        typedef RadixTreeNode<KeyT, ValueT, TraitsT> NodeType;
    public:
        PairFromRadixTreeNode(const NodeType* node)
            :   first(node)
            ,   second(node)
        {}
        PairFromRadixTreeNode& operator=(const PairFromRadixTreeNode& other)
        {
            first = other.first;
            second = other.second;
            return *this;
        }
        typedef KeyT first_type;
        typedef ValueT second_type;
        KeyFromRadixTreeNode<KeyT, ValueT, TraitsT> first;
        ValueFromRadixTreeNode<KeyT, ValueT, TraitsT> second;
    };

    template<typename KeyT, typename ValueT, typename TraitsT>
    class RadixTreeConstIterator 
    {
        typedef RadixTreeNode<KeyT, ValueT, TraitsT> NodeType;
        typedef PairFromRadixTreeNode<KeyT, ValueT, TraitsT> value_type;
        value_type m_value;
        const NodeType* m_node;
        const NodeType* m_rootNode;
    public:
        RadixTreeConstIterator(const NodeType* node, const NodeType* rootNode = nullptr)
            :   m_node(node)
            ,   m_value(node)
            ,   m_rootNode(rootNode)
        {}
        bool operator==(const RadixTreeConstIterator& other) const
        {
            return m_node == other.m_node;
        }
        bool operator!=(const RadixTreeConstIterator& other) const
        {
            return m_node != other.m_node;
        }

        const value_type* operator->() const
        {
            return &m_value;
        }
        const value_type& operator*() const 
        {
            return m_value;
        }
        RadixTreeConstIterator& operator++()
        {
            if(m_node)
                m_node = m_node->GetNext();
            m_value = value_type(m_node);
            return *this;
        }
        RadixTreeConstIterator& operator--()
        {
            if(!m_node)
                m_node = m_rootNode->GetLast();
            else
                m_node = m_node->GetPrior();
            m_value = value_type(m_node);
            return *this;
        }
    };

    template<typename _Elem,typename _Traits,typename KeyT, typename ValueT, typename TraitsT>
    std::basic_ostream<_Elem, _Traits>& operator<<(std::basic_ostream<_Elem, _Traits>& _Ostr, const RadixTreeConstIterator<KeyT, ValueT, TraitsT>& it)
    {        
        if(it == RadixTreeConstIterator<KeyT, ValueT, TraitsT>(0) )
            return _Ostr << "end";
        else
            return _Ostr << "<" << KeyT(it->first) << ", " << ValueT(it->second) << ">";
    }

}}}}
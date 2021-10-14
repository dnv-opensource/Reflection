#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "RadixTreeNode.h"

namespace DNVS {namespace MoFa { namespace TypeUtilities { namespace Detail {
    template<typename KeyT, typename ValueT, typename TraitsT, typename AllocatorT>
    class RadixTreeAllocator
    {
    public:
        typedef typename TraitsT::RemainderType RemainderType;
        typedef RadixTreeNodeWithRemainder<KeyT, ValueT, TraitsT>  NodeWithRemainderType;
        typedef RadixTreeNodeWithValue<KeyT, ValueT, TraitsT>  NodeWithValueType;
        typedef RadixTreeNodeWithValueAndRemainder<KeyT, ValueT, TraitsT> NodeWithValueAndRemainderType;
        typedef RadixTreeNode<KeyT, ValueT, TraitsT> NodeType;
        typedef typename NodeType::StoredValueType StoredValueType;
        typedef typename AllocatorT::template rebind<NodeWithRemainderType>::other NodeWithRemainderAllocator;
        typedef typename AllocatorT::template rebind<NodeWithValueType>::other NodeWithValueAllocator;
        typedef typename AllocatorT::template rebind<NodeWithValueAndRemainderType>::other NodeWithValueAndRemainderAllocator;
        typedef typename AllocatorT::template rebind<NodeType>::other NodeAllocator;
        NodeType* ConstructNodes(size_t count)
        {
            NodeType* memory = m_nodeAllocator.allocate(count);
            memset(memory, 0 ,sizeof(NodeType) * count);
            return memory;
        }
        NodeWithValueAndRemainderType* ConstructNodeWithValueAndRemainder(NodeType* parent, const StoredValueType& value, const RemainderType& remainder)
        {
            NodeWithValueAndRemainderType* node = m_nodeWithValueAndRemainderAllocator.allocate(1);
            m_nodeWithValueAndRemainderAllocator.construct(node, NodeWithValueAndRemainderType(parent, value, remainder));
            return node;
        }
        NodeWithValueType* ConstructNodeWithValue(NodeType* parent, const StoredValueType& value)
        {
            NodeWithValueType* node = m_nodeWithValueAllocator.allocate(1);
            m_nodeWithValueAllocator.construct(node, NodeWithValueType(parent, value));
            return node;
        }
        NodeWithRemainderType* ConstructNodeWithRemainder(NodeType* parent, const RemainderType& remainder)
        {
            NodeWithRemainderType* node = m_nodeWithRemainderAllocator.allocate(1);
            m_nodeWithRemainderAllocator.construct(node, NodeWithRemainderType(parent, remainder));
            return node;
        }
        void DestroyNodes(NodeType* node, size_t size)
        {
            m_nodeAllocator.deallocate(node, size);
        }
        void DestroyNode(NodeWithValueAndRemainderType* node)
        {
            m_nodeWithValueAndRemainderAllocator.destroy(node);
            m_nodeWithValueAndRemainderAllocator.deallocate(node, 1);
        }
        void DestroyNode(NodeWithValueType* node)
        {
            m_nodeWithValueAllocator.destroy(node);
            m_nodeWithValueAllocator.deallocate(node, 1);
        }
        void DestroyNode(NodeWithRemainderType* node)
        {
            m_nodeWithRemainderAllocator.destroy(node);
            m_nodeWithRemainderAllocator.deallocate(node, 1);
        }
    private:
        NodeWithRemainderAllocator m_nodeWithRemainderAllocator;
        NodeWithValueAllocator m_nodeWithValueAllocator;
        NodeWithValueAndRemainderAllocator m_nodeWithValueAndRemainderAllocator;
        NodeAllocator m_nodeAllocator;
    };
}}}}
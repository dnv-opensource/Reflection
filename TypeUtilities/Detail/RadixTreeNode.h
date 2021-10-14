#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "RadixTreeNodeMetaData.h"
#include <algorithm>

namespace DNVS {namespace MoFa { namespace TypeUtilities { namespace Detail {
    inline size_t FindFirstDifferingLocation(const char* text1, const char* text2, size_t length)
    {
        size_t index = 0;
        for(; index < length; ++index)
        {
            if(text1[index] != text2[index])
                break;
        }
        return index;
    }

    template<typename KeyT, typename ValueT, typename TraitsT>
    class RadixTreeNode;

    template<typename KeyT, typename ValueT, typename TraitsT, bool UseCompressedKey>
    struct RadixTreeValueTypeComputer
    {
        typedef std::pair<KeyT, ValueT> ValueType;
        typedef std::pair<KeyT, ValueT> StoredValueType;
        typedef const KeyT& KeyType;
        typedef ValueT MappedType;
        static const StoredValueType& GetStoredValue(const ValueType& value) { return value; }
        static ValueT& GetMappedValue(StoredValueType& value) {return value.second;}
        static const ValueT& GetMappedValue(const StoredValueType& value) {return value.second;}
        static const KeyT& GetKey(const RadixTreeNode<KeyT, ValueT, TraitsT>* node)
        {
            return node->GetStoredValue().first;
        }
    };
    template<typename KeyT, typename ValueT, typename TraitsT>
    struct RadixTreeValueTypeComputer<KeyT, ValueT, TraitsT, true>
    {
        typedef std::pair<KeyT, ValueT> ValueType;
        typedef ValueT StoredValueType;
        typedef KeyT KeyType;
        typedef ValueT MappedType;
        static const StoredValueType& GetStoredValue(const ValueType& value) { return value.second; }
        static ValueT& GetMappedValue(StoredValueType& value) {return value;}
        static const ValueT& GetMappedValue(const StoredValueType& value) {return value;}
        static KeyT GetKey(const RadixTreeNode<KeyT, ValueT, TraitsT>* node)
        {
            if(auto parent = node->GetParent())
            {
                return GetKey(parent) + node->GetCurrentCharacter() + node->GetRemainder();
            }
            else
                return node->GetRemainder();
        }
    };


    template<typename KeyT, typename ValueT, typename TraitsT>
    class RadixTreeNodeWithRemainder;

    template<typename KeyT, typename ValueT, typename TraitsT>
    class RadixTreeNodeWithValue;

    template<typename KeyT, typename ValueT, typename TraitsT>
    class RadixTreeNodeWithValueAndRemainder;

    template<typename KeyT, typename ValueT, typename TraitsT>
    class RadixTreeNode : public RadixTreeNodeMetaData
    {
    public:
        typedef RadixTreeValueTypeComputer<KeyT,ValueT,TraitsT,TraitsT::UseCompressedKey> ValueTypeComputer;
        typedef typename ValueTypeComputer::StoredValueType StoredValueType;
        typedef typename ValueTypeComputer::ValueType ValueType;
        typedef typename TraitsT::RemainderType RemainderType;

        RadixTreeNode(RadixTreeNode&& other)
            :   RadixTreeNodeMetaData(std::move(other))
            ,   m_parent(other.m_parent)
            ,   m_indexVector(other.m_indexVector)
        {
            other.m_indexVector = nullptr;
            other.m_parent = nullptr;
            UpdateChildren();
        }
        template<typename AllocatorPackT>
        void Copy(const RadixTreeNode& source, AllocatorPackT& allocatorPack)
        {
            SetCurrentCharacter(source.GetCurrentCharacter());
            Resize(source.m_startOfArray, source.m_arrayLength, allocatorPack);
            if(source.HasValue() && source.HasRemainder())
                SwapParent(allocatorPack, allocatorPack.ConstructNodeWithValueAndRemainder(GetParent(), source.GetStoredValue(), source.GetRemainder()));
            else if(source.HasRemainder())
                SwapParent(allocatorPack, allocatorPack.ConstructNodeWithRemainder(GetParent(), source.GetRemainder()));
            else if(source.HasValue())
                SwapParent(allocatorPack, allocatorPack.ConstructNodeWithValue(GetParent(), source.GetStoredValue()));
            SetHasValue(source.HasValue());
            SetHasRemainder(source.HasRemainder());
            for(size_t i = 0; i < m_arrayLength; ++i)
            {
                m_indexVector[i].Copy(source.m_indexVector[i], allocatorPack);
            }
        }
        RadixTreeNode()
            :   m_parent(0)
            ,   m_indexVector(0)
        {}
        RadixTreeNode& operator=(RadixTreeNode&& other)
        {
            RadixTreeNodeMetaData::operator=(std::move(other));
            std::swap(m_indexVector,other.m_indexVector);
            std::swap(m_parent,other.m_parent);
            UpdateChildren();
            return *this;
        }
        template<typename AllocatorPackT>
        RadixTreeNode* GetOrCreateNewNode(const char* text, const TraitsT& traits, AllocatorPackT& allocatorPack)
        {
            if(traits.IsTerminatingCharacter(*text))
            {
                if(HasRemainder())
                    MoveSelfOneLevelUp(0, allocatorPack);
                return this;
            }
            if(!HasRemainder() && !HasValue() && m_arrayLength == 0)
            {
                SetRemainder(text, allocatorPack);
                return this;
            }
            const RemainderType& remainder = GetRemainder();
            size_t i = FindFirstDifferingLocation(remainder.c_str(), text, remainder.length());
            if(i == 0)
            {
                if(HasRemainder())
                    MoveSelfOneLevelUp(0, allocatorPack);
                return CreateNodeAt(0, text, traits, allocatorPack);
            }
            else
            {
                auto newRemainder = remainder.substr(0,i);
                if(newRemainder == remainder)
                {
                    if(traits.IsTerminatingCharacter(text[i]))
                        return this;
                    ResizeToFitText(text[i], allocatorPack);
                    RadixTreeNode* nextNode = GetChildAt(text[i] - m_startOfArray);
                    return nextNode->GetOrCreateNewNode(text+i+1, traits, allocatorPack);
                }
                else
                {
                    MoveSelfOneLevelUp((int)i, allocatorPack);
                    if(traits.IsTerminatingCharacter(text[i]))
                        return this;
                    return CreateNodeAt((int)i, text, traits, allocatorPack);
                }
            }
        }
        const RadixTreeNode* FindImpl(const char* text, const TraitsT& traits) const
        {
            return const_cast<RadixTreeNode*>(this)->FindImpl(text, traits);
        }
        RadixTreeNode* FindImpl(const char* text, const TraitsT& traits)
        {
            if(HasRemainder())
            {
                const RemainderType& remainder = GetNodeWithRemainder()->GetRemainder();
                if(memcmp(remainder.c_str(), text, remainder.size())==0)
                    text += remainder.size();
                else
                    return nullptr;
            }
            if(traits.IsTerminatingCharacter(*text))
                return this;
            else
            {
                unsigned short index = *text - m_startOfArray;
                if( index < m_arrayLength)
                {
                    RadixTreeNode* node = GetChildAt(index);
                    return node->FindImpl(text + 1, traits);
                }
            }
            return nullptr;
        }
        const RadixTreeNode* LowerOrUpperBound(const char* text, const TraitsT& traits, bool isLower) const
        {
            if(HasRemainder())
            {
                const RemainderType& remainder = GetNodeWithRemainder()->GetRemainder();
                switch(memcmp(text, remainder.c_str(), remainder.size()))
                {
                case -1:
                    return GetFirst();
                case 0:
                    text += remainder.size();
                    break;
                case 1:
                    return GetNext();
                }
            }
            if(traits.IsTerminatingCharacter(*text))
            {
                if(isLower)
                    return this;
                else
                    return GetNext();
            }
            else
            {
                unsigned short index = *text - m_startOfArray;
                if( index < m_arrayLength)
                {
                    const RadixTreeNode* node = GetChildAt(index);
                    if(node->IsEmpty())
                        return GetNextAfter(*text);
                    else return node->LowerOrUpperBound(text + 1, traits, isLower);
                }
                else if(m_arrayLength > 0)
                {
                    return GetNextAfter(*text);
                }
            }
            return nullptr;
        }

        const StoredValueType& GetStoredValue() const {
            if(HasRemainder())
                return GetNodeWithValueAndRemainder()->GetStoredValue();
            else
                return GetNodeWithValue()->GetStoredValue();
        }
        ValueT& GetMappedValueForEdit()
        {
            if(HasRemainder())
                return ValueTypeComputer::GetMappedValue(GetNodeWithValueAndRemainder()->GetStoredValueForEdit());
            else
                return ValueTypeComputer::GetMappedValue(GetNodeWithValue()->GetStoredValueForEdit());
        }
        template<typename AllocatorPackT>
        void SwapParent(AllocatorPackT& allocatorPack, RadixTreeNode* newNode)
        {
            DestroyMembers(allocatorPack);
            m_parent = newNode;
        }
        const ValueT& GetMappedValue() const
        {
            return ValueTypeComputer::GetMappedValue(GetStoredValue());
        }
        typename ValueTypeComputer::KeyType GetKey() const
        {
            return ValueTypeComputer::GetKey(this);
        }
        template<typename AllocatorPackT>
        void SetValue(const ValueType& value, AllocatorPackT& allocatorPack)
        {
            if(HasRemainder() && HasValue())
                GetNodeWithValueAndRemainder()->SetStoredValue(ValueTypeComputer::GetStoredValue(value));
            else if(HasValue())
                GetNodeWithValue()->SetStoredValue(ValueTypeComputer::GetStoredValue(value));
            else if(HasRemainder())
                SwapParent(allocatorPack, allocatorPack.ConstructNodeWithValueAndRemainder(GetParent(), ValueTypeComputer::GetStoredValue(value), GetRemainder()));
            else
                m_parent = allocatorPack.ConstructNodeWithValue(GetParent(), ValueTypeComputer::GetStoredValue(value));
            SetHasValue(true);
        }
        template<typename AllocatorPackT>
        void CleanupChildrenIfEmpty(AllocatorPackT& allocatorPack)
        {
            for(size_t i = 0; i < m_arrayLength; ++i)
            {
                if(!m_indexVector[i].IsEmpty())
                    return;
            }
            Destroy(allocatorPack);
            m_startOfArray = 0;
            m_arrayLength = 0;
            m_indexVector = nullptr;
            if(!HasValue() && GetParent())
                GetParent()->CleanupChildrenIfEmpty(allocatorPack);
        }
        template<typename AllocatorPackT>
        void RemoveValue(AllocatorPackT& allocatorPack)
        {
            if(HasValue() && HasRemainder())
                SwapParent(allocatorPack, allocatorPack.ConstructNodeWithRemainder(GetParent(), GetRemainder()));
            else if(HasValue())
                SwapParent(allocatorPack, GetParent());
            SetHasValue(false);            
        }
        template<typename AllocatorPackT>
        void Destroy(AllocatorPackT& allocatorPack)
        {
            for(size_t i = 0; i < m_arrayLength; ++i)
            {
                m_indexVector[i].Destroy(allocatorPack);
                m_indexVector[i].DestroyMembers(allocatorPack);
            }
            allocatorPack.DestroyNodes(m_indexVector, m_arrayLength);
        }
        template<typename AllocatorPackT>
        void DestroyMembers(AllocatorPackT& allocatorPack)
        {
            if(HasValue() &&  HasRemainder())
                allocatorPack.DestroyNode(GetNodeWithValueAndRemainder());
            else if(HasValue())
                allocatorPack.DestroyNode(GetNodeWithValue());
            else if(HasRemainder())
                allocatorPack.DestroyNode(GetNodeWithRemainder());
        }

        ~RadixTreeNode()
        {
        }
        RadixTreeNode* GetLast()
        {
            for(int i = m_arrayLength - 1; i >= 0; --i)
            {
                RadixTreeNode* node = m_indexVector[i].GetLast();
                if(node != nullptr)
                    return node;
            }
            if(HasValue())
                return this;
            return nullptr;
        }
        const RadixTreeNode* GetLast() const
        {
            for(int i = m_arrayLength-1; i >= 0; --i)
            {
                const RadixTreeNode* node = m_indexVector[i].GetLast();
                if(node != nullptr)
                    return node;
            }
            if(HasValue())
                return this;
            return nullptr;
        }
        RadixTreeNode* GetFirst()
        {
            if(HasValue())
                return this;
            for(size_t i = 0; i < m_arrayLength; ++i)
            {
                RadixTreeNode* node = m_indexVector[i].GetFirst();
                if(node != nullptr)
                    return node;
            }
            return nullptr;
        }
        const RadixTreeNode* GetFirst() const
        {
            if(HasValue())
                return this;
            for(size_t i = 0; i < m_arrayLength; ++i)
            {
                const RadixTreeNode* node = m_indexVector[i].GetFirst();
                if(node != nullptr)
                    return node;
            }
            return nullptr;
        }
        const RadixTreeNode* GetNext() const
        {
            for(size_t i = 0; i < m_arrayLength; ++i)
            {
                const RadixTreeNode* node = m_indexVector[i].GetFirst();
                if(node != nullptr)
                    return node;
            }
            const RadixTreeNode* node = this;
            while(const RadixTreeNode* parent = node->GetParent())
            {
                const RadixTreeNode* nextNode = parent->InternalGetNextAfter(node->GetCurrentCharacter());
                if(nextNode)
                    return nextNode;
                else
                    node = parent;
            }
            return nullptr;
        }
        const RadixTreeNode* GetPrior() const
        {
            const RadixTreeNode* node = this;
            while(const RadixTreeNode* parent = node->GetParent())
            {
                if(parent->HasValue())
                    return parent;
                const RadixTreeNode* priorNode = parent->InternalGetPriorBefore(node->GetCurrentCharacter());
                if(priorNode)
                    return priorNode;
                else
                    node = parent;
            }
            return nullptr;
        }
        const RadixTreeNode* GetNextAfter(char character) const
        {
            int start = character - m_startOfArray + 1;
            if(start < 0) start = 0;
            for(size_t i = start; i < m_arrayLength; ++i)
            {
                const RadixTreeNode* node = m_indexVector[i].GetFirst();
                if(node != nullptr)
                    return node;
            }
            const RadixTreeNode* node = this;
            while(const RadixTreeNode* parent = node->GetParent())
            {
                const RadixTreeNode* nextNode = parent->InternalGetNextAfter(node->GetCurrentCharacter());
                if(nextNode)
                    return nextNode;
                else
                    node = parent;
            }
            return nullptr;
        }
        const RadixTreeNode* InternalGetPriorBefore(char character) const
        {
            int index = character - m_startOfArray - 1;
            for(int i = index; i >= 0; --i)
            {
                const RadixTreeNode* node = m_indexVector[i].GetLast();
                if(node != nullptr)
                    return node;
            }
            return nullptr;
        }
        const RadixTreeNode* InternalGetNextAfter(char character) const
        {
            for(size_t i = character - m_startOfArray + 1; i < m_arrayLength; ++i)
            {
                const RadixTreeNode* node = m_indexVector[i].GetFirst();
                if(node != nullptr)
                    return node;
            }
            return nullptr;
        }
        const RadixTreeNode* GetParent() const
        {
            if(HasValue() || HasRemainder())
                return m_parent->GetParent();            
            return m_parent;
        }
        RadixTreeNode* GetParent()
        {
            if(HasValue() || HasRemainder())
                return m_parent->GetParent();            
            return m_parent;
        }
        const RemainderType& GetRemainder() const
        {
            if(HasRemainder())
                return GetNodeWithRemainder()->GetRemainder();
            else
                return m_empty;
        }

    protected:
        RadixTreeNode(RadixTreeNode* parent, char currentCharacter)
            :   m_parent(parent)
            ,   m_indexVector(nullptr)
        {
            m_currentCharacter = currentCharacter;
        }
    private:
        void SetParent(RadixTreeNode* parent)
        {
            if(HasValue() || HasRemainder())
                m_parent->SetParent(parent);
            else
                m_parent = parent;
        }

        template<typename AllocatorPackT>
        void RemoveRemainderImpl(AllocatorPackT& allocatorPack)
        {
            if(HasValue() && HasRemainder())
                SwapParent(allocatorPack, allocatorPack.ConstructNodeWithValue(GetParent(), GetStoredValue()));
            else if(HasRemainder())
                SwapParent(allocatorPack, GetParent());
            SetHasRemainder(false);
        }
        template<typename AllocatorPackT>
        void SetRemainderImpl(const RemainderType& newRemainder, AllocatorPackT& allocatorPack)
        {
            if(HasRemainder())
                GetNodeWithRemainder()->SetRemainder(newRemainder);
            else if(HasValue())
                SwapParent(allocatorPack, allocatorPack.ConstructNodeWithValueAndRemainder(GetParent(), GetStoredValue(), newRemainder));
            else
                m_parent = allocatorPack.ConstructNodeWithRemainder(m_parent, newRemainder);
            SetHasRemainder(true);
        }
        template<typename AllocatorPackT>
        void SetRemainder(const RemainderType& newRemainder, AllocatorPackT& allocatorPack)
        {
            if(newRemainder.empty())
                RemoveRemainderImpl(allocatorPack);
            else
                SetRemainderImpl(newRemainder, allocatorPack);
        }

        const RadixTreeNode* GetChildAt(size_t index) const
        {
            return &m_indexVector[index];
        }
        RadixTreeNode* GetChildAt(size_t index)
        {
            return &m_indexVector[index];
        }

        void SetChildAt(size_t index, RadixTreeNode&& child)
        {
            m_indexVector[index] = std::move(child);
        }
        void UpdateChildren()
        {
            for(size_t i = 0; i < m_arrayLength; ++i)
            {
                m_indexVector[i].SetParent(this);
                m_indexVector[i].SetCurrentCharacter((char)(m_startOfArray + i));
            }
        }

        template<typename AllocatorPackT>
        void MoveSelfOneLevelUp(int index, AllocatorPackT& allocatorPack)
        {
            RemainderType remainder = GetRemainder();
            RadixTreeNode node(std::move(*this));
            node.SetRemainder(remainder.substr(index + 1), allocatorPack);
            SetCurrentCharacter(node.GetCurrentCharacter());
            node.SetCurrentCharacter(remainder[index]);
            SetRemainder(remainder.substr(0, index), allocatorPack);
            SetParent(node.GetParent());
            node.SetParent(this);
            ResizeToFitText(remainder[index], allocatorPack);
            SetChildAt(remainder[index] - m_startOfArray, std::move(node));
        }
        template<typename IsTerminatingCharacter, typename AllocatorPackT>
        RadixTreeNode* CreateNodeAt(int index, const char* text, const IsTerminatingCharacter& isTerminatingCharacter, AllocatorPackT& allocatorPack)
        {
            ResizeToFitText(text[index], allocatorPack);
            RadixTreeNode* node = GetChildAt(text[index] - m_startOfArray);
            return node->GetOrCreateNewNode(text + index + 1, isTerminatingCharacter, allocatorPack);
        }
        
        template<typename AllocatorPackT>
        void ResizeToFitText(char letter, AllocatorPackT& allocatorPack)
        {
            if(m_arrayLength == 0)
                Resize(letter, 1, allocatorPack);
            else if(letter < m_startOfArray)
                Resize(letter, m_arrayLength + m_startOfArray - letter, allocatorPack);
            else if(m_startOfArray + m_arrayLength - 1 < letter)
                Resize(m_startOfArray, letter - m_startOfArray + 1, allocatorPack);
        }
        
        template<typename AllocatorPackT>
        void Resize(char newStartOfArray, short newLengthOfArray, AllocatorPackT& allocatorPack)
        {
            RadixTreeNode* newArray = allocatorPack.ConstructNodes(newLengthOfArray);
            size_t offset = m_startOfArray - newStartOfArray;
            for(size_t i = 0; i < m_arrayLength; ++i)
                newArray[i + offset] = std::move(m_indexVector[i]);
            allocatorPack.DestroyNodes(m_indexVector, m_arrayLength);
            m_indexVector = newArray;
            m_startOfArray = newStartOfArray;
            m_arrayLength = newLengthOfArray;
            UpdateChildren();
        }

        RadixTreeNodeWithRemainder<KeyT, ValueT, TraitsT>* GetNodeWithRemainder() { return static_cast<RadixTreeNodeWithRemainder<KeyT, ValueT, TraitsT>*>(m_parent); }
        const RadixTreeNodeWithRemainder<KeyT, ValueT, TraitsT>* GetNodeWithRemainder() const { return static_cast<RadixTreeNodeWithRemainder<KeyT, ValueT, TraitsT>*>(m_parent); }
        RadixTreeNodeWithValueAndRemainder<KeyT, ValueT, TraitsT>* GetNodeWithValueAndRemainder() { return static_cast<RadixTreeNodeWithValueAndRemainder<KeyT, ValueT, TraitsT>*>(m_parent); }
        const RadixTreeNodeWithValueAndRemainder<KeyT, ValueT, TraitsT>* GetNodeWithValueAndRemainder() const { return static_cast<RadixTreeNodeWithValueAndRemainder<KeyT, ValueT, TraitsT>*>(m_parent); }
        RadixTreeNodeWithValue<KeyT, ValueT, TraitsT>* GetNodeWithValue() { return static_cast<RadixTreeNodeWithValue<KeyT, ValueT, TraitsT>*>(m_parent); }
        const RadixTreeNodeWithValue<KeyT, ValueT, TraitsT>* GetNodeWithValue() const { return static_cast<RadixTreeNodeWithValue<KeyT, ValueT, TraitsT>*>(m_parent); }

        static RemainderType m_empty;
        RadixTreeNode* m_parent;
        RadixTreeNode* m_indexVector;        
    };

    template<typename KeyT, typename ValueT, typename TraitsT>
    typename RadixTreeNode<KeyT, ValueT, TraitsT>::RemainderType RadixTreeNode<KeyT, ValueT, TraitsT>::m_empty;

    template<typename KeyT, typename ValueT, typename TraitsT>
    class RadixTreeNodeWithRemainder : public RadixTreeNode<KeyT, ValueT, TraitsT>
    {
    public:
        RadixTreeNodeWithRemainder(RadixTreeNode<KeyT, ValueT, TraitsT>* parent, const RemainderType& remainder)
            :   RadixTreeNode<KeyT, ValueT, TraitsT>(parent, 0)
            ,   m_remainder(remainder)
        {}
        RadixTreeNodeWithRemainder(RadixTreeNodeWithRemainder&& other)
            :   RadixTreeNode<KeyT, ValueT, TraitsT>(other.GetParent(), 0)
            ,   m_remainder(std::move(other.m_remainder))
        {}
        const RemainderType& GetRemainder() const { return m_remainder; }
        void SetRemainder(const RemainderType& remainder) { m_remainder = remainder; }
    private:
        RemainderType m_remainder;
    };

    template<typename KeyT, typename ValueT, typename TraitsT>
    class RadixTreeNodeWithValue : public RadixTreeNode<KeyT, ValueT, TraitsT>
    {
    public:
        RadixTreeNodeWithValue(RadixTreeNode<KeyT, ValueT, TraitsT>* parent, const StoredValueType& value)
            :   RadixTreeNode<KeyT, ValueT, TraitsT>(parent, 0)
            ,   m_value(value)
        {}
        RadixTreeNodeWithValue(RadixTreeNodeWithValue&& other)
            :   RadixTreeNode<KeyT, ValueT, TraitsT>(other.GetParent(), 0)
            ,   m_value(std::move(other.m_value))
        {}
        const StoredValueType& GetStoredValue() const { return m_value; }
        StoredValueType& GetStoredValueForEdit() { return m_value; }
        void SetStoredValue(const StoredValueType& value) { m_value = value; }
    private:
        StoredValueType m_value;
    };

    template<typename KeyT, typename ValueT, typename TraitsT>
    class RadixTreeNodeWithValueAndRemainder : public RadixTreeNodeWithRemainder<KeyT, ValueT, TraitsT>
    {
    public:
        RadixTreeNodeWithValueAndRemainder(RadixTreeNodeWithValueAndRemainder&& other)
            :   RadixTreeNodeWithRemainder<KeyT, ValueT, TraitsT>(std::move(other))
            ,   m_value(std::move(other.m_value))
        {}
        RadixTreeNodeWithValueAndRemainder(RadixTreeNode<KeyT, ValueT, TraitsT>* parent, const StoredValueType& value, const RemainderType& remainder)
            :   RadixTreeNodeWithRemainder<KeyT, ValueT, TraitsT>(parent, remainder)
            ,   m_value(value)
        {}
        const StoredValueType& GetStoredValue() const { return m_value; }
        StoredValueType& GetStoredValueForEdit() { return m_value; }
        void SetStoredValue(const StoredValueType& value) { m_value = value; }
    private:
        StoredValueType m_value;
    };
}}}}
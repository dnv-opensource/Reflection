//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Variants/Variant.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Variants {

    Variant::Variant() : m_data(nullptr)
    {

    }

    Variant::Variant(void* data, const Types::DecoratedTypeInfo& info) : m_data(data)
        , m_info(info)
    {

    }

    Variant::Variant(StoragePointer storage, const Types::DecoratedTypeInfo& info) : m_storage(storage)
        , m_data(storage->GetData())
        , m_info(info)
    {

    }

    Variant::Variant(const Variant& other) : m_storage(other.m_storage)
        , m_data(other.m_data)
        , m_info(other.m_info)
    {

    }

    Variant::~Variant()
    {

    }

    void* Variant::Data() const
    {
        if (m_storage && !m_storage->GetData())
            return nullptr;
        return m_data;
    }

    void Variant::SetData(void* data)
    {
        m_data = data;
    }

    void* Variant::GetData() const
    {
        return Data();
    }

    void Variant::SetStorage(StoragePointer storage)
    {
        m_storage = storage;
        m_data = m_storage->GetData();
    }

    bool Variant::IsValid() const
    {
        if (GetDecoratedTypeInfo().GetTypeInfoPointer() == nullptr)
            return false;
//         if (m_data != nullptr && m_storage && m_storage->GetData() == nullptr)
//             return false;
        return true;
    }

    Variants::Variant& Variant::operator=(const Variant& var)
    {
        if (this != &var)
        {
            m_storage = var.m_storage;
            m_data = var.m_data;
            m_info = var.m_info;
        }
        return *this;
    }

}}}}
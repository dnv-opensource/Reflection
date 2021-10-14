#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/Config.h"
#include "Reflection/Types/DecoratedTypeInfo.h"
#include "Reflection/Variants/StorageBase.h"

#include <string>
#include <vector>

namespace DNVS {namespace MoFa { namespace Reflection { namespace Variants {
    class REFLECTION_IMPORT_EXPORT Variant
    {
    public:
        Variant();
        Variant(void* data, const Types::DecoratedTypeInfo& info);
        
        Variant(StoragePointer storage, const Types::DecoratedTypeInfo& info);

        Variant(const Variant& other);        
        ~Variant();
        
        Variant& operator=(const Variant& var);

        void* Data() const;
        void SetData(void* data);
        void* GetData() const;
        const Types::DecoratedTypeInfo& GetDecoratedTypeInfo() const {return m_info; }
        void SetDecoratedTypeInfo(const Types::DecoratedTypeInfo& info) {m_info = info; }
        StoragePointer GetStorage() const {return m_storage; }
        void SetStorage(StoragePointer storage);
        bool IsValid() const;
    private:
        void* m_data = nullptr;
        Types::DecoratedTypeInfo m_info;
        StoragePointer m_storage;
    };

}}}}
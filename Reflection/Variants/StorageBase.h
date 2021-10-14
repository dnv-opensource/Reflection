#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "TypeUtilities\IntrusiveClass.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Variants {

    class StorageBase : public TypeUtilities::IntrusiveClass<StorageBase>
    {
    public:
        virtual ~StorageBase() {}
        virtual void* GetData() const {return m_data;}
    protected:
        void* m_data = nullptr;
    };

    typedef TypeUtilities::IntrusivePointer<StorageBase> StoragePointer;

}}}}
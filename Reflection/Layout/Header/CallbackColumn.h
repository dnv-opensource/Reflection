#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ColumnWithHeader.h"
#include "TypeUtilities/FunctionTraits.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Layout {
    template<typename CallbackT>
    class CallbackColumnClass : public GenericColumn<CallbackColumnClass<CallbackT>>
    {
    public:
        CallbackColumnClass(const std::string& header, const CallbackT& callback)
            : GenericColumn<CallbackColumnClass>(header)
            , m_callback(callback) 
        {}
        virtual Objects::Object Query(const Objects::Object& data) const override
        {
            typedef typename TypeUtilities::FunctionTraits<CallbackT>::Argument0 ArgumentType;
            typedef typename TypeUtilities::FunctionTraits<CallbackT>::ReturnType ReturnType;
            if (data.IsConvertibleTo<ArgumentType>())
                return Objects::Object::Create<ReturnType>(data.GetTypeLibrary(), m_callback(data.As<ArgumentType>()));
            else 
                return Objects::Object();
        }
        virtual Types::DecoratedTypeInfo GetType(TypeLibraries::TypeLibraryPointer typeLibrary, const Types::DecoratedTypeInfo& parentType) const override
        {
            typedef typename TypeUtilities::FunctionTraits<CallbackT>::ReturnType ReturnType;
            return Types::TypeId<ReturnType>();
        }
    private:
        CallbackT m_callback;
    };
    template<typename CallbackT>
    CallbackColumnClass<CallbackT> CallbackColumn(const std::string& header, const CallbackT& callback)
    {
        return CallbackColumnClass<CallbackT>(header, callback);
    }
}}}}

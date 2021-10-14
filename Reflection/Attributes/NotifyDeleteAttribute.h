#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <functional>
#include <list>
#include "Reflection/Objects/Object.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {
    class NotifyDeleteAttribute
    {
    public:
        using InnerCallback = std::function<void(const Variants::Variant& variant)>;
        using ConnectCallback = std::function<Variants::Variant(const InnerCallback&, const std::type_info&)>;

        template<size_t Arity>
        struct CreateCallback;
        template<typename T>
        struct GetFunctionArgument;
        template<typename T>
        struct GetFunctionArgument<std::function<void(T)>>
        {
            using Type = T;
        };
        template<>//Only value passed
        struct CreateCallback<1>
        {
            template<typename NotifyCallback>
            static ConnectCallback Create(NotifyCallback notifyCallback)
            {
                return [=](InnerCallback f, const std::type_info& typeInfo) -> Variants::Variant
                {
                    using ReturnType = typename TypeUtilities::FunctionTraits<NotifyCallback>::ReturnType;
                    using Argument0 = typename GetFunctionArgument<typename TypeUtilities::FunctionTraits<NotifyCallback>::Argument0>::Type;
                    auto fn = [f](Argument0 argument0) {
                        f(Variants::VariantService::Reflect(std::forward<Argument0>(argument0)));
                    };
                    return Variants::VariantService::Reflect(notifyCallback(fn));
                };
            }
        };
        template<>//Only value passed
        struct CreateCallback<2>
        {
            template<typename NotifyCallback>
            static ConnectCallback Create(NotifyCallback notifyCallback)
            {
                return [=](InnerCallback f, const std::type_info& typeInfo) -> Variants::Variant
                {
                    using ReturnType = typename TypeUtilities::FunctionTraits<NotifyCallback>::ReturnType;
                    using Argument0 = typename GetFunctionArgument<typename TypeUtilities::FunctionTraits<NotifyCallback>::Argument0>::Type;
                    auto fn = [f](Argument0 argument0) -> void {
                        f(Variants::VariantService::Reflect(std::forward<Argument0>(argument0)));
                    };
                    return Variants::VariantService::Reflect(notifyCallback(fn, typeInfo));
                };
            }
        };
        template<typename NotifyCallback>
        NotifyDeleteAttribute(NotifyCallback notifyCallback)
        {
            typedef typename TypeUtilities::FunctionTraits<NotifyCallback>::ReturnType ReturnType;
            typedef typename TypeUtilities::FunctionTraits<NotifyCallback>::Argument0 Argument0;
            m_notifyCallback = CreateCallback<TypeUtilities::FunctionTraits<NotifyCallback>::Arity>::Create(notifyCallback);
        }
        Variants::Variant ConnectNotifications(const InnerCallback& fn, const std::type_info& info) const
        {
            if (m_notifyCallback)
                return m_notifyCallback(fn, info);
            else
                return Variants::Variant();
        }
    private:
        ConnectCallback m_notifyCallback;
    };
}}}}
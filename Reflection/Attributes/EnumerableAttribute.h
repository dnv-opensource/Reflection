#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <functional>
#include <list>
#include "Reflection/Objects/Object.h"
#include "FunctionCallbackAttribute.h"
#include "IAttribute.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {
    /**
    The EnumerableAttribute is used in reflection in order to list all fixed choices for a variable.
    Example: This is how we add the values true and false to a list of alternatives for bool.  
    Class<bool> cls(typeLibrary, "bool");
    cls.AddAttribute<EnumerableAttribute>([]() -> std::list<Variants::Variant> 
    {
        TypeLibraryPointer typeLibrary = weakTypeLibrary.lock();
        std::list<Variants::Variant> objects;
        objects.push_back(Variants::VariantService::ReflectType<bool>(true));
        objects.push_back(Variants::VariantService::ReflectType<bool>(false));
    });
    */    
    namespace Impl {
        struct DisableSorting {};
    }
    constexpr Impl::DisableSorting DisableSorting;

    class EnumerableAttribute 
        : public FunctionCallbackAttribute<std::list<Variants::Variant>>
        , public IAttribute
    {
    public:
        template<typename... Args>
        EnumerableAttribute(Impl::DisableSorting, Args&&... args)
            :EnumerableAttribute(std::forward<Args>(args)...)
        {
            m_enableSorting = false;
        }
        template<typename Callback, typename... Args>
        EnumerableAttribute(Callback callback, Args&&... args)
            : m_enableSorting(true)
        {
            InitializeFunction(this, callback, std::forward<Args>(args)...);
        }
        template<typename Callback, typename... Args>
        EnumerableAttribute(const TypeLibraries::TypeLibraryPointer& typeLibrary, Callback callback, Args&&... args)
            : m_enableSorting(true)
        {
            InitializeFunction(this, typeLibrary, callback, std::forward<Args>(args)...);
        }

        std::list<Variants::Variant> EnumerateObjects() const
        {
            if (GetStringArguments().empty())
                return InvokeFunction({});
            else
                return {};
        }
        Variants::Variant ConnectNotifications(const std::function<void()>& fn) const
        {
            if (m_notifyCallback)
                return m_notifyCallback(fn);
            else
                return Variants::Variant();
        }
        void InitializeArguments() {}
        template<typename NotifyCallback>
        void InitializeArguments(NotifyCallback notifyCallback) {
            typedef typename TypeUtilities::FunctionTraits<NotifyCallback>::ReturnType ReturnType;
            m_notifyCallback = [=](const std::function<void()>& f)
            {
                return Variants::VariantService::Reflect(notifyCallback(f));
            };
        }
        bool EnableSorting() const { return m_enableSorting; }
    private:
        bool m_enableSorting;
        std::function<Reflection::Variants::Variant(const std::function<void()>&)> m_notifyCallback;
    };
}}}}

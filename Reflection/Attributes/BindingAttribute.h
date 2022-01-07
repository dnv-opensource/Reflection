#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "FunctionCallbackAttribute.h"
#include "IAttribute.h"
#include "TypeUtilities/FunctionTraits.h"


namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {
    /*
    Binding attribute. Allows you to bind this value to one or more other values.
    */

    namespace Impl {
        class ForceBinding {};
        class BindToExisting {};
    }
    constexpr Impl::ForceBinding ForceBinding;
    //Only bind if the bind expression represents existing data, such as an existing beam or similar. Do not bind if the expression represents for example a constructor expression.
    constexpr Impl::BindToExisting BindToExisting;

    class BindingAttribute 
        : public FunctionCallbackAttribute<Variants::Variant>
        , public IAttribute
    {
    public:
        template<typename BindingControlFunction, typename... StringArguments>
        BindingAttribute(BindingControlFunction bindingControlFunction, StringArguments&&... stringArguments)
            : m_forceBinding(false)
            , m_bindToExisting(false)
        {
            InitializeFunction(this, bindingControlFunction, std::forward<StringArguments>(stringArguments)...);
        }
        template<typename BindingControlFunction, typename... StringArguments>
        BindingAttribute(const TypeLibraries::TypeLibraryPointer& typeLibrary, BindingControlFunction bindingControlFunction, StringArguments&&... stringArguments)
            : m_forceBinding(false)
            , m_bindToExisting(false)
        {
            InitializeFunction(this, typeLibrary, bindingControlFunction, std::forward<StringArguments>(stringArguments)...);
        }
        template<typename... Args>
        BindingAttribute(Impl::ForceBinding forceBinding, Args&&... args)
            : BindingAttribute(std::forward<Args>(args)...)
        {
            InitializeArguments(forceBinding);
        }
        template<typename... Args>
        BindingAttribute(Impl::BindToExisting bindToExisting, Args&&... args)
            : BindingAttribute(std::forward<Args>(args)...)
        {
            InitializeArguments(bindToExisting);
        }
        Variants::Variant Get(const std::vector<Objects::Object>& arguments) const
        {
            return InvokeFunction(arguments);
        }
        void InitializeArguments() {}
        template<typename... Args>
        void InitializeArguments(Impl::ForceBinding forceBinding, Args&&... args) {
            m_forceBinding = true;
            InitializeArguments(std::forward<Args>(args)...);
        }
        template<typename... Args>
        void InitializeArguments(Impl::BindToExisting bindToExisting, Args&&... args) {
            m_bindToExisting = true;
            InitializeArguments(std::forward<Args>(args)...);
        }
        template<typename NotifyCallback, typename = std::enable_if_t<TypeUtilities::HasConvertibleSignature<NotifyCallback, TypeUtilities::AnyType(const std::function<void()>&)>>>
        void InitializeArguments(NotifyCallback notifyCallback) {
            typedef typename TypeUtilities::FunctionTraits<NotifyCallback>::ReturnType ReturnType;
            m_notifyCallback = [=](const std::function<void()>& f)
            {
                return Variants::VariantService::Reflect(notifyCallback(f));
            };
        }
        bool ForceBinding() const { return m_forceBinding; }
        bool BindToExisting() const { return m_bindToExisting; }
        Variants::Variant ConnectNotifications(const std::function<void()>& fn) const
        {
            if (m_notifyCallback)
                return m_notifyCallback(fn);
            else
                return Variants::Variant();
        }
    private:
        bool m_forceBinding;
        bool m_bindToExisting;
        std::function<Reflection::Variants::Variant(const std::function<void()>&)> m_notifyCallback;
    };
}}}}

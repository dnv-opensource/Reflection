#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Variants/VariantService.h"
#include "DefaultOptions.h"
#include "FunctionCallbackAttribute.h"
#include "TypeUtilities/IsCallable.h"
#include "IAttribute.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    /**
    Use the DefaultAttribute on any member property that you want to have a default value in a dialog.
    Alternatively, you can create a constructor with named arguments, and supply a default value there instead.
    However, this will change the behavior of the program, since you will be allowed to call the constructor with fewer constructor arguments.
    */
    class DefaultAttribute 
        : public FunctionCallbackAttribute<Variants::Variant>
        , public IAttribute
    {
        DefaultAttribute() : m_storeDefault(false) {}
    public:
        template<typename... Args>
        DefaultAttribute(Impl::StoreDefault storeDefault, Args&&... args)
            : DefaultAttribute(std::forward<Args>(args)...)
        {
            InitializeArguments(storeDefault);
        }
        template<typename... Args>
        DefaultAttribute(Args&&... args)
            : m_storeDefault(false)
        {
            InitializeDefaultValue(std::forward<Args>(args)...);
        }        
        bool StoreDefault() const { return m_storeDefault; }
        
        void InitializeArguments() {}
        template<typename... Args>
        void InitializeArguments(Impl::StoreDefault storeDefault, Args&&... args) {
            m_storeDefault = true;
            InitializeArguments(std::forward<Args>(args)...);
        }
    private:
        template<typename Function, typename... Args>
        std::enable_if_t<TypeUtilities::IsCallable<Function>> InitializeDefaultValue(const Reflection::TypeLibraries::TypeLibraryPointer& typeLibrary, Function callback, Args&&... args)
        {
            InitializeFunction(this, typeLibrary, callback, std::forward<Args>(args)...);
        }
        template<typename... Args>
        void InitializeDefaultValue(const Reflection::TypeLibraries::TypeLibraryPointer& typeLibrary, Members::MemberPointer member, Args&&... args)
        {
            InitializeFunction(this, typeLibrary, member, std::forward<Args>(args)...);
        }

        template<typename Function, typename... Args>
        std::enable_if_t<TypeUtilities::IsCallable<Function>> InitializeDefaultValue(Function callback, Args&&... args)
        {
            InitializeFunction(this, callback, std::forward<Args>(args)...);
        }
        template<typename T, typename... Args>
        std::enable_if_t<!TypeUtilities::IsCallable<T>> InitializeDefaultValue(const T& defaultValue, Args&&... args)
        {
            Variants::Variant var = Variants::VariantService::ReflectType(defaultValue);
            InitializeFunction(this, [=]() { return var; });
            InitializeArguments(std::forward<Args>(args)...);
        }
        bool m_storeDefault;
    };
}}}}

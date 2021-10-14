#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "FunctionCallbackAttribute.h"
#include "ArgumentTypeVisitorOffset.h"
#include "FunctionWrapperVararg.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {

    template<typename T, typename... Args>
    class FunctionCallbackAttribute<T(Args...)>
    {
    public:
        using ReturnType = T;
        template<typename FunctionT, typename... StringArguments>
        FunctionCallbackAttribute(FunctionT function, StringArguments&&... stringArguments)
        {
            InitializeFunction(this, function, std::forward<StringArguments>(stringArguments)...);
        }
        template<typename FunctionT, typename... StringArguments>
        FunctionCallbackAttribute(const TypeLibraries::TypeLibraryPointer& typeLibrary, FunctionT function, StringArguments&&... stringArguments)
        {
            InitializeFunction(this, typeLibrary, function, std::forward<StringArguments>(stringArguments)...);
        }
        const Types::DecoratedTypeInfo& GetReturnType() const { return m_returnType; }
        const std::vector<Types::DecoratedTypeInfo>& GetArgumentTypes() const { return m_argumentTypes; }
        const std::vector<std::string>& GetStringArguments() const { return m_stringArguments; }
        bool HasFunction() const { return m_function != nullptr; }
        T InvokeFunction(Args&&... fixedArgs, const std::vector<Objects::Object>& args) const {
            return m_function(std::forward<Args>(fixedArgs)..., args);
        }
        FunctionCallbackAttribute() {}
        template<typename DerivedT, typename FunctionT, typename... StringArguments>
        void InitializeFunction(DerivedT* derived, FunctionT function, StringArguments&&... stringArguments)
        {
            constexpr bool hasRightNumberOfArguments = std::is_same_v<DerivedT, FunctionCallbackAttribute> ? (TypeUtilities::FunctionTraits<FunctionT>::Arity - sizeof...(Args) == sizeof...(StringArguments)) : (TypeUtilities::FunctionTraits<FunctionT>::Arity - sizeof...(Args) <= sizeof...(StringArguments));
            static_assert(hasRightNumberOfArguments, "The number of named arguments must match the number of arguments to the controlFunction");
            m_function = FunctionWrapper<FunctionT, T(Args...)>(function);
            AddStringArguments(derived, std::integral_constant<size_t, TypeUtilities::FunctionTraits<FunctionT>::Arity - sizeof...(Args)>(), std::forward<StringArguments>(stringArguments)...);
            m_argumentTypes.resize(TypeUtilities::FunctionTraits<FunctionT>::Arity - sizeof...(Args));
            m_registerCallback = [](const Reflection::TypeLibraries::TypeLibraryPointer& typeLibrary)
            {
                TypeUtilities::FunctionTraits<FunctionT>::Visit(AutoReflectArgumentsOffset<sizeof...(Args)>(typeLibrary));
                FunctionWrapper<FunctionT, T(Args...)>::InitializeReturnType(typeLibrary);
            };
            TypeUtilities::FunctionTraits<FunctionT>::Visit(ArgumentTypeVisitorOffset<sizeof...(Args)>(m_argumentTypes));
            using ReturnType = typename TypeUtilities::FunctionTraits<FunctionT>::ReturnType;
            m_returnType = Types::TypeId<ReturnType>();
            FunctionWrapper<FunctionT, T>::ValidateReturnType();
        }
        void TryRegister(const TypeLibraries::TypeLibraryPointer& typeLibrary) const
        {
            if (m_registerCallback)
                m_registerCallback(typeLibrary);
        }
    protected:
        void InitializeRemainingAttributes(FunctionCallbackAttribute*) {}

        template<typename DerivedT, typename... Args>
        void InitializeRemainingAttributes(DerivedT* derived, Args&&... args)
        {
            derived->InitializeArguments(std::forward<Args>(args)...);
        }
        template<typename DerivedT, typename... Args>
        void AddStringArguments(DerivedT* derived, std::integral_constant<size_t, 0>, Args&&... args)
        {
            InitializeRemainingAttributes(derived, std::forward<Args>(args)...);
        }
        template<typename DerivedT, size_t Count, typename... Args>
        void AddStringArguments(DerivedT* derived, std::integral_constant<size_t, Count>, const std::string& arg, Args&&... args)
        {
            m_stringArguments.push_back(arg);
            AddStringArguments(derived, std::integral_constant<size_t, Count - 1>(), std::forward<Args>(args)...);
        }
        std::function<void(const Reflection::TypeLibraries::TypeLibraryPointer&)> m_registerCallback;
        Types::DecoratedTypeInfo m_returnType;
        std::vector<Types::DecoratedTypeInfo> m_argumentTypes;
        std::function<T(Args..., const std::vector<Objects::Object>&)> m_function;
        std::vector<std::string> m_stringArguments;
    };
}}}}

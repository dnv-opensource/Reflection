#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "FunctionWrapper.h"
#include "ArgumentTypeVisitor.h"
#include "Reflection/Types/DecoratedTypeInfo.h"
#include "Reflection/Classes/Class.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {

    template<typename T>
    class FunctionCallbackAttribute
    {
    public:
        using ReturnType = T;
        FunctionCallbackAttribute(const T& value)
            : FunctionCallbackAttribute([=]() {return value; })
        {}
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
        T InvokeFunction(const std::vector<Objects::Object>& args) const {
            return m_function(args);
        }
        FunctionCallbackAttribute() {}
        template<typename DerivedT,typename FunctionT, typename... StringArguments>
        void InitializeFunction(DerivedT* derived, FunctionT function, StringArguments&&... stringArguments)
        {
            InitializeFunctionImpl(derived, function, std::forward<StringArguments>(stringArguments)...);
            TypeUtilities::FunctionTraits<FunctionT>::Visit(ArgumentTypeVisitor(m_argumentTypes));
			using ReturnType = typename TypeUtilities::FunctionTraits<FunctionT>::ReturnType;
			m_returnType = Types::TypeId<ReturnType>();
			FunctionWrapper<FunctionT, T>::ValidateReturnType();
        }
        template<typename DerivedT, typename... StringArguments>
        void InitializeFunction(DerivedT* derived, Members::MemberPointer member, StringArguments&&... stringArguments)
        {
            static_assert(false, "In AddAttribute, you need to pass the typeLibrary as the first argument.");
        }
        template<typename DerivedT, typename... StringArguments>
        void InitializeFunction(DerivedT* derived, const TypeLibraries::TypeLibraryPointer& typeLibrary, Members::MemberPointer member, StringArguments&&... stringArguments)
        {
            InitializeRemainingAttributes(derived, std::forward<StringArguments>(stringArguments)...);
            m_returnType = member->GetReturnType();
            m_argumentTypes.resize(member->GetArity());
            m_stringArguments.resize(member->GetArity());
            for (size_t i = 0; i < member->GetArity(); ++i)
            {
                m_stringArguments[i] = member->GetArgumentInfo(i)->GetName();
                m_argumentTypes[i] = member->GetArgumentInfo(i)->GetTypeInfo();
            }
            std::weak_ptr<TypeLibraries::ITypeLibrary> weakTypeLibrary(typeLibrary);
            m_function = [=](const std::vector<Objects::Object>& args)
            {
                std::vector<Variants::Variant> vars(args.size());
                for (size_t i = 0; i < args.size(); ++i)
                    vars[i] = args[i].GetVariant();                
                return Objects::Object(weakTypeLibrary.lock(), member->Invoke(vars)).As<T>();
            };
        }
        template<typename DerivedT, typename FunctionT, typename... StringArguments>
        void InitializeFunction(DerivedT* derived, const TypeLibraries::TypeLibraryPointer& typeLibrary, FunctionT function, StringArguments&&... stringArguments)
        {
            InitializeFunctionImpl(derived, function, std::forward<StringArguments>(stringArguments)...);
            TypeUtilities::FunctionTraits<FunctionT>::Visit(ArgumentTypeVisitorWithTypeLibrary(typeLibrary, m_argumentTypes));
			using ReturnType = typename TypeUtilities::FunctionTraits<FunctionT>::ReturnType;
			m_returnType = Types::TypeId<ReturnType>();
			FunctionWrapper<FunctionT, T>::InitializeReturnType(typeLibrary);
		}
    protected:
        template<typename DerivedT, typename FunctionT, typename... StringArguments>
        void InitializeFunctionImpl(DerivedT* derived, FunctionT function, StringArguments&&... stringArguments)
        {
            constexpr bool hasRightNumberOfArguments = std::is_same_v<DerivedT, FunctionCallbackAttribute> ? (TypeUtilities::FunctionTraits<FunctionT>::Arity == sizeof...(StringArguments)) : (TypeUtilities::FunctionTraits<FunctionT>::Arity <= sizeof...(StringArguments));
            static_assert(hasRightNumberOfArguments, "The number of named arguments must match the number of arguments to the controlFunction");
            m_function = FunctionWrapper<FunctionT, T>(function);
            AddStringArguments(derived, std::integral_constant<size_t, TypeUtilities::FunctionTraits<FunctionT>::Arity>(), std::forward<StringArguments>(stringArguments)...);
            m_argumentTypes.resize(TypeUtilities::FunctionTraits<FunctionT>::Arity);
        }
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

		Types::DecoratedTypeInfo m_returnType;
        std::vector<Types::DecoratedTypeInfo> m_argumentTypes;
        std::function<T(const std::vector<Objects::Object>&)> m_function;
        std::vector<std::string> m_stringArguments;
    };
    template<>
    class FunctionCallbackAttribute<void>
    {
    public:
        using ReturnType = void;
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
        void InvokeFunction(const std::vector<Objects::Object>& args) const {
            m_function(args);
        }
        FunctionCallbackAttribute() {}
        template<typename DerivedT, typename FunctionT, typename... StringArguments>
        void InitializeFunction(DerivedT* derived, FunctionT function, StringArguments&&... stringArguments)
        {
            InitializeFunctionImpl(derived, function, std::forward<StringArguments>(stringArguments)...);
            TypeUtilities::FunctionTraits<FunctionT>::Visit(ArgumentTypeVisitor(m_argumentTypes));
            using ReturnType = typename TypeUtilities::FunctionTraits<FunctionT>::ReturnType;
            m_returnType = Types::TypeId<ReturnType>();
            FunctionWrapper<FunctionT, void>::ValidateReturnType();
        }
        template<typename DerivedT, typename... StringArguments>
        void InitializeFunction(DerivedT* derived, Members::MemberPointer member, StringArguments&&... stringArguments)
        {
            InitializeRemainingAttributes(derived, std::forward<StringArguments>(stringArguments)...);
            m_returnType = Types::TypeId<void>();
            m_argumentTypes.resize(member->GetArity());
            m_stringArguments.resize(member->GetArity());
            for (size_t i = 0; i < member->GetArity(); ++i)
            {
                m_stringArguments[i] = member->GetArgumentInfo(i)->GetName();
                m_argumentTypes[i] = member->GetArgumentInfo(i)->GetTypeInfo();
            }
            m_function = [=](const std::vector<Objects::Object>& args)
            {
                std::vector<Variants::Variant> vars(args.size());
                for (size_t i = 0; i < args.size(); ++i)
                    vars[i] = args[i].GetVariant();
                member->Invoke(vars);
            };
        }
        template<typename DerivedT, typename FunctionT, typename... StringArguments>
        void InitializeFunction(DerivedT* derived, const TypeLibraries::TypeLibraryPointer& typeLibrary, FunctionT function, StringArguments&&... stringArguments)
        {
            InitializeFunctionImpl(derived, function, std::forward<StringArguments>(stringArguments)...);
            TypeUtilities::FunctionTraits<FunctionT>::Visit(ArgumentTypeVisitorWithTypeLibrary(typeLibrary, m_argumentTypes));
            using ReturnType = typename TypeUtilities::FunctionTraits<FunctionT>::ReturnType;
            m_returnType = Types::TypeId<ReturnType>();
            FunctionWrapper<FunctionT, void>::InitializeReturnType(typeLibrary);
        }
    protected:
        template<typename DerivedT, typename FunctionT, typename... StringArguments>
        void InitializeFunctionImpl(DerivedT* derived, FunctionT function, StringArguments&&... stringArguments)
        {
            constexpr bool hasRightNumberOfArguments = std::is_same_v<DerivedT, FunctionCallbackAttribute> ? (TypeUtilities::FunctionTraits<FunctionT>::Arity == sizeof...(StringArguments)) : (TypeUtilities::FunctionTraits<FunctionT>::Arity <= sizeof...(StringArguments));
            static_assert(hasRightNumberOfArguments, "The number of named arguments must match the number of arguments to the controlFunction");
            m_function = FunctionWrapper<FunctionT, void>(function);
            AddStringArguments(derived, std::integral_constant<size_t, TypeUtilities::FunctionTraits<FunctionT>::Arity>(), std::forward<StringArguments>(stringArguments)...);
            m_argumentTypes.resize(TypeUtilities::FunctionTraits<FunctionT>::Arity);
        }
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

        Types::DecoratedTypeInfo m_returnType;
        std::vector<Types::DecoratedTypeInfo> m_argumentTypes;
        std::function<void(const std::vector<Objects::Object>&)> m_function;
        std::vector<std::string> m_stringArguments;
    };
}}}}

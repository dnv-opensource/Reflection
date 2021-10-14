#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "FunctionWrapper.h"
namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {
    /*
    Represents rules on whether a field is to be enabled or not.
    First comes a function with the rules for the enabling/disabling
    Then comes a list of property names that should be passed to this rule function.
    */

    template<typename... Args>
    class Tuple {
    public:
        Tuple(Args&&... args) : m_args(std::forward<Args>(args)...) {}
        template<typename T, size_t Index>
        Types::ReturnType<T> Get()
        {
            return std::get<Index>(m_args);
        }
    private:
        std::tuple<Args...> m_args;
    };
    template<typename Lhs,size_t LhsSize,typename Rhs>
    class SequentialArguments
    {
    public:
        SequentialArguments(Lhs&& lhs, Rhs&& rhs)
            : m_lhs(std::forward<Lhs>(lhs))
            , m_rhs(std::forward<Rhs>(rhs))
        {}
        template<typename T, size_t Index>
        Types::ReturnType<T> Get()
        {
            return GetImpl<T, Index>(std::integral_constant<bool, Index<LhsSize>());
        }
    private:
        template<typename T, size_t Index>
        Types::ReturnType<T> GetImpl(std::true_type)
        {
            return m_lhs.Get<T, Index>();
        }
        template<typename T, size_t Index>
        Types::ReturnType<T> GetImpl(std::false_type)
        {
            return m_rhs.Get<T, Index-LhsSize>();
        }
        Lhs m_lhs;
        Rhs m_rhs;
    };

    template<typename ControlFunction, typename ReturnType,typename... Args>
    class FunctionWrapper<ControlFunction,ReturnType(Args...)>
    {
    public:
        FunctionWrapper(ControlFunction controlFunction)
            : m_controlFunction(controlFunction)
        {}
        ReturnType operator()(Args&&... fixedArgs, const std::vector<Objects::Object>& customArgs) const
        {
            return TypeUtilities::Invoke(m_controlFunction, SequentialArguments<Tuple<Args...>,sizeof...(Args),CallableObjects>(Tuple<Args...>(std::forward<Args>(fixedArgs)...), CallableObjects(customArgs)));
        }
        static void ValidateReturnType()
        {
        }
        static void InitializeReturnType(const TypeLibraries::TypeLibraryPointer& typeLibrary)
        {
        }
    private:
        ControlFunction m_controlFunction;
    };
    template<typename ControlFunction, typename... Args>
    class FunctionWrapper<ControlFunction, Variants::Variant(Args...)>
    {
    public:
        FunctionWrapper(ControlFunction controlFunction)
            : m_controlFunction(controlFunction)
        {}
        Variants::Variant operator()(Args&&... fixedArgs, const std::vector<Objects::Object>& customArgs) const
        {
            return Variants::VariantService::Reflect(TypeUtilities::Invoke(m_controlFunction, SequentialArguments<Tuple<Args...>, sizeof...(Args), CallableObjects>(Tuple<Args...>(std::forward<Args>(fixedArgs)...), CallableObjects(customArgs))));
        }
        static void ValidateReturnType()
        {
            using ReturnType = typename TypeUtilities::FunctionTraits<ControlFunction>::ReturnType;
            static_assert(AutoReflect<ReturnType>::RequiresReflection == false, "In AddAttribute, you need to pass the typeLibrary as the first argument.");
        }
        static void InitializeReturnType(const TypeLibraries::TypeLibraryPointer& typeLibrary)
        {
            using ReturnType = typename TypeUtilities::FunctionTraits<ControlFunction>::ReturnType;
            AutoReflect<ReturnType>::Reflect(typeLibrary);
        }
    private:
        ControlFunction m_controlFunction;
    };
    template<typename ControlFunction, typename... Args>
    class FunctionWrapper<ControlFunction, std::list<Variants::Variant>(Args...)>
    {
    public:
        FunctionWrapper(ControlFunction controlFunction)
            : m_controlFunction(controlFunction)
        {}
        std::list<Variants::Variant> operator()(Args&&... fixedArgs, const std::vector<Objects::Object>& customArgs) const
        {
            std::list<Variants::Variant> result;
            for (auto arg : TypeUtilities::Invoke(m_controlFunction, SequentialArguments<Tuple<Args...>, sizeof...(Args), CallableObjects>(Tuple<Args...>(std::forward<Args>(fixedArgs)...), CallableObjects(customArgs))))
                result.emplace_back(Variants::VariantService::ReflectType(arg));
            return result;
        }
        static void ValidateReturnType()
        {
            using ReturnType = typename TypeUtilities::FunctionTraits<ControlFunction>::ReturnType;
            static_assert(AutoReflect<typename ReturnType::value_type>::RequiresReflection == false, "In AddAttribute, you need to pass the typeLibrary as the first argument.");
        }
        static void InitializeReturnType(const TypeLibraries::TypeLibraryPointer& typeLibrary)
        {
            using ReturnType = typename TypeUtilities::FunctionTraits<ControlFunction>::ReturnType;
            AutoReflect<typename ReturnType::value_type>::Reflect(typeLibrary);
        }
    private:
        ControlFunction m_controlFunction;
    };
}}}}
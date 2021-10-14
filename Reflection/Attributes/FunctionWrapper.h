#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include <functional>
#include <vector>
#include "TypeUtilities/FunctionTraits.h"
#include "Reflection/Objects/Object.h"
#include "Reflection/Variants/VariantService.h"
#include "TypeUtilities/Invoker.h"


namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {
    /*
    Represents rules on whether a field is to be enabled or not.
    First comes a function with the rules for the enabling/disabling
    Then comes a list of property names that should be passed to this rule function.
    */

    class CallableObjects
    {
    public:
        CallableObjects(const std::vector<Objects::Object>& args)
            : m_arguments(args)
        {
        }
        template<typename T, size_t Index>
        Types::ReturnType<T> Get()
        {
            return m_arguments[Index].As<T>();
        }
    private:
        const std::vector<Objects::Object>& m_arguments;
    };

    template<typename ControlFunction, typename ReturnTypeT>
    class FunctionWrapper
    {
    public:
        FunctionWrapper(ControlFunction controlFunction)
            : m_controlFunction(controlFunction)
        {}
        ReturnTypeT operator()(const std::vector<Objects::Object>& args) const
        {
            return TypeUtilities::Invoke(m_controlFunction, CallableObjects(args));
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
    template<typename ControlFunction>
    class FunctionWrapper<ControlFunction, Variants::Variant>
    {
    public:
        FunctionWrapper(ControlFunction controlFunction)
            : m_controlFunction(controlFunction)
        {}
        Variants::Variant operator()(const std::vector<Objects::Object>& args) const
        {
            return Variants::VariantService::Reflect(TypeUtilities::Invoke(m_controlFunction, CallableObjects(args)));
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
    template<typename ControlFunction>
    class FunctionWrapper<ControlFunction, std::list<Variants::Variant>>
    {
    public:
        FunctionWrapper(ControlFunction controlFunction)
            : m_controlFunction(controlFunction)
        {}
        std::list<Variants::Variant> operator()(const std::vector<Objects::Object>& args) const
        {
            std::list<Variants::Variant> result;
            for (auto arg : TypeUtilities::Invoke(m_controlFunction, CallableObjects(args)))
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
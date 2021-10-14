#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include "TypeUtilities/FunctionTraits.h"
#include "Reflection/Objects/Object.h"
#include "FunctionCallbackAttribute.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {
    /**
    ValidatorFunctionWrapper encapsulates a lambda function or function pointer validator. 
    It converts from an object to the actual type of the validator.
    */
    template<typename LambdaFunction>
    struct ValidationFunctionWrapper 
    {
    public:
        ValidationFunctionWrapper(const LambdaFunction& validator) : m_validator(validator) {}
        void operator()(const Objects::Object& object) const {
            typedef typename TypeUtilities::FunctionTraits<LambdaFunction>::Argument0 ArgumentType;
            m_validator(object.As<ArgumentType>());
        }
    private:
        LambdaFunction m_validator;
    };

    /**
    The ValidationAttribute is used in reflection in order to indicate whether a value is valid or not.
    Example:
    Class<double> cls(typeLibrary, "double");
    cls.AddAttribute<ValidationAttribute>([](double var) 
    {
       if(__isnan(var))
          throw std::runtime_error("NaN values not supported!");
    },"");

    This attribute can be used during for example error validation in GUI to see whether a value is acceptable.
    */
    class ValidationAttribute : public FunctionCallbackAttribute<void>
    {
    public: 
        template<typename LambdaFunction, typename... StringArguments>
        ValidationAttribute(LambdaFunction function, StringArguments&&... stringArguments)
            : FunctionCallbackAttribute<void>(function, std::forward<StringArguments>(stringArguments)...)
        {
        }
        template<typename LambdaFunction, typename... StringArguments>
        ValidationAttribute(const TypeLibraries::TypeLibraryPointer& typeLibrary, LambdaFunction function, StringArguments&&... stringArguments)
            : FunctionCallbackAttribute<void>(typeLibrary, function, std::forward<StringArguments>(stringArguments)...)
        {
        }
    };
}}}}
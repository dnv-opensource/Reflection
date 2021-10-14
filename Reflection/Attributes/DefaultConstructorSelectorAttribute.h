#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Objects/Object.h"
#include "TypeUtilities/FunctionTraits.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    template<typename LambdaFunctionT>
    class DefaultConstructorSelectorLambda
    {
    public:
        typedef typename TypeUtilities::FunctionTraits<LambdaFunctionT>::Argument0 ArgumentType;

        DefaultConstructorSelectorLambda(LambdaFunctionT lambdaFunction) : m_lambdaFunction(lambdaFunction) {}
        bool operator()(const Objects::Object& object)
        {
            if (!object.IsConvertibleTo<ArgumentType>())
                return false;
            return m_lambdaFunction(object.As<ArgumentType>());
        }
    private:
        LambdaFunctionT m_lambdaFunction;
    };

    /**
    If a class has more than one constructor, this attribute can be used to, for a given object, find out which constructor is the best match to that object.
    Usage:
    //This constructor is the generic one, always works.
    cls.Constructor<const CurveOffsetAtPointPointer&, const CurveOffsetAtPointPointer&, bool>()
    .AddAttribute<DefaultConstructorSelectorAttribute>([](const LinearVaryingCurveOffset& other) {return !other.CanUseSimplifiedConstructor(); });
    
    //This constructor only works under certain assumptions.
    cls.Constructor<const vec3d&, const vec3d&, bool>()
    .AddAttribute<DefaultConstructorSelectorAttribute>([](const LinearVaryingCurveOffset& other) {return other.CanUseSimplifiedConstructor(); });
    */
    class DefaultConstructorSelectorAttribute
    {
    public:
        template<typename LambdaFunctionT>
        DefaultConstructorSelectorAttribute(const LambdaFunctionT& selector) 
            : m_selector(DefaultConstructorSelectorLambda<LambdaFunctionT>(selector)) 
        {}
        bool IsDefaultConstructor(const Objects::Object& object) const
        {
            return m_selector(object);
        }
    private:
        std::function<bool(const Objects::Object& object)> m_selector;
    };
}}}}


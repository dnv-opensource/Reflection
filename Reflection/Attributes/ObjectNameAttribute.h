#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Objects/Object.h"
namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    template<typename LambdaFunction>
    class ObjectNameAttributeLambda 
    {
    public:
        typedef typename TypeUtilities::FunctionTraits<LambdaFunction>::Argument0 ArgumentType;
        ObjectNameAttributeLambda(const LambdaFunction& lambdaFunction)
            : m_lambdaFunction(lambdaFunction)
        {}
        std::string operator()(const Objects::Object& argument) const
        {
            if (!argument.IsConvertibleTo<ArgumentType>())
                return std::string();
            else
                return m_lambdaFunction(argument.As<ArgumentType>());            
        }
    private:
        LambdaFunction m_lambdaFunction;
    };
    /*
    Description:
    Some objects, like ICurveOffset, can be described in two ways;
    1. as a string describing how it was created, such as 'ConstantCurveOffset(Vector3d(1,2,3),true)'
    2. describing it by ownership, such as 'Bm1.CurveOffset'
    This attribute allows you to have the second representation, which will be used if IFormatterRules::ExpandExpression is false.
    Usage:
    Class<ICurveOffset, CurveOffsetPointer> cls(...);
    cls.AddAttribute<ObjectNameAttribute>(&GetNameFromCurveOffset);    
    */
    class ObjectNameAttribute
    {
    public:
        template<typename LambdaFunction>
        ObjectNameAttribute(const LambdaFunction& lambdaFunction)
            : m_callback(ObjectNameAttributeLambda<LambdaFunction>(lambdaFunction))
        {}
        std::string GetName(const Objects::Object& object) const
        {
            return m_callback(object);
        }
    private:
        std::function<std::string(const Objects::Object&)> m_callback;
    };

}}}}

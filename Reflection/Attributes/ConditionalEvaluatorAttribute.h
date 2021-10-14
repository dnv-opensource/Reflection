#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Objects/Object.h"
#include "TypeUtilities/FunctionTraits.h"
namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    class ConditionalEvaluatorAttribute {
    public:
        template<typename CallbackT>
        ConditionalEvaluatorAttribute(CallbackT callback)
        {
            static_assert(TypeUtilities::FunctionTraits<CallbackT>::Arity == 5, "Function must have 5 arguments.");
            using Arguments = typename TypeUtilities::FunctionTraits<CallbackT>::Arguments;
            static_assert(std::is_same_v<bool, typename std::tuple_element_t<2, Arguments>>, "Third argument must be bool.");
            static_assert(std::is_same_v<bool, typename std::tuple_element_t<4, Arguments>>, "Fifth argument must be bool.");
            static_assert(!std::is_same_v<void, typename TypeUtilities::FunctionTraits<CallbackT>::ReturnType>, "Return type must not be void.");
            m_function = [=](const Objects::Object& conditionalObject,
                             const Objects::Object& ifObject,
                             bool ifReturn,
                             const Objects::Object& ifNotObject,
                             bool ifNotReturn) -> Objects::Object
            {
                auto a0 = conditionalObject.As<typename std::tuple_element_t<0, Arguments>>();
                auto a1 = ifObject.As<typename std::tuple_element_t<1, Arguments>>();
                auto a3 = ifNotObject.As<typename std::tuple_element_t<3, Arguments>>();
                return Objects::Object(conditionalObject.GetTypeLibrary(), callback(a0, a1, ifReturn, a3, ifNotReturn));
            };
        }
        Objects::Object TryCreateConditional(const Objects::Object& conditionalObject,
                                             const Objects::Object& ifObject,
                                             bool ifReturn,
                                             const Objects::Object& ifNotObject,
                                             bool ifNotReturn) const
        {
            return m_function(conditionalObject, ifObject, ifReturn, ifNotObject, ifNotReturn);
        }
    private:
        std::function<Objects::Object(const Objects::Object&, const Objects::Object&, bool, const Objects::Object&, bool)> m_function;
    };
}}}}
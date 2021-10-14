#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <functional>
#include "Reflection/Attributes/FunctionWrapper.h"
#include "Reflection/Attributes/ArgumentTypeVisitor.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class EnableIf
    {
    public:
        template<typename Callback>
        EnableIf(const Callback& callback)
             : m_function(Attributes::FunctionWrapper<Callback, bool>(callback))
        {
            using FunctionSignature = TypeUtilities::FunctionTraits<Callback>;
            static_assert(FunctionSignature::Arity == 1, "The number of named arguments must match the number of arguments to the callback");
            m_argumentTypes.resize(FunctionSignature::Arity);
            FunctionSignature::Visit(ArgumentTypeVisitor(m_argumentTypes));
            //Add an empty name representing "this" property.
            m_stringArguments.push_back("");
        }
        const std::vector<Types::DecoratedTypeInfo>& GetArgumentTypes() const { return m_argumentTypes; }
        const std::vector<std::string>& GetStringArguments() const { return m_stringArguments; }
        bool HasFunction() const { return m_function != nullptr; }
        bool InvokeFunction(const std::vector<Objects::Object>& args) const {
            return m_function(args);
        }
    private:
        std::function<T(const std::vector<Objects::Object>&)> m_function;
        std::vector<Types::DecoratedTypeInfo> m_argumentTypes;
        std::vector<std::string> m_stringArguments;
    };
}}}}
#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "FunctionCallbackAttribute.h"
#include "IAttribute.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    class DefaultNameAttribute 
        : public FunctionCallbackAttribute<std::string> 
        , public IAttribute
    {
    public:
        DefaultNameAttribute(const std::string& defaultName) : FunctionCallbackAttribute<std::string>(defaultName) {}
        DefaultNameAttribute(const char* defaultName) : FunctionCallbackAttribute<std::string>(std::string(defaultName)) {}
        template<typename DefaultNameFunction, typename... StringArguments>
        DefaultNameAttribute(DefaultNameFunction defaultNameFunction, const StringArguments&... stringArguments)
            : FunctionCallbackAttribute<std::string>(defaultNameFunction, stringArguments...) {}
        template<typename DefaultNameFunction, typename... StringArguments>
        DefaultNameAttribute(const TypeLibraries::TypeLibraryPointer& typeLibrary, DefaultNameFunction defaultNameFunction, StringArguments&&... stringArguments)
            : FunctionCallbackAttribute<std::string>(typeLibrary, defaultNameFunction, std::forward<StringArguments>(stringArguments)...)
        {
        }
        const std::string GetDefaultName(const std::vector<Objects::Object>& arguments) const { return InvokeFunction(arguments); }
    };
}}}}

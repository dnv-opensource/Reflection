#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include <memory>
#include "mofaTools\ref.h"
#include "Reflection\Objects\Object.h"
class jsValue;

class IScriptingPropertyService
{
public:
    virtual ~IScriptingPropertyService() {}
    struct PropertyData {
        std::string Type;
        bool HasGetter;
        bool HasSetter;
        bool IsConst;
    };
    virtual std::unique_ptr<PropertyData> TryGetPropertyData(jsValue* value) const = 0;
    virtual mofa::ref<jsValue> CreateDelegate(const DNVS::MoFa::Reflection::Objects::Object& object, const std::string& identifier) = 0;
};
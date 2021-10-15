#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <string>
#include <istream>
#include <memory>
#include <stdexcept>

#include "Config.h"

namespace DNVS { namespace MoFa { namespace Reflection {
namespace TypeConversions {
    class IConversionGraph;
    typedef std::shared_ptr<IConversionGraph> ConversionGraphPointer;
}
namespace Objects {
    class Object;
}}}}

namespace DNVS { namespace MoFa { namespace Scripting {
    class IAutoCompletion;

    class EmptyScriptError : public std::runtime_error
    {
    public:
        EmptyScriptError(const std::string& message) : std::runtime_error(message)
        {
        }
    };

    std::string SCRIPTING_IMPORT_EXPORT ValidateAndFixScript(const std::string& str);
    Reflection::Objects::Object SCRIPTING_IMPORT_EXPORT Execute(const std::string& str, bool logging = true);
    Reflection::Objects::Object SCRIPTING_IMPORT_EXPORT Execute(std::istream& istr, bool logging = true);
    Reflection::Objects::Object SCRIPTING_IMPORT_EXPORT Test(const std::string& str);
    bool SCRIPTING_IMPORT_EXPORT IsLookupExpression(const std::string& expr);
    bool SCRIPTING_IMPORT_EXPORT HasMember(const std::string& name);
    void SCRIPTING_IMPORT_EXPORT DeleteMember(const std::string& name);
    void SCRIPTING_IMPORT_EXPORT RenameMember(const Reflection::Objects::Object& object, const std::string& newName);
    bool SCRIPTING_IMPORT_EXPORT IsAlive();
    bool SCRIPTING_IMPORT_EXPORT InAssignment();
    std::shared_ptr<IAutoCompletion> SCRIPTING_IMPORT_EXPORT TryAutoComplete(const std::string& text, bool functionCompletion = true);
}}}

#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection\Objects\Object.h"
#include "Reflection\TypeConversions\IConversionGraph.h"
#include "Reflection\TypeLibraries\ITypeLibrary.h"
#include <string>
#include <memory>

namespace DNVS {namespace MoFa {namespace Services { namespace Allocators {
    class ForwardingAllocatorSelector;
}}}}

namespace DNVS {namespace MoFa {namespace Scripting {
    class IAutoCompletion;
    enum class ExecutionStatus
    {
        NoExecution,
        ExecuteString,
        ExecuteFile,
    };
    class IScriptingService
    {
    public:
        virtual ~IScriptingService() {}
        virtual Reflection::Objects::Object Execute(const std::string& str, bool logging = false) = 0;
        virtual Reflection::Objects::Object Execute(std::istream& istr, bool logging = false) = 0;
        virtual Reflection::Objects::Object Test(const std::string& str) = 0;
        virtual Reflection::Objects::Object Invoke(const std::string& expression, const std::vector<Reflection::Objects::Object>& arguments) = 0;
        virtual Reflection::Objects::Object Lookup(const std::string& name) = 0;
        virtual bool IsLookupExpression(const std::string& expr) const = 0;
        virtual bool TrySplitFunctionArguments(const std::string& expr, std::string& functionName, std::vector<std::string>& arguments) const = 0;
        virtual Reflection::TypeConversions::ConversionGraphPointer GetConversionGraph() const = 0;
        virtual Reflection::TypeLibraries::TypeLibraryPointer GetTypeLibrary() const = 0;
        virtual Services::Allocators::ForwardingAllocatorSelector GetAllocator() const = 0;

        virtual bool TryGetName(const Reflection::Objects::Object& object, std::string& name) = 0;

        //Returns any type with this name, such as a enumeration or constructor exists.
        virtual bool HasIdentifier(const std::string& name) const = 0;
        //Returns true only if a named object with this name exists.
        virtual bool HasMember(const std::string& name) const = 0;
        virtual void DeleteMember(const std::string& name) = 0;
        virtual void RenameMember(const Reflection::Objects::Object& object, const std::string& newName, bool forceRename = false) = 0;
        virtual bool InAssignment() const = 0;
        virtual void ClearMembers() = 0;
        virtual ExecutionStatus GetExecutionStatus() const = 0;
        virtual std::set<std::string> GetAllMemberNames() const = 0;

        virtual std::shared_ptr<IAutoCompletion> TryAutoComplete(const std::string& text, bool functionCompletion = true) const = 0;
    };

    typedef std::shared_ptr<IScriptingService> ScriptingServicePointer;

}}}

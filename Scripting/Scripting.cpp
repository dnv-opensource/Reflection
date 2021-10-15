//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <boost/algorithm/string.hpp>

#include "Scripting.h"
#include "IScriptingService.h"
#include "Services/ServiceProvider.h"
#include "Reflection/Objects/Object.h"

using namespace std;
using DNVS::MoFa::Reflection::Objects::Object;
using DNVS::MoFa::Services::ServiceProvider;

namespace DNVS { namespace MoFa { namespace Scripting {
    std::string ValidateAndFixScript(const std::string& str)
    {
        string script = boost::algorithm::trim_right_copy_if(str, boost::algorithm::is_space());

        if (script.empty())
            throw DNVS::MoFa::Scripting::EmptyScriptError("Script is empty");
        else if (script.back() != ';' && script.back() != '}')
            return script + ';';
        else
            return script;
    }

    Object Execute(const string& str, bool logging)
    {
        return ServiceProvider::Instance().GetService<IScriptingService>()->Execute(ValidateAndFixScript(str), logging);
    }

    Object Execute(istream& istr, bool logging)
    {
        return ServiceProvider::Instance().GetService<IScriptingService>()->Execute(istr, logging);
    }

    Object Test(const string& str)
    {
        return ServiceProvider::Instance().GetService<IScriptingService>()->Test(ValidateAndFixScript(str));
    }

    bool HasMember(const std::string& name)
    {
        return ServiceProvider::Instance().GetService<IScriptingService>()->HasMember(name);
    }

    void DeleteMember(const std::string& name)
    {
        return ServiceProvider::Instance().GetService<IScriptingService>()->DeleteMember(name);
    }

    void RenameMember(const Reflection::Objects::Object& object, const std::string& newName)
    {
        return ServiceProvider::Instance().GetService<IScriptingService>()->RenameMember(object, newName);
    }

    bool IsAlive()
    {
        return ServiceProvider::Instance().TryGetService<IScriptingService>() != nullptr;
    }

    bool InAssignment()
    {
        return ServiceProvider::Instance().GetService<IScriptingService>()->InAssignment();
    }

    std::shared_ptr<DNVS::MoFa::Scripting::IAutoCompletion> TryAutoComplete(const std::string& text, bool functionCompletion)
    {
        return ServiceProvider::Instance().GetService<IScriptingService>()->TryAutoComplete(text, functionCompletion);
    }

    bool IsLookupExpression(const std::string& expr)
    {
        try {
            return ServiceProvider::Instance().GetService<IScriptingService>()->IsLookupExpression(ValidateAndFixScript(expr));
        }
        catch (...)
        {
            return false;
        }
    }
}}}

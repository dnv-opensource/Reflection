//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsScript/jsFunction.h>
#include <jsScript/jsCollections.h>
#include <jsScript/jsVTBL.h>
#include <jsScript/jsTypeLibrary.h>
#include <jsScript/jsTypeInfo.h>
#include <tuple>
#include "jsMemberWithArguments.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

jsFunction::jsFunction(jsTypeLibrary& typeLibrary, const std::string& member)
    : jsMember(typeLibrary, member, jsTypeInfo::jsFunction)
{
}

jsFunction::jsFunction(jsTypeLibrary& typeLibrary)
    : jsMember(typeLibrary)
{

}

jsFunction::~jsFunction()
{

}

std::string jsFunction::typeName()
{
    return "function";
}

void jsFunction::init()
{
    if (typeInfo()) {
        typeInfo()->arguments(param_size());
        for (int i = 0; i < param_size(); i++) {
            typeInfo()->argument(i, param_value(i));
        }
        typeInfo()->returned(return_value());
    }
}

int jsFunction::param_size()
{
    return 0;
}

std::string jsFunction::param_value(int i)
{
    return "";
}

std::string jsFunction::return_value()
{
    return "";
}

jsFunctionCollection::jsFunctionCollection(jsTypeLibrary& typeLibrary) : jsFunction(typeLibrary)
{
}

jsFunctionCollection::~jsFunctionCollection()
{
}

void jsFunctionCollection::LogFunctionDetails(const std::shared_ptr<DNVS::MoFa::TypeUtilities::IFunctionDetailService>& service)
{
    for (auto it = m_functions.begin(); it != m_functions.end(); ++it)
    {
        jsFunction* function = *it;
        function->LogFunctionDetails(service);
    }
}

jsValue* jsFunctionCollection::call(const Params& params)
{
    if (m_functions.size() == 0) throw std::exception("No functions found!");
    Functions::iterator it;
    std::set<jsMemberWithArgumentsPointer> all;
    for(const auto& function : m_functions)
    {
        jsMemberWithArgumentsPointer current(new jsMemberWithArguments(jsStack::stack()->GetConversionGraph(), function, params));
        if (current->IsOk())
            all.insert(current);
    }
    while (!all.empty())
    {
        jsMemberWithArgumentsPointer best;
        for (const auto& function : all)
        {
            best = jsMemberWithArguments::SelectBestOverload(best, function);
        }
        try {
            if (best && best->IsOk())
                return best->Invoke();
        }
        catch (invalid_argument&) {
            all.erase(best);
        }
        catch (incorrect_number_of_arguments&) {
            all.erase(best);
        }
    }
    throw invalid_overload_argument(this);
}


void jsFunctionCollection::add(jsFunction* constr)
{
    m_functions.push_back(constr);
}

void jsFunctionCollection::getTypeInfo(TypeInfoSet& members, int type, jsValue* parent)
{
    Functions::iterator it;
    for (it = m_functions.begin(); it != m_functions.end(); it++) {
        (*it)->getTypeInfo(members, type, parent);
    }
}
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsScript/jsConstructor.h>
#include <jsScript/jsTypeLibrary.h>
#include <jsScript/jsCollections.h>
#include <jsScript/jsVTBL.h>
#include "jsMemberWithArguments.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

jsConstructor::jsConstructor(jsTypeLibrary& typeLibrary, const std::string& member, bool isExplicit)
    : jsMember(typeLibrary, member, jsTypeInfo::jsConstructor)
    , m_isExplicit(isExplicit)
{

}

jsConstructor::jsConstructor(jsTypeLibrary& typeLibrary, bool isExplicit)
    : jsMember(typeLibrary)
    , m_isExplicit(isExplicit)
{

}

jsConstructor::~jsConstructor()
{

}

jsValue* jsConstructor::call(const Params& params)
{
    return construct(params);
}

std::string jsConstructor::typeName()
{
    return "constructor";
}

void jsConstructor::init()
{
    if (typeInfo()) {
        typeInfo()->arguments(param_size());
        for (int i = 0; i < param_size(); i++) {
            typeInfo()->argument(i, param_value(i));
        }
        typeInfo()->returned(return_value());
    }
}

int jsConstructor::param_size()
{
    return 0;
}

std::string jsConstructor::param_value(int i)
{
    return "";
}

std::string jsConstructor::return_value()
{
    return "";
}

jsValue* jsConstructor::prototype()
{
    jsVTBL* vtbl = GetTypeLibrary().lookup(return_value_info());
    if (!vtbl) throw vtbl_not_found(return_value_info());
    return vtbl;
}

jsConstructorCollection::jsConstructorCollection(jsTypeLibrary& typeLibrary) : jsConstructor(typeLibrary, false)
{
}

jsConstructorCollection::~jsConstructorCollection()
{
}

void jsConstructorCollection::LogFunctionDetails(const std::shared_ptr<DNVS::MoFa::TypeUtilities::IFunctionDetailService>& service)
{
    for (auto it = m_constructors.begin(); it != m_constructors.end(); ++it)
    {
        jsConstructor* constructor = *it;
        constructor->LogFunctionDetails(service);
    }
}


jsValue* jsConstructorCollection::construct(const Params& params)
{
    if (m_constructors.size() == 0) throw std::exception("No constructors found!");
    Constructors::iterator it;
    std::set<jsMemberWithArgumentsPointer> all;
    for (const auto& constructor : m_constructors)
    {
        jsMemberWithArgumentsPointer current(new jsMemberWithArguments(jsStack::stack()->GetConversionGraph(), constructor, params));
        if (current->IsOk())
            all.insert(current);
    }
    while (!all.empty())
    {
        jsMemberWithArgumentsPointer best;
        for (const auto& constructor : all)
        {
            best = jsMemberWithArguments::SelectBestOverload(best, constructor);
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

void jsConstructorCollection::add(jsConstructor* constr)
{
    m_constructors.push_back(constr);
}

void jsConstructorCollection::getTypeInfo(TypeInfoSet& members, int type, jsValue* parent)
{
    Constructors::iterator it;
    for (it = m_constructors.begin(); it != m_constructors.end(); it++) {
        (*it)->getTypeInfo(members, type, parent);
    }
}

const type_info& jsConstructorCollection::return_value_info()
{
    Constructors::iterator it;
    for (it = m_constructors.begin(); it != m_constructors.end(); it++) {
        return (*it)->return_value_info();
    }
    throw std::exception("No constructors in constructor collection");
}

std::string jsConstructorCollection::return_value()
{
    Constructors::iterator it;
    for (it = m_constructors.begin(); it != m_constructors.end(); it++) {
        return (*it)->return_value();
    }
    throw std::exception("No constructors in constructor collection");
}
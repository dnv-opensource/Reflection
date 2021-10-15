//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <memory>
#include <algorithm>
#include <iterator>
#include <vector>

#include <boost/foreach.hpp>

#include <jsScript/jsTypeInfo.h>
#include <jsScript/jsStack.h>
#include <mofaTools/lexical_cast.h>

using namespace std;
using namespace DNVS::MoFa::Reflection::Members;

struct jsArgInfoImpl
{
    jsArgInfoImpl(const string& name) : m_name(name), m_varName(1), m_docShort(1)
    {
    }

    string m_name;
    vector<string> m_varName;
    vector<string> m_docShort;
};

struct jsTypeInfoImpl
{
    jsTypeInfoImpl(const string& name, int parameters);
    ~jsTypeInfoImpl();

    string m_name;
    int m_valueType;
    string m_docShort;
    string m_docVerbose;
    string m_docExample;
    string m_localNameSpace;

    vector<unique_ptr<jsArgInfo>> m_argumentTypes;

    vector<jsExampleInfo> m_examples;
    unique_ptr<jsArgInfo> m_returnType;
    jsValue* m_value;

    static string m_nameSpace;
};

string jsTypeInfoImpl::m_nameSpace;

jsTypeInfoImpl::jsTypeInfoImpl(const string& name, int parameters)
    : m_name(name), m_argumentTypes(parameters)
{
    m_localNameSpace = m_nameSpace;
}

jsTypeInfoImpl::~jsTypeInfoImpl()
{
}

jsArgInfo::jsArgInfo(const string& name) : m_impl(new jsArgInfoImpl(name))
{
}

jsArgInfo::~jsArgInfo()
{
}

void jsArgInfo::varName(const string& varName, size_t index)
{
    if (m_impl->m_varName.size() <= index)
        m_impl->m_varName.resize(index + 1);

    m_impl->m_varName[index] = varName;
}

void jsArgInfo::varDoc(const string& varDoc, size_t index)
{
    if (m_impl->m_docShort.size() <= index)
        m_impl->m_docShort.resize(index + 1);

    m_impl->m_docShort[index] = varDoc;
}

string jsArgInfo::name() const
{
    return jsStack::stack()->typeName(m_impl->m_name);
}

void jsArgInfo::setName(const string& name)
{
    m_impl->m_name = name;
}

string jsArgInfo::varName(size_t index) const
{
    if (index >= m_impl->m_varName.size())
        return "";

    return m_impl->m_varName[index];
}

size_t jsArgInfo::varNameCount() const
{
    return m_impl->m_varName.size();
}

string jsArgInfo::docShort(size_t index) const
{
    if (index >= m_impl->m_docShort.size())
        return "";

    return m_impl->m_docShort[index];
}

void jsArgInfo::document(const string& varName, const string& varDoc, size_t index)
{
    this->varName(varName, index);
    this->varDoc(varDoc, index);
}

void jsArgInfo::document(const string& documentation, size_t index)
{
    if (varName(index).size() == 0)
        varName(documentation, index);
    else
        varDoc(documentation, index);
}

jsExampleInfo::jsExampleInfo()
{
}

jsExampleInfo::jsExampleInfo(const string& title) : m_title(title)
{
}

void jsExampleInfo::AddScript(const string& script)
{
    m_script += script;
    m_script += "<br/>";
}

void jsExampleInfo::AddComment(const string& comment)
{
    m_comment += comment;
    m_comment += "<br/>";
}

vector<jsExampleInfo> jsTypeInfo::getExamples() const
{
    vector<jsExampleInfo> complexExamples;

    copy_if(begin(m_impl->m_examples), end(m_impl->m_examples), back_inserter(complexExamples),
    [](const jsExampleInfo& info)
    {
        return !info.IsSimple();
    });

    return complexExamples;
}

jsTypeInfo::jsTypeInfo(jsValue* value, const string& name, int type, int parameters)
    : m_impl(new jsTypeInfoImpl(name, parameters))
{
    m_impl->m_valueType = type;
    m_impl->m_value = value;
}

void jsTypeInfo::arguments(size_t nargs)
{
    m_impl->m_argumentTypes.resize(nargs);
}

jsTypeInfo::~jsTypeInfo()
{
}

void jsTypeInfo::setType(int type)
{
    m_impl->m_valueType = type;
}

int jsTypeInfo::type() const
{
    return m_impl->m_valueType;
}

string jsTypeInfo::name() const
{
    return m_impl->m_name;
}

void jsTypeInfo::document(const string& text, jsDocumentType docType)
{
    switch (docType)
    {
    case jsAutomatic:
        if (m_impl->m_docShort.size() == 0)
            m_impl->m_docShort = text;
        break;

    case jsVerbose:
        if (m_impl->m_docVerbose.size() > 0)
            m_impl->m_docVerbose += " ";

        m_impl->m_docVerbose += text;
        break;

    case jsShort:
        m_impl->m_docShort = text;
        break;

    case jsExample:
        m_impl->m_docExample += text;
        break;
    }
}

string jsTypeInfo::documentation(jsDocumentType docType) const
{
    switch (docType)
    {
    case jsAutomatic:
        if (!m_impl->m_docShort.empty() && !m_impl->m_docVerbose.empty())
            return m_impl->m_docShort + m_impl->m_docVerbose;

        if (m_impl->m_docVerbose.size() > 0)
            return m_impl->m_docVerbose;

        if (m_impl->m_docShort.size() > 0)
            return m_impl->m_docShort;

        return "";

    case jsVerbose:
        return m_impl->m_docVerbose;

    case jsShort:
        return m_impl->m_docShort;

    case jsExample:
        if (m_impl->m_examples.size() == 1 && m_impl->m_examples.front().IsSimple())
            return m_impl->m_examples.front().getTitle();
    }

    return "";
}

void jsTypeInfo::document(const string& text, const string& argNames)
{
    document(text, jsShort);
    nameArguments(argNames);
}

void jsTypeInfo::one_liner(const string& text, const string& argNames)
{
    document(text, jsShort);
    nameArguments(argNames);
}

jsTypeInfo& jsTypeInfo::AddSignatureFromArguments(const vector<ArgumentInfoPointer>& args)
{
    int i = 0;

    for_each(begin(args), end(args),
    [&](const ArgumentInfoPointer& info)
    {
        argument(i)->document(info->GetName(), info->GetDocumentation());

        auto typeAlias = info->TypeAlias();
        if (!typeAlias.empty())
            argument(i)->setName(typeAlias);

        i++;
    });

    return *this;
}

jsArgInfo* jsTypeInfo::returned(const string& name)
{
    if (!m_impl->m_returnType)
    {
        if (name == "")
            return nullptr;

        m_impl->m_returnType.reset(new jsArgInfo(name));
        m_impl->m_returnType->varName(name);
    }

    return m_impl->m_returnType.get();
}

jsArgInfo* jsTypeInfo::argument(size_t index, const string& name)
{
    if (m_impl->m_argumentTypes.size() <= index)
    {
        string error;

        if (m_impl->m_value)
            error = m_impl->m_value->typeName() + ".";

        error += this->name();
        error += " has only " + mofa::lexical_cast<string>(arguments()) + " arguments. ";
        error += " trying to document argument # " + mofa::lexical_cast<string>(index + 1);

        throw exception(error.c_str());
    }

    if (m_impl->m_argumentTypes[index] == nullptr)
    {
        if (name == "")
            return nullptr;

        m_impl->m_argumentTypes[index].reset(new jsArgInfo(name));
    }

    if (!name.empty())
        m_impl->m_argumentTypes[index]->setName(name);

    return m_impl->m_argumentTypes[index].get();
}

jsValue* jsTypeInfo::value()
{
    return m_impl->m_value;
}

size_t jsTypeInfo::arguments() const
{
    return m_impl->m_argumentTypes.size();
}

void jsTypeInfo::nameArguments(const string& argNames)
{
    if (argNames.size() > 0)
    {
        size_t i = 0;
        size_t iPrev = i;
        size_t iarg = 0;
        int iUntypedArg = 0;

        do
        {
            i = argNames.find(",", i);
            string arg;

            if (i == string::npos)
                arg = argNames.substr(iPrev, argNames.size() - iPrev);
            else
            {
                arg = argNames.substr(iPrev, i - iPrev);
                iPrev = ++i;
            }

            if (arg.size() > 0)
            {
                if (iarg < arguments())
                    argument(iarg++)->varName(arg);
                else
                    argument(iarg - 1)->varName(arg, ++iUntypedArg);
            }
        } while (i != string::npos);
    }
}

string jsTypeInfo::format(bool sortable)
{
    if (type() & (jsFunction | jsConstructor))
    {
        string value;

        if (sortable && nameSpace().size() > 0)
            value += nameSpace() + "::";
        else if (sortable)
            value += "z";

        if (returned() && (type() & jsFunction) && !sortable)
        {
            value += returned()->name();
            value += " ";
        }

        value += name();
        value += "(";

        for (size_t i = 0; i < arguments(); i++)
        {
            jsArgInfo* info = argument(i);
            value += info->name();

            if (info->varName().size() > 0 && !sortable)
            {
                value += " ";
                value += info->varName();
            }

            if (i < arguments() - 1)
                value += ", ";
        }

        value += ")";

        return value;
    }
    else
    {
        if (sortable && nameSpace().size() > 0)
            return nameSpace() + "::" + name();
        else if (sortable)
            return "z" + name();
    }

    return "";
}

string const& jsTypeInfo::nameSpace() const
{
    return m_impl->m_localNameSpace;
}

string const& jsTypeInfo::getNameSpaceStatic()
{
    return jsTypeInfoImpl::m_nameSpace;
}

void jsTypeInfo::setNameSpaceStatic(const string& ns)
{
    jsTypeInfoImpl::m_nameSpace = ns;
}

jsTypeInfo& jsTypeInfo::AddDocumentation(const string& doc)
{
    // First invocation will add short description. Every other will append to verbose description.
    if (m_impl->m_docShort.size() == 0)
        m_impl->m_docShort = doc;
    else
    {
        if (m_impl->m_docVerbose.size() > 0)
            m_impl->m_docVerbose += " ";

        m_impl->m_docVerbose += doc;
    }

    return *this;
}

jsExampleInfo& jsTypeInfo::AddExample(const string& example)
{
    m_impl->m_examples.emplace_back(example);

    return m_impl->m_examples.back();
}

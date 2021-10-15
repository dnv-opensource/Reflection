//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSINTERPRETER_H
#define JSINTERPRETER_H

#include <jsScript/jsScript_config.h>
#include <jsScript/jsValue.h>
#include <jsScript/AutoCompletion/jsAutoCompletion.h>
#include <jsScript/jsType.h>
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"

#include <string>
#include <mofaTools/ref.h>
#include "jsStack.h"
#include <memory>

class jsScriptable;
class jsValue;
class jsTypeLibrary;
class jsInterpreterInternal;

class reserved_name : public std::exception {
public:
    reserved_name(const std::string& name);
private:
    static std::string createMessage(const std::string& name);
};

inline reserved_name::reserved_name(const std::string& name)
    :  std::exception(reserved_name::createMessage(name).c_str())
{
}

inline std::string reserved_name::createMessage(const std::string& name)
{
    return "the name \"" + name + "\" is reserved";
}

enum jsExecutionStatus
{
    jsNoExecution,
    jsExecuteString,
    jsExecuteFile,
};

class JSSCRIPT_IMPORT_EXPORT jsInterpreter
{
public:
    jsInterpreter(jsTypeLibrary* typeLibrary);
    virtual ~jsInterpreter();
    jsTypeLibrary& VTBLMap();
    virtual mofa::ref<jsValue> execute(std::istream &istr,bool logging=false) = 0;
    virtual mofa::ref<jsValue> execute(std::string const &str,bool logging=false) = 0;
    virtual mofa::ref<jsValue> test(std::string const &str) = 0;
    virtual std::shared_ptr<jsAutoCompletion> autoComplete(const std::string& str,bool functionCompletion=true) = 0;
    virtual mofa::ref<jsValue> lookup(const std::string& str) = 0;

    virtual void renameMember(jsValue* value,const std::string& identifier) = 0;
    virtual void deleteMember(jsValue* value) = 0;
    virtual void deleteMember(const std::string& identifier) = 0;
    virtual bool hasMember(const std::string& identifier) = 0;

    virtual bool getValueName(const jsValue* value,std::string& name) const = 0;
    virtual void document(const std::string& path,const std::string& programName,const std::string& version,const std::string& versionDate);
    virtual jsScriptable* thisObject() = 0;
    virtual void supportUnits(bool support) = 0;
    virtual jsValue* prototype(const std::string& typeName);
    virtual bool caseInsensitiveFunctions() {return false;}

    void inAssignment(bool inAssignment);
    bool inAssignment() const;

    virtual void setLineNumber(int lineNumber);
    virtual void setLastLineNumber(int lineNumber);

    void addReservedWord(const std::string& word);
    bool isReservedWord(const std::string& word) const;
    virtual void validateIdentifier(const std::string& identifier);

    virtual jsExecutionStatus getExecutionStatus() const = 0;
private:
    jsInterpreterInternal* m_internal;
    bool m_inAssignment;
};

#endif
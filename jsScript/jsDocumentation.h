//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSDOCUMENTATION_H
#define JSDOCUMENTATION_H


#pragma warning(disable:4786)
#include <jsScript/jsScript_config.h>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include "Documentation/TypeMap.h"
#include <list>

class jsValue;
class jsTypeInfo;
class jsExampleInfo;
class jsTypeLibrary;

class JSSCRIPT_IMPORT_EXPORT jsDocumentation
{
public:
    //Don't generate any documentation. just populate the ip.
    jsDocumentation(jsTypeLibrary& typeLibrary);
    jsDocumentation(jsTypeLibrary& typeLibrary, const std::string& path,
        const std::string& programName,
        const std::string& version,
        const std::string& versionDate);
    virtual ~jsDocumentation();
    void style(bool screen);
    void document(const DNVS::MoFa::Doc::TypePointer& type);
    void addTitle(std::ostream& out, const DNVS::MoFa::Doc::TypePointer& type);
    void addBody(std::ostream& out, const DNVS::MoFa::Doc::TypePointer& type);
    void addClassHierarchy(std::ostream& out, const DNVS::MoFa::Doc::TypePointer& type);
    void addClassChildren(std::ostream& out, const DNVS::MoFa::Doc::TypePointer& type);
    void addClassData(std::ostream& out, const DNVS::MoFa::Doc::TypePointer& type);
    void addConstructorSummary(std::ostream& out, const DNVS::MoFa::Doc::TypePointer& type);
    void addFunctionSummaryContent(std::map<std::string, std::string >& functionInfo,
        const DNVS::MoFa::Doc::TypePointer& type, bool inherited);
    void addFunctionSummary(std::ostream& out, const DNVS::MoFa::Doc::TypePointer& type);
    void addPropertySummaryContent(std::map<std::string, std::string >& functionInfo,
        const DNVS::MoFa::Doc::TypePointer& type, bool inherited);

    void addPropertySummary(std::ostream& out, const DNVS::MoFa::Doc::TypePointer& type);
    void addItems(std::ostream& out, const DNVS::MoFa::Doc::TypePointer& type);
    void addClassHierarchy();
    void addIndexItems(std::ostream& out);
    void addIndex();
    void addNavBar(std::ostream& out);
    void addDetail(std::ostream& out, const DNVS::MoFa::Doc::TypePointer& type);
    void addConstructorDetail(std::ostream& out, const DNVS::MoFa::Doc::TypePointer& type);
    void addFunctionDetail(std::ostream& out, const DNVS::MoFa::Doc::TypePointer& type);
    void addPropertyDetail(std::ostream& out, const DNVS::MoFa::Doc::TypePointer& type);
    DNVS::MoFa::Doc::TypeMap& GetTypeMap() { return m_typeMap; }
    std::string makeOverviewLine(const std::string& nameSpace, const std::string& headerTxt, const std::string& explanationTxt);
    std::string makeExample(std::string exampleTxt, bool legal = true);
    std::string makeExamples(const std::vector<jsExampleInfo>& examples);

    std::string makeHREF(const std::string& typeName, const DNVS::MoFa::Doc::TypePointer& type, bool bold = false, bool href = true, std::string titleTip = "") const;
    std::string makeHREF(const DNVS::MoFa::Doc::MemberPointer& member, const DNVS::MoFa::Doc::TypePointer& type, bool bold = false, bool href = true, std::string titleTip = "") const;
    std::string argument(const DNVS::MoFa::Doc::TypePointer& type, const DNVS::MoFa::Doc::MemberPointer& member, std::size_t iarg, bool stripped = false) const;
    std::string argumentList(const DNVS::MoFa::Doc::TypePointer& type, const DNVS::MoFa::Doc::MemberPointer& member) const;
    std::string argumentDocumentation(const DNVS::MoFa::Doc::MemberPointer& member) const;
    std::string argumentReturnDocumentation(const DNVS::MoFa::Doc::MemberPointer& member) const;
    std::string memberDocumentation(const DNVS::MoFa::Doc::MemberPointer& member) const;
    typedef std::map<std::string, std::string> SortedString;
    const SortedString& GetLeaves() const { return m_leaves; }
    void addType(jsValue* value);
private:
    std::string GetMethodHeader(const std::string& methodType, const std::string& headerType, size_t colSpan = 1) const;

    typedef std::map<std::string, DNVS::MoFa::Doc::TypePointer> Sorted;
    Sorted m_documented;
    std::string m_title;
    std::string m_version;
    std::string m_versionDate;
    std::string m_directory;
    jsTypeLibrary& m_typeLibrary;
    typedef std::map<std::string, int> DocError;
    DocError m_docError;
    SortedString m_leaves;
    DNVS::MoFa::Doc::TypeMap m_typeMap;
    std::string memberToString(const DNVS::MoFa::Doc::MemberPointer& member, const DNVS::MoFa::Doc::TypePointer& type);
    std::list<DNVS::MoFa::Doc::TypePointer> GetAllBases(const DNVS::MoFa::Doc::TypePointer& type) const;
    void AddLeaves(const std::vector<DNVS::MoFa::Doc::MemberPointer>& members, const DNVS::MoFa::Doc::TypePointer& type);
};

#endif

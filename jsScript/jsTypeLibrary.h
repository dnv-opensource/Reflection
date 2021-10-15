//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSVTBLMAP_H
#define JSVTBLMAP_H


#pragma warning(disable:4786)
#include <jsScript/jsScript_config.h>
#include <map>
#include <typeinfo>
#include <string>
#include <set>
#include <memory>
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"

class jsVTBL;
namespace DNVS { namespace MoFa { namespace TypeUtilities {
    class IFunctionDetailService;
}}}

class JSSCRIPT_IMPORT_EXPORT jsTypeLibrary
{
public:
    jsTypeLibrary(const DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer& reflectionTypeLibrary);
    virtual ~jsTypeLibrary();
    void SetTypeName(const type_info& info, const std::string& name);
    std::string GetTypeName(const std::string& info) const;
    std::string GetTypeName(const type_info& info) const;
    jsVTBL* add(const type_info& type, const type_info& parent_type);
    jsVTBL* lookup(const type_info& type) const;
    jsVTBL* lookup(const std::string& typeName) const;
    void unite(const type_info& type1, const type_info& type2);
    void enable(const type_info& type, bool flag);
    bool isEnabled(const type_info& info) const;
    void LogFunctionDetails(const std::shared_ptr<DNVS::MoFa::TypeUtilities::IFunctionDetailService>& service);
    virtual bool IsCaseSensitive() const;
    const DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer& GetReflectionTypeLibrary() const;
    void addReservedWord(const std::string& word);
    bool isReservedWord(const std::string& word) const;
	typedef std::map<const std::string, jsVTBL*> VTBLMap;
	const VTBLMap& GetVTBLMap() const;
private:
    static void replace(std::string& name, const std::string& problem, const std::string& substitute);
    VTBLMap m_vtblMap;
    typedef std::map<std::string, std::string> NameMap;
    NameMap m_nameMap;
    DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer m_reflectionTypeLibrary;
    std::set<std::string> m_reservedWords;
    std::set<std::string> m_caseLessReservedWords;
};

#endif
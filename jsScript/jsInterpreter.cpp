//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsScript/jsInterpreter.h>
#include <jsScript/jsInterpreterInternal.h>
#include <jsScript/jsScriptable.h>
#include <jsScript/jsVTBL.h>
#include <jsScript/jsDocumentation.h>
#include <iostream>
#include <boost/algorithm/string/case_conv.hpp>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

using namespace std;

jsInterpreter::jsInterpreter(jsTypeLibrary* typeLibrary)
:  m_inAssignment(false)
{
   m_internal=new jsInterpreterInternal(typeLibrary);
}

jsInterpreter::~jsInterpreter()
{
   delete m_internal;
}

void jsInterpreter::inAssignment(bool inAssignment)
{
   m_inAssignment=inAssignment;
}

bool jsInterpreter::inAssignment() const
{
   return m_inAssignment;
}

void jsInterpreter::document(const std::string& path,const std::string& programName,const std::string& version,const std::string& versionDate)
{
   jsDocumentation(VTBLMap(),path,programName,version,versionDate);
}

jsTypeLibrary& jsInterpreter::VTBLMap()
{
   return m_internal->vtblMap();
}

jsValue* jsInterpreter::prototype(const std::string& typeName)
{
   return VTBLMap().lookup(typeName);
}

void jsInterpreter::setLineNumber(int lineNumber)
{
}

void jsInterpreter::setLastLineNumber(int lineNumber)
{
}

void jsInterpreter::addReservedWord(const std::string& word)
{
    VTBLMap().addReservedWord(word);
}

bool jsInterpreter::isReservedWord(const std::string& word) const
{
    return const_cast<jsInterpreter*>(this)->VTBLMap().isReservedWord(word);
}

void jsInterpreter::validateIdentifier(const std::string& identifier)
{
    if(isReservedWord(identifier)) throw std::runtime_error(identifier + " is reserved, and cannot be used as a named variable.");
}
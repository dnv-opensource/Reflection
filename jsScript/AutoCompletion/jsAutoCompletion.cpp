//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "jsAutoCompletion.h"
#include <algorithm>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

jsAutoCompletion::jsAutoCompletion(const std::string& text,int startCompletion)
:  m_text(text)
,  m_startCompletion(startCompletion)
,  m_startFunction(-1)
{
}

jsAutoCompletion::~jsAutoCompletion()
{
}

void jsAutoCompletion::setFunction(int startFunction)
{
   m_startFunction=startFunction;
}

void jsAutoCompletion::addFunction(const std::string& name)
{
   m_functions.insert(name);
}

void jsAutoCompletion::addVariable(const std::string& name)
{
   std::string complete=completeText();
   std::string compare=name;

   std::transform(complete.begin(),complete.end(),complete.begin(),tolower);
   std::transform(compare.begin(),compare.end(),compare.begin(),tolower);

   if(!compare.compare(0,complete.size(),complete)) {
      m_variables.insert(name);
   }
}

bool jsAutoCompletion::inFunction() const
{
   return m_startFunction!=-1;
}

jsAutoCompletion::iterator jsAutoCompletion::function_begin() const
{
   return m_functions.begin();
}

jsAutoCompletion::iterator jsAutoCompletion::function_end() const
{
   return m_functions.end();
}

size_t   jsAutoCompletion::function_size() const
{
   return m_functions.size();
}

const std::set<std::string>& jsAutoCompletion::function_set() const
{
   return m_functions;
}

jsAutoCompletion::iterator jsAutoCompletion::variable_begin() const
{
   return m_variables.begin();
}

jsAutoCompletion::iterator jsAutoCompletion::variable_end() const
{
   return m_variables.end();
}

size_t   jsAutoCompletion::variable_size() const
{
   return m_variables.size();
}

const std::set<std::string>& jsAutoCompletion::variable_set() const
{
   return m_variables;
}

std::string jsAutoCompletion::completeText() const
{
   if(m_startCompletion<0) return "";
   return m_text.substr(m_startCompletion,m_text.size()-m_startCompletion);
}

std::string jsAutoCompletion::lineText() const
{
   return m_text.substr(0,m_startCompletion);
}

std::string jsAutoCompletion::functionText() const
{
   return m_text.substr(m_startFunction,m_text.size()-m_startFunction);
}

std::string jsAutoCompletion::text() const
{
   return m_text;
}

int jsAutoCompletion::startCompletion() const
{
   return m_startCompletion;
}

int jsAutoCompletion::startFunction() const
{
   return m_startFunction;
}

std::set<std::string> jsAutoCompletion::GetFunctionSet() const
{
    return function_set();
}

std::set<std::string> jsAutoCompletion::GetVariableSet() const
{
    return variable_set();
}

bool jsAutoCompletion::IsInFunction() const
{
    return inFunction();
}

std::string jsAutoCompletion::GetFunctionName() const
{
    return functionText();
}

int jsAutoCompletion::GetFunctionStartPosition() const
{
    return startFunction();
}

int jsAutoCompletion::GetStartOfCompletion() const
{
    return startCompletion();
}

//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#ifndef JSAUTOCOMPLETION_H
#define JSAUTOCOMPLETION_H

#include <jsScript/jsScript_config.h>
#include <string>
#include <set>
#include "Scripting/IAutoCompletion.h"

#pragma warning(push)
#pragma warning(disable:4275)
class JSSCRIPT_IMPORT_EXPORT jsAutoCompletion : public DNVS::MoFa::Scripting::IAutoCompletion
{
public:
	jsAutoCompletion(const std::string& text,int startCompletion);
	virtual ~jsAutoCompletion();
   void setFunction(int startFunction);
   void addFunction(const std::string& name);
   void addVariable(const std::string& name);
   bool inFunction() const;

   typedef std::set<std::string>::const_iterator iterator;
   iterator function_begin() const;
   iterator function_end() const;
   size_t   function_size() const;
   const std::set<std::string>& function_set() const;

   iterator variable_begin() const;
   iterator variable_end() const;
   size_t   variable_size() const;
   const std::set<std::string>& variable_set() const;

   std::string completeText() const;
   std::string lineText() const;
   std::string functionText() const;
   std::string text() const;
   int startCompletion() const;
   int startFunction() const;

   virtual std::set<std::string> GetFunctionSet() const override;
   virtual std::set<std::string> GetVariableSet() const override;
   virtual bool IsInFunction() const override;
   virtual std::string GetFunctionName() const override;
   virtual int GetFunctionStartPosition() const override;
   virtual int GetStartOfCompletion() const override;
private:
   int m_startCompletion;
   int m_startFunction;
   std::set<std::string> m_functions;
   std::set<std::string> m_variables;
   std::string m_text;
};
#pragma warning(pop)

#endif
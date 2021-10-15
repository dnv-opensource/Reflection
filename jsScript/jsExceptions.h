//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#ifndef JSEXCEPTION_H
#define JSEXCEPTION_H

#include <jsScript/jsScript_config.h>
#include <jsScript/jsStack.h>
#include <string>
#include <mofaTools/lexical_cast.h>
#include <stdexcept>


class argument_exception : public std::runtime_error {
public:
   argument_exception(const std::string& what) : std::runtime_error(what) {}
};

class incorrect_number_of_arguments : public argument_exception {
public:
   incorrect_number_of_arguments(size_t nargs) : argument_exception(errorMessage(nargs)) {}
   incorrect_number_of_arguments(size_t nargs,size_t nactArgs) : argument_exception(errorMessage(nargs,nactArgs)) {}
private:
   static std::string errorMessage(size_t nargs);
   static std::string errorMessage(size_t nargs,size_t nactArgs);
};

inline std::string incorrect_number_of_arguments::errorMessage(size_t nargs,size_t nactArgs)
{
   std::string error="expected " + mofa::lexical_cast<std::string>(nactArgs) + " parameters, found " + mofa::lexical_cast<std::string>(nargs) + " parameters";
   return error;
}

inline std::string incorrect_number_of_arguments::errorMessage(size_t noargs)
{
   std::string error="method does not take " + mofa::lexical_cast<std::string>(noargs) + " parameters";
   return error;
}

class invalid_overload_argument : public argument_exception {
public:
   invalid_overload_argument(jsValue* member) : argument_exception(errorMessage(member)) {}
private:
   static std::string errorMessage(jsValue* member);
};

inline std::string invalid_overload_argument::errorMessage(jsValue* member)
{
   std::string error="no appropriate overloaded " + member->typeName() + " found";
   return error;
}

class bad_type_conversion : public std::exception {
public:
   bad_type_conversion(const type_info& from,const type_info& to) : std::exception(errorMessage(from,to).c_str()) {}
   bad_type_conversion(const type_info& from) : std::exception(errorMessage(from).c_str()) {}
   bad_type_conversion(const std::string& from,const type_info& to) : std::exception(errorMessage(from,to).c_str()) {}
   bad_type_conversion(const std::string& from,const std::string& to) : std::exception(errorMessage(from,to).c_str()) {}
private:
   static std::string errorMessage(const std::string& from,const type_info& to);
   static std::string errorMessage(const std::string& from,const std::string& to);
   static std::string errorMessage(const type_info& from,const type_info& to);
   static std::string errorMessage(const type_info& from);
};

inline std::string bad_type_conversion::errorMessage(const std::string& from,const type_info& to)
{
   std::string error="cannot convert from \'" + from + "\' to \'" + jsStack::stack()->typeName(to) + "\'";
   return error;
}

inline std::string bad_type_conversion::errorMessage(const std::string& from,const std::string& to)
{
   std::string error="cannot convert from \'" + from + "\' to \'" + to + "\'";
   return error;
}

inline std::string bad_type_conversion::errorMessage(const type_info& from,const type_info& to)
{
   std::string error="cannot convert from \'" + jsStack::stack()->typeName(from) + "\' to \'" + jsStack::stack()->typeName(to) + "\'";
   return error;
}

inline std::string bad_type_conversion::errorMessage(const type_info& from)
{
   std::string error="cannot convert from \'" + jsStack::stack()->typeName(from) + "\'";
   return error;
}

class invalid_argument : public argument_exception {
public:
   invalid_argument(int iarg,const bad_type_conversion& e) : argument_exception(errorMessage(iarg,e)) {}
private:
   static std::string errorMessage(int iarg,const bad_type_conversion& e);
   static std::string errorMessage(const std::string& str,const bad_type_conversion& e);
};

inline std::string invalid_argument::errorMessage(int iarg,const bad_type_conversion& e)
{
   if(iarg==0) return errorMessage("'this' ",e);
   else return errorMessage("parameter " + mofa::lexical_cast<std::string>(iarg) + " ",e);
}

inline std::string invalid_argument::errorMessage(const std::string& str,const bad_type_conversion& e)
{
   std::string error=e.what();
   error.insert(15,str);
   return error;
}

class line_exception : public std::exception {
public:
   line_exception(const std::string& error,int line) : std::exception(error.c_str()) , m_line(line) {}
   virtual ~line_exception() {}
   int line() {return m_line;}
private:
   int m_line;
};
   /*
   class reserved_name : public std::exception {
   public:
      reserved_name(const std::string& identifier) : reserved_name(errorMessage(identifier)) {}
   private:
      static std::string errorMessage(const std::string& identifier);
   };

   inline std::string reserved_name::errorMessage(const std::string& identifier)
   {
      std::string error="Identifier \'" + identifier + "\' is reserved";
   }*/

/*
	function_exception
	vtbl_not_found
	writeonly_property
	readonly_property
	rename_error
	delete_error

compiletime_exception
	invalid_member_overload
*/
#endif
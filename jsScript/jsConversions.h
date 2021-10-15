//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#ifndef JSCONVERSIONS_H
#define JSCONVERSIONS_H

#include <jsScript/jsValue.h>
#include <jsScript/jsType.h>
#include <jsScript/jsExceptions.h>
#include "jsTypeConverter.h"

template<typename T>
T TryConvertOrDefault(jsValue* value,T defaultValue) {
    try {
        return fromJScript(value,jsType<T>());
    }
    catch(...) {
        return defaultValue;
    }
}

template<class T>
T fromJScript(jsValue* value,jsType<T>) {
    jsTypeConverter<jsValue*,T> converter(jsStack::stack());
    return converter.Convert(value);
}

template<class T>
jsValue* toJScript(T value) {
    jsTypeConverter<T,jsValue*> converter(jsStack::stack());
    return converter.Convert(value);
}

template<class T>
jsValue* toJScriptAllowReferences(T&& value) {
	return toJScript(value);
}

template<class T>
jsValue* toJScriptAllowReferences(T& value) {
	jsTypeConverter<T&, jsValue*> converter(jsStack::stack());
	return converter.Convert(value);
}

template<class Arg>
void verifyNoArguments(size_t arguments,const jsValue::Params& params,jsType<Arg>)
{
   if(params.size()!=arguments+1) throw incorrect_number_of_arguments(params.size()-1);
}

inline void verifyNoArguments(size_t arguments,const jsValue::Params& params,jsType<const jsValue::Params&>)
{
   if(params.size()<arguments) throw incorrect_number_of_arguments(params.size()-1);
}

template<class Type>
Type paramsFromJScript(const jsValue::Params& params,size_t arg,jsValue::Params& args,jsType<Type>)
{
   return fromJScript(params[arg],jsType<Type>());
}

inline const jsValue::Params& paramsFromJScript(const jsValue::Params& params,size_t arg,jsValue::Params& args,jsType<const jsValue::Params&>)
{
   for(size_t i=arg;i<params.size();i++) {
      args.push_back(params[i]);
   }
   return args;
}

#endif

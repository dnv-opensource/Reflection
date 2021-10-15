//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#ifndef JSARRAY_H
#define JSARRAY_H

#include <jsScript/jsScriptable.h>
#include "jsScript\jsValueIterator.h"
#include "boost/foreach.hpp"
#include <type_traits>

class JSSCRIPT_IMPORT_EXPORT jsArray : public jsScriptable
{
public: //external
   jsArray(const Params& params);

   unsigned int get_length() const;
   void put_length(unsigned int length);

   unsigned int push(const Params& params);
   jsValue* pop();

   jsValueIterator child_begin() const;
   jsValueIterator child_end() const;
   size_t          child_size() const;
public: //internal
	jsArray(unsigned int length);
	virtual ~jsArray();

   jsValue* operator[](unsigned int index) const;
   jsValue* operator[](unsigned int index);

   virtual jsValue* subscript(jsValue* index);

   static void init(jsTypeLibrary& typeLibrary, bool RegisterConstructor = true);
private:
   std::vector<mofa::ref<jsValue> > m_vector;
};

//Converts to a jsArray. ScriptType is currently not used.
template<typename ScriptType,typename ContainerType>
jsArray* ToArray(const ContainerType& container) {
    jsArray* array=new jsArray(container.size());
    std::size_t index=0;
    BOOST_FOREACH(auto item,container) {
        jsValue* value=(*array)[index++];
        value->assign(NULL,toJScript(item));
    }
    return array;        
}
//Converts from a jsArray to a container. You need to specify the type you want to convert to as ScriptType.
template<typename ScriptType,typename ContainerType>
void FromArray(jsArray* array,ContainerType& container,std::enable_if_t<!std::is_class_v<ScriptType>, void*> = 0) {
    for(jsValueIterator it=array->child_begin();it!=array->child_end();++it) {
        jsValue* value=*it;
        ScriptType scriptType=fromJScript(value,jsType<ScriptType>());
        container.insert(container.end(),scriptType);
    }
}

template<typename ScriptType,typename ContainerType>
void FromArray(jsArray* array,ContainerType& container, std::enable_if_t<std::is_class_v<ScriptType>, void*> = 0) {
    for(jsValueIterator it=array->child_begin();it!=array->child_end();++it) {
        jsValue* value=*it;
        const ScriptType& scriptType=fromJScript(value,jsType<const ScriptType&>());
        container.insert(container.end(),scriptType);
    }
}

#endif

//  (C) Copyright David Abrahams 2000. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  The author gratefully acknowleges the support of Dragon Systems, Inc., in
//  producing this work.
//

//
#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

#include <jsScript/jsConversions.h>

class jsValue;

// Calling C++ from JScript

struct getter
{
   template <class R,class T>
   static jsValue* duplicate(R (T::*pmf)(), jsValue* owner) {
      int iarg = 0;
      try {
         T& target = fromJScript(owner, jsType<T&>());
         iarg++;
         if(jsStack::stack()->dummyMode()) {
            if(jsValue* value=jsStack::stack()->getDummy(typeid(R),typeid(target))) return value;
         }
         return toJScriptAllowReferences((target.*pmf)());
      }
      catch(bad_type_conversion& e) {
         if(iarg>0) throw e;
         else throw invalid_argument(iarg, e);
      }
   }

   template <class R,class T>
   static jsValue* duplicate(R (T::*pmf)() const, jsValue* owner) {
      int iarg = 0;
      try {
         T& target = fromJScript(owner, jsType<T&>());
         iarg++;
         return toJScriptAllowReferences((target.*pmf)());
      }
      catch(bad_type_conversion& e) {
         if(iarg>0) throw e;
         else throw invalid_argument(iarg, e);
      }
   }
   template <class R>
   static jsValue* duplicate(R (*pf)(), jsValue* owner) {
      int iarg = 0;
      try {
         iarg++;
         if(jsStack::stack()->dummyMode()) {
            if(jsValue* value=jsStack::stack()->getDummy(typeid(R))) return value;
         }
         return toJScriptAllowReferences(pf());
      }
      catch(bad_type_conversion& e) {
         if(iarg>0) throw e;
         else throw invalid_argument(iarg, e);
      }
   }
   static jsValue* duplicate(jsValue* value,jsValue* owner) {
      return toJScript(value->duplicate(owner));
   }
};

struct setter 
{
   template <class T, class A1>
   static jsValue* assign(void (T::*pmf)(A1),jsValue* owner,jsValue* op2) {
      int iarg = 0;
      try {
         jsValue::Params args;
         T& target = fromJScript(owner, jsType<T&>());
         A1 arg1 = fromJScript(op2,jsType<A1>()); //In case the last param is a parameter list
         iarg++;
         if(jsStack::stack()->dummyMode()) return NULL;
         ((target.*pmf)(arg1));
         return NULL;
      }
      catch(bad_type_conversion& e) {
         if(iarg>1) throw e;
         else throw invalid_argument(iarg, e);
      }
   }
   template <class A1>
   static jsValue* assign(void (*pf)(A1),jsValue* owner,jsValue* op2) {
      int iarg = 0;
      try {
         jsValue::Params args;
         A1 arg1 = fromJScript(op2,jsType<A1>()); //In case the last param is a parameter list
         iarg++;
         if(jsStack::stack()->dummyMode()) return NULL;
         (pf(arg1));
         return NULL;
      }
      catch(bad_type_conversion& e) {
         if(iarg>1) throw e;
         else throw invalid_argument(iarg, e);
      }
   }
};


#endif

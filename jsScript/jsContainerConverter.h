//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#ifndef AFX_JSCONTAINERCONVERTER_H
#define AFX_JSCONTAINERCONVERTER_H

#include <jsScript/jsValue.h>
#include <jsScript/jsArray.h>
#include <jsScript/jsValueIterator.h>
#include <jsScript/jsConversions.h>

template<typename Container>
bool jsContainerConverter(const jsValue::Params& params,Container& container,int iarg=0,int imax=-1,int istart=0)
{
   typedef Container::value_type value_type;
   if(imax==-1) imax=params.size()-istart;
   if(imax>(int)params.size()-istart) return false;
   try {
      for(int i=istart;i<istart+imax;++i) {
         iarg++;
         container.insert(container.end(),fromJScript(params[i],jsType<value_type>()));
      }
   }
   catch(bad_type_conversion& e) {
      throw invalid_argument(iarg, e);
   }      
   return true;
}

template<typename Container>
bool jsContainerConverter(const jsArray& params,Container& container,int iarg=0)
{
   typedef Container::value_type value_type;
   try {
      for(jsValueIterator it=params.child_begin();it!=params.child_end();++it) {
         iarg++;
         container.insert(container.end(),fromJScript(*it,jsType<value_type>()));
      }
   }
   catch(bad_type_conversion& e) {
      throw invalid_argument(iarg, e);
   }     
   return true;
}
#endif

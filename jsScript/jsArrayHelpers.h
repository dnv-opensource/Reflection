//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#ifndef jsScript_jsArrayHelpers_h
#define jsScript_jsArrayHelpers_h

#include <jsScript/jsArray.h>
#include <vector>
#include <set>
#include <string>

namespace jsArrayHelpers {
   // Convert from jsArray to database containers
   // Convert an array of entries to a vector with the following call:
   // std::vector<double> result;
   // jsArrayHelpers::ConvertVector<jsLength>(array, &result);
   template <class UnitClass>
   class ConvertVector {
   public:
      template <class V>
      ConvertVector(const jsArray& array, V *result) {
	      size_t size  = array.get_length();

         result->resize(size);
         for (size_t i=0; i<size; ++i) {
            try {
		         (*result)[i] = fromJScript(array[i]->duplicate(), jsType<const UnitClass&>());         
            }
            catch (...) {
               throw std::exception("Unable to convert array entries to double.  May be due to wrong unit specification.");
            }
         }
      }
   };
   
   // Convert an array of entries to a set with the following call:
   // Set<double> result;
   // jsArrayHelpers::ConvertSet<jsLength>(array, &result);
   template <class UnitClass>
   class ConvertSet {
   public:
      template <class V>
      ConvertSet(const jsArray& array, V *result) {
	      size_t size  = array.get_length();
         result->clear();
         for (size_t i=0; i<size; ++i) {
            try {
		         result->insert(fromJScript(array[i]->duplicate(), jsType<const UnitClass&>()));         
            }
            catch (...) {
               throw std::exception("Unable to convert array entries to double.  May be due to wrong unit specification.");
            }
         }
      }
   };

   // Convert from database containers to string
   // std::string result = jsArrayHelpers::GetString<jsLength, pVector<double> >(my_vector)();
   template <class UnitClass, class ContainerClass>
   class GetString {
   public:
      GetString(const ContainerClass& container) {
         m_text += "Array(";
         for (ContainerClass::const_iterator itr=container.begin(); itr!=container.end(); ++itr) {
            m_text += UnitClass(*itr);
            if ((itr+1) != container.end())
               m_text += ",";
         }
         m_text += ")";
      }
      const std::string& operator() () const {return m_text;}
   private:
      std::string m_text; 
   };

   // Create an array of the key values in a map
   // std::string result = jsArrayHelpers::GetMapFirst<jsLength, pMap<double, double> >(my_map)();
   template <class UnitClass, class MapClass>
   class GetMapFirst {
   public:
      GetMapFirst(const MapClass& container) {
         m_text += "Array(";
         for (MapClass::const_iterator itr=container.begin(); itr!=container.end();) {
            m_text += UnitClass(itr->first);
            ++itr;
            if (itr != container.end())
               m_text += ",";
         }
         m_text += ")";
      }
      const std::string& operator() () const {return m_text;}
   private:
      std::string m_text; 
   };

   // Create an array of the second values in a map
   // std::string result = jsArrayHelpers::GetMapSecond<jsLength, pMap<double, double> >(my_map)();
   template <class UnitClass, class MapClass>
   class GetMapSecond {
   public:
      GetMapSecond(const MapClass& container) {
         m_text += "Array(";
         for (MapClass::const_iterator itr=container.begin(); itr!=container.end();) {
            m_text += UnitClass(itr->second);
            ++itr;
            if (itr != container.end())
               m_text += ",";
         }
         m_text += ")";
      }
      const std::string& operator() () const {return m_text;}
   private:
      std::string m_text; 
   };
}

#endif
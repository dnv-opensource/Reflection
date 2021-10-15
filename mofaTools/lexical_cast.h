//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef LEXICAL_CAST_H
#define LEXICAL_CAST_H

#include <sstream>
#include <string>

namespace mofa {

template<typename Target, typename Source>
Target lexical_cast(Source arg)
{
  std::stringstream interpreter;
  Target result;

  if(!(interpreter << arg) || !(interpreter >> result) ||
     !(interpreter >> std::ws).eof())
  {
     std::string error = "Bad lexical cast: from ";
     error += typeid(Source).name();
     error += " to ";
     error += typeid(Target).name();

     throw std::exception(error.c_str());
  }

  return result;
}

}

#endif
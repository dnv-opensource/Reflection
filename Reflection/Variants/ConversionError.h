#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <stdexcept>
#include <typeindex>
#include <string>

namespace DNVS {namespace MoFa {namespace Reflection {namespace Variants {

    class ConversionError : public std::runtime_error 
    {
    public:
        ConversionError(const std::type_index& from,const std::type_index& to) 
            : std::runtime_error("Unable to convert from " + std::string(from.name()) + " to " + std::string(to.name()))
        {}
        virtual ~ConversionError() {}
    };

}}}}
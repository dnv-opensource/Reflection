#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "ArgWithDefault.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Members {

    /**Defines an argument in a signature. Use to add argument documentation.
    Example: AddSignature( (Arg("first"),Arg("second")) );
    Note the extra set of parenthesis
    */
    class Arg : public NamedArgumentList<1> {
    public:
        Arg(const std::string& name, const std::string& documentation = "")
            : NamedArgumentList<1>(new ArgumentInfo(name, documentation))
        {
        }

        Arg(const char* name, const std::string& documentation = "")
            : NamedArgumentList<1>(new ArgumentInfo(name, documentation))
        {
        }

        /// Assigns a default value to this argument.
        template <typename T>
        ArgWithDefault operator=(const T& inDefault)
        {
            return ArgWithDefault(at(0), Variants::VariantService().ReflectType<T>(inDefault));
        }
    };

    class RenamedArg : public NamedArgumentList<1> {
    public:
        RenamedArg(const std::string typeAlias, const std::string& name, const std::string& documentation = "")
            : NamedArgumentList<1>(new ArgumentInfo(typeAlias, name, documentation))
        {
        }

        RenamedArg(const std::string typeAlias)
            : NamedArgumentList<1>(new ArgumentInfo(typeAlias, "", ""))
        {
        }

        RenamedArg(const char* typeAlias, const char* name, const std::string& documentation = "")
            : NamedArgumentList<1>(new ArgumentInfo(typeAlias, name, documentation))
        {
        }

        RenamedArg(const char* typeAlias)
            : NamedArgumentList<1>(new ArgumentInfo(typeAlias, "", ""))
        {
        }

        /// Assigns a default value to this argument.
        template <typename T>
        ArgWithDefault operator=(const T& inDefault)
        {
            return ArgWithDefault(at(0), Variants::VariantService().ReflectType<T>(inDefault));
        }
    };

}}}}

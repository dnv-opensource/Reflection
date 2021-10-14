#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <initializer_list>
#include <vector>
namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {
    enum class ShortSyntaxArgument {
        //Evaluate argument using short syntax notation. 
        //Example: 
        // 'ConstantLocalSystem(Vector3d(1,2,3),false)' becomes
        // '1 2 3' if first argument is ShortSyntax and second is Implicit
        ShortSyntax, 
        //Evaluate argument using regular script syntax notation
        //Example: 
        // 'ConstantLocalSystem(Vector3d(1,2,3),false)' becomes
        // 'Vector3d(1, 2, 3)' if first argument is ScriptSyntax and second is Implicit
        ScriptSyntax,
        // Ignore this argument. During reconstruction of an object from the argument, 
        // deduce this value from previous valid object or from the default value of this argument.
        Implicit
    };

    /**
    Use this attribute on a constructor or method with the ListAsConstructor attribute to control short-syntax evaluation of
    this type.
    Usage:
    Class<ConstantCurveOffset> cls(...);
    cls.Constructor<const Vector3d, bool>()
       .AddSignature("constantOffset", "useLocalSystem")
       .AddAttribute<ShortSyntaxAttribute>(ShortSyntaxArgument::ShortSyntax, ShortSyntaxArgument::Implicit,"0.1 m 0.3 m 0 m");
              
    Formatting an object of type ConstantCurveOffset with short syntax evaluation enabled:
    'ConstantCurveOffset(Vector3d(1 m, 2 m, 0 m), false)' 
    will be formatted as:
    '1 m 2 m 0 m'    

    Parsing of invalid short syntax text '0.1 4' will give the error message:
    "Invalid short syntax for type ConstantCurveOffset, expected syntax: '0.1 m 0.3 m 0m'"


    */
    class ShortSyntaxAttribute 
    {
    public:
        template<typename... Arguments>
        ShortSyntaxAttribute(const Arguments&... arguments)
        {
            DeduceArgument(arguments...);
        }
        const std::vector<ShortSyntaxArgument>& GetArguments() const { return m_arguments; }
        const std::string& GetSampleShortSyntax() const { return m_sampleShortSyntax; }
    private:
        void DeduceArgument(const std::string& sampleShortSyntax)
        {
            m_sampleShortSyntax = sampleShortSyntax;
        }
        void DeduceArgument()
        {}
        template<typename... Arguments>
        void DeduceArgument(ShortSyntaxArgument arg, const Arguments&... tail)
        {
            m_arguments.push_back(arg);
            DeduceArgument(tail...);
        }
        std::vector<ShortSyntaxArgument> m_arguments;
        std::string m_sampleShortSyntax;
    };
}}}}

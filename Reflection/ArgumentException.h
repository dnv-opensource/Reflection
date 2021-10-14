#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <exception>
#include <map>
#include <string>

namespace DNVS { namespace MoFa { namespace Reflection {
    class ArgumentException : public std::exception
    {
    public:
        using ArgumentExceptionMap = std::map<size_t, std::string>;
        ArgumentException(const ArgumentExceptionMap& argumentExceptions)
            : m_argumentExceptions(argumentExceptions)
        {
            for (const auto& argumentPair : m_argumentExceptions)
            {
                if (!m_errorMessage.empty())
                    m_errorMessage += "\n";
                m_errorMessage += "Argument: " + std::to_string(argumentPair.first) + " - " + argumentPair.second;
            }
        }
        virtual char const* what() const
        {
            return m_errorMessage.c_str();
        }
        const ArgumentExceptionMap& GetArgumentExceptions() const { return m_argumentExceptions; }
    private:
        std::map<size_t, std::string> m_argumentExceptions;
        std::string m_errorMessage;
    };
}}}
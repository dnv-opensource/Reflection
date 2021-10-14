#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Object.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Objects {

    /**
    A Delegate is a representation of a member function pointer in C++.
    More technically, it resembles closer the Delegate in C#, because the object of the member function pointer is passed in as part of the expression.
    Usage:
    std::string str("Hello world";
    //Represents str as an Object.
    Object obj(typeLibrary, str);
    Delegate d = obj.Lookup("size"); <- Creates a delegate    
    EXPECT_EQ(11, d().As<size_t>()); <- Invokes the delegate (no arguments)
    EXPECT_EQ("Hello", obj.Lookup("substr")(0,5).As<std::string>()); <- Invokes the delegate with two arguments.
    This of course assumes that the std::string type has been registered for reflection already, otherwise this will just throw an exception.    
    */
    class REFLECTION_IMPORT_EXPORT Delegate
    {
    public:
        Delegate(const Object& object, const std::string& member, Members::MemberType type);
        template<typename... Args>
        Object operator()(Args&&... args)
        {
            std::vector<Object> arguments;
            AddArguments(arguments, std::forward<Args>(args)...);
            return m_object.Invoke(m_member, arguments, m_type);
        }
        Members::MemberPointer AsMember();
        template<typename T>
        T As() 
        {
            return (*this)().As<T>();
        }
        Delegate& operator=(const Object& other);
    private:
        //Termintates
        void AddArguments(std::vector<Object>& arguments) {}
        //Object
        template<typename... Args>
        void AddArguments(std::vector<Object>& arguments, const Object& arg, Args&&... args)
        {
            arguments.push_back(arg);
            AddArguments(arguments, std::forward<Args>(args)...);
        }
        template<typename... Args>
        void AddArguments(std::vector<Object>& arguments, Object&& arg, Args&&... args)
        {
            arguments.push_back(arg);
            AddArguments(arguments, std::forward<Args>(args)...);
        }
        template<typename... Args>
        void AddArguments(std::vector<Object>& arguments, Object& arg, Args&&... args)
        {
            arguments.push_back(arg);
            AddArguments(arguments, std::forward<Args>(args)...);
        }
        //Variant
        template<typename... Args>
        void AddArguments(std::vector<Object>& arguments, const Variants::Variant& arg, Args&&... args)
        {
            arguments.push_back(Objects::Object(m_object.GetTypeLibrary(), arg));
            AddArguments(arguments, std::forward<Args>(args)...);
        }
        template<typename... Args>
        void AddArguments(std::vector<Object>& arguments, Variants::Variant&& arg, Args&&... args)
        {
            arguments.push_back(Objects::Object(m_object.GetTypeLibrary(), arg));
            AddArguments(arguments, std::forward<Args>(args)...);
        }
        template<typename... Args>
        void AddArguments(std::vector<Object>& arguments, Variants::Variant& arg, Args&&... args)
        {
            arguments.push_back(Objects::Object(m_object.GetTypeLibrary(), arg));
            AddArguments(arguments, std::forward<Args>(args)...);
        }

        template<typename T, typename... Args>
        void AddArguments(std::vector<Object>& arguments, T && arg, Args&&... args)
        {
            arguments.push_back(Object::Create<T>(m_object.GetTypeLibrary(), std::forward<T>(arg)));
            AddArguments(arguments, std::forward<Args>(args)...);
        }
        template<typename T, typename... Args>
        void AddArgument(std::vector<Object>& arguments, T*& arg, Args&&... args)
        {
            arguments.push_back(Object::Create<T*>(m_object.GetTypeLibrary(), arg));
            AddArguments(arguments, std::forward<Args>(args)...);
        }
        template<typename T, int N, typename... Args>
        void AddArguments(std::vector<Object>& arguments, const T (&arg)[N], Args&&... args)
        {
            arguments.push_back(Object::Create<const T*>(m_object.GetTypeLibrary(), (const T*)(arg)));
            AddArguments(arguments, std::forward<Args>(args)...);
        }
        Object m_object;
        std::string m_member;
        Members::MemberType m_type;
    };
}}}}

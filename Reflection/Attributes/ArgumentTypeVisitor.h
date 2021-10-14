#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Types/DecoratedTypeInfo.h"
#include "Reflection/AutoReflect.h"
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"
#include <vector>
namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {

/*
    The purpose of ArgumentTypeVisitor is to, for each argument type in a function signature, 
    such as SomeFunction(pos3d, std::vector<double>, int),
    do an action for each of the arguments.
    In this case, we want to store the type of each of the arguments in a vector.
    This is exactly what ArgumentTypeVisitor does.
    Inspecting a function signature is rather intricate, since a function signature can contain 
    any number of arguments.
    This class is used in connection with the FunctionTraits helper class, 
    which implements the Visit method that iterates through the argument types of the function and 
    calls a Visit function for each of them.
*/

    class ArgumentTypeVisitor {
    public:
        ArgumentTypeVisitor(std::vector<Types::DecoratedTypeInfo>& argumentTypes)
            : m_argumentTypes(argumentTypes)
        {}
        template<typename T, size_t Index>
        void Visit() {
            if (Index < m_argumentTypes.size())
                m_argumentTypes[Index] = Types::TypeId<T>();
            static_assert(AutoReflect<T>::RequiresReflection == false,"In AddAttribute, you need to pass the typeLibrary as the first argument.");
        }
    private:
        std::vector<Types::DecoratedTypeInfo>& m_argumentTypes;
    };

/*
    This class extends the role of the ArgumentTypeVisitor class.
    In addition to extracting the type of each argument in a function, 
    we also try to call AutoReflect on the types.
    If some arguments happens to be containers, AutoReflect will automatically 
    register reflection information for that container.
    This is required in order to allow using containers from the reflection library.
*/

    class ArgumentTypeVisitorWithTypeLibrary {
    public:
        ArgumentTypeVisitorWithTypeLibrary(const TypeLibraries::TypeLibraryPointer& typeLibrary, std::vector<Types::DecoratedTypeInfo>& argumentTypes)
            : m_argumentTypes(argumentTypes)
            , m_typeLibrary(typeLibrary)
        {}
        template<typename T, size_t Index>
        void Visit() {
            if (Index < m_argumentTypes.size())
                m_argumentTypes[Index] = Types::TypeId<T>(); 
            AutoReflect<T>::Reflect(m_typeLibrary);
        }
    private:
        std::vector<Types::DecoratedTypeInfo>& m_argumentTypes;
        TypeLibraries::TypeLibraryPointer m_typeLibrary;
    };
}}}}
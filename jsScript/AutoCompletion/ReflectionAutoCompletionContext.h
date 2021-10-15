#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IAutoCompletionContext.h"

#include "jsScript\jsValue.h"
#include "Reflection\TypeLibraries\IType.h"
#include "Reflection\TypeLibraries\TypeLibraryPointer.h"

namespace DNVS { namespace MoFa { namespace Scripting {
    class IFunctionWrapper {
    public:
        virtual ~IFunctionWrapper() {}
        virtual Reflection::Objects::Object WrapFunction(const Reflection::Objects::Object& object, const std::string& method) = 0;
        virtual bool TryUnwrapFunction(const Reflection::Objects::Object& function, Reflection::Objects::Object& object, std::string& method) = 0;
    };
    class JSSCRIPT_IMPORT_EXPORT ReflectionAutoCompletionContext : public IAutoCompletionContext
    {
    public:
        ReflectionAutoCompletionContext(const Reflection::TypeLibraries::TypeLibraryPointer& typeLibrary, const std::shared_ptr<IFunctionWrapper>& functionWrapper);
        virtual bool IsGlobalContext() const override;
        virtual void CollectMembers(std::shared_ptr<jsAutoCompletion> autoComplete) const override;
        virtual void CollectFunctions(const Reflection::Objects::Object& function, std::shared_ptr<jsAutoCompletion> autoComplete) const override;
        virtual bool HasContext() const override;
        virtual Reflection::Objects::Object lookup(const std::string& function) override;
        virtual void SetNestedContext(const Reflection::Objects::Object& nestedContext) override;
        virtual void SetGlobalContext() override;
    private:
        void CollectMembersTypeRecursive(std::shared_ptr<jsAutoCompletion> autoComplete, Reflection::TypeLibraries::TypePointer type, Reflection::Members::MemberType types) const;
        void CollectMembersType(std::shared_ptr<jsAutoCompletion> autoComplete, Reflection::TypeLibraries::TypePointer type, Reflection::Members::MemberType types) const;
        Reflection::Objects::Object m_context;
        Reflection::TypeLibraries::TypeLibraryPointer m_typeLibrary;
        std::shared_ptr<IFunctionWrapper> m_functionWrapper;
    };
}}}
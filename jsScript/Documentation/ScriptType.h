#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IType.h"
class jsValue;
namespace DNVS { namespace MoFa { namespace Doc {
    class TypeMap;
    class ScriptType : public IType 
    {
    public:
        ScriptType(jsValue* prototype, TypeMap& typeMap);
        virtual std::string GetName() const override;
        virtual std::set<TypePointer> GetBaseClasses() const override;
        virtual std::set<TypePointer> GetDerivedClasses() const override;
        virtual bool HasDocumentation() const override;
        virtual std::string GetDocumentation() const override;
        virtual std::string GetExample() const override;
        virtual std::vector<jsExampleInfo> GetExamples() const override;
        virtual std::vector<MemberPointer> GetMembers(IMember::MemberType memberType) const override;
        virtual std::string ToString(bool sortable) const override;
        virtual std::string GetNamespace() const override;
    private:
        jsValue* m_prototype;
        TypeMap& m_typeMap;
    };
}}}

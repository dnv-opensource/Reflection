#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IMember.h"
#include "Reflection\Members\IMember.h"

namespace DNVS { namespace MoFa { namespace Doc {
    class TypeMap;
    class ReflectionMember : public IMember 
    {
    public:
        ReflectionMember(const Reflection::Members::MemberPointer& member, TypeMap& typeMap);
        virtual MemberType GetMemberType() const;
        virtual bool HasArgument(size_t index) const override;
        virtual std::string GetArgumentTypeName(size_t index) const override;
        virtual std::string GetReturnedTypeName() const override;
        virtual std::string GetReturnedDocumentation() const override;
        virtual std::string GetVariableName(size_t index, size_t varargIndex = 0) const;
        virtual std::string GetArgumentDocumentation(size_t index, size_t varargIndex = 0) const;
        virtual std::size_t GetNamedVarargCount(size_t index) const;
        virtual std::size_t GetArity() const override;
        virtual std::string GetName() const override;
        virtual std::string GetDocumentation(bool full = true) const override;
        virtual std::vector<jsExampleInfo> GetExamples() const override;
        virtual std::string ToString(bool sortable) const override;
        virtual std::string GetNamespace() const override;

        static MemberType ConvertMemberType(DNVS::MoFa::Reflection::Members::MemberType memberType);
    private:
        bool IsMember() const;
        bool IsProperty() const;
        size_t GetModifiedArgumentIndex(size_t index) const;
        Reflection::Members::MemberPointer m_member;
        TypeMap& m_typeMap;
    };
}}}


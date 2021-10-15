#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <memory>
#include <string>
#include <vector>

class jsExampleInfo;

namespace DNVS { namespace MoFa { namespace Doc {
    class IType;
    typedef std::shared_ptr<IType> TypePointer;

    class IMember {
    public:
        enum MemberType {
            Unknown = 0,
            Function = 1,
            Constructor = 2,
            PropertyGet = 4,
            PropertySet = 8,
            Property = 12,
            Operator = 16,
        };
        virtual ~IMember() {}
        virtual MemberType GetMemberType() const = 0;
        virtual bool HasArgument(size_t index) const = 0;
        virtual std::string GetArgumentTypeName(size_t index) const = 0;
        virtual std::string GetReturnedTypeName() const = 0;
        virtual std::string GetReturnedDocumentation() const = 0;
        virtual std::string GetVariableName(size_t index, size_t varargIndex = 0) const = 0;
        virtual std::string GetArgumentDocumentation(size_t index, size_t varargIndex = 0) const = 0;
        virtual std::size_t GetNamedVarargCount(size_t index) const = 0;
        virtual std::size_t GetArity() const = 0;
        virtual std::string GetName() const = 0;
        virtual std::string GetDocumentation(bool full = true) const = 0;
        virtual std::vector<jsExampleInfo> GetExamples() const = 0;
        virtual std::string ToString(bool sortable) const = 0;
        virtual std::string GetNamespace() const = 0;
    };
    typedef std::shared_ptr<IMember> MemberPointer;
}}}

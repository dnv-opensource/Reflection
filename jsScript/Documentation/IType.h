#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include <memory>
#include <set>
#include <vector>
#include "IMember.h"

class jsExampleInfo;

namespace DNVS { namespace MoFa { namespace Doc {
    class IType;
    typedef std::shared_ptr<IType> TypePointer;
    class IType {
    public:
        virtual ~IType() {}
        virtual std::string GetName() const = 0;
        virtual std::set<TypePointer> GetBaseClasses() const = 0;
        virtual std::set<TypePointer> GetDerivedClasses() const = 0;
        virtual bool HasDocumentation() const = 0;
        virtual std::string GetDocumentation() const = 0;
        virtual std::string GetExample() const = 0;
        //Needs to be refactored when reflection gets this kind of information.
        virtual std::vector<jsExampleInfo> GetExamples() const = 0;
        virtual std::vector<MemberPointer> GetMembers(IMember::MemberType memberType) const = 0;
        virtual std::string ToString(bool sortable) const = 0;
        virtual std::string GetNamespace() const = 0;
    };
}}}

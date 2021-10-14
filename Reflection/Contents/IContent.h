#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Types/DecoratedTypeInfo.h"
#include "Reflection/Objects/Object.h"
#include "Reflection/Members/IMemberFwd.h"

namespace DNVS {namespace MoFa {namespace Reflection { namespace Contents { 
    class REFLECTION_IMPORT_EXPORT IContent {
    public:
        virtual ~IContent() {}
        virtual bool IsConvertibleTo(const Types::DecoratedTypeInfo& typeInfo) const = 0;
        virtual Types::DecoratedTypeInfo GetTypeInfo() const = 0;
        virtual Objects::Object GetObject() const = 0;
        virtual Objects::Object GetObjectConvertTo(const Types::DecoratedTypeInfo& typeInfo) const = 0;
        virtual bool HasStoredObject() const = 0;
        virtual std::string GetDisplayText() const = 0;
        virtual std::string GetScriptText() const = 0;
        virtual bool HasScriptText() const = 0;
        virtual bool IsTextEqual(const IContent& other) const;
        virtual bool IsContentEqual(const IContent& other) const = 0;
        virtual bool IsValid() const = 0;
        virtual bool HasMetaData() const { return true; }
        virtual std::shared_ptr<IContent> TryGetArgument(const Members::MemberPointer& member, size_t index) const = 0;
        virtual std::shared_ptr<IContent> TryChangeType(const Types::DecoratedTypeInfo& typeInfo, const std::shared_ptr<IContent>& existingContent = nullptr) const;
        virtual std::shared_ptr<IContent> TryChangeMember(const Members::MemberPointer& member, const std::shared_ptr<IContent>& existingContent = nullptr) const;
        virtual Members::MemberPointer TryGetBestMember() const;
        virtual Members::MemberPointer GetStoredMember() const;
        virtual std::vector<std::string> DoErrorCheck() const = 0;
        virtual std::shared_ptr<IContent> InvokeProperty(const std::string& propertyName) const = 0;
        virtual std::shared_ptr<IContent> InvokeProperty(const Members::MemberPointer& propertyGetter) const = 0;
        virtual std::shared_ptr<IContent> TryInvoke(const Members::MemberPointer& member, const std::vector<std::shared_ptr<IContent>>& arguments, bool trySimplify = true) const = 0;
        virtual std::vector<std::shared_ptr<IContent>> Split() const = 0;
        virtual bool IsCollection() const = 0;
    };
}}}}
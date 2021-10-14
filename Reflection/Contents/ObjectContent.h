#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IContent.h"
namespace DNVS {namespace MoFa {namespace Reflection { namespace Contents { 
    class REFLECTION_IMPORT_EXPORT ObjectContent : public IContent
    {
    public:
        ObjectContent(const Objects::Object& object);
        bool IsConvertibleTo(const Types::DecoratedTypeInfo& typeInfo) const override;
        Types::DecoratedTypeInfo GetTypeInfo() const override;
        Objects::Object GetObject() const override;
        std::string GetDisplayText() const override;
        std::string GetScriptText() const override;
        virtual bool HasScriptText() const override;
        bool IsContentEqual(const IContent& other) const override;
        bool HasStoredObject() const override;
        bool IsValid() const override;
        std::vector<std::string> DoErrorCheck() const override;
        std::shared_ptr<IContent> TryChangeType(const Types::DecoratedTypeInfo& typeInfo, const std::shared_ptr<IContent>& existingContent) const override;
        std::shared_ptr<IContent> TryInvoke(const Members::MemberPointer& member, const std::vector<std::shared_ptr<IContent>>& arguments, bool trySimplify = true) const override;

        std::shared_ptr<IContent> TryGetArgument(const Members::MemberPointer& member, size_t index) const override;
        std::shared_ptr<IContent> InvokeProperty(const std::string& propertyName) const override;
        std::shared_ptr<IContent> InvokeProperty(const Members::MemberPointer& propertyGetter) const override;


        Objects::Object GetObjectConvertTo(const Types::DecoratedTypeInfo& typeInfo) const override;


        std::vector<std::shared_ptr<IContent>> Split() const override;


        bool IsCollection() const override;

    private:
        Objects::Object m_object;
    };
}}}}
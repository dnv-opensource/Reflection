#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IContent.h"
namespace DNVS {namespace MoFa {namespace Reflection { namespace Contents { 
    class REFLECTION_IMPORT_EXPORT TypedTextContent : public IContent
    {
    public:
        TypedTextContent(const std::string& text, const Types::DecoratedTypeInfo& typeInfo, const std::shared_ptr<IContent>& existingContent = nullptr);
        TypedTextContent(const TypedTextContent& other) = default;
        TypedTextContent(const std::string& text, const Types::DecoratedTypeInfo& typeInfo, const Objects::Object& object, bool useTextAsScriptText = true);
        static std::shared_ptr<IContent> Parse(const std::string& text, const Types::DecoratedTypeInfo& typeInfo, const std::shared_ptr<IContent>& existingContent = nullptr);
        bool IsConvertibleTo(const Types::DecoratedTypeInfo& typeInfo) const override;
        Types::DecoratedTypeInfo GetTypeInfo() const override;
        Objects::Object GetObject() const override;
        std::string GetDisplayText() const override;
        std::string GetScriptText() const override;
        bool IsContentEqual(const IContent& other) const override;
        bool HasStoredObject() const override;
        bool IsValid() const override;
        std::vector<std::string> DoErrorCheck() const override;
        std::shared_ptr<IContent> TryChangeType(const Types::DecoratedTypeInfo& typeInfo, const std::shared_ptr<IContent>& existingContent = nullptr) const override;
        virtual std::shared_ptr<IContent> TryChangeMember(const Members::MemberPointer& member, const std::shared_ptr<IContent>& existingContent = nullptr) const;
        virtual std::shared_ptr<IContent> TryGetArgument(const Members::MemberPointer& member, size_t index) const;
        virtual bool HasScriptText() const override;
        std::shared_ptr<IContent> TryInvoke(const Members::MemberPointer& member, const std::vector<std::shared_ptr<IContent>>& arguments, bool trySimplify = true) const override;

        std::shared_ptr<IContent> InvokeProperty(const std::string& propertyName) const override;
        std::shared_ptr<IContent> InvokeProperty(const Members::MemberPointer& propertyGetter) const override;


        Objects::Object GetObjectConvertTo(const Types::DecoratedTypeInfo& typeInfo) const override;


        std::vector<std::shared_ptr<IContent>> Split() const override;


        bool IsCollection() const override;

    private:
        std::string m_displayText;
        std::string m_scriptText;
        std::string m_parseError;
        Types::DecoratedTypeInfo m_typeInfo;
        Objects::Object m_object;
    };
}}}}
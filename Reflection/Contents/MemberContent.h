#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IContent.h"
#include "Reflection/Members/IMember.h"
namespace DNVS {namespace MoFa {namespace Reflection { namespace Contents { 
    class MemberContent : public IContent
    {
    public:
        MemberContent(const Members::MemberPointer& member, const std::vector<std::shared_ptr<IContent>>& arguments = {}, const std::string& text = "");
        static std::shared_ptr<IContent> Parse(const Members::MemberPointer& member, const std::string& text, const std::shared_ptr<IContent>& existingContent = nullptr);
        bool IsConvertibleTo(const Types::DecoratedTypeInfo& typeInfo) const override;
        Types::DecoratedTypeInfo GetTypeInfo() const override;
        Objects::Object GetObject() const override;

        bool HasStoredObject() const override;
        std::string GetDisplayText() const override;
        std::string GetScriptText() const override;
        bool IsContentEqual(const IContent& other) const override;
        bool IsValid() const override;
        std::shared_ptr<IContent> TryGetArgument(const Members::MemberPointer& member, size_t index) const override;
        Members::MemberPointer TryGetBestMember() const override;
        Members::MemberPointer GetStoredMember() const override;

        std::vector<std::string> DoErrorCheck() const override;
        std::shared_ptr<IContent> InvokeProperty(const std::string& propertyName) const override;
        std::shared_ptr<IContent> InvokeProperty(const Members::MemberPointer& propertyGetter) const override;
        std::shared_ptr<IContent> TryChangeType(const Types::DecoratedTypeInfo& typeInfo, const std::shared_ptr<IContent>& existingContent) const override;
        virtual bool HasScriptText() const override;
        std::shared_ptr<IContent> TryInvoke(const Members::MemberPointer& member, const std::vector<std::shared_ptr<IContent>>& arguments, bool trySimplify = true) const override;

        Objects::Object GetObjectConvertTo(const Types::DecoratedTypeInfo& typeInfo) const override;
        std::vector<std::shared_ptr<IContent>> Split() const override;
        bool IsCollection() const override;

    private:
		bool CanConvertToObject(const TypeLibraries::TypeLibraryPointer& typeLibrary) const;
        size_t GetActualArity() const;
        std::string GetScriptTextUsingAttribute() const;
        bool IsDefaultArgument(size_t index) const;
        Members::MemberPointer m_member;
        std::vector<std::shared_ptr<IContent>> m_arguments;
        std::string m_text;
        void SplitRecursive(std::vector<std::shared_ptr<IContent>>& results, std::vector<std::shared_ptr<IContent>>& args, int index) const;
    };
}}}}
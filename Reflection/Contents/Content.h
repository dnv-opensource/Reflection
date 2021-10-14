#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IContent.h"
#include "Reflection/Members/IMemberFwd.h"
#include "../AutoReflect.h"

namespace DNVS {namespace MoFa {namespace Reflection { namespace Contents { 
    class REFLECTION_IMPORT_EXPORT Content {
    public:
        Content(std::nullptr_t);
        Content(const Content&) = default;
        Content(Content&&) = default;
        Content& operator=(const Content&) = default;
        Content& operator=(Content&&) = default;
        Content();
        Content(const Types::DecoratedTypeInfo& typeInfo);
        Content(const std::shared_ptr<IContent>& content);
        Content(const std::string& text);
        Content(const Objects::Object& object);
        template<typename T>
        Content(const TypeLibraries::TypeLibraryPointer& typeLibrary, T argument)
            : Content(Objects::Object(typeLibrary,argument))
        {
			AutoReflect<T>::Reflect(typeLibrary);
		}
        Content(const Variants::Variant& variant);
        Content(const Objects::Object& object, const Types::DecoratedTypeInfo& typeInfo);
        Content(const Objects::Object& object, const Types::DecoratedTypeInfo& typeInfo, const std::string& text, bool useTextAsScriptText = true);
        Content(const std::string& text, const Types::DecoratedTypeInfo& typeInfo, const Content& oldContent = Content());
        Content(const Members::MemberPointer& member, const std::vector<Content>& arguments, const std::string& text = "");
        Content(const Members::MemberPointer& member);
        Content(const std::vector<Content>& collection);
        Content Assign(const Content& other) const;
        bool IsConvertibleTo(const Types::DecoratedTypeInfo& typeInfo) const;
        Types::DecoratedTypeInfo GetTypeInfo() const;
        Objects::Object GetObject() const;
        Objects::Object GetObjectConvertTo(const Types::DecoratedTypeInfo& typeInfo) const;

        bool HasStoredObject() const;
        std::string GetDisplayText() const;
        std::string GetScriptText() const;
        bool HasScriptText() const;
        bool IsTextEqual(const Content& other) const;
        bool IsContentEqual(const Content& other) const;
        bool AreConstructorArgumentsEqual(const Content& other) const;
        bool IsValid() const;
        Content CopyChangeType(const Types::DecoratedTypeInfo& typeInfo, const Content& oldContent = Content()) const;
        Content CopyChangeMember(const Members::MemberPointer& member, bool allowRecursion = true, const Content& oldContent = Content()) const;
        const std::shared_ptr<IContent>& GetImpl() const;
        Content TryInvoke(const Members::MemberPointer& member, const std::vector<Content>& arguments, bool trySimplify = true) const;
        bool operator==(const Content& other) const;
        Content TryGetArgument(const Members::MemberPointer& member, size_t index) const;
        Members::MemberPointer TryGetBestMember() const;
        std::vector<std::string> DoErrorCheck() const;
        Content InvokeProperty(const std::string& propertyName) const;
        Content InvokeProperty(const Members::MemberPointer& propertyGetter) const;
        explicit operator bool() const { return !IsNull(); }
        template<typename T>
        bool IsConvertibleTo() const noexcept
        {
            return IsConvertibleTo(Types::TypeId<T>());
        }
        template<typename T>
        T As() const
        {
            return GetObjectConvertTo(Types::TypeId<T>()).As<T>();
        }
        std::vector<Content> Split() const;
        bool IsCollection() const;
    private:
        bool IsNull() const;
        std::shared_ptr<IContent> m_content;
    };
}}}}


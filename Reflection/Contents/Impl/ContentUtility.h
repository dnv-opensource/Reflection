#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Contents/IContent.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Contents {
    class ContentUtility {
    public:
        static std::shared_ptr<IContent> TryGetArgument(const Objects::Object& object, const Members::MemberPointer& member, size_t index);
        static Members::MemberPointer TryGetBestMember(const Objects::Object& object);
        template<typename T>
        static std::shared_ptr<IContent> TryInvoke(const T* self, const Members::MemberPointer& member, const std::vector<std::shared_ptr<IContent>>& arguments, bool trySimplify = true)
        {
            if (!self || !member)
                return nullptr;
            return TryInvoke(std::make_shared<T>(*self), member, arguments, trySimplify);
        }
        static std::shared_ptr<IContent> TryInvoke(const std::shared_ptr<IContent>& self, const Members::MemberPointer& member, const std::vector<std::shared_ptr<IContent>>& arguments, bool trySimplify = true);
        static std::shared_ptr<IContent> InvokeProperty(const IContent& content, const std::string& propertyName);
        static std::shared_ptr<IContent> InvokeProperty(const IContent& content, const Members::MemberPointer& propertyGetter);

        static Objects::Object ConvertTo(const Objects::Object& object, const Types::DecoratedTypeInfo& to);
        static bool IsConvertibleTo(const Objects::Object& object, const Types::DecoratedTypeInfo& to);
        static bool IsConvertibleTo(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to);
        static bool IsContainerType(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Types::DecoratedTypeInfo& typeInfo);
        static bool IsConvertibleToContainer(const Objects::Object& object, const Types::DecoratedTypeInfo& typeInfo);
        static bool IsConvertibleToContainer(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& containerType);
        static Objects::Object TryCreateContainer(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Types::DecoratedTypeInfo& typeInfo);
        static bool TryAddItemToContainer(const Objects::Object& container, const Objects::Object& object);
        static std::string StripQuotes(std::string text);
        static bool IsQuoted(const std::string& text);
        static std::string ConditionallyAddQuotes(std::string text, const Types::DecoratedTypeInfo& typeInfo);
    };
}}}}

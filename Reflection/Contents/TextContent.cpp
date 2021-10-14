//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TextContent.h"
#include "IReflectionContentsService.h"
#include "Impl/ContentUtility.h"
#include "MemberContent.h"

namespace DNVS {namespace MoFa {namespace Reflection { namespace Contents { 

    TextContent::TextContent(const std::string& text) 
        : m_text(text)
    {
    }

    bool TextContent::IsConvertibleTo(const Types::DecoratedTypeInfo& typeInfo) const
    {
        return ContentUtility::IsConvertibleTo(GetObject(), typeInfo);
    }

    Types::DecoratedTypeInfo TextContent::GetTypeInfo() const
    {
        return GetObject().ConvertToDynamicType().GetDecoratedTypeInfo();
    }

    Objects::Object TextContent::GetObject() const
    {
        try {
            if (auto service = Services::ServiceProvider::Instance().TryGetService<IReflectionContentsService>())
            {
                return service->Test(m_text);
            }
            return Objects::Object();
        }
        catch (...)
        {
            return Objects::Object();
        }
    }

    Objects::Object TextContent::GetObjectConvertTo(const Types::DecoratedTypeInfo& typeInfo) const
    {
        return ContentUtility::ConvertTo(GetObject(), typeInfo);
    }

    std::string TextContent::GetDisplayText() const
    {
        return m_text;
    }

    std::string TextContent::GetScriptText() const
    {
        return m_text;
    }

    bool TextContent::IsContentEqual(const IContent& other) const
    {
        if (typeid(other) == typeid(MemberContent))
            return other.IsContentEqual(*this);
        return GetObjectConvertTo(other.GetTypeInfo()) == other.GetObject();
    }

    bool TextContent::HasStoredObject() const
    {
        return false;
    }

    bool TextContent::IsValid() const
    {
        return GetObject().IsValid();
    }

    std::vector<std::string> TextContent::DoErrorCheck() const
    {
        if (m_text.empty())
            return { "The input is empty" };
        try {
            if (auto service = Services::ServiceProvider::Instance().TryGetService<IReflectionContentsService>())
            {
                service->Test(m_text);
                return {};
            }
            else
            {
                return { "IReflectionContentsService service not registered" };
            }
        }
        catch (...)
        {
            return {
                "Input does not represent an object",
                "Invalid script expression: '" + m_text + "'"
            };
        }
    }

    bool TextContent::HasScriptText() const
    {
        return true;
    }

    std::shared_ptr<IContent> TextContent::TryInvoke(const Members::MemberPointer& member, const std::vector<std::shared_ptr<IContent>>& arguments, bool trySimplify) const
    {
        return ContentUtility::TryInvoke(this, member, arguments, trySimplify);
    }

    std::shared_ptr<IContent> TextContent::TryGetArgument(const Members::MemberPointer& member, size_t index) const
    {
        return ContentUtility::TryGetArgument(GetObject(), member, index);
    }

    std::shared_ptr<IContent> TextContent::InvokeProperty(const std::string& propertyName) const
    {
        return ContentUtility::InvokeProperty(*this, propertyName);
    }

    std::shared_ptr<IContent> TextContent::InvokeProperty(const Members::MemberPointer& propertyGetter) const
    {
        return ContentUtility::InvokeProperty(*this, propertyGetter);
    }

    std::vector<std::shared_ptr<IContent>> TextContent::Split() const
    {
        return { std::make_shared<TextContent>(*this) };
    }

    bool TextContent::IsCollection() const
    {
        return false;
    }

}}}}


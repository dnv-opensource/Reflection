//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Content.h"
#include "TextContent.h"
#include "TypedTextContent.h"
#include "MemberContent.h"
#include "ObjectContent.h"
#include "TypeContent.h"
#include "MemberSelectorContent.h"
#include "CollectionContent.h"
#include "Impl/ContentUtility.h"
#include "IContent.h"
#include "AssignmentContent.h"
#include "../Types/DynamicTypeTraits.h"

namespace DNVS {namespace MoFa {namespace Reflection { namespace Contents { 

    Content::Content(const std::shared_ptr<IContent>& content) : m_content(content)
    {

    }

    Content::Content(const std::string& text)
        : Content(std::make_shared<TextContent>(text))
    {
    }

    Content::Content(const std::string& text, const Types::DecoratedTypeInfo& typeInfo, const Content& oldContent)
        : Content(TypedTextContent::Parse(text, typeInfo, oldContent.GetImpl()))
    {

    }
    std::vector<std::shared_ptr<IContent>> ConvertContentToContentPointers(const Members::MemberPointer& member, const std::vector<Content>& arguments)
    {
        std::vector<std::shared_ptr<IContent>> content(arguments.size());
        for (size_t i = 0; i < arguments.size(); ++i)
        {
            content[i] = arguments[i].CopyChangeType(member->GetArgumentInfo(i)->GetTypeInfo()).GetImpl();
        }
        return content;
    }
    Content::Content(const Members::MemberPointer& member, const std::vector<Content>& arguments, const std::string& text)
        : Content(std::make_shared<MemberContent>(member, ConvertContentToContentPointers(member, arguments), text))
    {        
    }

    Content::Content(const Objects::Object& object)
        : Content(std::make_shared<ObjectContent>(object))
    {

    }

    Content::Content()
        : m_content(nullptr)
    {
    }

    Content::Content(const Types::DecoratedTypeInfo& typeInfo)
        : Content(std::make_shared<TypeContent>(typeInfo))
    {
    }

    Content::Content(const Members::MemberPointer& member)
        : Content(std::make_shared<MemberSelectorContent>(member))
    {
    }

    Content::Content(const Objects::Object& object, const Types::DecoratedTypeInfo& typeInfo)
    {
        if (object.IsConvertibleTo(typeInfo))
            m_content = std::make_shared<ObjectContent>(object.ConvertTo(typeInfo));
        else
            m_content = std::make_shared<ObjectContent>(object);
    }

    Content::Content(const Variants::Variant& variant)
    {
        m_content = std::make_shared<ObjectContent>(Objects::Object(Services::ServiceProvider::Instance().GetService<TypeLibraries::ITypeLibrary>(), variant));
    }

    Content::Content(const std::vector<Content>& collection)
    {
        std::vector<std::shared_ptr<IContent>> result;
        for (const Content& content : collection)
        {
            if (content.GetImpl())
                result.push_back(content.GetImpl());
        }
        if (result.size() == 1)
            m_content = result.back();
        else if (!result.empty())
            m_content = std::make_shared<CollectionContent>(result);        
    }

    Content::Content(const Objects::Object& object, const Types::DecoratedTypeInfo& typeInfo, const std::string& text, bool useTextAsScriptText)
    {
        m_content = std::make_shared<TypedTextContent>(text, typeInfo, object, useTextAsScriptText);
    }

    Content::Content(std::nullptr_t)
    {
    }

    Content Content::Assign(const Content& other) const
    {
        return std::make_shared<AssignmentContent>(m_content, other.GetImpl());
    }

    bool Content::IsConvertibleTo(const Types::DecoratedTypeInfo& typeInfo) const
    {
        if (IsNull())
        {
            if (Types::IsPointer(typeInfo))
                return true;
            if (ContentUtility::IsContainerType(Services::ServiceProvider::Instance().GetService<TypeLibraries::ITypeLibrary>(), typeInfo))
                return true;
            return false;
        }
        return m_content->IsConvertibleTo(typeInfo);
    }

    std::vector<Content> Content::Split() const
    {
        if (IsNull())
            return {};
        std::vector<Content> result;
        for (const auto& content : m_content->Split())
        {
            result.push_back(content);
        }
        return result;
    }

    bool Content::IsCollection() const
    {
        if (IsNull())
            return false;
        return m_content->IsCollection();
    }

    Types::DecoratedTypeInfo Content::GetTypeInfo() const
    {
        if (IsNull())
            return Types::DecoratedTypeInfo();
        return m_content->GetTypeInfo();
    }

    Objects::Object Content::GetObject() const
    {
        if (IsNull())
            return Objects::Object();
        return m_content->GetObject();
    }

    Objects::Object Content::GetObjectConvertTo(const Types::DecoratedTypeInfo& typeInfo) const
    {
        if (IsNull())
        {
            if (Types::IsPointer(typeInfo))
                return Objects::Object(Services::ServiceProvider::Instance().GetService<TypeLibraries::ITypeLibrary>(), Variants::Variant(nullptr, typeInfo));
            else
                return ContentUtility::TryCreateContainer(Services::ServiceProvider::Instance().GetService<TypeLibraries::ITypeLibrary>(), typeInfo);
        }            
        return m_content->GetObjectConvertTo(typeInfo);
    }

    bool Content::HasStoredObject() const
    {
        if (IsNull())
            return false;
        return m_content->HasStoredObject();
    }

    std::string Content::GetDisplayText() const
    {
        if (IsNull())
            return "";
        return m_content->GetDisplayText();
    }

    std::string Content::GetScriptText() const
    {
        if (IsNull())
            return "";
        return m_content->GetScriptText();
    }

    bool Content::HasScriptText() const
    {
        if (IsNull())
            return false;
        else
            return m_content->HasScriptText();
    }

    bool Content::IsTextEqual(const Content& other) const
    {
        if (IsNull() != other.IsNull())
            return false;
        if (IsNull())
            return true;
        return m_content->IsTextEqual(*other.m_content);
    }

    bool Content::IsContentEqual(const Content& other) const
    {
        if (IsNull() != other.IsNull())
            return false;
        if (IsNull())
            return true;
        return m_content->IsContentEqual(*other.m_content);
    }

    bool Content::AreConstructorArgumentsEqual(const Content& other) const
    {
        if (IsNull() != other.IsNull())
            return false;
        if (IsNull())
            return true;
        Members::MemberPointer bestMember1 = m_content->GetStoredMember();
        Members::MemberPointer bestMember2 = other.m_content->GetStoredMember();
        if (!bestMember1 && !bestMember2)
            return false;
        Members::MemberPointer bestMember;
        if (!bestMember1)
            bestMember = bestMember2;
        else if (!bestMember2)
            bestMember = bestMember1;
        else if (bestMember1 != bestMember2)
            return false;
        size_t arity = bestMember->GetArity();
        if (bestMember->GetMinimumArgumentCount() == 0 && bestMember->GetMemberType() == Members::MemberType::TypePropertyGet)
            arity = 0;
        for (size_t i = 0; i < arity; ++i)
        {
            if (!TryGetArgument(bestMember, i).IsContentEqual(other.TryGetArgument(bestMember, i)))
                return false;
        }
        return true;
    }

    bool Content::IsValid() const
    {
        if (IsNull())
            return false;
        return m_content->IsValid();
    }

    Content Content::CopyChangeType(const Types::DecoratedTypeInfo& typeInfo, const Content& oldContent) const
    {
        if (IsNull())
            return *this;

        return m_content->TryChangeType(typeInfo, oldContent.GetImpl());
    }

    Content Content::CopyChangeMember(const Members::MemberPointer& member, bool allowRecursion, const Content& oldContent) const
    {
        if (IsNull())
            return *this;
        Content result = m_content->TryChangeMember(member, oldContent.GetImpl());
        if (result)
            return result;
        Content bestContent;
        if (!m_content->HasMetaData() && allowRecursion)
            bestContent = MemberContent::Parse(member, GetDisplayText(), oldContent.GetImpl());
        if (bestContent.IsValid())
            return bestContent;

        auto actualMember = TryGetBestMember();
        if (actualMember == member)
            return *this;
        //If member is plane3d::operator-() const, then the below construct will pass this as an argument to this member.
        if (actualMember && member->GetMinimumArgumentCount() == 1 && member->GetArgumentInfo(0)->GetTypeInfo().GetTypeInfo() == actualMember->GetReturnType().GetTypeInfo())
            return Content(member, { *this });
        if (!member || member->GetArity() == 0)
            return Content();
        std::vector<Content> arguments;        
        for (size_t i = 0; i < member->GetArity(); ++i)
        {
            arguments.push_back(m_content->TryGetArgument(member, i));
        }
        result = Content(member, arguments);
        if (!allowRecursion || result.IsValid())
            return result;
        if (actualMember)
        {
            //This handles the case where we have JoinedCurveOffsetPointer operator+(CurveOffsetPointer a, CuyrveOffsetPointer b);
            //If a is ConstantCurveOffset and b is AlignedCurveOffset, and the member you try to convert to is AlignedCurveOffset, it will select the second argument.
            for (size_t i = 0; i < actualMember->GetArity(); ++i)
            {
                auto arg = TryGetArgument(actualMember, i);
                Content resultFromArgument = arg.CopyChangeMember(member, false);
                if (resultFromArgument.IsValid())
                    return resultFromArgument;
            }
        }
        for(const Content& arg : arguments)
            if(!arg.IsNull())
                return result;
        for (size_t i = 0; i < member->GetArity(); ++i)
        {
            Content defaultMember(member);
            bool isConverted = false;
            for (size_t i = 0; i < member->GetArity(); ++i)
            {
                arguments[i] = defaultMember.TryGetArgument(member, i);
                if (!isConverted && IsConvertibleTo(arguments[i].GetTypeInfo()))
                {
                    arguments[i] = *this;
                    isConverted = true;
                }
            }
            if (isConverted)
                return Content(member, arguments);
            if (IsConvertibleTo(member->GetArgumentInfo(i)->GetTypeInfo()))
            {
                arguments[i] = *this;
                return Content(member, arguments);
            }
        }
        return bestContent;
    }

    bool Content::IsNull() const
    {
        return m_content == nullptr;
    }

    const std::shared_ptr<IContent>& Content::GetImpl() const
    {
        return m_content;
    }

    Content Content::TryInvoke(const Members::MemberPointer& member, const std::vector<Content>& arguments, bool trySimplify) const
    {
        if (!m_content)
            return Content();
        else
        {
            std::vector<std::shared_ptr<IContent>> args;
            args.reserve(arguments.size());
            for (const auto& arg : arguments)
                args.push_back(arg.GetImpl());
            return m_content->TryInvoke(member, args, trySimplify);
        }
    }

    bool Content::operator==(const Content& other) const
    {
        return IsContentEqual(other);
    }

    Content Content::TryGetArgument(const Members::MemberPointer& member, size_t index) const
    {
        if (IsNull())
            return Content();
        return m_content->TryGetArgument(member, index);
    }

    Members::MemberPointer Content::TryGetBestMember() const
    {
        if (IsNull())
            return nullptr;
        return m_content->TryGetBestMember();
    }

    std::vector<std::string> Content::DoErrorCheck() const
    {
        if (IsNull())
            return { "No input specified" };
        else
            return m_content->DoErrorCheck();
    }

    Content Content::InvokeProperty(const std::string& propertyName) const
    {
        if (IsNull())
            return Content();
        else
            return m_content->InvokeProperty(propertyName);
    }

    Content Content::InvokeProperty(const Members::MemberPointer& propertyGetter) const
    {
        if (IsNull())
            return Content();
        else
            return m_content->InvokeProperty(propertyGetter);
    }

}}}}

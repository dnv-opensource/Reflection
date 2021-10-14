//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ObjectContent.h"
#include "Reflection/Types/DynamicTypeTraits.h"
#include "Reflection/Attributes/UndefinedAttribute.h"
#include "IReflectionContentsService.h"
#include "Impl/ContentUtility.h"


namespace DNVS {namespace MoFa {namespace Reflection { namespace Contents { 

    ObjectContent::ObjectContent(const Objects::Object& object)
    {
        try {
            m_object = object.ToLValue();
        }
        catch (...)
        {
            m_object = object;
        }
    }

    bool ObjectContent::IsConvertibleTo(const Types::DecoratedTypeInfo& typeInfo) const
    {
        return ContentUtility::IsConvertibleTo(m_object, typeInfo);
    }

    Types::DecoratedTypeInfo ObjectContent::GetTypeInfo() const
    {
        if (!Reflection::Types::IsConst(m_object.GetDecoratedTypeInfo()) && Reflection::Types::IsReference(m_object.GetDecoratedTypeInfo()))
            return Reflection::Types::AddConst(m_object.GetDecoratedTypeInfo());
        return m_object.GetDecoratedTypeInfo();
    }

    Objects::Object ObjectContent::GetObject() const
    {
        return m_object;
    }

    std::string ObjectContent::GetDisplayText() const
    {
        try {
            if (auto service = Services::ServiceProvider::Instance().TryGetService<IReflectionContentsService>())
            {
                return Objects::ToString(m_object, service->GetDisplayFormatter());
            }            
        }
        catch (...) {}
        return "";
    }

    std::string ObjectContent::GetScriptText() const
    {
        try {
            if (auto service = Services::ServiceProvider::Instance().TryGetService<IReflectionContentsService>())
            {
                std::string result = Objects::ToString(m_object, service->GetScriptFormatter());
                if (m_object.GetDecoratedTypeInfo().GetTypeInfo() == typeid(std::string))
                    return "\"" + result + "\"";
                else
                    return result;
            }
        }
        catch(...){}
        return "";
    }

    bool ObjectContent::HasScriptText() const
    {
        return false;
    }

    bool ObjectContent::IsContentEqual(const IContent& other) const
    {
        if (!other.HasStoredObject())
            return other.IsContentEqual(*this);
        auto obj1 = GetObject();
        auto obj2 = other.GetObject();
        if (Attributes::IsUndefined(obj1) && Attributes::IsUndefined(obj2))
            return true;
        return obj1 == obj2;
    }

    bool ObjectContent::HasStoredObject() const
    {
        return true;
    }

    bool ObjectContent::IsValid() const
    {
        return m_object.IsValid();
    }

    std::vector<std::string> ObjectContent::DoErrorCheck() const
    {
        return {};
    }

    std::shared_ptr<IContent> ObjectContent::TryChangeType(const Types::DecoratedTypeInfo& typeInfo, const std::shared_ptr<IContent>& existingContent) const
    {
        if (m_object.IsConvertibleTo(typeInfo))
        {
            return std::make_shared<ObjectContent>(m_object.ConvertTo(typeInfo));
        }
        return std::make_shared<ObjectContent>(m_object);
    }

    std::shared_ptr<IContent> ObjectContent::TryInvoke(const Members::MemberPointer& member, const std::vector<std::shared_ptr<IContent>>& arguments, bool trySimplify) const
    {
        return ContentUtility::TryInvoke(this, member, arguments, trySimplify);
    }

    std::shared_ptr<IContent> ObjectContent::TryGetArgument(const Members::MemberPointer& member, size_t index) const
    {
        return ContentUtility::TryGetArgument(GetObject(), member, index);
    }

    std::shared_ptr<IContent> ObjectContent::InvokeProperty(const std::string& propertyName) const
    {
        return ContentUtility::InvokeProperty(*this, propertyName);
    }

    std::shared_ptr<IContent> ObjectContent::InvokeProperty(const Members::MemberPointer& propertyGetter) const
    {
        return ContentUtility::InvokeProperty(*this, propertyGetter);
    }

    Objects::Object ObjectContent::GetObjectConvertTo(const Types::DecoratedTypeInfo& typeInfo) const
    {
        return ContentUtility::ConvertTo(m_object, typeInfo);
    }

    std::vector<std::shared_ptr<IContent>> ObjectContent::Split() const
    {
        return { std::make_shared<ObjectContent>(*this) };
    }

    bool ObjectContent::IsCollection() const
    {
        return false;
    }

}}}}


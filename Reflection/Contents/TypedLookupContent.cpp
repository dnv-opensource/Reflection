//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypedLookupContent.h"
#include "Reflection/TypeLibraries/TypeLibraryNamedTypeFormatter.h"
#include "Services/ServiceProvider.h"
#include "Reflection/Types/DefaultTypeFormatter.h"
#include "Reflection/Attributes/ValidationAttribute.h"
#include "Reflection/Attributes/AttributeCollection.h"
#include "Reflection/Attributes/UndefinedAttribute.h"
#include "Reflection/Types/DynamicTypeTraits.h"
#include "IReflectionContentsService.h"
#include "Impl/ContentUtility.h"
#include "../Attributes/AttributeCollectionService.h"

namespace DNVS {namespace MoFa {namespace Reflection { namespace Contents { 

    TypedLookupContent::TypedLookupContent(const std::string& text, const Types::DecoratedTypeInfo& typeInfo)
        : m_lookupText(ContentUtility::ConditionallyAddQuotes(text,typeInfo))
        , m_typeInfo(typeInfo)
    {
    }

    bool TypedLookupContent::IsConvertibleTo(const Types::DecoratedTypeInfo& typeInfo) const
    {
        return ContentUtility::IsConvertibleTo(GetObject(), typeInfo);
    }

    Types::DecoratedTypeInfo TypedLookupContent::GetTypeInfo() const
    {
        return m_typeInfo;
    }

    Objects::Object TypedLookupContent::GetObject() const
    {
        try {
            Objects::Object object;
            if(auto service = Services::ServiceProvider::Instance().TryGetService<IReflectionContentsService>())
            { 
                object = service->Test(m_lookupText);
                if (object.IsConvertibleTo(m_typeInfo))
                    return object.ConvertTo(m_typeInfo);
            }
            return object;
        }
        catch (...)
        {
            return Objects::Object();
        }
    }

    std::string TypedLookupContent::GetDisplayText() const
    {
        return ContentUtility::StripQuotes(m_lookupText);
    }

    std::string TypedLookupContent::GetScriptText() const
    {
        return m_lookupText;
    }

    bool TypedLookupContent::IsContentEqual(const IContent& other) const
    {
        auto service = Services::ServiceProvider::Instance().TryGetService<IReflectionContentsService>();

        if (other.HasStoredObject() || (service && service->IsLookupExpression(other.GetScriptText()+";")))
        {
            auto object1 = GetObject();
            auto object2 = other.GetObject();
            if (Attributes::IsUndefined(object1) && Attributes::IsUndefined(object2))
                return true;
            if(object1 == object2)
            {
                if(Types::IsPointer(object1.GetVariant().GetDecoratedTypeInfo()))
                {
                    if (object1.GetVariant().GetData() == nullptr)
                        return IsTextEqual(other);
                }
                return true;
            }
            else
                return false;
        }
        else
            return false;
    }

    bool TypedLookupContent::HasStoredObject() const
    {
        return false;
    }

    bool TypedLookupContent::IsValid() const
    {
        return GetObject().IsConvertibleTo(m_typeInfo);
    }

    std::vector<std::string> TypedLookupContent::DoErrorCheck() const
    {
        auto typeLibrary = Services::ServiceProvider::Instance().GetService<TypeLibraries::ITypeLibrary>();
        auto typeFormatter = std::make_shared<TypeLibraries::TypeLibraryNamedTypeFormatter>(typeLibrary.get(), std::make_shared<Types::DefaultTypeFormatter>());

        if (m_lookupText.empty())
            return { "The input is empty" };
        try {
            Objects::Object object = GetObject();
            if (!object.IsConvertibleTo(m_typeInfo))
            {
                return {
                    "Input does not represent a " + typeFormatter->FormatType(m_typeInfo),
                    "'" + m_lookupText + "' represents a " + typeFormatter->FormatType(object.ConvertToDynamicType().GetDecoratedTypeInfo())
                };
            }
            auto type = typeLibrary->LookupType(m_typeInfo.GetTypeInfo());
            if(auto attribute = Attributes::GetPointerToAttributeOrNull<Attributes::ValidationAttribute>(type))
            {
                if (attribute->GetStringArguments().size() == 1 && attribute->GetStringArguments().at(0) == "")
                {
                    try {
                        attribute->InvokeFunction({ object });
                    }
                    catch (std::exception& e)
                    {
                        return {
                            type->GetName() + " is invalid.",
                            e.what()
                        };
                    }
                }
            }
        }
        catch (...)
        {
            return {
                "Input does not represent a " + typeFormatter->FormatType(m_typeInfo),
                "Invalid script expression: '" + m_lookupText + "'"
            };
        }
        return {};
    }

    bool TypedLookupContent::HasScriptText() const
    {
        return true;
    }

    std::shared_ptr<IContent> TypedLookupContent::TryInvoke(const Members::MemberPointer& member, const std::vector<std::shared_ptr<IContent>>& arguments, bool trySimplify) const
    {
        return ContentUtility::TryInvoke(this, member, arguments, trySimplify);
    }

    std::shared_ptr<IContent> TypedLookupContent::TryGetArgument(const Members::MemberPointer& member, size_t index) const
    {
        return ContentUtility::TryGetArgument(GetObject(), member, index);
    }

    std::shared_ptr<IContent> TypedLookupContent::InvokeProperty(const std::string& propertyName) const
    {
        return ContentUtility::InvokeProperty(*this, propertyName);
    }

    std::shared_ptr<IContent> TypedLookupContent::InvokeProperty(const Members::MemberPointer& propertyGetter) const
    {
        return ContentUtility::InvokeProperty(*this, propertyGetter);
    }

    Objects::Object TypedLookupContent::GetObjectConvertTo(const Types::DecoratedTypeInfo& typeInfo) const
    {
        return ContentUtility::ConvertTo(GetObject(), typeInfo);
    }

    std::vector<std::shared_ptr<IContent>> TypedLookupContent::Split() const
    {
        return { std::make_shared<TypedLookupContent>(*this) };
    }

    bool TypedLookupContent::IsCollection() const
    {
        return false;
    }

}}}}


//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ContentUtility.h"
#include "Reflection/Utilities/ArgumentFromObjectExtracter.h"
#include "Reflection/Members/IMember.h"
#include "Reflection/Contents/ObjectContent.h"
#include "Reflection/Contents//MemberContent.h"
#include "Reflection/Utilities/ConstructorSelector.h"
#include "Reflection/Utilities/CallableConstructor.h"
#include "Reflection/Attributes/ContainerAttribute.h"
#include "Reflection/Attributes/AttributeCollectionService.h"
#include "Reflection/Members/MemberWithArguments.h"
#include <iomanip>
#include "../../Types/DynamicTypeTraits.h"
#include "Services/Allocators/PersistenceControl.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Contents {

    std::shared_ptr<IContent> ContentUtility::TryGetArgument(const Objects::Object& object, const Members::MemberPointer& member, size_t index)
    {
        if (!member)
            return nullptr;
        try {            
            if (!object.IsConvertibleTo(member->GetReturnType()))
                return nullptr;
            Objects::Object objectOfRightType = object.ConvertTo(member->GetReturnType());
            if (index > member->GetMaximumArgumentCount())
                return nullptr;
            Utilities::ArgumentFromObjectExtracter extracter(object.GetTypeLibrary(), member, index);
            Objects::Object result(object.GetTypeLibrary(), extracter(objectOfRightType.GetVariant()));
            if (!result.IsConvertibleTo(member->GetArgumentInfo(index)->GetTypeInfo()))
                return nullptr;
            return std::make_shared<ObjectContent>(result);
        }
        catch (...)
        {
            return nullptr;
        }
    }

    Members::MemberPointer ContentUtility::TryGetBestMember(const Objects::Object& object)
    {
        return Utilities::ConstructorSelector(object.ConvertToDynamicType()).GetBestConstructor().GetMember();
    }

    std::shared_ptr<IContent> ContentUtility::TryInvoke(const std::shared_ptr<IContent>& self, const Members::MemberPointer& member, const std::vector<std::shared_ptr<IContent>>& arguments, bool trySimplify)
    {
        if (!self || !member)
            return nullptr;
        std::vector<std::shared_ptr<IContent>> localArgs;
        localArgs.reserve(arguments.size() + 1);
        localArgs.push_back(self);
        localArgs.insert(localArgs.end(), arguments.begin(), arguments.end());
        auto result = std::make_shared<MemberContent>(member, localArgs);
        if (member->IsConst() && trySimplify)
            return std::make_shared<ObjectContent>(result->GetObject());
        else
            return result;
    }

    std::shared_ptr<IContent> ContentUtility::InvokeProperty(const IContent& content, const std::string& propertyName)
    {
        Objects::Object object = content.GetObject();
        try {
            auto constructor = content.TryGetBestMember();
            if (constructor)
            {
                auto variant = Utilities::ArgumentFromObjectExtracter(object.GetTypeLibrary(), content.GetTypeInfo().GetTypeInfo(), propertyName, constructor)(object.GetVariant());
                Objects::Object result(object.GetTypeLibrary(), variant);
                if (result.IsValid())
                    return std::make_shared<ObjectContent>(result);
            }
        }
        catch (...)
        {
        }
        return nullptr;
    }

    std::shared_ptr<IContent> ContentUtility::InvokeProperty(const IContent& content, const Members::MemberPointer& propertyGetter)
    {
        try {
            Services::Allocators::PersistenceControl control(false);
            Objects::Object object = content.GetObject();
            Objects::Object result(object.GetTypeLibrary(), propertyGetter->Invoke({ object.GetVariant() }));
            return std::make_shared<ObjectContent>(result);
        }
        catch (...)
        {
            return nullptr;
        }
    }

    Objects::Object ContentUtility::ConvertTo(const Objects::Object& object, const Types::DecoratedTypeInfo& to)
    {
		if (Types::IsPointer(to) && Types::IsPointer(object.GetDecoratedTypeInfo()) && object.GetVariant().GetData() == nullptr && object.IsConvertibleTo(to))
			return Objects::Object(object.GetTypeLibrary(), Variants::Variant(nullptr, to));
        Objects::Object result = object.ConvertTo(to);
        if (!result.IsValid())
        {
            Objects::Object container = TryCreateContainer(object.GetTypeLibrary(), to);
            if (TryAddItemToContainer(container, object))
                result = container;
        }
        return result;
    }

    bool ContentUtility::IsConvertibleTo(const Objects::Object& object, const Types::DecoratedTypeInfo& to)
    {
        if (object.IsConvertibleTo(to))
            return true;

        return IsConvertibleToContainer(object, to);
    }

    bool ContentUtility::IsConvertibleTo(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to)
    {
        auto sequence = typeLibrary->GetConversionGraph()->GetConversionSequence(from, to);
        if (sequence && sequence->IsValid())
            return true;
        return IsConvertibleToContainer(typeLibrary, from, to);
    }

    bool ContentUtility::IsContainerType(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Types::DecoratedTypeInfo& typeInfo)
    {
        return Attributes::GetPointerToAttributeOrNull<Attributes::ContainerAttribute>(typeLibrary, typeInfo) != nullptr;
    }

    bool ContentUtility::IsConvertibleToContainer(const Objects::Object& object, const Types::DecoratedTypeInfo& typeInfo)
    {
        const Attributes::ContainerAttribute* attribute = Attributes::GetPointerToAttributeOrNull<Attributes::ContainerAttribute>(object.GetTypeLibrary(), typeInfo);
        if (!attribute)
            return false;
        return object.IsConvertibleTo(attribute->GetValueType());
    }

    bool ContentUtility::IsConvertibleToContainer(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& containerType)
    {
        const Attributes::ContainerAttribute* attribute = Attributes::GetPointerToAttributeOrNull<Attributes::ContainerAttribute>(typeLibrary, containerType);
        if (!attribute)
            return false;
        auto conversion = typeLibrary->GetConversionGraph()->GetConversionSequence(from, attribute->GetValueType());
        return conversion && conversion->IsValid();            
    }

    Objects::Object ContentUtility::TryCreateContainer(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Types::DecoratedTypeInfo& typeInfo)
    {
        if(!typeLibrary)
            return Objects::Object();
        auto type = typeLibrary->LookupType(typeInfo.GetTypeInfo());
        if (!type)
            return Objects::Object();
        const Attributes::ContainerAttribute* attribute = Attributes::GetPointerToAttributeOrNull<Attributes::ContainerAttribute>(type);
        if (!attribute)
            return Objects::Object();
        //First, we need to construct a container of the correct type.
        auto constructor = type->Lookup(Reflection::Members::ConstructorString);
        if (!constructor)
            return Objects::Object();
        auto invokableConstructor = constructor->PrepareInvoke({});
        if (!invokableConstructor)
            return Objects::Object();
        Services::Allocators::PersistenceControl control(false);
        return Objects::Object(typeLibrary, invokableConstructor->Invoke());
    }

    bool ContentUtility::TryAddItemToContainer(const Objects::Object& container, const Objects::Object& object)
    {
        if (!container.IsValid())
            return false;
        try {
            Services::Allocators::PersistenceControl control(false);
            container.Invoke("Add", { object });
            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    std::string ContentUtility::StripQuotes(std::string text)
    {
        if (!IsQuoted(text))
            return text;

        std::stringstream ss;
        ss << text;
        ss >> std::quoted(text);
        return text;
    }

    bool ContentUtility::IsQuoted(const std::string& text)
    {
        if (text.size() >= 2)
        {
            if (text.front() == '"' && text.back() == '"')
                return true;
            if (text.front() == '\'' && text.back() == '\'')
                return true;
        }
        return false;
    }

    std::string ContentUtility::ConditionallyAddQuotes(std::string text, const Types::DecoratedTypeInfo& typeInfo)
    {
        if (typeInfo.GetTypeInfo() != typeid(std::string))
            return text;
        if (IsQuoted(text))
            return text;
        std::stringstream ss;
        ss << std::quoted(text);
        return ss.str();
    }

}}}}


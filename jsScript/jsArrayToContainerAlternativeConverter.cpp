//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "jsArrayToContainerAlternativeConverter.h"
#include "Reflection\Members\IMember.h"
#include "Reflection\Objects\Object.h"
#include "Reflection\Objects\Delegate.h"
#include "jsArray.h"
#include "Reflection\Attributes\ContainerAttribute.h"
#include "Reflection\Attributes\AttributeCollection.h"

using namespace DNVS::MoFa::Reflection;
jsArrayToContainerConverter::jsArrayToContainerConverter(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Types::DecoratedTypeInfo& returnType) 
    : m_typeLibrary(typeLibrary)
    , m_returnType(returnType)
{

}

Variants::Variant jsArrayToContainerConverter::Convert(const Variants::Variant& variant)
{
    auto containerType = m_typeLibrary->LookupType(m_returnType.GetTypeInfo());
    auto constructor = containerType->Lookup(Members::ConstructorString);
    Objects::Object container(m_typeLibrary, constructor->Invoke({}));

    jsArray* myArray = Variants::InternalVariantService::UnreflectUnchecked<jsArray*>(variant);
    for (size_t i = 0; i < myArray->child_size(); ++i)
    {
        container.Lookup("Add")((*myArray)[(unsigned int)i]);
    }
    return container.GetVariant();
}

void jsArrayToContainerConverter::IntrusiveConvert(Variants::Variant& variant)
{
    variant = Convert(variant);
}

jsArrayToContainerAlternativeConverter::jsArrayToContainerAlternativeConverter(const TypeLibraries::TypeLibraryPointer& typeLibrary) 
    : m_typeLibrary(typeLibrary)
{

}

bool jsArrayToContainerAlternativeConverter::CanConvert(const Variants::Variant& variant, const Types::DecoratedTypeInfo& returnType) const
{
    if (variant.GetDecoratedTypeInfo().GetTypeInfo() != typeid(jsArray))
        return false;
    auto typeLibrary = m_typeLibrary.lock();
    if (!typeLibrary)
        return false;
    auto containerType = typeLibrary->LookupType(returnType.GetTypeInfo());
    if (!containerType)
        return false;
    if (!containerType->GetAttributeCollection().HasAttribute<Attributes::ContainerAttribute>())
        return false;

    return AreAllItemsConvertibleTo(variant, containerType->GetAttributeCollection().GetAttribute<Attributes::ContainerAttribute>().GetValueType());
}

TypeConversions::ConversionPointer jsArrayToContainerAlternativeConverter::CreateConverter(const Types::DecoratedTypeInfo& returnType) const
{
    return std::make_shared<jsArrayToContainerConverter>(m_typeLibrary.lock(), returnType);
}

TypeConversions::ConversionType::Type jsArrayToContainerAlternativeConverter::GetConversionType() const
{
    return TypeConversions::ConversionType::UserConversion;
}

bool jsArrayToContainerAlternativeConverter::AreAllItemsConvertibleTo(const Variants::Variant& variant, const Types::DecoratedTypeInfo& valueType) const
{
    auto typeLibrary = m_typeLibrary.lock();
    jsArray* myArray = Variants::InternalVariantService::UnreflectUnchecked<jsArray*>(variant);
    for (size_t i = 0; i < myArray->child_size(); ++i)
    {
        jsValue* value = (*myArray)[(unsigned int)i];
        if (!Objects::Object(typeLibrary, value).IsConvertibleTo(valueType))
            return false;
    }
    return true;
}

AlternativeContainerTojsArrayConverter::AlternativeContainerTojsArrayConverter(const TypeLibraries::TypeLibraryPointer& typeLibrary) : m_typeLibrary(typeLibrary)
{

}

bool AlternativeContainerTojsArrayConverter::CanConvert(const Variants::Variant& variant, const Types::DecoratedTypeInfo& returnType) const
{
    if (returnType != Types::TypeId<mofa::ref<jsArray>>() && returnType != Types::TypeId<const jsArray&>())
        return false;
    auto typeLibrary = m_typeLibrary.lock();
    if (!typeLibrary)
        return false;
    Objects::Object object(typeLibrary, variant);
    auto containerType = object.GetType();
    if (!containerType)
        return false;
    if (!containerType->GetAttributeCollection().HasAttribute<Attributes::ContainerAttribute>())
        return false;

    return true;
}

TypeConversions::ConversionPointer AlternativeContainerTojsArrayConverter::CreateConverter(const Types::DecoratedTypeInfo& returnType) const
{
    return std::make_shared<ContainerTojsArrayConverter>(m_typeLibrary.lock(), returnType);
}

TypeConversions::ConversionType::Type AlternativeContainerTojsArrayConverter::GetConversionType() const
{
    return TypeConversions::ConversionType::UserConversion;
}

ContainerTojsArrayConverter::ContainerTojsArrayConverter(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Types::DecoratedTypeInfo& returnType) : m_typeLibrary(typeLibrary)
, m_returnType(returnType)
{

}

Variants::Variant ContainerTojsArrayConverter::Convert(const Variants::Variant& variant)
{
    Objects::Object container(m_typeLibrary, variant);
    jsValue::Params params;
    std::vector<mofa::ref<jsValue>> storedArgs;
    params.reserve(container.Lookup("size")().As<size_t>());
    storedArgs.reserve(params.capacity());
    auto end = container.Lookup("end")();
    auto it = container.Lookup("begin")().ToLValue();
    for (; it != end; ++it)
    {
        auto object = (*it).ToLValue();
        mofa::ref<jsValue> val = object.As<mofa::ref<jsValue>>();
        params.push_back(val);
        storedArgs.push_back(val);
    }
    mofa::ref<jsArray> array(new jsArray(params));
    return Variants::VariantService::ReflectType<mofa::ref<jsArray>>(array);
}

void ContainerTojsArrayConverter::IntrusiveConvert(Variants::Variant& variant)
{
    variant = Convert(variant);
}

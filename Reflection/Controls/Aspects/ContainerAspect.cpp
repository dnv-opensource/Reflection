//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ContainerAspect.h"
#include "Reflection/Types/TypeFormatter.h"
#include "Reflection/Types/DynamicTypeTraits.h"
#include "Reflection/Objects/Delegate.h"
#include "Reflection/Members/IMember.h"
#include "Reflection/Members/MemberWithArguments.h"
#include "Reflection/Members/GlobalType.h"
#include "Services/Allocators/PersistenceControl.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    ContainerAspect::ContainerAspect(const Types::DecoratedTypeInfo& containerType, const Attributes::ContainerAttribute& containerAttribute)
        : m_containerType(containerType)
        , m_containerAttribute(containerAttribute)
    {
    }

    ContainerAspect::~ContainerAspect()
    {

    }

    const Attributes::AttributeCollection& ContainerAspect::GetAttributeCollection() const
    {
        return Attributes::AttributeCollection::GetEmptyCollection();
    }

    std::string ContainerAspect::ToString() const
    {
        return "ContainerAspect(" + Types::TypeFormatter().FormatType(m_containerType) + ", ContainerAttribute(" + Types::TypeFormatter().FormatType(m_containerAttribute.GetValueType()) + ", " + std::to_string(m_containerAttribute.HasSorting()) + "))";
    }

    const Types::DecoratedTypeInfo& ContainerAspect::GetContainerType() const
    {
        return m_containerType;
    }

    const Attributes::ContainerAttribute& ContainerAspect::GetContainerAttribute() const
    {
        return m_containerAttribute;
    }

    std::vector<Contents::Content> ContainerAspect::Split(const Contents::Content& content) const
    {
        std::vector<Contents::Content> contents;
        for (const Contents::Content& item : content.Split())
        {
            if (item.IsConvertibleTo(m_containerAttribute.GetValueType()))
            {
                contents.push_back(item);
                continue;
            }
            auto member = content.TryGetBestMember();
            if (member && member->GetActualName() == "Array" && member->IsVarargMember())
            {
                for (size_t i = 0; i < member->GetMaximumArgumentCount(); ++i)
                {
                    auto arg = content.TryGetArgument(member, i);
                    if (arg)
                        contents.push_back(arg);
                    else
                        break;
                }
                continue;
            }
            Objects::Object object = item.GetObject();
            if(!object.IsValid())
                continue;
            auto type = object.GetType();
            if(!type || !type->GetAttributeCollection().HasAttribute<Attributes::ContainerAttribute>())
                continue;
            try {
                //object is for example a std::vector<MyType>.
                //Extract the begin iterator. Make a copy of it (That's why we call ToLValue())
                Reflection::Objects::Object it = object.Lookup("begin")().ToLValue();
                //Extract the end iterator.
                Reflection::Objects::Object iend = object.Lookup("end")();
                //Iterate over the container stored in object.
                for (; it != iend; ++it) //<- ++it will call the iterator ++ operator using reflection.
                {
                    //Get a reference to the value stored in it.
                    Reflection::Objects::Object value = *it;
                    //We want to get a copy of this value. However, ToLValue only works for const T&.
                    //value has the type T&. For ToLValue to work, we need to force value to be of type const T&.
                    Reflection::Variants::Variant var = value.GetVariant();
                    //Check if type is T& or const T&.
                    if (Reflection::Types::IsReference(var.GetDecoratedTypeInfo()))
                    {
                        //Add const T& in case type was T&.
                        var.SetDecoratedTypeInfo(Reflection::Types::AddConst(var.GetDecoratedTypeInfo()));
                        value.Reset(var);
                    }
                    //Call ToLValue to make a copy of value.
                    if(value.IsConvertibleTo(m_containerAttribute.GetValueType()))
                        contents.push_back(value);
                }
            }
            catch (...) {}
        }
        return contents;
    }

    Contents::Content ContainerAspect::JoinNoSimplify(const TypeLibraries::TypeLibraryPointer& typeLibrary, const std::vector<Contents::Content>& contents) const
    {
        auto globalType = typeLibrary->LookupType(typeid(Members::GlobalType));
        if (!globalType)
            return Contents::Content();
        auto arrayConstructor = globalType->Lookup("Array", Variants::Variant());
        if (!arrayConstructor)
            return Contents::Content();
        for (const Members::MemberPointer& constructor : arrayConstructor->GetOverloads())
        {
            if (constructor->IsVarargMember() && constructor->GetMinimumArgumentCount() == 0)
                return Contents::Content(constructor, contents);
        }
        return Contents::Content();
    }

    Contents::Content ContainerAspect::Join(const TypeLibraries::TypeLibraryPointer& typeLibrary, const std::vector<Contents::Content>& contents, bool simplify) const
    {
        if (!simplify)
        {
            if (Contents::Content content = JoinNoSimplify(typeLibrary, contents))
                return content;
        }
        Objects::Object container(typeLibrary);
        auto containerType = typeLibrary->LookupType(m_containerType.GetTypeInfo());
        if (!containerType)
            throw std::runtime_error("Container is not registered in the type library");
        auto constructor = containerType->Lookup(Members::ConstructorString);
        if (!constructor)
            throw std::runtime_error("Unable to construct new container");
        auto preparedConstructor = constructor->PrepareInvoke({});
        if (!preparedConstructor)
            throw std::runtime_error("Unable to construct new container");
        Services::Allocators::PersistenceControl control(false);
        container.Reset(preparedConstructor->Invoke());
        for (const Contents::Content& item : contents)
            container.Lookup("Add")(item.GetObject());
        return container;
    }

    bool ContainerAspect::operator==(const ContainerAspect& other) const
    {
        return m_containerType == other.m_containerType &&
            m_containerAttribute.GetValueType() == other.m_containerAttribute.GetValueType() &&
            m_containerAttribute.HasSorting() == other.m_containerAttribute.HasSorting();
    }

}}}}


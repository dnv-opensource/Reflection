//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeLibraries/TypeLibrary.h"
#include "Reflection/TypeLibraries/Type.h"
#include "Reflection/Types/DefaultTypeFormatter.h"
#include "Reflection/Types/DecoratedTypeFormatter.h"
#include "Reflection/TypeLibraries/TypeLibraryNamedTypeFormatter.h"
#include "Reflection/Members/GlobalType.h"
#include "TypeNotCaseSensitive.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeLibraries {

    TypeLibrary::TypeLibrary( TypeConversions::ConversionGraphPointer conversionGraph, bool caseSensitive)
        :   m_conversionGraph(conversionGraph)
        ,   m_caseSensitive(caseSensitive)
        ,   m_serviceProvicer(Services::IServiceContainer::Session)
    {
        m_typeFormatter.reset(new Types::DefaultTypeFormatter);
        m_typeFormatter.reset(new TypeLibraryNamedTypeFormatter(this, m_typeFormatter));
        m_typeFormatter.reset(new Types::DecoratedTypeFormatter(m_typeFormatter));
    }

    TypePointer TypeLibrary::CreateType( const std::type_info& typeInfo, const std::string& name )
    {
        TypePointer& type = m_types[typeInfo];
        if (!type)
        {
            type.reset(new Type(shared_from_this(), typeInfo, name));
            if (!m_caseSensitive)
            {
                type.reset(new TypeNotCaseSensitive(type));
            }
        }
        if(type->GetName().empty()) 
            type->SetName(name);
        return type;
    }

    TypePointer TypeLibrary::LookupType( const std::type_index& typeInfo) const
    {
        auto it = m_types.find(typeInfo);
        if(it != m_types.end()) 
            return it->second;
        else 
            return TypePointer();
    }

    std::map<std::type_index, TypePointer> TypeLibrary::GetAllTypes() const
    {
        return m_types;
    }

    TypeConversions::ConversionGraphPointer TypeLibrary::GetConversionGraph() const
    {
        return m_conversionGraph;   
    }

    std::shared_ptr<Types::ITypeFormatter> TypeLibrary::GetTypeFormatter() const
    {
        return m_typeFormatter;
    }

    void TypeLibrary::SetTypeFormatter(const std::shared_ptr<Types::ITypeFormatter>& typeFormatter)
    {
        m_typeFormatter = typeFormatter;
    }

    std::shared_ptr<Variants::IVariantFormatter> TypeLibrary::GetVariantFormatter() const
    {
        return m_variableFormatter;
    }

    void TypeLibrary::SetVariantFormatter( const std::shared_ptr<Variants::IVariantFormatter>& formatter )
    {
        m_variableFormatter = formatter;
    }

    void TypeLibrary::Initialize()
    {
        m_conversionGraph->AddConversion(Types::TypeId<void>(), Types::TypeId<Variants::Variant>(), TypeConversions::ConversionType::ReflectionConversion, nullptr);
        CreateType(typeid(Members::GlobalType), "");
    }

    void TypeLibrary::SetTypeReflected(const std::type_info& typeInfo)
    {
        m_reflectedTypes.insert(typeInfo);
    }

    bool TypeLibrary::IsTypeReflected(const std::type_info& typeInfo) const
    {
        return m_reflectedTypes.count(typeInfo) > 0;
    }

    DNVS::MoFa::Services::ServiceProvider& TypeLibrary::GetServiceProvider()
    {
        return m_serviceProvicer;
    }

}}}}
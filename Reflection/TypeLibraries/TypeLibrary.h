#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "ITypeLibrary.h"
#include "Reflection/TypeConversions/IConversionGraph.h"
#include "Reflection/TypeLibraries/TypeLibrary.h"
#include <map>
#include <memory>

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeLibraries {
    class REFLECTION_IMPORT_EXPORT TypeLibrary 
        : public ITypeLibrary 
        , public std::enable_shared_from_this<TypeLibrary>
    {
    public:
        TypeLibrary(TypeConversions::ConversionGraphPointer conversionGraph, bool caseSensitive = true);
        virtual TypePointer CreateType(const std::type_info& typeInfo,const std::string& name) override;
        virtual TypePointer LookupType(const std::type_index& typeInfo) const override;
        virtual std::map<std::type_index,TypePointer> GetAllTypes() const override;
        virtual TypeConversions::ConversionGraphPointer GetConversionGraph() const override;
        virtual std::shared_ptr<Types::ITypeFormatter> GetTypeFormatter() const override;
        virtual std::shared_ptr<Variants::IVariantFormatter> GetVariantFormatter() const override;
        virtual void SetVariantFormatter(const std::shared_ptr<Variants::IVariantFormatter>& formatter) override;
        virtual void SetTypeFormatter(const std::shared_ptr<Types::ITypeFormatter>& typeFormatter) override;
        void Initialize();
        virtual void SetTypeReflected(const std::type_info& typeInfo);
        virtual bool IsTypeReflected(const std::type_info& typeInfo) const;
        virtual Services::ServiceProvider& GetServiceProvider();
    private:
        std::map<std::type_index,TypePointer> m_types;
        TypeConversions::ConversionGraphPointer m_conversionGraph;
        std::shared_ptr<Types::ITypeFormatter> m_typeFormatter;
        std::shared_ptr<Variants::IVariantFormatter> m_variableFormatter;
        bool m_caseSensitive;
        std::set<std::type_index> m_reflectedTypes;
        Services::ServiceProvider m_serviceProvicer;
    };
}}}}
#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeLibraries/ITypeLibrary.h"
#include "jsScript_config.h"

namespace DNVS { namespace MoFa { namespace Scripting {
    class JSSCRIPT_IMPORT_EXPORT jsDummyTypeLibrary : public Reflection::TypeLibraries::ITypeLibrary
    {
    public:
        jsDummyTypeLibrary(const Reflection::TypeLibraries::TypeLibraryPointer& typeLibrary);
        virtual Reflection::TypeLibraries::TypePointer CreateType(const std::type_info& typeInfo, const std::string& name) override;
        virtual Reflection::TypeLibraries::TypePointer LookupType(const std::type_index& typeInfo) const override;
        virtual std::map<std::type_index, Reflection::TypeLibraries::TypePointer> GetAllTypes() const override;
        virtual Reflection::TypeConversions::ConversionGraphPointer GetConversionGraph() const override;
        virtual std::shared_ptr<Reflection::Types::ITypeFormatter> GetTypeFormatter() const override;
        virtual std::shared_ptr<Reflection::Variants::IVariantFormatter> GetVariantFormatter() const override;
        virtual void SetVariantFormatter(const std::shared_ptr<Reflection::Variants::IVariantFormatter>& formatter) override;
        virtual void SetTypeReflected(const std::type_info& typeInfo) override;
        virtual bool IsTypeReflected(const std::type_info& typeInfo) const override;
        virtual Services::ServiceProvider& GetServiceProvider() override;
        void SetTypeFormatter(const std::shared_ptr<Reflection::Types::ITypeFormatter>& typeFormatter) override;

    private:
        Reflection::TypeLibraries::TypeLibraryPointer m_typeLibrary;
    };
}}}
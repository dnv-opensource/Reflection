#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/Config.h"

#include "IType.h"
#include "TypeLibraryPointer.h"

#include "Reflection/TypeConversions/IConversionGraph.h"

#include "Reflection/Types/ITypeFormatter.h"

#include "Reflection/Variants/IVariantFormatter.h"

#include <string>
#include <map>
#include "Services/ServiceProvider.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeLibraries {

    class REFLECTION_IMPORT_EXPORT ITypeLibrary {
    public:
        virtual ~ITypeLibrary() {}
        virtual TypePointer CreateType(const std::type_info& typeInfo,const std::string& name) = 0;
        virtual TypePointer LookupType(const std::type_index& typeInfo) const = 0;
        virtual std::map<std::type_index,TypePointer> GetAllTypes() const = 0;
        virtual TypeConversions::ConversionGraphPointer GetConversionGraph() const = 0;
        virtual std::shared_ptr<Types::ITypeFormatter> GetTypeFormatter() const = 0;
        virtual std::shared_ptr<Variants::IVariantFormatter> GetVariantFormatter() const = 0;
        virtual void SetVariantFormatter(const std::shared_ptr<Variants::IVariantFormatter>& formatter) = 0;
        virtual void SetTypeReflected(const std::type_info& typeInfo) = 0;
        virtual void SetTypeFormatter(const std::shared_ptr<Types::ITypeFormatter>& typeFormatter) = 0;
        virtual bool IsTypeReflected(const std::type_info& typeInfo) const = 0;
        virtual Services::ServiceProvider& GetServiceProvider() = 0;
    };

    inline std::string GetTypeName(TypeLibraryPointer typeLibrary,const Variants::Variant& var) {
        return typeLibrary->GetTypeFormatter()->FormatType(typeLibrary->GetConversionGraph()->GetDynamicType(var).GetDecoratedTypeInfo());
    }

}}}}

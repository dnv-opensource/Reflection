#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeConversions/IAlternativeConverter.h"
#include "Reflection/TypeLibraries/ITypeLibrary.h"
#include "jsScript_config.h"

class jsArrayToContainerConverter : public DNVS::MoFa::Reflection::TypeConversions::IConversion
{
public:
    jsArrayToContainerConverter(const DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer& typeLibrary, const DNVS::MoFa::Reflection::Types::DecoratedTypeInfo& returnType);
    virtual DNVS::MoFa::Reflection::Variants::Variant Convert(const DNVS::MoFa::Reflection::Variants::Variant& variant) override;
    virtual void IntrusiveConvert(DNVS::MoFa::Reflection::Variants::Variant& variant) override;
private:
    DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer m_typeLibrary;
    DNVS::MoFa::Reflection::Types::DecoratedTypeInfo m_returnType;
};

class JSSCRIPT_IMPORT_EXPORT jsArrayToContainerAlternativeConverter : public DNVS::MoFa::Reflection::TypeConversions::IAlternativeConverter
{
public:
    jsArrayToContainerAlternativeConverter(const DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer& typeLibrary);
    virtual bool CanConvert(const DNVS::MoFa::Reflection::Variants::Variant& variant, const DNVS::MoFa::Reflection::Types::DecoratedTypeInfo& returnType) const;
    DNVS::MoFa::Reflection::TypeConversions::ConversionPointer CreateConverter(const DNVS::MoFa::Reflection::Types::DecoratedTypeInfo& returnType) const;
    virtual DNVS::MoFa::Reflection::TypeConversions::ConversionType::Type GetConversionType() const override;
private:
    virtual bool AreAllItemsConvertibleTo(const DNVS::MoFa::Reflection::Variants::Variant& variant, const DNVS::MoFa::Reflection::Types::DecoratedTypeInfo& valueType) const;
    std::weak_ptr<DNVS::MoFa::Reflection::TypeLibraries::ITypeLibrary> m_typeLibrary;
};

class ContainerTojsArrayConverter : public DNVS::MoFa::Reflection::TypeConversions::IConversion
{
public:
    ContainerTojsArrayConverter(const DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer& typeLibrary, const DNVS::MoFa::Reflection::Types::DecoratedTypeInfo& returnType);
    virtual DNVS::MoFa::Reflection::Variants::Variant Convert(const DNVS::MoFa::Reflection::Variants::Variant& variant) override;

    virtual void IntrusiveConvert(DNVS::MoFa::Reflection::Variants::Variant& variant) override;

private:
    DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer m_typeLibrary;
    DNVS::MoFa::Reflection::Types::DecoratedTypeInfo m_returnType;
};

class JSSCRIPT_IMPORT_EXPORT AlternativeContainerTojsArrayConverter : public DNVS::MoFa::Reflection::TypeConversions::IAlternativeConverter
{
public:
    AlternativeContainerTojsArrayConverter(const DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer& typeLibrary);
    virtual bool CanConvert(const DNVS::MoFa::Reflection::Variants::Variant& variant, const DNVS::MoFa::Reflection::Types::DecoratedTypeInfo& returnType) const;
    DNVS::MoFa::Reflection::TypeConversions::ConversionPointer CreateConverter(const DNVS::MoFa::Reflection::Types::DecoratedTypeInfo& returnType) const;
    virtual DNVS::MoFa::Reflection::TypeConversions::ConversionType::Type GetConversionType() const override;
private:
    std::weak_ptr<DNVS::MoFa::Reflection::TypeLibraries::ITypeLibrary> m_typeLibrary;
};

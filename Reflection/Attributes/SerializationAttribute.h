#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include "TypeUtilities/BitFlagEnum.h"
#include "Reflection/Config.h"
#include <list>
#include <functional>
#include "Reflection/Variants/Variant.h"
#include <vector>
#include "FunctionCallbackAttribute.h"
#include "IAttribute.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    class IImportExportOverride {};
    class REFLECTION_IMPORT_EXPORT SerializationOptions : public TypeUtilities::BitFlagEnum<SerializationOptions>
    {
        friend struct Base;
        SerializationOptions(int v) : Base(v) {}
    public:
        //Postpones import until later, probably because some attribute is not imported yet.
        static const SerializationOptions None;
        static const SerializationOptions Postpone;
        static const SerializationOptions Optional;
        //Enforce a property to be serialized as an attribute.
        static const SerializationOptions Attribute;
        struct REFLECTION_IMPORT_EXPORT SerializationIgnore {
            operator SerializationOptions() const;
        };
        static const SerializationIgnore Ignore;
    };
    struct SerializeIf
    {
        explicit SerializeIf(const std::string& conditionalProperty)
            : m_conditionalProperty(conditionalProperty)
        {}
        const std::string& GetConditionalProperty() const { return m_conditionalProperty; }
    private:
        std::string m_conditionalProperty;
    };

    //If we are serializing based on a function (for example getDamping(i,j)) we need to know which combinations of i and j should be serialized.
    class SerializationSelector : public FunctionCallbackAttribute<bool>
    {
    public:
        template<typename Callback, typename... StringArguments>
        explicit SerializationSelector(Callback callback, StringArguments&&... stringArguments)
            : FunctionCallbackAttribute<bool>(callback, std::forward<StringArguments>(stringArguments)...)
        {
        }
    };

    class SerializationSetter : public FunctionCallbackAttribute<void>
    {
    public:
        template<typename Callback>
        explicit SerializationSetter(Callback callback)
            : FunctionCallbackAttribute<void>(callback, "","")
        {
        }
    };

    struct RequiredSerializationAttribute : public FunctionCallbackAttribute<std::string>
    {
    public:
        RequiredSerializationAttribute(const std::string& name, const char* value) : RequiredSerializationAttribute(name, std::string(value)) {}
        RequiredSerializationAttribute(const std::string& name, std::string value)
            : RequiredSerializationAttribute(name, [=]() {return value; })
        {}
        template<typename Callback, typename... StringArguments>
        RequiredSerializationAttribute(const std::string& name, const Callback& callback, StringArguments&&... stringArguments)
            : FunctionCallbackAttribute<std::string>(callback, std::forward<StringArguments>(stringArguments)...)
            , m_name(name)
        {}
        const std::string& GetName() const { return m_name; }
    private:
        std::string m_name;
    };
    class ImportExportOverrideImpl {
    public:
        template<typename ImportExportOverrideT>
        ImportExportOverrideImpl(ImportExportOverrideT& importExportOverride)
        {
            using ExportType = typename TypeUtilities::FunctionTraits<decltype(&ImportExportOverrideT::Export)>::ReturnType;
            using ImportType = typename TypeUtilities::FunctionTraits<decltype(&ImportExportOverrideT::Import)>::ReturnType;
            m_overrideType = Types::TypeId<ExportType>();
            m_importer = [](const Variants::Variant& input)
            {
                return Variants::VariantService::Reflect(ImportExportOverrideT::Import(Variants::InternalVariantService::UnreflectUnchecked<ExportType>(input)));
            };
            m_exporter = [](const Variants::Variant& input)
            {
                return Variants::VariantService::Reflect(ImportExportOverrideT::Export(Variants::InternalVariantService::UnreflectUnchecked<ImportType>(input)));
            };
        }
        const Types::DecoratedTypeInfo& GetOverrideType() { return m_overrideType; }
        Variants::Variant Import(const Variants::Variant& value) const
        {
            return m_importer(value);
        }
        Variants::Variant Export(const Variants::Variant& value) const
        {
            return m_exporter(value);
        }
    private:
        Types::DecoratedTypeInfo m_overrideType;
        std::function<Variants::Variant(const Variants::Variant&)> m_importer;
        std::function<Variants::Variant(const Variants::Variant&)> m_exporter;
    };
    //Specifies the serialization name of a constructor or property. Used as name in xml.
    class SerializationAttribute : public IAttribute
    {
    public:
        SerializationAttribute(const SerializationAttribute& other) = delete;
        SerializationAttribute(SerializationAttribute&& other) = default;
        template<typename... Args>
        SerializationAttribute(SerializationOptions::SerializationIgnore ignore)
            : m_options(ignore)
        {}
        template<typename... Args>
        SerializationAttribute(const std::string& name, const Args&... args)
            : m_name(name)
            , m_options(SerializationOptions::None)
        {
            ParseArgs(args...);
        }
        const std::string& GetName() const { return m_name; }
        SerializationOptions GetOptions() const { return m_options; }
        const std::list<std::string>& GetAlternativeNames() const { return m_alternativeNames; }
        const std::unique_ptr<ImportExportOverrideImpl>& GetImportExportOverride() const { return m_importExportOverride; }
        const std::unique_ptr<SerializeIf>& GetCondition() const { return m_condition; }
        const std::list<RequiredSerializationAttribute>& GetRequiredAttributes() const { return m_requiredAttributes; }
        const std::unique_ptr<SerializationSelector>& GetSerializationSelector() const { return m_serializationSelector; }
        const std::unique_ptr<SerializationSetter>& GetSerializationSetter() const { return m_serializationSetter; }
    private:
        void ParseArgs()
        {}
        template<typename... Args>
        void ParseArgs(const std::string& alternativeName, const Args&... args)
        {
            m_alternativeNames.push_back(alternativeName);
            ParseArgs(args...);
        }
        template<typename... Args>
        void ParseArgs(SerializationOptions option, const Args&... args)
        {
            m_options |= option;
            ParseArgs(args...);
        }
        template<typename... Args>
        void ParseArgs(const SerializationSelector& selector, const Args&... args)
        {
            m_serializationSelector = std::make_unique<SerializationSelector>(selector);
            ParseArgs(args...);
        }
        template<typename... Args>
        void ParseArgs(const SerializationSetter& setter, const Args&... args)
        {
            m_serializationSetter = std::make_unique<SerializationSetter>(setter);
            ParseArgs(args...);
        }
        template<typename... Args>
        void ParseArgs(const RequiredSerializationAttribute& requiredAttribute, const Args&... args)
        {
            m_requiredAttributes.emplace_back(requiredAttribute);
            ParseArgs(args...);
        }
        template<typename... Args>
        void ParseArgs(const SerializeIf& condition, const Args&... args)
        {
            m_condition = std::make_unique<SerializeIf>(condition);
            ParseArgs(args...);
        }
        template<typename ImportExportOverrideT, typename... Args>
        std::enable_if_t<std::is_base_of_v<IImportExportOverride, ImportExportOverrideT>> ParseArgs(const ImportExportOverrideT& importExportOverride, const Args&... args)
        {
            m_importExportOverride = std::make_unique<ImportExportOverrideImpl>(importExportOverride);
            ParseArgs(args...);
        }
        std::string m_name;
        std::list<std::string> m_alternativeNames;
        SerializationOptions m_options;
        std::unique_ptr<ImportExportOverrideImpl> m_importExportOverride;
        std::unique_ptr<SerializeIf> m_condition;
        std::unique_ptr<SerializationSelector> m_serializationSelector;
        std::unique_ptr<SerializationSetter> m_serializationSetter;
        std::list<RequiredSerializationAttribute> m_requiredAttributes;
    };
}}}}
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "jsDummyTypeLibrary.h"
#include "jsScopedDummyMode.h"

namespace DNVS { namespace MoFa { namespace Scripting {
    using namespace Reflection::TypeConversions;
    using namespace Reflection;
    class jsDummyConversionSequence : public IConversionSequence
    {
    public:
        jsDummyConversionSequence(const ConversionSequencePointer& conversionSequence)
            : m_conversionSequence(conversionSequence)
        {}

        virtual bool IsValid() const override
        {
            return m_conversionSequence->IsValid();
        }


        virtual Variants::Variant Convert(const Variants::Variant& input) const override
        {
            jsScopedDummyMode dummy;
            return m_conversionSequence->Convert(input);
        }


        virtual void IntrusiveConvert(Variants::Variant& input) const override
        {
            jsScopedDummyMode dummy;
            m_conversionSequence->IntrusiveConvert(input);
        }


        virtual const ArgumentConversionQuality& Quality() const override
        {
            return m_conversionSequence->Quality();
        }


        virtual bool Join(ConversionSequencePointer sequence) override
        {
            return m_conversionSequence->Join(sequence);
        }


        virtual bool JoinBefore(ConversionSequencePointer sequence) override
        {
            return m_conversionSequence->JoinBefore(sequence);
        }

    private:
        ConversionSequencePointer m_conversionSequence;
    };
    class jsDummyConversionGraph : public IConversionGraph
    {
    public:
        jsDummyConversionGraph(const ConversionGraphPointer& conversionGraph)
            : m_conversionGraph(conversionGraph)
        {
        }

        virtual void AddConversion(const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to, ConversionType::Type conversionType, const ConversionPointer& conversion) override
        {
            m_conversionGraph->AddConversion(from, to, conversionType, conversion);
        }


        virtual void AddAlternativeConverter(const Types::DecoratedTypeInfo& from, const AlternativeConverterPointer& alternativeConverter) override
        {
            m_conversionGraph->AddAlternativeConverter(from, alternativeConverter);
        }


        virtual ConversionSequencePointer GetConversionSequence(const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to) const override
        {
            jsScopedDummyMode dummyMode;
            ConversionSequencePointer conversionSequence = m_conversionGraph->GetConversionSequence(from, to);
            if (conversionSequence->IsValid())
                return std::make_shared<jsDummyConversionSequence>(conversionSequence);
            else
                return conversionSequence;
        }


        virtual ConversionSequencePointer GetConversionSequence(const Variants::Variant& from, const Types::DecoratedTypeInfo& to) const override
        {
            jsScopedDummyMode dummyMode;
            ConversionSequencePointer conversionSequence = m_conversionGraph->GetConversionSequence(from, to);
            if (conversionSequence->IsValid())
                return std::make_shared<jsDummyConversionSequence>(conversionSequence);
            else
                return conversionSequence;
        }


        virtual Variants::Variant GetDynamicType(const Variants::Variant& from) const override
        {
            jsScopedDummyMode dummyMode;
            return m_conversionGraph->GetDynamicType(from);
        }


        virtual bool HasDynamicConversion(const std::type_info& type) const override
        {
            return m_conversionGraph->HasDynamicConversion(type);
        }


        virtual Variants::Variant CreateLValue(const Variants::Variant& from) const override
        {
            jsScopedDummyMode dummyMode;
            return m_conversionGraph->CreateLValue(from);
        }


        virtual Variants::Variant TryWrapValue(const Variants::Variant& from) const override
        {
            jsScopedDummyMode dummyMode;
            return m_conversionGraph->TryWrapValue(from);
        }


        virtual bool SupportsWrapping(const Types::DecoratedTypeInfo& from) const override
        {
            return m_conversionGraph->SupportsWrapping(from);
        }
        virtual bool SupportsUnwrapping(const Types::DecoratedTypeInfo& from) const override
        {
            return m_conversionGraph->SupportsUnwrapping(from);
        }


        virtual Variants::Variant TryUnwrapValue(const Variants::Variant& from) const override
        {
            jsScopedDummyMode dummyMode;
            return m_conversionGraph->TryUnwrapValue(from);
        }


        void AddWrapper(const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to, const ConversionPointer& conversionFrom, const ConversionPointer& conversionTo) override
        {
            m_conversionGraph->AddWrapper(from, to, conversionFrom, conversionTo);
        }


        Types::DecoratedTypeInfo TryUnwrapType(const Types::DecoratedTypeInfo& from) const override
        {
            return m_conversionGraph->TryUnwrapType(from);
        }

    private:
        ConversionGraphPointer m_conversionGraph;
    };
    jsDummyTypeLibrary::jsDummyTypeLibrary(const Reflection::TypeLibraries::TypeLibraryPointer& typeLibrary)
        : m_typeLibrary(typeLibrary)
    {

    }

    Reflection::TypeLibraries::TypePointer jsDummyTypeLibrary::CreateType(const std::type_info& typeInfo, const std::string& name)
    {
        return m_typeLibrary->CreateType(typeInfo, name);
    }

    Reflection::TypeLibraries::TypePointer jsDummyTypeLibrary::LookupType(const std::type_index& typeInfo) const
    {
        return m_typeLibrary->LookupType(typeInfo);
    }

    std::map<std::type_index, Reflection::TypeLibraries::TypePointer> jsDummyTypeLibrary::GetAllTypes() const
    {
        return m_typeLibrary->GetAllTypes();
    }

    ConversionGraphPointer jsDummyTypeLibrary::GetConversionGraph() const
    {
        return std::make_shared<jsDummyConversionGraph>(m_typeLibrary->GetConversionGraph());
    }

    std::shared_ptr<Reflection::Types::ITypeFormatter> jsDummyTypeLibrary::GetTypeFormatter() const
    {
        return m_typeLibrary->GetTypeFormatter();
    }

    std::shared_ptr<Reflection::Variants::IVariantFormatter> jsDummyTypeLibrary::GetVariantFormatter() const
    {
        return m_typeLibrary->GetVariantFormatter();
    }

    void jsDummyTypeLibrary::SetVariantFormatter(const std::shared_ptr<Reflection::Variants::IVariantFormatter>& formatter)
    {
        m_typeLibrary->SetVariantFormatter(formatter);
    }

    void jsDummyTypeLibrary::SetTypeReflected(const std::type_info& typeInfo)
    {
        m_typeLibrary->SetTypeReflected(typeInfo);
    }

    bool jsDummyTypeLibrary::IsTypeReflected(const std::type_info& typeInfo) const
    {
        return m_typeLibrary->IsTypeReflected(typeInfo);
    }

    Services::ServiceProvider& jsDummyTypeLibrary::GetServiceProvider()
    {
        return m_typeLibrary->GetServiceProvider();
    }

    void jsDummyTypeLibrary::SetTypeFormatter(const std::shared_ptr<Types::ITypeFormatter>& typeFormatter)
    {
        m_typeLibrary->SetTypeFormatter(typeFormatter);
    }

}}}


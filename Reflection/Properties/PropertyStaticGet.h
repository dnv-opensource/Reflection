#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <TypeUtilities/FunctionTraits.h>

#include "Reflection/Members/ArgumentTypeVisitor.h"
#include "Reflection/Members/Method.h"
#include "Reflection/Members/CallableArguments.h"
#include "Reflection/Members/MemberLogger.h"
#include "Formatting/ReplaceSourceText.h"
#include "Reflection/Attributes/AliasAttribute.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Properties {

    /** 
    Defines a Get Property that will return a constant value stored int this class.
    */
    template<typename T>
    class PropertyStaticGet : public Members::Method
    {
    public:
        typedef typename T ReturnType;
        typedef typename Variants::Variant SelfType;
        BOOST_STATIC_CONSTANT(size_t, Arity = 1);
        PropertyStaticGet(const std::string& name, TypeConversions::ConversionGraphPointer conversionGraph, T value) 
            :   Members::Method(name, conversionGraph)
            ,   m_value(value)
        {
            SetArgumentCount(Arity, 0, Arity);
            SetArgumentInfo(0, new Members::ArgumentInfo("this", Types::TypeId<SelfType>()));
        }
        virtual Members::MemberType GetMemberType() const 
        {
            return Members::MemberType::TypePropertyGet;
        }

        ///Gets the arity of the member, i.e. how many arguments it takes
        virtual size_t GetArity() const {return Arity; }

        PropertyStaticGet& AddDocumentation(const std::string& doc)
        {
            Members::Method::AddDocumentation(doc);
            return *this;
        }
        template<typename AttributeT, typename... Args>
        PropertyStaticGet& AddAttribute(Args&&... args)
        {
            GetAttributeCollectionCreateIfMissing().AddAttribute(AttributeT(std::forward<Args>(args)...));
            return *this;
        }

        PropertyStaticGet& AddFormatter(const DNVS::MoFa::Formatting::IStringFormatter& formatter)
        {
            SetFormatter(formatter);
            return *this;
        }
        PropertyStaticGet& AddFormatter(const std::string& replacementText)
        {
            SetFormatter(DNVS::MoFa::Formatting::ReplaceSourceText(replacementText));
            return *this;
        }

        PropertyStaticGet& AddAlias(T alias)
        {
            AddAttribute<Attributes::AliasAttribute>(alias);
            return *this;
        }
        virtual Variants::Variant Invoke(const std::vector<Variants::Variant>& arguments, Members::MemberType type = Members::MemberType::TypeAll)
        {
            Members::MemberLogger logger(this);
            return Variants::VariantService::ReflectType<T>(m_value);
        }
        virtual Variants::Variant InvokeNative(const std::vector<Variants::Variant>& arguments)
        {
            return Variants::VariantService::ReflectType<T>(m_value);
        }
        virtual Types::DecoratedTypeInfo GetReturnType() const
        {
            return Types::TypeId<ReturnType>();
        }
        virtual void Validate( const std::vector<Variants::Variant>& arguments, Members::MemberType type = Members::MemberType::TypeAll )
        {
        }
        virtual bool HasValidation() const { return false; }
        virtual std::string ToString(const std::vector<std::string>& arguments) const
        {
            return GetName();
        }

        virtual bool IsConst() const
        {
            return true;
        }
        void RegisterMemberDetails(const std::shared_ptr<Members::MemberLoggerContext>& context)
        {
            this->SetMemberLoggerContext(context);
            Members::MemberLogger(this, false);
        }
    private:
        T m_value;
    };
}}}}
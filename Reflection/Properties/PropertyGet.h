#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <TypeUtilities/Invoker.h>
#include <TypeUtilities/FunctionTraits.h>

#include "Reflection/Members/ArgumentTypeVisitor.h"
#include "Reflection/Members/Method.h"
#include "Reflection/Members/CallableArguments.h"
#include "Reflection/Members/ArgumentForwarder/NoConvertArgumentList.h"
#include "Reflection/Members/MemberLogger.h"
#include "Reflection/Attributes/AttributeCollection.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Properties {

    /** Defines a new Get Property. Encapsulates a c++ function, either member function pointer or regular function pointer.
    Signature is a function signature of any kind with arity 1. 
    */
    template<typename Signature>
    class PropertyGet : public Members::Method
    {
    public:
        static_assert(TypeUtilities::FunctionTraits<Signature>::Arity == 1, "Arity of Signature must be 1");
        typedef typename TypeUtilities::FunctionTraits<Signature>::ReturnType ReturnType;
        typedef typename TypeUtilities::FunctionTraits<Signature>::Argument0 SelfType;
        BOOST_STATIC_CONSTANT(size_t, Arity = 1);
        PropertyGet(const std::string& name, TypeConversions::ConversionGraphPointer conversionGraph, Signature signature) 
            :   Members::Method(name, conversionGraph)
            ,   m_signature(signature)
        {
            SetArgumentCount(Arity, Arity, Arity);
            SetArgumentInfo(0, new Members::ArgumentInfo("this", Types::TypeId<SelfType>()));
        }
        virtual Members::MemberType GetMemberType() const 
        {
            return Members::MemberType::TypePropertyGet;
        }

        ///Gets the arity of the member, i.e. how many arguments it takes
        virtual size_t GetArity() const {return Arity; }

        PropertyGet& AddDocumentation(const std::string& doc)
        {
            Members::Method::AddDocumentation(doc);
            return *this;
        }
        template<typename AttributeT, typename... Args>
        PropertyGet& AddAttribute(Args&&... args)
        {
            GetAttributeCollectionCreateIfMissing().AddAttribute(AttributeT(std::forward<Args>(args)...));
            return *this;
        }
        virtual Variants::Variant Invoke(const std::vector<Variants::Variant>& arguments, Members::MemberType type = Members::MemberType::TypeAll)
        {
            Members::MemberLogger logger(this);
            return Variants::VariantService().ReflectType<ReturnType>(TypeUtilities::Invoke(m_signature, Members::CallableArguments(arguments, this)));
        }
        virtual Variants::Variant InvokeNative(const std::vector<Variants::Variant>& arguments)
        {
            return Variants::VariantService().ReflectType<ReturnType>(TypeUtilities::Invoke(m_signature, Members::ArgumentForwarder::NoConvertArgumentList(arguments)));
        }
        virtual Types::DecoratedTypeInfo GetReturnType() const
        {
            return Types::TypeId<ReturnType>();
        }

        virtual bool IsConst() const
        {
            return std::is_const_v<std::remove_reference_t<std::remove_pointer_t<SelfType>>>;
        }

        virtual void Validate( const std::vector<Variants::Variant>& arguments, Members::MemberType type = Members::MemberType::TypeAll )
        {
        }
        virtual bool HasValidation() const { return false; }
        virtual std::string ToString(const std::vector<std::string>& arguments) const
        {
            if(arguments.size() != 1)
                throw std::runtime_error("Method expects one argument");
            return arguments.at(0) + "." + GetName();
        }
        void RegisterMemberDetails(const std::shared_ptr<Members::MemberLoggerContext>& context)
        {
            this->SetMemberLoggerContext(context);
            Members::MemberLogger(this, false);
        }
    private:
        Signature m_signature;
    };
}}}}
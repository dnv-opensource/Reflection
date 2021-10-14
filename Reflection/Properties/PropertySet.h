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
#include "Reflection/Members/ArgumentList.h"
#include "Reflection/Members/CallableArguments.h"
#include "Reflection/Members/ArgumentForwarder/NoConvertArgumentList.h"
#include "Reflection/Members/MemberLogger.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Properties {

    /** Defines a new Set Property. Encapsulates a c++ function, either member function pointer or regular function pointer.
    Signature is a function signature of any kind with arity 2. 
    */
    template<typename Signature>
    class PropertySet : public Members::Method
    {
    public:
        static_assert(TypeUtilities::FunctionTraits<Signature>::Arity == 2, "Arity of Signature must be 2");
        typedef typename TypeUtilities::FunctionTraits<Signature>::Argument0 SelfType;
        typedef typename TypeUtilities::FunctionTraits<Signature>::Argument1 ArgumentType;
        BOOST_STATIC_CONSTANT(size_t, Arity = 2);
        PropertySet(const std::string& name, TypeConversions::ConversionGraphPointer conversionGraph, Signature signature) 
            :   Members::Method(name, conversionGraph)
            ,   m_signature(signature)
        {
            SetArgumentCount(Arity, Arity, Arity);
            SetArgumentInfo(0, new Members::ArgumentInfo("this", Types::TypeId<SelfType>()));
            SetArgumentInfo(1, new Members::ArgumentInfo("value", Types::TypeId<ArgumentType>()));
        }
        virtual Members::MemberType GetMemberType() const 
        {
            return Members::MemberType::TypePropertySet;
        }

        ///Gets the arity of the member, i.e. how many arguments it takes
        virtual size_t GetArity() const {return Arity; }

        PropertySet& AddDocumentation(const std::string& doc)
        {
            Members::Method::AddDocumentation(doc);
            return *this;
        }
        template<typename AttributeT, typename... Args>
        PropertySet& AddAttribute(Args&&... args)
        {
            GetAttributeCollectionCreateIfMissing().AddAttribute(AttributeT(std::forward<Args>(args)...));
            return *this;
        }

        typedef typename TypeUtilities::FunctionTraits<Signature>::ReturnType ReturnType;
        virtual Variants::Variant Invoke(const std::vector<Variants::Variant>& arguments, Members::MemberType type = Members::MemberType::TypeAll)
        {
            Members::MemberLogger logger(this);
            TypeUtilities::Invoke(m_signature, Members::CallableArguments(arguments, this));
            return Variants::Variant();
        }
        virtual Variants::Variant InvokeNative(const std::vector<Variants::Variant>& arguments)
        {
            TypeUtilities::Invoke(m_signature, Members::ArgumentForwarder::NoConvertArgumentList(arguments));
            return Variants::Variant();
        }
        virtual void Validate( const std::vector<Variants::Variant>& arguments, Members::MemberType type = Members::MemberType::TypeAll )
        {
        }
        virtual bool HasValidation() const { return false; }
        virtual Types::DecoratedTypeInfo GetReturnType() const
        {
            return Types::TypeId<ReturnType>();
        }

        virtual bool IsConst() const
        {
            return std::is_const_v<std::remove_reference_t<std::remove_pointer_t<SelfType>>>;
        }
        virtual std::string ToString(const std::vector<std::string>& arguments) const
        {
            if (arguments.size() != 2)
                throw std::runtime_error("Method expects two arguments");
            return arguments.at(0) + "." + GetName() + " = " + arguments.at(1);
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
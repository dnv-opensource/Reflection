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
#include "Reflection/Members/MethodWithSignature.h"
#include "Reflection/Members/ArgumentForwarder/NoConvertArgumentListSelector.h"
#include "Reflection/Members/MemberLogger.h"
#include "Reflection/Types/DynamicTypeTraits.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Constructors {

    /** Defines a new Function. Encapsulates a c++ function, either member function pointer or regular function pointer.
    Signature is a function signature of any kind. 
    IsMemberFunction indicates whether the first argument should be treated as this.
    */
    template<typename Signature, typename HeldType, typename Modifier>
    class ConstructorFunction : public Members::MethodWithSignature<Signature, Modifier>
    {
    public:
        ConstructorFunction(const std::string& name, const std::string& actualName, TypeConversions::ConversionGraphPointer conversionGraph, Signature signature)
            :   Members::MethodWithSignature<Signature, Modifier>(name, conversionGraph)
            ,   m_signature(signature)
            ,   m_actualName(actualName)
        {
        }
        virtual Members::MemberType GetMemberType() const 
        {
            return Members::MemberType::TypeConstructor;
        }

        void DefineImplicitConstructorConversion() 
        {
            ConstructorConversionAdder<Signature, Modifier, Arity>::Apply(GetConversionGraph(), this);
        }

        template<typename AttributeT, typename... Args>
        ConstructorFunction& AddAttribute(Args&&... args)
        {
            GetAttributeCollectionCreateIfMissing().AddAttribute(AttributeT(std::forward<Args>(args)...));
            return *this;
        }
        /**
        Adds documentation + default arguments to the function signature. Syntax: AddSignature( (Arg("param1"),Arg("param2","This is the second param")=4) );
        Note the extra set of parenthesis. They are required.
        */
        template<typename... Args>
        ConstructorFunction& AddSignature(Args&&... args)
        {
            static_assert(!(sizeof...(Args) < Arity), "The function signature given in AddSignature(...) contains too few arguments");
            static_assert(!(sizeof...(Args) > Arity), "The function signature given in AddSignature(...) contains too many arguments");

            AddSignatureImpl(Members::NamedArgumentList<0>(), false, std::forward<Args>(args)...);
            DefineImplicitConstructorConversion();
            return *this;
        }

        ConstructorFunction& AddDocumentation(const std::string& doc)
        {
            Members::Method::AddDocumentation(doc);
            return *this;
        }
        template<typename T>
        Variants::Variant WrapReturnValue(T* value)
        {
            return Variants::VariantService().Reflect(HeldType(value));
        }
        template<typename T>
        Variants::Variant WrapReturnValue(const T& value)
        {
            Variants::Variant variant = Variants::VariantService().ReflectType<T>(value);
            variant.SetDecoratedTypeInfo(Types::RemoveConst(variant.GetDecoratedTypeInfo()));
            return variant;
        }
        typedef typename TypeUtilities::FunctionTraits<Signature>::ReturnType ReturnType;
        virtual Variants::Variant Invoke(const std::vector<Variants::Variant>& arguments, Members::MemberType type = Members::MemberType::TypeAll)
        {
            Members::MemberLogger logger(this);
            ReturnType result = TypeUtilities::Invoke(m_signature, Members::CallableArgumentsSelector<Arity, IsVarargValue>(arguments, this));
            return WrapReturnValue(result);
        }        
        virtual Variants::Variant InvokeNative(const std::vector<Variants::Variant>& arguments)
        {
            ReturnType result = TypeUtilities::Invoke(m_signature, Members::ArgumentForwarder::NoConvertArgumentListSelector<Arity, IsVarargValue>(arguments));
            return WrapReturnValue(result);
        }
        virtual bool IsConst() const
        {
            return false;
        }
        virtual std::string ToString(const std::vector<std::string>& arguments) const
        {
            if (arguments.size() < GetMinimumArgumentCount())
                throw std::runtime_error("Too few arguments to function");
            else if (arguments.size() > GetMaximumArgumentCount())
                throw std::runtime_error("Too many arguments to function");
            std::string result = m_actualName + "(";
            for (size_t i = 0; i < arguments.size(); ++i)
            {
                if (i > 0)
                    result += ", ";
                result += arguments.at(i);
            }
            result += ")";
            return result;
        }
        void RegisterMemberDetails(const std::shared_ptr<Members::MemberLoggerContext>& context)
        {
            this->SetMemberLoggerContext(context);
            Members::MemberLogger(this, false);
        }
        virtual std::string GetActualName() const { return m_actualName; }
    private:
        typedef typename TypeUtilities::FunctionTraits<Signature>::StorageType StoredSignature;
        StoredSignature m_signature;
        std::string m_actualName;
    };
}}}}
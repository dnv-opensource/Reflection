#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <TypeUtilities/Invoker.h>
#include <TypeUtilities/FunctionTraits.h>

#include "Reflection/Members/MethodWithSignature.h"
#include "Reflection/Members/CallableArgumentsSelector.h"
#include "Reflection/Members/ArgumentForwarder/NoConvertArgumentListSelector.h"
#include "Reflection/Members/MemberLogger.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Functions {

    /** Defines a new Function. Encapsulates a c++ function, either member function pointer or regular function pointer.
    Signature is a function signature of any kind. 
    IsMemberFunction indicates whether the first argument should be treated as this.
    */
    template<typename Signature, typename Modifier, bool IsMemberFunction>
    class Function : public Members::MethodWithSignature<Signature, Modifier>
    {
    public:
        Function(const std::string& name, TypeConversions::ConversionGraphPointer conversionGraph, Signature signature) 
            :   Members::MethodWithSignature<Signature, Modifier>(name, conversionGraph)
            ,   m_signature(signature)
        {
        }
        virtual Members::MemberType GetMemberType() const 
        {
            return IsMemberFunction?Members::MemberType::TypeMemberFunction:Members::MemberType::TypeStaticFunction;
        }

        BOOST_STATIC_CONSTANT(size_t, SelfCount = (IsMemberFunction?1:0));
        BOOST_STATIC_CONSTANT(size_t, SignatureArity = Arity-VarargArity-SelfCount);

        /**
        Adds documentation + default arguments to the function signature. Syntax: AddSignature("param1", Arg("param2","This is the second param")=4);
        Note the extra set of parenthesis. They are required.
        */
        template<typename... Args>
        Function& AddSignature(Args&&... args)
        {
            static_assert(!(sizeof...(Args) < SignatureArity), "The function signature given in AddSignature(...) contains too few arguments");
            static_assert(!(sizeof...(Args) > SignatureArity), "The function signature given in AddSignature(...) contains too many arguments");

            AddSignatureImpl(Members::NamedArgumentList<0>(), IsMemberFunction, std::forward<Args>(args)...);
            return *this;
        }
        template<typename AttributeT, typename... Args>
        Function& AddArgumentAttribute(size_t index, Args&&... args)
        {
            GetArgumentInfo(index)->GetAttributeCollection().AddAttribute(AttributeT(std::forward<Args>(args)...));
            return *this;
        }
        template<typename AttributeT, typename... Args>
        Function& AddArgumentAttribute(const char* argumentName, Args&&... args)
        {
            for (size_t i = 0; i < Arity; ++i)
            {
                if (GetArgumentInfo(i)->GetName() == argumentName)
                    return AddArgumentAttribute<AttributeT>(i,std::forward<Args>(args)...);
            }
            throw std::runtime_error("Argument named " + std::string(argumentName) + " not found for member " + GetName());
        }
        template<typename AttributeT, typename... Args>
        Function& AddAttribute(Args&&... args)
        {
            GetAttributeCollectionCreateIfMissing().AddAttribute(AttributeT(std::forward<Args>(args)...));
            return *this;
        }
        Function& AddDocumentation(const std::string& doc)
        {
            Members::Method::AddDocumentation(doc);
            return *this;
        }
        Function& AddValidation(const ValidationFunction& validator)
        {
            SetValidationFunction(validator);
            return *this;
        }
        typedef typename TypeUtilities::FunctionTraits<Signature>::ReturnType ReturnType;
        virtual Variants::Variant Invoke(const std::vector<Variants::Variant>& arguments, Members::MemberType type = Members::MemberType::TypeAll)
        {
            Members::MemberLogger logger(this);
            Validate(arguments, type);
            return InvokeImpl<ReturnType>::Apply(this, arguments);
        }

        virtual Variants::Variant InvokeNative(const std::vector<Variants::Variant>& arguments)
        {
            return InvokeNativeImpl<ReturnType>::Apply(this, arguments);
        }
        virtual bool IsConst() const
        {
            return IsConstImpl<IsMemberFunction>::Apply<Signature>();
        }
        virtual std::string ToString(const std::vector<std::string>& arguments) const
        {
            if (arguments.size() < GetMinimumArgumentCount())
                throw std::runtime_error("Too few arguments to function");
            else if (arguments.size() > GetMaximumArgumentCount())
                throw std::runtime_error("Too many arguments to function");
            std::string result;
            size_t startIndex = SelfCount;
            if (IsMemberFunction)
                result += arguments.at(0) + "." + GetName() + "(";
            else
                result += GetName() + "(";
            for (size_t i = startIndex; i < arguments.size(); ++i)
            {
                if (i > startIndex)
                    result += ", ";
                result += arguments.at(i);
            }
            result += ")";
            return result;
        }
        template<bool>
        struct IsConstImpl 
        {
            template<typename Signature>
            static bool Apply() 
            {
                return std::is_const_v<std::remove_reference_t<std::remove_pointer_t<typename TypeUtilities::FunctionTraits<Signature>::Argument0>>>;
            }
        };
        template<>
        struct IsConstImpl<false> 
        {
            template<typename Signature>
            static bool Apply() 
            {
                return false;
            }
        };
        void RegisterMemberDetails(const std::shared_ptr<Members::MemberLoggerContext>& context)
        {
            this->SetMemberLoggerContext(context);
            Members::MemberLogger(this, false);
        }
    protected:
        template<typename T>
        struct InvokeImpl 
        {
            static Variants::Variant Apply(Function* function, const std::vector<Variants::Variant>& arguments) 
            {
                return Variants::VariantService().ReflectType<T>(
                    TypeUtilities::Invoke(function->m_signature, Members::CallableArgumentsSelector<Arity, IsVarargValue>(arguments, function)));
            }
        };
        template<>
        struct InvokeImpl<void> 
        {
            static Variants::Variant Apply(Function* function, const std::vector<Variants::Variant>& arguments) 
            {
                TypeUtilities::Invoke(function->m_signature, Members::CallableArgumentsSelector<Arity, IsVarargValue>(arguments, function));
                return Variants::Variant();
            }
        };
        template<typename T>
        struct InvokeNativeImpl 
        {
            static Variants::Variant Apply(Function* function, const std::vector<Variants::Variant>& arguments) 
            {
                return Variants::VariantService().ReflectType<T>(
                    TypeUtilities::Invoke(function->m_signature, Members::ArgumentForwarder::NoConvertArgumentListSelector<Arity, IsVarargValue>(arguments)));
            }
        };
        template<>
        struct InvokeNativeImpl<void> 
        {
            static Variants::Variant Apply(Function* function, const std::vector<Variants::Variant>& arguments) 
            {
                TypeUtilities::Invoke(function->m_signature, Members::ArgumentForwarder::NoConvertArgumentListSelector<Arity, IsVarargValue>(arguments));
                return Variants::Variant();
            }
        };
    private:
        Signature m_signature;
    };
}}}}
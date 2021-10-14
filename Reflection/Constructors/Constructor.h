#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "ConstructorConversion.h"

#include "TypeUtilities/FunctionTraits.h"
#include "TypeUtilities/Constructor.h"
#include "TypeUtilities/PointerHolderConstructor.h"

#include "Reflection/Members/MethodWithSignature.h"
#include "Reflection/Members/CallableArgumentsSelector.h"
#include "Reflection/Members/ArgumentForwarder/NoConvertArgumentListSelector.h"
#include "Reflection/Members/MemberLogger.h"
#include "Reflection/Types/DynamicTypeTraits.h"



namespace DNVS {namespace MoFa { namespace Reflection { namespace Constructors {

    template<typename Signature, typename AllocatorT, typename HeldType, typename Modifier>
    class Constructor : public Members::MethodWithSignature<Signature, Modifier>
    {
    public:
        virtual Members::MemberType GetMemberType() const {return Members::MemberType::TypeConstructor; }
        static const bool HasAllocatorAsLastArgument = Members::IsAllocatorAsLastArgument<Modifier>::value;

        Variants::Variant RemoveConst(Variants::Variant variant)
        {
            if (Types::IsConst(variant.GetDecoratedTypeInfo()))
                variant.SetDecoratedTypeInfo(Types::RemoveConst(variant.GetDecoratedTypeInfo()));
            return variant;
        }
        virtual Variants::Variant Invoke( const std::vector<Variants::Variant>& arguments, Members::MemberType type = Members::MemberType::TypeAll ) 
        {
            Members::MemberLogger logger(this);
            Validate(arguments, type);
            ReturnType pointer = TypeUtilities::Constructor<Signature, HasAllocatorAsLastArgument>::Invoke(Members::CallableArgumentsSelector<Arity, IsVarargValue>(arguments, this), m_allocator);
            return RemoveConst(Variants::VariantService().Reflect(TypeUtilities::PointerHolderConstructor<HeldType, AllocatorT>::WrapPointer(pointer, m_allocator)));            
        }
        virtual Variants::Variant InvokeNative(const std::vector<Variants::Variant>& arguments)
        {
            ReturnType pointer = TypeUtilities::Constructor<Signature, HasAllocatorAsLastArgument>::Invoke(Members::ArgumentForwarder::NoConvertArgumentListSelector<Arity, IsVarargValue>(arguments), m_allocator);
            return RemoveConst(Variants::VariantService().Reflect(TypeUtilities::PointerHolderConstructor<HeldType, AllocatorT>::WrapPointer(pointer, m_allocator)));
        }

        Constructor(const std::string& name, const std::string& actualName, TypeConversions::ConversionGraphPointer conversionGraph, AllocatorT allocator) 
            :   Members::MethodWithSignature<Signature, Modifier>(name, conversionGraph) 
            ,   m_allocator(allocator)
            ,   m_actualName(actualName)
        {
        }
        void DefineImplicitConstructorConversion() 
        {
            ConstructorConversionAdder<Signature, Modifier, Arity>::Apply(GetConversionGraph(), this);
        }
        template<typename AttributeT, typename... Args>
        Constructor& AddAttribute(Args&&... args)
        {
            GetAttributeCollectionCreateIfMissing().AddAttribute(AttributeT(std::forward<Args>(args)...));
            return *this;
        }
        BOOST_STATIC_CONSTANT(size_t, SignatureArity = Arity - VarargArity);

        /**
        Adds documentation + default arguments to the constructor signature. Syntax: AddSignature("param1", Arg("param2","This is the second param")=4);
        Note the extra set of parenthesis. They are required.
        */
        template<typename... Args>
        Constructor& AddSignature(Args&&... args)
        {
            static_assert(!(sizeof...(Args) < SignatureArity), "The function signature given in AddSignature(...) contains too few arguments");
            static_assert(!(sizeof...(Args) > SignatureArity), "The function signature given in AddSignature(...) contains too many arguments");
            AddSignatureImpl(Members::NamedArgumentList<0>(), false, std::forward<Args>(args)...);
            DefineImplicitConstructorConversion();
            return *this;
        }

        Constructor& AddDocumentation(const std::string& documentation)
        {
            Members::Method::AddDocumentation(documentation);
            return *this;
        }
        Constructor& AddValidation(const ValidationFunction& validation)
        {
            SetValidationFunction(validation);
            return *this;
        }
        template<typename AttributeT, typename... Args>
        Constructor& AddArgumentAttribute(size_t index, Args&&... args)
        {
            GetArgumentInfo(index)->GetAttributeCollection().AddAttribute(AttributeT(std::forward<Args>(args)...));
            return *this;
        }
        template<typename AttributeT, typename... Args>
        Constructor& AddArgumentAttribute(const char* argumentName, Args&&... args)
        {
            for (size_t i = 0; i < Arity; ++i)
            {
                if (GetArgumentInfo(i)->GetName() == argumentName)
                    return AddArgumentAttribute<AttributeT>(i, std::forward<Args>(args)...);
            }
            throw std::runtime_error("Argument named " + std::string(argumentName) + " not found for member " + GetName());
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
        AllocatorT m_allocator;
        std::string m_actualName;
    };
}}}}

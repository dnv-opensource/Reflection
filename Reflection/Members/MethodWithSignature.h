#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IMember.h"
#include "Reflection/TypeConversions/IConversionGraph.h"
#include "Method.h"
#include "ArgumentTypeVisitorSelector.h"
#include "ArgumentList.h"


namespace DNVS {namespace MoFa {namespace Reflection {namespace Members {

    template<typename Signature, typename Modifier>
    class MethodWithSignature : public Method
    {
    public:
        BOOST_STATIC_CONSTANT(size_t, Arity = TypeUtilities::FunctionTraits<Signature>::Arity);
        template<typename Args>
        struct ValidationFunctionGenerator;
        template<typename... Args>
        struct ValidationFunctionGenerator<std::tuple<Args...>> {
            typedef std::function<void(Args...)> Type;
        };
        using ValidationFunction = typename ValidationFunctionGenerator<typename TypeUtilities::FunctionTraits<Signature>::Arguments>::Type;
        BOOST_STATIC_CONSTANT(bool, IsVarargValue = IsVararg<Modifier>::value);
        BOOST_STATIC_CONSTANT(int, VarargArity = (IsVarargValue?1:0));
        MethodWithSignature(const std::string& name, TypeConversions::ConversionGraphPointer conversionGraph) : Method(name, conversionGraph) 
        {
            size_t maxArity = Arity;
            if(IsVarargValue) 
                maxArity = (std::numeric_limits<size_t>::max)();
            SetArgumentCount(Arity, Arity-VarargArity, maxArity);
            for(size_t i = 0; i<Arity; ++i) 
            {
                SetArgumentInfo(i, new Members::ArgumentInfo("", GetArgumentType(i)));
            }
        }
        virtual bool IsVarargMember() const {return IsVarargValue; }
        ///Gets the arity of the member, i.e. how many arguments it takes
        virtual size_t GetArity() const
        {
            return Arity;
        }
        virtual Members::ArgumentInfoPointer GetArgumentInfo( size_t iarg ) const
        {
            if(IsVarargValue && iarg >= Arity-1) 
                return Members::Method::GetArgumentInfo(Arity-1);
            else 
                return Members::Method::GetArgumentInfo(iarg);
        }
        
        Types::DecoratedTypeInfo GetArgumentType(size_t i) const 
        {
            ArgumentTypeVisitorSelector<Arity, IsVarargValue> visitor(i);
            TypeUtilities::FunctionTraits<Signature>::Visit(visitor);
            return visitor.GetTypeInfo();
        }       
        typedef typename TypeUtilities::FunctionTraits<Signature>::ReturnType ReturnType;
        virtual Types::DecoratedTypeInfo GetReturnType() const
        {
            return Types::TypeId<ReturnType>();
        }
        
        void SetValidationFunction(ValidationFunction validation)
        {
            m_validationFunction = validation;
        }
        ValidationFunction GetValidationFunction() const
        {
            return m_validationFunction;
        }
        void Validate(const std::vector<Variants::Variant>& arguments, MemberType type = MemberType::TypeAll)
        {
            if(GetValidationFunction()) 
                TypeUtilities::Invoke(GetValidationFunction(), Members::CallableArgumentsSelector<Arity, IsVarargValue>(arguments, this));
        }
        virtual bool HasValidation() const override
        {
            return GetValidationFunction() != nullptr;
        }
    protected:
        //Adds names and default values to the argument list.
        template<typename Derived, int SignatureArity>
        void AddSignatureImpl(const ArgumentList<SignatureArity, Derived>& args, bool isMemberFunction)
        {
            size_t selfCount = isMemberFunction ? 1 : 0;
            size_t signatureArity = Arity - selfCount - VarargArity;
            size_t minCount = Derived::MinimumCount + selfCount - VarargArity;
            size_t maxCount = IsVarargValue ? (std::numeric_limits<size_t>::max)() : Derived::MaximumCount + selfCount;
            SetArgumentCount(Arity, minCount, maxCount);
            for (size_t i = 0; i < signatureArity; ++i)
            {
                Members::ArgumentInfoPointer info = args[i];
                info->SetTypeInfo(GetArgumentType(i + selfCount));
                SetArgumentInfo(i + selfCount, info);
            }
        }
        //Overload to match the case AddSignature(Arg("a") = 1.0, "b"); <- Illegal, since we cannot define an argument without default after one with default.
        template<size_t Index, size_t Defaults, typename... Args>
        void AddSignatureImpl(const Members::NamedArgumentListWithDefaults<Index, Defaults>& head, bool isMemberFunction, const Members::NamedArgumentList<1>& current, Args&&... tail)
        {
            static_assert(false, "Cannot have arguments without default after a value with default argument");
        }

        //Overload to match the case AddSignature("a", "b"); <- Two consecutive arguments without default values.
        template<size_t Index, typename... Args>
        void AddSignatureImpl(const Members::NamedArgumentList<Index>& head, bool isMemberFunction, const Members::NamedArgumentList<1>& current, Args&&... tail)
        {
            AddSignatureImpl(Members::NamedArgumentList<Index + 1>(head, current), isMemberFunction, std::forward<Args>(tail)...);
        }

        //Overload to match the case AddSignature(Arg("a") = 4, Arg("b") = 1.2); <- Two consecutive values with default arguments.
        template<size_t Index, size_t Defaults, typename... Args>
        void AddSignatureImpl(const Members::NamedArgumentListWithDefaults<Index, Defaults>& head, bool isMemberFunction, const Members::NamedArgumentListWithDefaults<1, 1>& current, Args&&... tail)
        {
            AddSignatureImpl(Members::NamedArgumentListWithDefaults<Index + 1, Defaults + 1>(head, current), isMemberFunction, std::forward<Args>(tail)...);
        }

        //Overload to match the case AddSignature("a", Arg("b") = 1.2); <- Transition between value without default and value with default.
        template<size_t Index, typename... Args>
        void AddSignatureImpl(const Members::NamedArgumentList<Index>& head, bool isMemberFunction, const Members::NamedArgumentListWithDefaults<1, 1>& current, Args&&... tail)
        {
            AddSignatureImpl(Members::NamedArgumentListWithDefaults<Index + 1, 1>(head, current), isMemberFunction, std::forward<Args>(tail)...);
        }
    private:
        ValidationFunction m_validationFunction;
    };
}}}}

#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "This.h"

#include "Operators/Stringizer.h"
#include "Operators/Invoker.h"

#include "Reflection/TypeConversions/TypeConversionService.h"
#include "Reflection/Members/Method.h"
#include "Reflection/Members/OperatorNames.h"
#include "Reflection/Members/ArgumentList.h"
#include "Reflection/Members/MemberLogger.h"
#include "Reflection/Variants/Variant.h"

#include "TypeUtilities/FunctionTraits.h"

#include <vector>

namespace DNVS { namespace MoFa { namespace Reflection { namespace Operators {
    template<typename Callback, typename Subject, typename Tag>
    struct UnaryCallback {
        UnaryCallback(const Callback& callback) : m_callback(callback) {}
        auto operator()(Subject subject) const -> typename TypeUtilities::FunctionTraits<Callback>::ReturnType
        {
            return m_callback(subject);
        }
    private:
        Callback m_callback;
    };
    template<typename Subject, typename Tag>
    struct UnaryCallback<void*, Subject, Tag>
    {
        auto operator()(Subject subject) const -> decltype(DNVS::MoFa::Operators::Invoker<Tag>::Invoke(subject))
        { 
            return DNVS::MoFa::Operators::Invoker<Tag>::Invoke(subject);
        }
    };
    template<typename Subject, typename Tag, typename Callback = void*>
    class UnaryOperator : public Members::Method 
    {
    public:
        UnaryOperator(const std::string& name, TypeConversions::ConversionGraphPointer graph)
            : Members::Method(name, graph) 
        {
            SetArgumentCount(1, 1, 1);
            SetArgumentInfo(0, new Members::ArgumentInfo("subject", Types::TypeId<Subject>()));
        }
        UnaryOperator(const std::string& name, TypeConversions::ConversionGraphPointer graph, const Callback& callback)
            : Members::Method(name, graph)
            , m_callback(callback)
        {
            SetArgumentCount(1, 1, 1);
            SetArgumentInfo(0, new Members::ArgumentInfo("subject", Types::TypeId<Subject>()));
        }
        ///Invokes the unary operator with a list of one arguments. Self is ignored.
        virtual Variants::Variant Invoke(const std::vector<Variants::Variant>& arguments, Members::MemberType type = Members::MemberType::TypeAll) 
        {
            Members::MemberLogger logger(this);
            std::vector<Variants::Variant> local_arguments(arguments);
            TypeConversions::TypeConversionService service(GetConversionGraph());            
            //Convert the argument.
            Types::ReturnType<Subject> subject = service.IntrusiveAs<Subject>(local_arguments[0]);
            ///Need special invoker method in order to handle void return types correctly.
            return InvokeImpl<decltype(m_callback(subject))>::Apply(m_callback, subject);
        }
        ///Invokes the binary operator with a list of two arguments. Self is ignored.
        virtual Variants::Variant InvokeNative(const std::vector<Variants::Variant>& arguments) 
        {
            ///Need special invoker method in order to handle void return types correctly.
            return InvokeImpl<decltype(m_callback(std::declval<Subject>()))>::Apply(m_callback, 
                Variants::InternalVariantService::UnreflectUnchecked<Subject>(arguments[0]));
        }

        virtual void Validate(const std::vector<Variants::Variant>& arguments, Members::MemberType type = Members::MemberType::TypeAll) {}
        virtual bool HasValidation() const { return false; }
        template<typename AttributeT, typename... Args>
        UnaryOperator& AddAttribute(Args&&... args)
        {
            Members::Method::GetAttributeCollectionCreateIfMissing().AddAttribute(AttributeT(std::forward<Args>(args)...));
            return *this;
        }

        UnaryOperator& AddSignature(const Members::NamedArgumentList<1>& arg)
        {
            Members::ArgumentInfoPointer info = arg.at(0);
            info->SetTypeInfo(GetArgumentInfo(0)->GetTypeInfo());
            SetArgumentInfo(0, info);
            return *this;
        }
        template<typename AttributeT, typename... Args>
        UnaryOperator& AddArgumentAttribute(Args&&... args)
        {
            GetArgumentInfo(0)->GetAttributeCollection().AddAttribute(AttributeT(std::forward<Args>(args)...));
            return *this;
        }
        UnaryOperator& AddDocumentation(const std::string& doc)
        {
            Members::Method::AddDocumentation(doc);
            return *this;
        }

        ///Gets the arity; the number of arguments expected to this method.
        virtual size_t GetArity() const
        {
            return 1;
        }

        virtual Members::MemberType GetMemberType() const
        {
            return Members::MemberType::TypeOperator;
        }

        virtual Types::DecoratedTypeInfo GetReturnType() const
        {
            return Types::TypeId<decltype(m_callback(std::declval<Subject>()))>();
        }

        virtual bool IsConst() const
        {
            return std::is_const_v<std::remove_reference_t<std::remove_pointer_t<Subject>>>;
        }
        virtual std::string ToString(const std::vector<std::string>& arguments) const
        {
            if (arguments.size() != 1)
                throw std::runtime_error("Method expects one argument");
            return DNVS::MoFa::Operators::Stringizer<Tag>::Stringize(arguments.at(0));
        }
        void RegisterMemberDetails(const std::shared_ptr<Members::MemberLoggerContext>& context)
        {
            this->SetMemberLoggerContext(context);
            Members::MemberLogger(this, false);
        }
    private:
        UnaryCallback<Callback, Subject, Tag> m_callback;
        template<typename ReturnType>
        struct InvokeImpl
        {
            template<typename CallbackT>
            static Variants::Variant Apply(const CallbackT& callback, Subject subject)
            {
                return Variants::VariantService().ReflectType<ReturnType>(callback(subject));
            }
        };
        template<>
        struct InvokeImpl<void>
        {
            template<typename CallbackT>
            static Variants::Variant Apply(const CallbackT& callback, Subject subject)
            {
                callback(subject);
                return Variants::Variant(0, Types::TypeId<void>());
            }
        };
    };
    namespace ThisOperators {
        using namespace DNVS::MoFa::Operators;

        //Operator +, UnaryPlus
        template<int Decoration>
        inline UnaryOperatorGenerator<This<Decoration>, Tags::UnaryPlus> operator +(This<Decoration>) 
        {
            return UnaryOperatorGenerator<This<Decoration>, Tags::UnaryPlus>(); 
        }

        template<typename T>
        inline UnaryOperatorGenerator<T, Tags::UnaryPlus> operator +(Other<T>)
        {
            return UnaryOperatorGenerator<T, Tags::UnaryPlus>();
        }

        //Operator -, Negate
        template<int Decoration>
        inline UnaryOperatorGenerator<This<Decoration>, Tags::Negate> operator -(This<Decoration>) 
        {
            return UnaryOperatorGenerator<This<Decoration>, Tags::Negate>(); 
        }
        template<typename T>
        inline UnaryOperatorGenerator<T, Tags::Negate> operator -(Other<T>)
        {
            return UnaryOperatorGenerator<T, Tags::Negate>();
        }

        //Operator *, Dereference
        template<int Decoration>
        inline UnaryOperatorGenerator<This<Decoration>, Tags::Dereference> operator *(This<Decoration>) 
        {
            return UnaryOperatorGenerator<This<Decoration>, Tags::Dereference>(); 
        }

        //Operator ~, Complement
        template<int Decoration>
        inline UnaryOperatorGenerator<This<Decoration>, Tags::Complement> operator ~(This<Decoration>) 
        {
            return UnaryOperatorGenerator<This<Decoration>, Tags::Complement>(); 
        }

        //Operator &, AddressOf
        template<int Decoration>
        inline UnaryOperatorGenerator<This<Decoration>, Tags::AddressOf> operator &(This<Decoration>) 
        {
            return UnaryOperatorGenerator<This<Decoration>, Tags::AddressOf>(); 
        }

        //Operator !, LogicalNot
        template<int Decoration>
        inline UnaryOperatorGenerator<This<Decoration>, Tags::LogicalNot> operator !(This<Decoration>) 
        {
            return UnaryOperatorGenerator<This<Decoration>, Tags::LogicalNot>(); 
        }

        //Operator ++, PreInc
        template<int Decoration>
        inline UnaryOperatorGenerator<This<Decoration>, Tags::PreInc> operator ++(This<Decoration>) 
        {
            return UnaryOperatorGenerator<This<Decoration>, Tags::PreInc>(); 
        }

        //Operator --, PreDec
        template<int Decoration>
        inline UnaryOperatorGenerator<This<Decoration>, Tags::PreDec> operator --(This<Decoration>) 
        {
            return UnaryOperatorGenerator<This<Decoration>, Tags::PreDec>(); 
        }

        //Operator ++, PostInc
        template<int Decoration>
        inline UnaryOperatorGenerator<This<Decoration>, Tags::PostInc> operator ++(This<Decoration>, int) 
        {
            return UnaryOperatorGenerator<This<Decoration>, Tags::PostInc>(); 
        }

        //Operator --, PostDec
        template<int Decoration>
        inline UnaryOperatorGenerator<This<Decoration>, Tags::PostDec> operator --(This<Decoration>, int) 
        {
            return UnaryOperatorGenerator<This<Decoration>, Tags::PostDec>(); 
        }
    }
}}}}
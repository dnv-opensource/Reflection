#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "This.h"

#include "Operators/Stringizer.h"

#include "Reflection/TypeConversions/TypeConversionService.h"
#include "Reflection/Members/Method.h"
#include "Reflection/Members/OperatorNames.h"
#include "Reflection/Members/ArgumentList.h"
#include "Reflection/Members/MemberLogger.h"
#include "Reflection/Variants/Variant.h"

#include "TypeUtilities/FunctionTraits.h"
#include <vector>

namespace DNVS { namespace MoFa { namespace Reflection { namespace Operators {
    template<typename Callback, typename Lhs, typename Rhs, typename Tag>
    struct BinaryCallback {
        BinaryCallback(Callback callback) : m_callback(callback) {}
        auto operator()(Lhs lhs, Rhs rhs) const -> typename TypeUtilities::FunctionTraits<Callback>::ReturnType
        {
            return m_callback(lhs, rhs);
        }
    private:
        Callback m_callback;
    };
    template<typename Lhs, typename Rhs, typename Tag>
    struct BinaryCallback<void*, Lhs, Rhs, Tag>
    {
        auto operator()(Lhs lhs, Rhs rhs) const ->decltype(DNVS::MoFa::Operators::Invoker<Tag>::Invoke(lhs, rhs))
        {
            return DNVS::MoFa::Operators::Invoker<Tag>::Invoke(lhs, rhs);
        }
    };
    template<typename Lhs, typename Rhs, typename Tag,typename Callback = void*>
    class BinaryOperator : public Members::Method 
    {
    public:
        BinaryOperator(const std::string& name, TypeConversions::ConversionGraphPointer graph) : Members::Method(name, graph)
        {
            SetArgumentCount(2, 2, 2);
            SetArgumentInfo(0, new Members::ArgumentInfo("lhs", Types::TypeId<Lhs>()));
            SetArgumentInfo(1, new Members::ArgumentInfo("rhs", Types::TypeId<Rhs>()));
        }
        BinaryOperator(const std::string& name, TypeConversions::ConversionGraphPointer graph, Callback callback) : Members::Method(name, graph)
            , m_callback(callback)
        {
            SetArgumentCount(2, 2, 2);
            SetArgumentInfo(0, new Members::ArgumentInfo("lhs", Types::TypeId<Lhs>()));
            SetArgumentInfo(1, new Members::ArgumentInfo("rhs", Types::TypeId<Rhs>()));           
        }
        ///Invokes the binary operator with a list of two arguments. Self is ignored.
        virtual Variants::Variant Invoke(const std::vector<Variants::Variant>& arguments, Members::MemberType type = Members::MemberType::TypeAll) 
        {
            Members::MemberLogger logger(this);
            std::vector<Variants::Variant> local_arguments(arguments);
            TypeConversions::TypeConversionService service(GetConversionGraph());
            //Convert the arguments.
            Types::ReturnType<Lhs> lhs = service.IntrusiveAs<Lhs>(local_arguments[0]);
            Types::ReturnType<Rhs> rhs = service.IntrusiveAs<Rhs>(local_arguments[1]);
            ///Need special invoker method in order to handle void return types correctly.
            return InvokeImpl<decltype(m_callback(lhs, rhs))>::Apply(m_callback, lhs, rhs);
        }
        ///Invokes the binary operator with a list of two arguments. Self is ignored.
        virtual Variants::Variant InvokeNative(const std::vector<Variants::Variant>& arguments)
        {
            ///Need special invoker method in order to handle void return types correctly.
            return InvokeImpl<decltype(m_callback(std::declval<Lhs>(), std::declval<Rhs>()))>::Apply(m_callback,
                Variants::InternalVariantService::UnreflectUnchecked<Lhs>(arguments[0]),
                Variants::InternalVariantService::UnreflectUnchecked<Rhs>(arguments[1]));
        }

        virtual void Validate(const std::vector<Variants::Variant>& arguments, Members::MemberType type = Members::MemberType::TypeAll) {}
        virtual bool HasValidation() const { return false; }
        template<typename AttributeT, typename... Args>
        BinaryOperator& AddAttribute(Args&&... args)
        {
            Members::Method::GetAttributeCollectionCreateIfMissing().AddAttribute(AttributeT(std::forward<Args>(args)...));
            return *this;
        }
        BinaryOperator& AddSignature(const Members::NamedArgumentList<1>& arg1, const Members::NamedArgumentList<1>& arg2)
        {
            SetSignatureInfo(arg1.at(0), 0);
            SetSignatureInfo(arg2.at(0), 1);
            return *this;
        }
        template<typename AttributeT, typename... Args>
        BinaryOperator& AddArgumentAttribute(size_t index, Args&&... args)
        {
            GetArgumentInfo(index)->GetAttributeCollection().AddAttribute(AttributeT(std::forward<Args>(args)...));
            return *this;
        }
        template<typename AttributeT, typename... Args>
        BinaryOperator& AddArgumentAttribute(const char* argumentName, Args&&... args)
        {
            for (size_t i = 0; i < 2; ++i)
            {
                if (GetArgumentInfo(i)->GetName() == argumentName)
                    return AddArgumentAttribute<AttributeT>(i, std::forward<Args>(args)...);
            }
            throw std::runtime_error("Argument named " + std::string(argumentName) + " not found for member " + GetName());
        }
        BinaryOperator& AddDocumentation(const std::string& doc)
        {
            Members::Method::AddDocumentation(doc);
            return *this;
        }

        ///Gets the arity; the number of arguments expected to this method.
        virtual size_t GetArity() const
        {
            return 2;
        }

        virtual Members::MemberType GetMemberType() const
        {
            return Members::MemberType::TypeOperator;
        }

        virtual Types::DecoratedTypeInfo GetReturnType() const
        {
            return Types::TypeId<decltype(m_callback(std::declval<Lhs>(), std::declval<Rhs>()))>();
        }

        virtual bool IsConst() const
        {
            return std::is_const_v<std::remove_reference_t<std::remove_pointer_t<Lhs>>>;
        }
        virtual std::string ToString(const std::vector<std::string>& arguments) const
        {
            if (arguments.size() != 2)
                throw std::runtime_error("Method expects two arguments");
            return DNVS::MoFa::Operators::Stringizer<Tag>::Stringize(arguments.at(0), arguments.at(1));
        }
        virtual void RegisterMemberDetails(const std::shared_ptr<Members::MemberLoggerContext>& context)
        {
            this->SetMemberLoggerContext(context);
            Members::MemberLogger(this, false);
        }
    private:
        void SetSignatureInfo(Members::ArgumentInfoPointer info, size_t index)
        {
            info->SetTypeInfo(GetArgumentInfo(index)->GetTypeInfo());
            SetArgumentInfo(index, info);
        }
        BinaryCallback<Callback, Lhs, Rhs, Tag> m_callback;
        template<typename ReturnType>
        struct InvokeImpl
        {
            template<typename CallbackT>
            static Variants::Variant Apply(CallbackT callback, Lhs lhs, Rhs rhs)
            {
                return Variants::VariantService().ReflectType<ReturnType>(callback(lhs, rhs));
            }
        };
        template<>
        struct InvokeImpl<void>
        {
            template<typename CallbackT>
            static Variants::Variant Apply(CallbackT callback, Lhs lhs, Rhs rhs)
            {
                callback(lhs, rhs);
                return Variants::Variant(0, Types::TypeId<void>());
            }
        };
    };
    namespace ThisOperators {
        using namespace DNVS::MoFa::Operators;
        
        //Operator <<, ShiftLeft
        template<int LhsDecoration, int RhsDecoration>
        inline BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::ShiftLeft> operator <<(This<LhsDecoration>, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::ShiftLeft>(); 
        }
        template<int LhsDecoration, typename Rhs>
        inline BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::ShiftLeft> operator <<(This<LhsDecoration>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::ShiftLeft>(); 
        }
        template<typename Lhs, int RhsDecoration>
        inline BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::ShiftLeft> operator <<(const Lhs&, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::ShiftLeft>(); 
        }

        //Operator >>, ShiftRight
        template<int LhsDecoration, int RhsDecoration>
        inline BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::ShiftRight> operator >>(This<LhsDecoration>, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::ShiftRight>(); 
        }
        template<int LhsDecoration, typename Rhs>
        inline BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::ShiftRight> operator >>(This<LhsDecoration>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::ShiftRight>(); 
        }
        template<typename Lhs, int RhsDecoration>
        inline BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::ShiftRight> operator >>(const Lhs&, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::ShiftRight>(); 
        }

        ///Operator *, Multiplies
        template<int LhsDecoration, int RhsDecoration>
        inline BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::Multiplies> operator *(This<LhsDecoration>, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::Multiplies>(); 
        }
        template<int LhsDecoration, typename Rhs>
        inline BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::Multiplies> operator *(This<LhsDecoration>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::Multiplies>(); 
        }
        template<typename Lhs, int RhsDecoration>
        inline BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::Multiplies> operator *(const Lhs&, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::Multiplies>(); 
        }

        ///Operator /, Divides
        template<int LhsDecoration, int RhsDecoration>
        inline BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::Divides> operator /(This<LhsDecoration>, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::Divides>(); 
        }
        template<int LhsDecoration, typename Rhs>
        inline BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::Divides> operator /(This<LhsDecoration>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::Divides>(); 
        }
        template<typename Lhs, int RhsDecoration>
        inline BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::Divides> operator /(const Lhs&, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::Divides>(); 
        }

        ///Operator %, Modulus
        template<int LhsDecoration, int RhsDecoration>
        inline BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::Modulus> operator %(This<LhsDecoration>, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::Modulus>(); 
        }
        template<int LhsDecoration, typename Rhs>
        inline BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::Modulus> operator %(This<LhsDecoration>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::Modulus>(); 
        }
        template<typename Lhs, int RhsDecoration>
        inline BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::Modulus> operator %(const Lhs&, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::Modulus>(); 
        }

        ///Operator +, Plus
        template<int LhsDecoration, int RhsDecoration>
        inline BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::Plus> operator +(This<LhsDecoration>, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::Plus>(); 
        }
        template<int LhsDecoration, typename Rhs>
        inline BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::Plus> operator +(This<LhsDecoration>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::Plus>(); 
        }
        template<typename Lhs, int RhsDecoration>
        inline BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::Plus> operator +(const Lhs&, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::Plus>(); 
        }

        ///Operator -, Minus
        template<int LhsDecoration, int RhsDecoration>
        inline BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::Minus> operator -(This<LhsDecoration>, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::Minus>(); 
        }
        template<int LhsDecoration, typename Rhs>
        inline BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::Minus> operator -(This<LhsDecoration>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::Minus>(); 
        }
        template<typename Lhs, int RhsDecoration>
        inline BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::Minus> operator -(const Lhs&, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::Minus>(); 
        }
        
        ///Operator <, Less
        template<int LhsDecoration, int RhsDecoration>
        inline BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::Less> operator <(This<LhsDecoration>, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::Less>(); 
        }
        template<int LhsDecoration, typename Rhs>
        inline BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::Less> operator <(This<LhsDecoration>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::Less>(); 
        }
        template<typename Lhs, int RhsDecoration>
        inline BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::Less> operator <(const Lhs&, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::Less>(); 
        }

        ///Operator >, Greater
        template<int LhsDecoration, int RhsDecoration>
        inline BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::Greater> operator >(This<LhsDecoration>, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::Greater>(); 
        }
        template<int LhsDecoration, typename Rhs>
        inline BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::Greater> operator >(This<LhsDecoration>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::Greater>(); 
        }
        template<typename Lhs, int RhsDecoration>
        inline BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::Greater> operator >(const Lhs&, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::Greater>(); 
        }

        ///Operator <=, LessEqual
        template<int LhsDecoration, int RhsDecoration>
        inline BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::LessEqual> operator <=(This<LhsDecoration>, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::LessEqual>(); 
        }
        template<int LhsDecoration, typename Rhs>
        inline BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::LessEqual> operator <=(This<LhsDecoration>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::LessEqual>(); 
        }
        template<typename Lhs, int RhsDecoration>
        inline BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::LessEqual> operator <=(const Lhs&, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::LessEqual>(); 
        }

        ///Operator >=, GreaterEqual
        template<int LhsDecoration, int RhsDecoration>
        inline BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::GreaterEqual> operator >=(This<LhsDecoration>, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::GreaterEqual>(); 
        }
        template<int LhsDecoration, typename Rhs>
        inline BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::GreaterEqual> operator >=(This<LhsDecoration>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::GreaterEqual>(); 
        }
        template<typename Lhs, int RhsDecoration>
        inline BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::GreaterEqual> operator >=(const Lhs&, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::GreaterEqual>(); 
        }

        ///Operator ==, EqualTo
        template<int LhsDecoration, int RhsDecoration>
        inline BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::EqualTo> operator ==(This<LhsDecoration>, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::EqualTo>(); 
        }
        template<int LhsDecoration, typename Rhs>
        inline BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::EqualTo> operator ==(This<LhsDecoration>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::EqualTo>(); 
        }
        template<typename Lhs, int RhsDecoration>
        inline BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::EqualTo> operator ==(const Lhs&, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::EqualTo>(); 
        }

        ///Operator !=, NotEqualTo
        template<int LhsDecoration, int RhsDecoration>
        inline BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::NotEqualTo> operator !=(This<LhsDecoration>, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::NotEqualTo>(); 
        }
        template<int LhsDecoration, typename Rhs>
        inline BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::NotEqualTo> operator !=(This<LhsDecoration>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::NotEqualTo>(); 
        }
        template<typename Lhs, int RhsDecoration>
        inline BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::NotEqualTo> operator !=(const Lhs&, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::NotEqualTo>(); 
        }

        ///Operator ||, LogicalOr
        template<int LhsDecoration, int RhsDecoration>
        inline BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::LogicalOr> operator ||(This<LhsDecoration>, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::LogicalOr>(); 
        }
        template<int LhsDecoration, typename Rhs>
        inline BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::LogicalOr> operator ||(This<LhsDecoration>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::LogicalOr>(); 
        }
        template<typename Lhs, int RhsDecoration>
        inline BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::LogicalOr> operator ||(const Lhs&, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::LogicalOr>(); 
        }

        ///Operator &&, LogicalAnd
        template<int LhsDecoration, int RhsDecoration>
        inline BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::LogicalAnd> operator &&(This<LhsDecoration>, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::LogicalAnd>(); 
        }
        template<int LhsDecoration, typename Rhs>
        inline BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::LogicalAnd> operator &&(This<LhsDecoration>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::LogicalAnd>(); 
        }
        template<typename Lhs, int RhsDecoration>
        inline BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::LogicalAnd> operator &&(const Lhs&, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::LogicalAnd>(); 
        }

        ///Operator &, BitwiseAnd
        template<int LhsDecoration, int RhsDecoration>
        inline BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::BitwiseAnd> operator &(This<LhsDecoration>, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::BitwiseAnd>(); 
        }
        template<int LhsDecoration, typename Rhs>
        inline BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::BitwiseAnd> operator &(This<LhsDecoration>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::BitwiseAnd>(); 
        }
        template<typename Lhs, int RhsDecoration>
        inline BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::BitwiseAnd> operator &(const Lhs&, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::BitwiseAnd>(); 
        }

        ///Operator |, BitwiseOr
        template<int LhsDecoration, int RhsDecoration>
        inline BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::BitwiseOr> operator |(This<LhsDecoration>, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::BitwiseOr>(); 
        }
        template<int LhsDecoration, typename Rhs>
        inline BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::BitwiseOr> operator |(This<LhsDecoration>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::BitwiseOr>(); 
        }
        template<typename Lhs, int RhsDecoration>
        inline BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::BitwiseOr> operator |(const Lhs&, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::BitwiseOr>(); 
        }

        ///Operator ^, BitwiseXor
        template<int LhsDecoration, int RhsDecoration>
        inline BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::BitwiseXor> operator ^(This<LhsDecoration>, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, This<RhsDecoration>, Tags::BitwiseXor>(); 
        }
        template<int LhsDecoration, typename Rhs>
        inline BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::BitwiseXor> operator ^(This<LhsDecoration>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<LhsDecoration>, Rhs, Tags::BitwiseXor>(); 
        }
        template<typename Lhs, int RhsDecoration>
        inline BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::BitwiseXor> operator ^(const Lhs&, This<RhsDecoration>) 
        {
            return BinaryOperatorGenerator<Lhs, This<RhsDecoration>, Tags::BitwiseXor>(); 
        }

        ///Operator <<=, ShiftLeftAssign
        template<typename Rhs>
        inline BinaryOperatorGenerator<This<false>, Rhs, Tags::ShiftLeftAssign> operator <<=(This<false>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<false>, Rhs, Tags::ShiftLeftAssign>(); 
        }

        ///Operator >>=, ShiftRightAssign
        template<typename Rhs>
        inline BinaryOperatorGenerator<This<false>, Rhs, Tags::ShiftRightAssign> operator >>=(This<false>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<false>, Rhs, Tags::ShiftRightAssign>(); 
        }

        ///Operator *=, MultipliesAssign
        template<typename Rhs>
        inline BinaryOperatorGenerator<This<false>, Rhs, Tags::MultipliesAssign> operator *=(This<false>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<false>, Rhs, Tags::MultipliesAssign>(); 
        }

        ///Operator /=, DividesAssign
        template<typename Rhs>
        inline BinaryOperatorGenerator<This<false>, Rhs, Tags::DividesAssign> operator /=(This<false>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<false>, Rhs, Tags::DividesAssign>(); 
        }

        ///Operator %=, ModulusAssign
        template<typename Rhs>
        inline BinaryOperatorGenerator<This<false>, Rhs, Tags::ModulusAssign> operator %=(This<false>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<false>, Rhs, Tags::ModulusAssign>(); 
        }

        ///Operator +=, PlusAssign
        template<typename Rhs>
        inline BinaryOperatorGenerator<This<false>, Rhs, Tags::PlusAssign> operator +=(This<false>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<false>, Rhs, Tags::PlusAssign>(); 
        }

        ///Operator -=, MinusAssign
        template<typename Rhs>
        inline BinaryOperatorGenerator<This<false>, Rhs, Tags::MinusAssign> operator -=(This<false>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<false>, Rhs, Tags::MinusAssign>(); 
        }

        ///Operator &=, BitwiseAndAssign
        template<typename Rhs>
        inline BinaryOperatorGenerator<This<false>, Rhs, Tags::BitwiseAndAssign> operator &=(This<false>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<false>, Rhs, Tags::BitwiseAndAssign>(); 
        }
        
        ///Operator |=, BitwiseOrAssign
        template<typename Rhs>
        inline BinaryOperatorGenerator<This<false>, Rhs, Tags::BitwiseOrAssign> operator |=(This<false>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<false>, Rhs, Tags::BitwiseOrAssign>(); 
        }

        ///Operator ^=, BitwiseXorAssign
        template<typename Rhs>
        inline BinaryOperatorGenerator<This<false>, Rhs, Tags::BitwiseXorAssign> operator ^=(This<false>, const Rhs&) 
        {
            return BinaryOperatorGenerator<This<false>, Rhs, Tags::BitwiseXorAssign>(); 
        }
    }
}}}}
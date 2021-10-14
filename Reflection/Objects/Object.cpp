//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Delegate.h"
#include "InvokeBinaryOperator.h"
#include "InvokeUnaryOperator.h"
#include "Object.h"
#include "Operators/Tags.h"
#include "Reflection/Members/IMember.h"
#include "Reflection/TypeConversions/IConversionSequence.h"
#include "Reflection/TypeConversions/TypeConversionService.h"
#include "Reflection/Types/DynamicTypeTraits.h"
#include "Reflection/Utilities/AutoGenerateStringFromConstructors.h"
#include "TryInvoke.h"
#include "Reflection/TypeConversions/IConversion.h"
#include "Reflection/Classes/Class.h"


namespace DNVS {namespace MoFa {namespace Reflection {namespace Objects {
    TypeLibraries::TypeLibraryPointer GetTypeLibraryImpl(const Object& lhs, const Object& rhs)
    {
        if (lhs.GetTypeLibrary() != nullptr)
            return lhs.GetTypeLibrary();
        else if (rhs.GetTypeLibrary() != nullptr)
            return rhs.GetTypeLibrary();
        else
            throw std::runtime_error("Unable to get type library");
    }

    Object::Object( TypeLibraries::TypeLibraryPointer typeLibrary, const Variants::Variant& var ) 
        :   m_typeLibrary(typeLibrary)
        ,   m_variable(var)
    {

    }

    Object::Object( TypeLibraries::TypeLibraryPointer typeLibrary, const Object& obj )
        :	m_typeLibrary(typeLibrary)
        ,	m_variable(obj.m_variable)
    {
    }

    Object::Object(TypeLibraries::TypeLibraryPointer typeLibrary) : m_typeLibrary(typeLibrary)
    {

    }

    Object::Object()
    {

    }

    bool Object::HasMember(const std::string& method) const
    {
        if (!m_typeLibrary)
            return false;
        TypeLibraries::TypePointer type = GetType();
        if (!type)
            return false;
        return type->Lookup(method, m_variable) != nullptr;
    }

    Object Object::Invoke(const std::string& method, const std::vector<Object>& objects, Members::MemberType memberType) const
    {
        if(!m_typeLibrary) 
            throw std::runtime_error("TypeLibrary not set");
        TypeLibraries::TypePointer type = GetType();
        if(type) 
        {
            std::vector<Variants::Variant> arguments = BuildArgumentVector(type, method, objects);
            Members::MemberWithArgumentsPointer delegate = type->PrepareInvoke(method, arguments, memberType);
            if(!delegate || !delegate->IsOk())
            {
                if (objects.size() == 1 && delegate->GetMember()->GetMemberType() == Members::MemberType::TypePropertyGet)
                    throw std::runtime_error(method + " is read only and can not be assigned to.");
                else if(objects.size() == 0 && delegate->GetMember()->GetMemberType() == Members::MemberType::TypePropertySet)
                    throw std::runtime_error(method + " is write only and can not be read from.");
                else
                    throw std::runtime_error("Unable to invoke " + method);
            }
            return Object(m_typeLibrary, delegate->Invoke());
        }
        throw std::runtime_error("Unable to find type");
    }

    Members::MemberWithArgumentsPointer Object::PrepareInvoke( const std::string& method, const std::vector<Object>& objects, Members::MemberType memberType) const
    {
        if(!m_typeLibrary) 
            return Members::MemberWithArgumentsPointer();
        TypeLibraries::TypePointer type = GetType();
        if(!type) 
            return Members::MemberWithArgumentsPointer();
        std::vector<Variants::Variant> arguments = BuildArgumentVector(type, method, objects);
        return type->PrepareInvoke(method, arguments, memberType);
    }

    Object& Object::Assign(const Object& rhs)
    {
        if (!m_typeLibrary)
            m_typeLibrary = rhs.m_typeLibrary;
        if (m_variable.IsValid())
        {
            try {
                m_variable = InvokeBinaryModifyingOperator<DNVS::MoFa::Operators::Tags::Assign>(m_typeLibrary, m_variable, rhs.m_variable);
                return *this;
            }
            catch (...) {}
        }
        Members::MemberPointer constructor = rhs.GetType()->Lookup(Members::ConstructorString);
        if (constructor)
            m_variable = constructor->Invoke({ rhs.m_variable });
        return *this;
    }

    Object& Object::operator=(const Object& rhs)
    {
        m_typeLibrary = rhs.m_typeLibrary;
        m_variable = rhs.m_variable;
        return *this;
    }
    Object Object::operator[](const Object& rhs)
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryOperator<DNVS::MoFa::Operators::Tags::Subscript>(typeLibrary, m_variable, rhs.m_variable));
    }

    Object::operator TypeSafeBool::Type() const
    {
        if (!IsConvertibleTo<bool>())
            return TypeSafeBool::Convert(false);

        return TypeSafeBool::Convert(As<bool>());
    }

    bool Object::IsConvertibleTo(const Types::DecoratedTypeInfo& info) const
    {
        if (!IsValid())
            return false;
        if (!m_typeLibrary)
            return m_variable.GetDecoratedTypeInfo() == info;

        return TypeConversions::TypeConversionService(m_typeLibrary->GetConversionGraph()).IsConvertibleTo(info, m_variable);
    }

    Object Object::ToLValue() const
    {
        if (!m_variable.IsValid() || !m_typeLibrary)
            return *this;
        return Object(m_typeLibrary, m_typeLibrary->GetConversionGraph()->CreateLValue(m_variable));
    }

    Object Object::ConvertTo(const Types::DecoratedTypeInfo& info) const
    {
        if (m_typeLibrary)
        {
            TypeConversions::ConversionSequencePointer sequence = m_typeLibrary->GetConversionGraph()->GetConversionSequence(m_variable, info);
            if (sequence && sequence->IsValid())
            {
                Variants::Variant result = sequence->Convert(m_variable);
                if(Types::IsPointer(info) && result.GetData() == nullptr)
                    result.SetDecoratedTypeInfo(info);
                return Object(m_typeLibrary, result);
            }                
        }
        else if (m_variable.GetDecoratedTypeInfo() == info)
            return *this;
        return Object();
    }

    Object Object::ConvertToDynamicType() const
    {
        if (m_typeLibrary)
        {
            return Object(m_typeLibrary, m_typeLibrary->GetConversionGraph()->GetDynamicType(m_variable));
        }
        else return *this;
    }

    void Object::Reset()
    {
        m_variable = Variants::Variant();
    }

    void Object::Reset(const Object& obj)
    {
        m_variable = obj.GetVariant();
        m_typeLibrary = obj.GetTypeLibrary();
    }

    void Object::Reset(const Variants::Variant& var)
    {
        m_variable = var;
    }

    const TypeLibraries::TypeLibraryPointer& Object::GetTypeLibrary() const
    {
        return m_typeLibrary;
    }

    const Variants::Variant& Object::GetVariant() const
    {
        return m_variable;
    }

    bool Object::IsValid() const
    {
        return m_variable.IsValid();
    }

    TypeLibraries::TypePointer Object::GetType() const
    {
        if (!m_typeLibrary)
            return nullptr;
        return m_typeLibrary->LookupType(m_typeLibrary->GetConversionGraph()->GetDynamicType(m_variable).GetDecoratedTypeInfo().GetTypeInfo());
    }

    Types::DecoratedTypeInfo Object::GetDecoratedTypeInfo() const
    {
        return m_variable.GetDecoratedTypeInfo();
    }

    Object Object::operator+(const Object& rhs) const
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryOperator<DNVS::MoFa::Operators::Tags::Plus>(typeLibrary, m_variable, rhs.m_variable));
    }
    Object Object::operator-(const Object& rhs) const 
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryOperator<DNVS::MoFa::Operators::Tags::Minus>(typeLibrary, m_variable, rhs.m_variable));
    }
    Object Object::operator*(const Object& rhs) const 
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryOperator<DNVS::MoFa::Operators::Tags::Multiplies>(typeLibrary, m_variable, rhs.m_variable));
    }
    Object Object::operator/(const Object& rhs) const 
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryOperator<DNVS::MoFa::Operators::Tags::Divides>(typeLibrary, m_variable, rhs.m_variable));
    }
    Object Object::operator^(const Object& rhs) const 
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryOperator<DNVS::MoFa::Operators::Tags::BitwiseXor>(typeLibrary, m_variable, rhs.m_variable));
    }
    Object Object::operator<<(const Object& rhs) const 
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryOperator<DNVS::MoFa::Operators::Tags::ShiftLeft>(typeLibrary, m_variable, rhs.m_variable));
    }
    Object Object::operator>>(const Object& rhs) const 
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryOperator<DNVS::MoFa::Operators::Tags::ShiftRight>(typeLibrary, m_variable, rhs.m_variable));
    }

    Object Object::operator%(const Object& rhs) const 
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryOperator<DNVS::MoFa::Operators::Tags::Modulus>(typeLibrary, m_variable, rhs.m_variable));
    }

    Object Object::operator<(const Object& rhs) const 
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryOperator<DNVS::MoFa::Operators::Tags::Less>(typeLibrary, m_variable, rhs.m_variable));
    }

    Object Object::operator>(const Object& rhs) const 
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryOperator<DNVS::MoFa::Operators::Tags::Greater>(typeLibrary, m_variable, rhs.m_variable));
    }

    Object Object::operator<=(const Object& rhs) const 
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryOperator<DNVS::MoFa::Operators::Tags::LessEqual>(typeLibrary, m_variable, rhs.m_variable));
    }

    Object Object::operator>=(const Object& rhs) const 
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryOperator<DNVS::MoFa::Operators::Tags::GreaterEqual>(typeLibrary, m_variable, rhs.m_variable));
    }

    Object Object::operator==(const Object& rhs) const 
    {
        try 
        {
            if (IsValid() != rhs.IsValid())
                return false;
            auto dynamic1 = ConvertToDynamicType();
            auto dynamic2 = rhs.ConvertToDynamicType();

            if (Types::IsPointer(dynamic1.GetVariant().GetDecoratedTypeInfo()) && Types::IsPointer(dynamic2.GetVariant().GetDecoratedTypeInfo()))
            {
                if (dynamic1.GetVariant().GetData() == dynamic2.GetVariant().GetData())
                    return Object(m_typeLibrary, true);
                else if(dynamic1.GetVariant().GetData() == nullptr || dynamic2.GetVariant().GetData() == nullptr)
                    return Object(m_typeLibrary, false);
            }
            auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
            return Object(typeLibrary, InvokeBinaryOperator<DNVS::MoFa::Operators::Tags::EqualTo>(typeLibrary, m_variable, rhs.m_variable));
        }
        catch(...) 
        {
            return Object(m_typeLibrary, false);
        }
    }

    Object Object::operator!=(const Object& rhs) const 
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryOperator<DNVS::MoFa::Operators::Tags::NotEqualTo>(typeLibrary, m_variable, rhs.m_variable));
    }

    Object Object::operator||(const Object& rhs) const 
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryOperator<DNVS::MoFa::Operators::Tags::LogicalOr>(typeLibrary, m_variable, rhs.m_variable));
    }

    Object Object::operator&&(const Object& rhs) const 
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryOperator<DNVS::MoFa::Operators::Tags::LogicalAnd>(typeLibrary, m_variable, rhs.m_variable));
    }

    Object Object::operator&(const Object& rhs) const 
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryOperator<DNVS::MoFa::Operators::Tags::BitwiseAnd>(typeLibrary, m_variable, rhs.m_variable));
    }

    Object Object::operator|(const Object& rhs) const 
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryOperator<DNVS::MoFa::Operators::Tags::BitwiseOr>(typeLibrary, m_variable, rhs.m_variable));
    }

    Object Object::operator<<=(const Object& rhs) 
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryModifyingOperator<DNVS::MoFa::Operators::Tags::ShiftLeftAssign>(typeLibrary, m_variable, rhs.m_variable));
    }

    Object Object::operator>>=(const Object& rhs) 
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryModifyingOperator<DNVS::MoFa::Operators::Tags::ShiftRightAssign>(typeLibrary, m_variable, rhs.m_variable));
    }

    Object Object::operator*=(const Object& rhs) 
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryModifyingOperator<DNVS::MoFa::Operators::Tags::MultipliesAssign>(typeLibrary, m_variable, rhs.m_variable));
    }

    Object Object::operator/=(const Object& rhs) 
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryModifyingOperator<DNVS::MoFa::Operators::Tags::DividesAssign>(typeLibrary, m_variable, rhs.m_variable));
    }

    Object Object::operator%=(const Object& rhs) 
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryModifyingOperator<DNVS::MoFa::Operators::Tags::ModulusAssign>(typeLibrary, m_variable, rhs.m_variable));
    }

    Object Object::operator+=(const Object& rhs) 
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryModifyingOperator<DNVS::MoFa::Operators::Tags::PlusAssign>(typeLibrary, m_variable, rhs.m_variable));
    }

    Object Object::operator-=(const Object& rhs) 
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryModifyingOperator<DNVS::MoFa::Operators::Tags::MinusAssign>(typeLibrary, m_variable, rhs.m_variable));
    }

    Object Object::operator&=(const Object& rhs) 
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryModifyingOperator<DNVS::MoFa::Operators::Tags::BitwiseAndAssign>(typeLibrary, m_variable, rhs.m_variable));
    }

    Object Object::operator|=(const Object& rhs) 
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryModifyingOperator<DNVS::MoFa::Operators::Tags::BitwiseOrAssign>(typeLibrary, m_variable, rhs.m_variable));
    }

    Object Object::operator^=(const Object& rhs) 
    {
        auto typeLibrary = GetTypeLibraryImpl(*this, rhs);
        return Object(typeLibrary, InvokeBinaryModifyingOperator<DNVS::MoFa::Operators::Tags::BitwiseXorAssign>(typeLibrary, m_variable, rhs.m_variable));
    }

    Object Object::operator+() const 
    {
        if(!m_typeLibrary)
            throw std::runtime_error("Unable to get type library");
        return Object(m_typeLibrary, InvokeUnaryOperator<DNVS::MoFa::Operators::Tags::UnaryPlus>(m_typeLibrary, m_variable));
    }
    Object Object::operator-() const 
    {
        if (!m_typeLibrary)
            throw std::runtime_error("Unable to get type library");
        return Object(m_typeLibrary, InvokeUnaryOperator<DNVS::MoFa::Operators::Tags::Negate>(m_typeLibrary, m_variable));
    }
    Object Object::operator*() const 
    {
        if (!m_typeLibrary)
            throw std::runtime_error("Unable to get type library");
        return Object(m_typeLibrary, InvokeUnaryOperator<DNVS::MoFa::Operators::Tags::Dereference>(m_typeLibrary, m_variable));
    }
    Object Object::operator~() const 
    {
        if (!m_typeLibrary)
            throw std::runtime_error("Unable to get type library");
        return Object(m_typeLibrary, InvokeUnaryOperator<DNVS::MoFa::Operators::Tags::Complement>(m_typeLibrary, m_variable));
    }
//     Object Object::operator&() const {
//         return Object(m_typeLibrary,InvokeUnaryOperator<DNVS::MoFa::Operators::Tags::AddressOf>(m_typeLibrary,m_variable));
//     }
    Object Object::operator!() const 
    {
        if (!m_typeLibrary)
            throw std::runtime_error("Unable to get type library");
        return Object(m_typeLibrary, InvokeUnaryOperator<DNVS::MoFa::Operators::Tags::LogicalNot>(m_typeLibrary, m_variable));
    }

    Object Object::operator++() 
    {
        if (!m_typeLibrary)
            throw std::runtime_error("Unable to get type library");
        return Object(m_typeLibrary, InvokeUnaryModifyingOperator<DNVS::MoFa::Operators::Tags::PreInc>(m_typeLibrary, m_variable));
    }

    Object Object::operator--() 
    {
        if (!m_typeLibrary)
            throw std::runtime_error("Unable to get type library");
        return Object(m_typeLibrary, InvokeUnaryModifyingOperator<DNVS::MoFa::Operators::Tags::PreDec>(m_typeLibrary, m_variable));
    }
    Object Object::operator++(int) 
    {
        if (!m_typeLibrary)
            throw std::runtime_error("Unable to get type library");
        return Object(m_typeLibrary, InvokeUnaryModifyingOperator<DNVS::MoFa::Operators::Tags::PostInc>(m_typeLibrary, m_variable));
    }

    Object Object::operator--(int) 
    {
        if (!m_typeLibrary)
            throw std::runtime_error("Unable to get type library");
        return Object(m_typeLibrary, InvokeUnaryModifyingOperator<DNVS::MoFa::Operators::Tags::PostDec>(m_typeLibrary, m_variable));
    }

    Delegate Object::Lookup( const std::string& method, Members::MemberType type) const
    {
        return Delegate(*this, method, type);
    }

    std::vector<Variants::Variant> Object::BuildArgumentVector( TypeLibraries::TypePointer type, const std::string& method, const std::vector<Object> &objects ) const
    {
        Members::MemberPointer member = type->Lookup(method);
        std::vector<Variants::Variant> arguments;
        if(member && !member->GetMemberType().IsMember()) 
            arguments.reserve(objects.size());
        else 
        {
            arguments.reserve(1 + objects.size());
            arguments.push_back(m_variable);
        }
        for(size_t i = 0; i<objects.size(); ++i) 
        {
            arguments.push_back(objects[i].GetVariant());
        }
        return std::move(arguments);
    }

    bool Object::CanWrapValue() const
    {
        if (!m_variable.IsValid() || !m_typeLibrary)
            return false;
        return m_typeLibrary->GetConversionGraph()->SupportsWrapping(m_variable.GetDecoratedTypeInfo());
    }

    Object Object::WrapValue() const
    {
        if (m_typeLibrary)
        {
            auto wrapped = m_typeLibrary->GetConversionGraph()->TryWrapValue(m_variable);
            if (wrapped.IsValid())
                return Object(m_typeLibrary, wrapped);
        }
        return *this;
    }

    bool Object::CanUnwrapValue() const
    {
        if (!m_variable.IsValid() || !m_typeLibrary)
            return false;
        return m_typeLibrary->GetConversionGraph()->SupportsUnwrapping(m_variable.GetDecoratedTypeInfo());
    }

    Object Object::UnwrapValue() const
    {
        if (m_typeLibrary)
        {
            auto unwrapped = m_typeLibrary->GetConversionGraph()->TryUnwrapValue(m_variable);
            if (unwrapped.IsValid())
                return Object(m_typeLibrary, unwrapped);
        }
        return *this;
    }

    class ConvertObjectToDynamic : public TypeConversions::IConversion
    {
    public:
        virtual Variants::Variant Convert(const Variants::Variant& variable) override
        {
            const Object& wrapper = Variants::InternalVariantService::UnreflectUnchecked<const Object&>(variable);
            return wrapper.GetVariant();
        }

        virtual void IntrusiveConvert(Variants::Variant& variable) override
        {
            variable = Convert(variable);
        }
    };
    class ConvertVariantToObject : public TypeConversions::IConversion
    {
    public:
        ConvertVariantToObject(const TypeLibraries::TypeLibraryPointer& typeLibrary)
            : m_typeLibrary(typeLibrary)
        {}        

        virtual Variants::Variant Convert(const Variants::Variant& variable) override
        {
            if (auto typeLibrary = m_typeLibrary.lock())
            {
                return Variants::VariantService::ReflectType<Object>(Object(typeLibrary, variable));
            }
            else
                throw std::runtime_error("type library is deleted");
        }


        virtual void IntrusiveConvert(Variants::Variant& variable) override
        {
            variable = Convert(variable);
        }

    private:
        std::weak_ptr<TypeLibraries::ITypeLibrary> m_typeLibrary;
    };

    void REFLECTION_IMPORT_EXPORT DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, Object**)
    {
        using namespace Classes;
        Class<Object> cls(typeLibrary, "Object");
        typeLibrary->GetConversionGraph()->AddConversion(
            Types::TypeId<void>(),
            Types::TypeId<Object>(),
            TypeConversions::ConversionType::ReflectionConversion,
            std::make_shared<ConvertVariantToObject>(typeLibrary)
        );
        typeLibrary->GetConversionGraph()->AddConversion(
            Types::TypeId<Object>(),
            TypeId<void>(),
            TypeConversions::ConversionType::DynamicTypeConversion,
            std::make_shared<ConvertObjectToDynamic>()
        );
    }

    std::string ToString(const Object& object, const Formatting::FormattingService& formattingService)
    {
        try {
            std::string text;
            if (TryInvoke(object, "ToString", formattingService).TryGetValue(text))
                return text;
            if (TryInvoke(object, "toString").TryGetValue(text))
                return text;
        }
        catch (...)
        {}
        return Utilities::AutoGenerateStringFromConstructors(object, formattingService);
    }

}}}}

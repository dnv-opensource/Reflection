#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "boost/optional/optional.hpp"
#include "Reflection/Classes/Class.h"
#include "Reflection/Attributes/PostfixAttribute.h"
#include "Reflection/Attributes/EnumerableAttribute.h"
#include "Reflection/Attributes/ValidationAttribute.h"
#include "Reflection/DoReflect.h"
#include "Formatting/FormattingService.h"
#include "Formatting/DefaultFormatterRules.h"
#include "Formatting/ToString.h"
#include "Reflection/Attributes/EnabledAttribute.h"
#include "Reflection/Attributes/MethodToStringAttribute.h"
#include "Reflection/Attributes/ParserAttribute.h"
namespace DNVS {namespace MoFa {namespace Reflection {

    template<typename Enum, Enum manual, typename Type = double>
    class EnumOrValue {
    public:
        typedef Enum EnumType;
        typedef Type ValueType;

        static const Enum Manual = manual;
        bool IsManual() const { return m_option == Manual; }
        bool IsValueValid() const { return m_value.is_initialized(); }

        EnumOrValue() : m_option(Manual), m_value() {}
        EnumOrValue(Enum option) : m_option(option) {}
        EnumOrValue(const Type& value, Enum option = Manual) : m_value(value), m_option(option) {}
        template<typename T>
        EnumOrValue(const T& value, Enum option = Manual, std::enable_if_t<std::is_convertible_v<T, Type>, void*> = nullptr)
            : m_value(value)
            , m_option(option)
        {}
        EnumOrValue(Enum option, const Type& value) : m_value(value), m_option(option) {}
        EnumOrValue(const EnumOrValue& other) : m_option(other.m_option), m_value(other.m_value) {}

        EnumOrValue& operator=(const EnumOrValue& other) {
            if (this != &other) {
                m_value = other.m_value;
                m_option = other.m_option;
            }
            return *this;
        }
        EnumOrValue& operator=(const Type& value)
        {
            m_value = value;
            m_option = Manual;
            return *this;
        }
        EnumOrValue& operator=(const Enum& option)
        {
            m_value.reset();
            m_option = option;
            return *this;
        }

        bool operator==(const EnumOrValue& other) const
        {
            if (m_option != other.m_option)
                return false;
            if (IsValueValid() && other.IsValueValid())
                return GetActualValue() == other.GetActualValue();
            return IsValueValid() == other.IsValueValid();
        }

        //Whenever we want to pass something other than this as the first value of an operator, we need to
        //either make it a friend operator (allowing it to take two arguments) or move it out of the class.
        //Here I have chosen to make it a friend.
        friend bool operator==(const Type& lhs, const EnumOrValue& rhs)
        {
            if (rhs.IsValueValid())
                return lhs == rhs.GetActualValue();
            else
                return false;
        }
        friend bool operator==(const EnumOrValue& lhs, const Type& rhs)
        {
            return rhs == lhs;
        }
        friend bool operator==(const Enum& lhs, const EnumOrValue& rhs)
        {
            return lhs == rhs.GetEnum();
        }
        friend bool operator==(const EnumOrValue& lhs, const Enum& rhs)
        {
            return rhs == lhs;
        }

        friend bool operator!=(const EnumOrValue& lhs, const EnumOrValue& rhs)
        {
            return !(lhs == rhs);
        }
        friend bool operator!=(const Type& lhs, const EnumOrValue& rhs)
        {
            return !(lhs == rhs);
        }
        friend bool operator!=(const EnumOrValue& lhs, const Type& rhs)
        {
            return !(lhs == rhs);
        }
        friend bool operator!=(const Enum& lhs, const EnumOrValue& rhs)
        {
            return !(lhs == rhs);
        }
        friend bool operator!=(const EnumOrValue& lhs, const Enum& rhs)
        {
            return !(lhs == rhs);
        }

        bool operator<(const EnumOrValue& other) const
        {
            if (IsValueValid() && other.IsValueValid())
            {
                if (GetActualValue() != other.GetActualValue())
                    return GetActualValue() < other.GetActualValue();
                else
                    return m_option < other.m_option;
            }
            else if (IsValueValid()) return false;
            else if (other.IsValueValid()) return true;
            else return m_option < other.m_option;
        }

        friend bool operator<(const Type& lhs, const EnumOrValue& rhs)
        {
            return EnumOrValue(lhs) < rhs;
        }
        friend bool operator<(const EnumOrValue& lhs, const Type& rhs)
        {
            return lhs < EnumOrValue(rhs);
        }
        friend bool operator<(const Enum& lhs, const EnumOrValue& rhs)
        {
            return EnumOrValue(lhs) < rhs;
        }
        friend bool operator<(const EnumOrValue& lhs, const Enum& rhs)
        {
            return lhs < EnumOrValue(rhs);
        }
        const Enum& GetEnum() const { return m_option; }
        const boost::optional<Type>& GetValue() const { return m_value; }

        const Type& ValueOrDefault(const Type& default_value) const
        {
            return GetValue().get_value_or(default_value);
        }
    private:
        const Type& GetActualValue() const { return GetValue().get(); }
        boost::optional<Type> m_value;
        Enum m_option;
    };

    template<typename Enum, Enum manual, typename Type>
    std::string ToString(const EnumOrValue<Enum, manual, Type>& val, const DNVS::MoFa::Formatting::FormattingService& service)
    {
        using namespace DNVS::MoFa::Formatting;
        //Brings ToString for enums into scope.
        auto rules =service.GetFormatterOrDefault<IFormatterRules, DefaultFormatterRules>();
        if(val.IsManual())
        {
            if(val.IsValueValid())
                return ToString(*val.GetValue(), service);
            else 
                return ToString(manual, service);
        }
        else if (val.IsValueValid() && !rules->RequireValidScript())
        {
            return IFormatterRules::ConditionallyEncloseExpression(ToString(*val.GetValue(), service), rules);
        }
        else
            return ToString(val.GetEnum(), service);
    }
    /**
    Overload of the Reflector class for EnumOrValue.
    Usage:
    typedef Utility::EnumOrValue<BendingCoefficientEnum,Manual,Units::Length> BendingOrLength;
    Reflector<BendingOrLength>::Reflect(typeLibrary,"BendingOrLength");
    */
    template<typename Enum, Enum manual, typename Type>
    void DoReflect(Reflection::TypeLibraries::TypeLibraryPointer typeLibrary, const char* name, EnumOrValue<Enum, manual, Type>**)
    {
        typedef Enum EnumType;
        typedef Type ValueType;
        using namespace Reflection::Classes;
        typedef EnumOrValue<Enum, manual, Type> EnumOrValueT;
        Class<EnumOrValueT> cls(typeLibrary, name);
        //Ensures that type pointers exists for the value type and the enum type.
        auto valueType = typeLibrary->CreateType(typeid(ValueType), "");
        auto enumType = typeLibrary->CreateType(typeid(EnumType), "");
        std::weak_ptr<Reflection::TypeLibraries::IType> weakType = cls.GetType();

        //Adds validation attribute.
        cls.AddAttribute<ValidationAttribute>([valueType](const EnumOrValueT& enumOrValue)
        {
            if (enumOrValue.IsManual() && !enumOrValue.IsValueValid())
            {
                throw std::runtime_error("Please enter a valid " + valueType->GetName());
            }
        },"");

        //Adds a postfix attribute to this class. Let it inherit the postfix attribute of the ValueType class.
        cls.AddAttribute<PostfixAttribute>([valueType]() -> std::string
        {
            if (valueType->GetAttributeCollection().HasAttribute<PostfixAttribute>())
                return valueType->GetAttributeCollection().GetAttribute<PostfixAttribute>().GetPostfix();
            else return "";
        });
        cls.AddAttribute<ParserAttribute>([enumType](const std::string& text, const DNVS::MoFa::Formatting::FormattingService& formattingService)
        {
            return enumType->GetAttributeCollection().GetAttribute<ParserAttribute>().Parse(text, formattingService);
        });
        //Adds an enumerable attribute to this class. Let it inherit the enumerable attribute of the EnumType, but add another value to the list; 
        //The empty (and invalid) EnumOrValue with Manual set and without a valid value. 
        cls.AddAttribute<EnumerableAttribute>([enumType]() -> std::list<Reflection::Variants::Variant>
        {
            std::list<Reflection::Variants::Variant> objects;
            if (enumType->GetAttributeCollection().HasAttribute<EnumerableAttribute>())
                objects = enumType->GetAttributeCollection().GetAttribute<EnumerableAttribute>().EnumerateObjects();
            objects.push_back(Reflection::Variants::VariantService::Reflect(EnumOrValueT()));
            return objects;
        });

        cls.Constructor<EnumType, ValueType>()
            .AddSignature("option", Arg("value") = ValueType())
            .AddAttribute<MethodToStringAttribute>([](const std::vector<std::pair<std::string, Reflection::Objects::Object>>& args)
        {
            if (args.size() == 2)
            {
                if (args.front().second.As<EnumType>() == manual)
                    return args.back().first;
                else
                    return args.front().first;
            }
            throw std::runtime_error("Invalid combination of arguments for toString");
        });

        //Allow class to be constructed with both an enum of EnumType and a value of ValueType.
        cls.Constructor<EnumType>(Alias);
        cls.Constructor<ValueType>(Alias);
        //Allow extraction of enum and value
        //Comparison, not only with another EnumOrValue, but also with an enum and a value.
        cls.Operator(This.Const == This.Const);
        cls.Operator(This.Const != This.Const);
        cls.Operator(This.Const < This.Const);
        cls.Operator(Other<ValueType>() == This.Const);
        cls.Operator(Other<ValueType>() != This.Const);
        cls.Operator(Other<ValueType>() < This.Const);
        cls.Operator(Other<EnumType>() == This.Const);
        cls.Operator(Other<EnumType>() != This.Const);
        cls.Operator(Other<EnumType>() < This.Const);
        cls.Operator(This.Const == Other<ValueType>());
        cls.Operator(This.Const != Other<ValueType>());
        cls.Operator(This.Const < Other<ValueType>());
        cls.Operator(This.Const == Other<EnumType>());
        cls.Operator(This.Const != Other<EnumType>());
        cls.Operator(This.Const < Other<EnumType>());
        cls.ImplicitConversion(&EnumOrValueT::GetEnum);
        auto valueGetter = [weakType, valueType](const EnumOrValueT& arg) -> ValueType
        {
            if (arg.GetValue().is_initialized())
                return *arg.GetValue();
            else
            {
                auto thisType = weakType.lock();
                std::string name;
                if (thisType)
                    name = thisType->GetName();
                else
                    name = "EnumOrValue";
                throw std::runtime_error("Unable to convert '" + name + "' to '" + valueType->GetName());
            }
        };
        cls.ImplicitConversion(valueGetter);
        cls.Get("Value", valueGetter)
           .AddAttribute<EnabledAttribute>([](EnumType enumValue) {return enumValue == manual; },"Option")
        ;
        cls.Get("Option", &EnumOrValueT::GetEnum)
           .AddAttribute<EnumerableAttribute>(DisableSorting, [enumType]() -> std::list<Reflection::Variants::Variant>
        {
            std::list<Reflection::Variants::Variant> objects;
            if (enumType->GetAttributeCollection().HasAttribute<EnumerableAttribute>())
                objects = enumType->GetAttributeCollection().GetAttribute<EnumerableAttribute>().EnumerateObjects();
            objects.push_back(Reflection::Variants::VariantService::Reflect(EnumOrValueT()));
            return objects;
        }); 
        //Implement conversion to string (Expression shall be scriptable)
        RegisterToStringFunction(cls);
    }
    
}}}

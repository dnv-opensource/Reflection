#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/TypeConversions/IConversion.h"
#include "Reflection/Variants/VariantService.h"
#include <tuple>
#include "TypeUtilities/TupleInvoke.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace TypeConversions {
    /*
    A user conversion is defined as a conversion supplied by the user, either in the form of a conversion operator or as a conversion constructor.
    Examples from C++:
    - std::string(const char* text)
      This creates a user conversion between const char* and std::string, so you can pass a text string anywhere a std::string is expected.
    - class A {
      public:
         operator int() const;
      };
      This creates a user conversion between A and int.
    */

    /*
    Encapsulates a user conversion.
    */
    template<typename From, typename To>
    class UserConversion : public IConversion
    {
        //Unwrap the incoming variant and change its type.
        virtual Variants::Variant Convert(const Variants::Variant& var) 
        {
            Variants::Variant result(var);
            IntrusiveConvert(result);
            result.SetDecoratedTypeInfo(Types::TypeId<To>());
            return result;
        }
        //Extract the actual data from the variant and do a user conversion to the To type.
        //This depends on the conversion to be a valid C++ expression.
        virtual void IntrusiveConvert( Variants::Variant& variable ) 
        {
            From from = Variants::InternalVariantService::UnreflectUnchecked<From>(variable);
            Variants::InternalVariantService::SetVariantData<To>(variable, To(from));
        }
    };

    /*
    In some cases, you may want a conversion to be valid in reflection even though it is not a valid C++ conversion.
    In such cases, you can supply a function pointer or lambda and register this as your user conversion.
    This version of the class is used for smart pointers.
    */
    template<typename From, typename To, typename Fn, typename HolderClass = To>
    class FunctionConversion : public IConversion 
    {
    public:
        FunctionConversion(Fn fn) : m_fn(fn) {}
        virtual Variants::Variant Convert(const Variants::Variant& var) 
        {
            Variants::Variant result(var);
            IntrusiveConvert(result);
            result.SetDecoratedTypeInfo(Types::TypeId<To>());
            return result;
        }
        virtual void IntrusiveConvert( Variants::Variant& variable ) 
        {
            From from = Variants::InternalVariantService::UnreflectUnchecked<From>(variable);
            To to = TypeUtilities::TupleInvoke(m_fn, std::make_tuple(from));
            Variants::InternalVariantService::SetVariantData<HolderClass>(variable, HolderClass(to));
        }
    private:
        Fn m_fn;
    };

    /*
    In some cases, you may want a conversion to be valid in reflection even though it is not a valid C++ conversion.
    In such cases, you can supply a function pointer or lambda and register this as your user conversion.
    */
    template<typename From, typename To, typename Fn>
    class FunctionConversion<From, To, Fn, To> : public IConversion 
    {
    public:
        FunctionConversion(Fn fn) : m_fn(fn) {}
        virtual Variants::Variant Convert(const Variants::Variant& var) 
        {
            Variants::Variant result(var);
            IntrusiveConvert(result);
            result.SetDecoratedTypeInfo(Types::TypeId<To>());
            return result;
        }
        virtual void IntrusiveConvert( Variants::Variant& variable ) 
        {
            From from = Variants::InternalVariantService::UnreflectUnchecked<From>(variable);
            To to = TypeUtilities::TupleInvoke(m_fn, std::make_tuple(from));
            Variants::InternalVariantService::SetVariantData<To>(variable, to);
        }
    private:
        Fn m_fn;
    };

    /*
    Register a constructor as a conversion. Here, we also encapsulate the resulting expression in a smart pointer.
    Example:
    If you have registered the following:
    Class<std::string, std::shared_ptr<std::string> > cls(...);
    then:
    cls.Constructor<const char*>();
    If we now want to convert const char* into std::string, we want it to become a std::shared_ptr<std::string>, since
    we have specified that std::shared_ptr<std::string> is the storage type for std::string. 
    This conversion makes sure this happens.
    */
    template<typename From, typename To, typename HolderClass>
    class ConstructorConversion : public IConversion 
    {
    public:
        ConstructorConversion() {}
        virtual Variants::Variant Convert(const Variants::Variant& var) 
        {
            Variants::Variant result(var);
            IntrusiveConvert(result);
            result.SetDecoratedTypeInfo(Types::TypeId<To*>());
            return result;
        }
        virtual void IntrusiveConvert( Variants::Variant& variable ) 
        {
            From from = Variants::InternalVariantService::UnreflectUnchecked<From>(variable);
            Variants::InternalVariantService::SetVariantData<HolderClass>(variable, HolderClass(new To(from)));
        }
    };

    /*
    Register a constructor as a conversion. Forwards to UserConversion. Only needed because we want to trap the case where the result should be encapsulated in a smart pointer.
    */
    template<typename From, typename To>
    class ConstructorConversion<From, To, To> : public UserConversion<From,To>
    {};

    //Methods for simplify creating the type conversion described above.
    template<typename From, typename To, typename HolderClass>
    void AddConstructorConversion(const ConversionGraphPointer& conversionGraph, ConversionType::Type conversionType = ConversionType::UserConversion)
    {
        conversionGraph->AddConversion(Types::TypeId<From>(), Types::TypeId<To*>(), conversionType, std::make_shared<ConstructorConversion<From, To, HolderClass>>());
    }
    template<typename From, typename To>
    void AddConstructorConversion(const ConversionGraphPointer& conversionGraph, ConversionType::Type conversionType = ConversionType::UserConversion)
    {
        conversionGraph->AddConversion(Types::TypeId<From>(), Types::TypeId<To>(), conversionType, std::make_shared<ConstructorConversion<From, To, To>>());
    }

    template<typename From, typename To>
    void AddUserConversion(const ConversionGraphPointer& conversionGraph) 
    {
        AddUserConversion<From,To>(conversionGraph, ConversionType::UserConversion);
    }

    template<typename From, typename To>
    void AddUserConversion(const ConversionGraphPointer& conversionGraph, ConversionType::Type conversionType)
    {
        conversionGraph->AddConversion(Types::TypeId<From>(), Types::TypeId<To>(), conversionType, std::make_shared<UserConversion<From, To>>());
    }

    template<typename From, typename To, typename Fn>
    void AddUserConversion(const ConversionGraphPointer& conversionGraph, Fn fn) 
    {
        AddUserConversion<From,To>(conversionGraph, ConversionType::UserConversion, fn);
    }
    template<typename From, typename To, typename Fn>
    void AddUserConversion(const ConversionGraphPointer& conversionGraph, ConversionType::Type conversionType, Fn fn)
    {
        conversionGraph->AddConversion(Types::TypeId<From>(), Types::TypeId<To>(), conversionType, std::make_shared<FunctionConversion<From, To, Fn, To>>(fn));
    }

    template<typename From, typename To, typename Fn, typename HolderT>
    void AddUserConversion(const ConversionGraphPointer& conversionGraph, Fn fn) 
    {
        conversionGraph->AddConversion(Types::TypeId<From>(), Types::TypeId<To>(), ConversionType::UserConversion, std::make_shared<FunctionConversion<From, To, Fn, HolderT>>(fn));
    }
}}}}
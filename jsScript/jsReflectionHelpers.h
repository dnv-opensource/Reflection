#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection\TypeConversions\InheritanceConversions.h"
#include "Reflection\TypeConversions\TrivialConversion.h"
#include "Reflection\TypeConversions\DynamicTypeDeduction.h"
#include "Reflection\TypeConversions\PointerReferenceConversion.h"
#include "Reflection/TypeConversions/UserConversion.h"
#include "TypeUtilities\FunctionTraits.h"

namespace DNVS {namespace MoFa {namespace Reflection {
    template<typename ClassT,typename BaseT>
    void AddClassWithBase(TypeConversions::ConversionGraphPointer conversionGraph) {
        if(!conversionGraph) return;
        TypeConversions::AddInheritanceConversions<BaseT,ClassT>(jsStack::stack()->GetConversionGraph());
        TypeConversions::AddTrivialConversions<ClassT>(jsStack::stack()->GetConversionGraph());
        TypeConversions::AddDynamicTypeDeduction<ClassT>(jsStack::stack()->GetConversionGraph());
        TypeConversions::AddPointerReferenceConversions<ClassT>(jsStack::stack()->GetConversionGraph());
    }
    template<typename Signature>
    struct AddConstructorConditionalIfArityOne {
        static void Apply(const TypeConversions::ConversionGraphPointer& conversionGraph,bool isUserConversion=true) {}
    };
    template<typename ClassT,typename ArgumentT>
    struct AddConstructorConditionalIfArityOne<ClassT(ArgumentT)> {
        static void Apply(const TypeConversions::ConversionGraphPointer& conversionGraph,bool isUserConversion=true) {
            if(!conversionGraph) return;
            TypeConversions::ConversionType::Type type;
            if(isUserConversion) type =TypeConversions::ConversionType::UserConversion;
            else type=TypeConversions::ConversionType::StandardConversion;
            conversionGraph->AddConversion(Types::TypeId<ArgumentT>(),Types::TypeId<ClassT*>(),type,std::make_shared<TypeConversions::ConstructorConversion<ArgumentT,ClassT,mofa::ref<ClassT>>>());
        }
    };
    template<typename Signature>
    void AddConstructorFunction(const TypeConversions::ConversionGraphPointer& conversionGraph,Signature signature,bool isUserConversion=true) {
        if(!conversionGraph) return;
        TypeConversions::ConversionType::Type type;
        if(isUserConversion) type =TypeConversions::ConversionType::UserConversion;
        else type=TypeConversions::ConversionType::StandardConversion;
        typedef TypeUtilities::FunctionTraits<Signature>::ReturnType To;
        typedef TypeUtilities::FunctionTraits<Signature>::Argument0 From;
        conversionGraph->AddConversion(Types::TypeId<From>(),Types::TypeId<To>(),type,std::make_shared<TypeConversions::FunctionConversion<From,To,Signature>>(signature));
    }

    template<typename ClassT,typename FunctionSignature>
    struct AddConstructorFunctionConditionalIfArityOne {
        static void Apply(FunctionSignature fn,const TypeConversions::ConversionGraphPointer& conversionGraph,bool isUserConversion=true) {}
    };
    template<typename ClassT,typename ArgumentT>
    struct AddConstructorFunctionConditionalIfArityOne<ClassT,ClassT*(*)(ArgumentT)> 
    {
        typedef ClassT* To;
        typedef ArgumentT From;
        typedef To(*FunctionSignature)(From);
        static void Apply(FunctionSignature fn,const TypeConversions::ConversionGraphPointer& conversionGraph,bool isUserConversion=true) {
            if(!conversionGraph) return;
            TypeConversions::ConversionType::Type type=isUserConversion?TypeConversions::ConversionType::UserConversion:TypeConversions::ConversionType::StandardConversion;
            conversionGraph->AddConversion(Types::TypeId<From>(),Types::TypeId<To>(),type,std::make_shared<TypeConversions::FunctionConversion<From,To,FunctionSignature,mofa::ref<ClassT> > >(fn));
        }
    };

    template<typename From,typename To>
    struct UniteConstructConversionDummy : public TypeConversions::IConversion {
    public:
        virtual Variants::Variant Convert( const Variants::Variant& variable ) {
            Variants::Variant result(variable);
            IntrusiveConvert(result);
            result.SetDecoratedTypeInfo(Types::TypeId<To*>());
            return result;
        }

        virtual void IntrusiveConvert( Variants::Variant& variable ) 
        {
            Variants::InternalVariantService::SetVariantData<mofa::ref<To>>(variable,mofa::ref<To>(new To));
        }
    };

    template<typename From,typename To>
    struct UniteConstructConversion : public TypeConversions::IConversion {
    public:
        virtual Variants::Variant Convert( const Variants::Variant& variable ) {
            Variants::Variant result(variable);
            IntrusiveConvert(result);
            result.SetDecoratedTypeInfo(Types::TypeId<To*>());
            return result;
        }

        virtual void IntrusiveConvert( Variants::Variant& variable ) 
        {
            From* from=Variants::InternalVariantService::UnreflectUnchecked<From*>(variable);
            Variants::InternalVariantService::SetVariantData<mofa::ref<To>>(variable,mofa::ref<To>(new To(from)));
        }
    };

    template<typename From,typename To>
    struct UniteCastConversion : public TypeConversions::IConversion {
    public:
        virtual Variants::Variant Convert( const Variants::Variant& variable ) {
            Variants::Variant result(variable);
            IntrusiveConvert(result);
            result.SetDecoratedTypeInfo(Types::TypeId<To*>());
            return result;
        }

        virtual void IntrusiveConvert( Variants::Variant& variable ) 
        {            
            From* from=Variants::InternalVariantService::UnreflectUnchecked<From*>(variable);
            Variants::InternalVariantService::SetVariantData<mofa::ref<To>>(variable,mofa::ref<To>(*from));
        }
    };

    template<typename From,typename To>
    struct ScriptClassPointerConversion : public TypeConversions::IConversion {
    public:
        virtual Variants::Variant Convert( const Variants::Variant& variable ) {
            Variants::Variant result(variable);
            IntrusiveConvert(result);
            result.SetDecoratedTypeInfo(Types::TypeId<To*>());
            return result;
        }

        virtual void IntrusiveConvert( Variants::Variant& variable ) 
        {            
            const From& from=Variants::InternalVariantService::UnreflectUnchecked<const From&>(variable);
            Variants::InternalVariantService::SetVariantData<mofa::ref<To>>(variable,mofa::ref<To>(new From(from)));
        }
    };
    template<typename T,typename U>
    void AddImplicitConversionToScriptClass(const TypeConversions::ConversionGraphPointer& conversionGraph) {
        if(!conversionGraph) return;
        conversionGraph->AddConversion(Types::TypeId<const T&>(),Types::TypeId<U*>(),TypeConversions::ConversionType::UserConversion,std::make_shared<ScriptClassPointerConversion<T,U>>());
    }
    template<bool IsAbstract>
    struct RegisterConversionToJsValueIfNot {
        template<typename T>
        static void Apply(const TypeConversions::ConversionGraphPointer& conversionGraph) {
            AddImplicitConversionToScriptClass<T,jsValue>(conversionGraph);
        }
    };
    template<>
    struct RegisterConversionToJsValueIfNot<true> {
        template<typename T>
        static void Apply(const TypeConversions::ConversionGraphPointer& conversionGraph) {
        }
    };

    template<typename From,typename To,bool EnableIf=std::is_convertible<From*,To>::value>
    struct AddUniteConstructorConversionIfConvertible {
        static void Apply(const TypeConversions::ConversionGraphPointer& conversionGraph) {
            conversionGraph->AddConversion(Types::TypeId<From*>(),Types::TypeId<To*>(),TypeConversions::ConversionType::UserConversion,std::make_shared<UniteConstructConversion<From,To>>());
            conversionGraph->AddConversion(Types::TypeId<To*>(),Types::TypeId<From*>(),TypeConversions::ConversionType::UserConversion,std::make_shared<UniteCastConversion<To,From>>());
        }
    };
    template<typename From,typename To>
    struct AddUniteConstructorConversionIfConvertible<From,To,false> {
        static void Apply(const TypeConversions::ConversionGraphPointer& conversionGraph) {
            conversionGraph->AddConversion(Types::TypeId<From*>(),Types::TypeId<To*>(),TypeConversions::ConversionType::UserConversion,std::make_shared<UniteConstructConversionDummy<From,To>>());
        }
    };

    template<typename From,typename To>
    void AddUniteConversion(const TypeConversions::ConversionGraphPointer& conversionGraph) {
        if(!conversionGraph) return;
        AddUniteConstructorConversionIfConvertible<From,To>::Apply(conversionGraph);
    }

    template<typename From,typename To,typename FunctionT>
    void AddImplicitConversion(const TypeConversions::ConversionGraphPointer& conversionGraph,FunctionT fn,bool isUserConversion)
    {
        if(!conversionGraph) return;
        TypeConversions::ConversionType::Type type;
        if(isUserConversion) type =TypeConversions::ConversionType::UserConversion;
        else type=TypeConversions::ConversionType::StandardConversion;
        conversionGraph->AddConversion(Types::TypeId<From>(),Types::TypeId<To>(),type,std::make_shared<TypeConversions::FunctionConversion<From,To,FunctionT,To>>(fn));
    }
}}}

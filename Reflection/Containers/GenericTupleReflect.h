#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <vector>
#include "Reflection/AutoReflect.h"
#include "Reflection/Classes/Class.h"
#include "../Attributes/TupleAttribute.h"
#include "../Variants/VariantService.h"
#include "../Attributes/GenerateConstructorArgumentsAttribute.h"
#include "../Attributes/AttributeCollectionService.h"

namespace DNVS { namespace MoFa { namespace Reflection {

	class TupleConverter : public TypeConversions::IConversion
	{
	public:
		TupleConverter(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Types::DecoratedTypeInfo& to, const Attributes::TupleAttribute& tupleAttributeFrom, const Attributes::TupleAttribute& tupleAttributeTo)
		: m_typeLibrary(typeLibrary) 
		, m_to(to)
		, m_tupleAttributeTo(tupleAttributeTo)
		, m_tupleAttributeFrom(tupleAttributeFrom)
		{
		}

		virtual Variants::Variant Convert(const Variants::Variant& variable) override
		{
			Objects::Object from(m_typeLibrary, variable);
			std::vector<Variants::Variant> constructorArguments;
			constructorArguments.reserve(m_tupleAttributeFrom.GetArgumentTypes().size());
			for (size_t i = 0; i < m_tupleAttributeFrom.GetArgumentTypes().size(); ++i)
				constructorArguments.push_back(from.Invoke("Get", {Objects::Object(m_typeLibrary, i)}).GetVariant());
			auto type = m_typeLibrary->LookupType(m_to.GetTypeInfo());
			auto constructor = type->Lookup(Members::ConstructorString);
			if (constructor)
				return constructor->Invoke(constructorArguments);
			else
				throw std::runtime_error("Unable to invoke constructor.");
		}

		virtual void IntrusiveConvert(Variants::Variant& variable) override
		{
			variable = Convert(variable);
		}

	private:
		Types::DecoratedTypeInfo m_to;
		const Attributes::TupleAttribute& m_tupleAttributeTo;
		const Attributes::TupleAttribute& m_tupleAttributeFrom;
		TypeLibraries::TypeLibraryPointer m_typeLibrary;
	};
	class AlternativeTupleConverter : public TypeConversions::IAlternativeConverter
	{
	public:
		AlternativeTupleConverter(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Attributes::TupleAttribute& tupleAttributeFrom)
			: m_typeLibrary(typeLibrary)
			, m_tupleAttributeFrom(tupleAttributeFrom)
		{}

		virtual bool CanConvert(const Variants::Variant& variant, const Types::DecoratedTypeInfo& returnType) const override
		{
			auto typeLibrary = m_typeLibrary.lock();
			if (!typeLibrary)
				return false;
			auto* tupleAttributeTo = Attributes::GetPointerToAttributeOrNull<Attributes::TupleAttribute>(typeLibrary, returnType);
			if (!tupleAttributeTo)
				return false;
			if (m_tupleAttributeFrom.GetArgumentTypes().size() != tupleAttributeTo->GetArgumentTypes().size())
				return false;
			for (size_t i = 0; i < m_tupleAttributeFrom.GetArgumentTypes().size(); ++i)
			{
				auto conversionSequence = typeLibrary->GetConversionGraph()->GetConversionSequence(m_tupleAttributeFrom.GetArgumentTypes()[i], tupleAttributeTo->GetArgumentTypes()[i]);
				if (!conversionSequence || !conversionSequence->IsValid())
					return false;
			}
			return true;				
		}


		virtual TypeConversions::ConversionPointer CreateConverter(const Types::DecoratedTypeInfo& returnType) const override
		{
			auto typeLibrary = m_typeLibrary.lock();
			if (!typeLibrary)
				return nullptr;
			auto* tupleAttributeTo = Attributes::GetPointerToAttributeOrNull<Attributes::TupleAttribute>(typeLibrary, returnType);
			if (!tupleAttributeTo)
				return nullptr;
			return std::make_shared<TupleConverter>(m_typeLibrary.lock(), returnType, m_tupleAttributeFrom, *tupleAttributeTo);
		}


		virtual TypeConversions::ConversionType::Type GetConversionType() const override
		{
			return TypeConversions::ConversionType::UserConversion;
		}

	private:
		std::weak_ptr<TypeLibraries::ITypeLibrary> m_typeLibrary;
		const Attributes::TupleAttribute& m_tupleAttributeFrom;
	};

	template<typename TupleT>
	Variants::Variant GetAt(const TupleT& tuple, size_t index, std::integral_constant<size_t, 0>)
	{
		if (index == 0)
			return Variants::VariantService::ReflectDynamic(std::get<0>(tuple));
		else
			throw std::runtime_error("Index out of bounds");
	}

	template<typename TupleT, size_t Index>
	Variants::Variant GetAt(const TupleT& tuple, size_t index, std::integral_constant<size_t, Index>)
	{
		if (index == Index)
			return Variants::VariantService::ReflectDynamic(std::get<Index>(tuple));
		return GetAt(tuple, index, std::integral_constant<size_t, Index - 1>());
	}

	template<typename TupleT>
	void SetAt(TupleT& tuple, size_t index, std::integral_constant<size_t, 0>, const Objects::Object& object)
	{
		if (index == 0)
			std::get<0>(tuple) = object.As<std::tuple_element_t<0, TupleT>>();
		else
			throw std::runtime_error("Index out of bounds");
	}

	template<typename TupleT, size_t Index>
	void SetAt(TupleT& tuple, size_t index, std::integral_constant<size_t, Index>, const Objects::Object& object)
	{
		if (index == Index)
			std::get<Index>(tuple) = object.As<std::tuple_element_t<Index, TupleT>>();
		SetAt(tuple, index, std::integral_constant<size_t, Index - 1>(), object);
	}
    template<typename T>
    struct ConstructorArgumentGenerator;
    template<typename First, typename Second>
    struct ConstructorArgumentGenerator<std::pair<First, Second>>
    {
        static bool Generate(const std::pair<First, Second>& lhs, std::remove_const_t<First>& first, std::remove_const_t<Second>& second)
        {
            first = lhs.first;
            second = lhs.second;
            return true;
        }
    };

    template<typename... Args>
    struct ConstructorArgumentGenerator<std::tuple<Args...>>
    {
        static bool Generate(const std::tuple<Args...>& lhs, std::add_lvalue_reference_t<std::remove_const_t<Args>>... args)
        {
            GenerateImpl(lhs, args);
            return true;
        }
        static void GenerateImpl(const std::tuple<Args...>& lhs)
        {}
        template<typename T,typename... RemainderT>
        static void GenerateImpl(const std::tuple<Args...>& lhs, T& first, RemainderT&... remainder)
        {
            first = std::get<sizeof(Args...) - sizeof(RemainderT...) - 1>(lhs);
            GenerateImpl(lhs, remainder);
        }
    };

	template<typename... Args, typename... ClassArgs>
	void DefineTupleConstructor(Classes::Class<std::tuple<Args...>, ClassArgs...>& cls)
	{
		auto& constructor = cls.Constructor<Args...>();
        constructor.AddAttribute<Attributes::GenerateConstructorArgumentsAttribute>(ConstructorArgumentGenerator<std::tuple<Args...>>::Generate);
        for (size_t i = 0; i < constructor.GetArity(); ++i)
            constructor.GetArgumentInfo(i)->SetName("arg" + std::to_string(i+1));
	}
	template<typename First, typename Second, typename... ClassArgs>
	void DefineTupleConstructor(Classes::Class<std::pair<First, Second>, ClassArgs...>& cls)
	{
        cls.Constructor<First, Second>()
            .AddSignature("arg0", "arg1")
            .AddAttribute<Attributes::GenerateConstructorArgumentsAttribute>(ConstructorArgumentGenerator<std::pair<First, Second>>::Generate);
	}
	template<typename TupleT, typename NonConstTupleT = TupleT>
	void ReflectConstTuple(const TypeLibraries::TypeLibraryPointer& typeLibrary)
	{
		using namespace Classes;
		Class<TupleT, IgnoreAutoReflector, TupleT> cls(typeLibrary, "Tuple");
		cls.AddAttribute<TupleAttribute>(TypeId<NonConstTupleT>(), (TupleT*)nullptr);
		
		typeLibrary->GetConversionGraph()->AddAlternativeConverter(Types::TypeId<TupleT>(), std::make_shared<AlternativeTupleConverter>(typeLibrary, cls.GetType()->GetAttributeCollection().GetAttribute<TupleAttribute>()));
		DefineTupleConstructor(cls);
		cls.Function("Get", [](const TupleT& tuple, size_t index)
		{
			return GetAt(tuple, index, std::integral_constant<size_t, std::tuple_size<TupleT>::value - 1>());
		}).AddSignature("index");
		cls.StaticGet("Size", std::tuple_size_v<TupleT>);
	}
	template<typename TupleT>
	void ReflectNonConstTuple(const TypeLibraries::TypeLibraryPointer& typeLibrary)
	{
		using namespace Classes;
		ReflectConstTuple<TupleT, TupleT>(typeLibrary);
		Class<TupleT, IgnoreAutoReflector, TupleT> cls(typeLibrary, "Tuple");
		cls.Constructor();		
		cls.Function("Set", [](TupleT& tuple, size_t index, const Objects::Object& object)
		{
			SetAt(tuple, index, std::integral_constant<size_t, std::tuple_size<TupleT>::value -1>(), object);
		}).AddSignature("index","object");
		cls.Operator(This.Const == This.Const);
		cls.Operator(This.Const != This.Const);
	}

	template<typename TupleT, typename NonConstTupleT>
	struct GenericTupleReflector {
	public:
		static void Reflect(const TypeLibraries::TypeLibraryPointer& typeLibrary)
		{
			ReflectConstTuple<TupleT, NonConstTupleT>(typeLibrary);
			ReflectNonConstTuple<NonConstTupleT>(typeLibrary);
		}
	};
	template<typename TupleT>
	struct GenericTupleReflector<TupleT,TupleT> {
	public:
		static void Reflect(const TypeLibraries::TypeLibraryPointer& typeLibrary)
		{
			ReflectNonConstTuple<TupleT>(typeLibrary);
		}
	};
}}}

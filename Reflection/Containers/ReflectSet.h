#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <set>
#include "Reflection/AutoReflect.h"
#include "Reflection/Classes/Class.h"
#include "Reflection/Attributes/ContainerAttribute.h"
#include "../Operators/ConditionallyAddOperator.h"
#include "TypeUtilities/HasOperator.h"
#define REFLECTOR_LIST_INCLUDED

namespace DNVS { namespace MoFa { namespace Reflection {
    template<typename T>
    struct AutoReflectImplemented<T, std::enable_if_t<TypeUtilities::IsSet<T>>>
    {
        static const bool value = true;
		static void Reflect(const DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer& typeLibrary)
		{
			using namespace DNVS::MoFa::Reflection::Classes;
			using Container = std::remove_const_t<std::remove_reference_t<T>>;
			if (typeLibrary->IsTypeReflected(typeid(Container)))
				return;
			typeLibrary->SetTypeReflected(typeid(Container));
			using value_type = typename Container::value_type;
			DNVS::MoFa::Reflection::AutoReflect<value_type>::Reflect(typeLibrary);

			Class<Container::const_iterator, DNVS::MoFa::Reflection::IgnoreAutoReflector> const_iterator(typeLibrary, "__ConstIterator__");
			const_iterator.Operator(++This);
			const_iterator.Operator(--This);
			const_iterator.Operator(This++);
			const_iterator.Operator(This--);
			const_iterator.Operator(This.Const == This.Const);
			const_iterator.Operator(This.Const != This.Const);
			const_iterator.Operator(*This.Const);

			Class<Container::iterator, Public<Container::const_iterator>, DNVS::MoFa::Reflection::IgnoreAutoReflector> iterator(typeLibrary, "__Iterator__");
			iterator.Operator(++This);
			iterator.Operator(--This);
			iterator.Operator(This++);
			iterator.Operator(This--);
			iterator.Operator(This.Const == This.Const);
			iterator.Operator(This.Const != This.Const);
			iterator.Operator(*This.Const);

			Class<Container, DNVS::MoFa::Reflection::IgnoreAutoReflector, std::unique_ptr<Container>> cls(typeLibrary, "__Set__");
			cls.AddAttribute<ContainerAttribute>(DNVS::MoFa::Reflection::Types::TypeId<value_type>(), true);
			cls.Constructor();
            Operators::ConditionalOperator<TypeUtilities::HasEqualTo<value_type>>::Operator(cls, This.Const == This.Const);
            Operators::ConditionalOperator<TypeUtilities::HasNotEqualTo<value_type>>::Operator(cls, This.Const != This.Const);
			cls.Function<Container::iterator()>("begin", &Container::begin);
			cls.Function<Container::const_iterator(), Const>("begin", &Container::begin);
			cls.Function<Container::iterator()>("end", &Container::end);
			cls.Function<Container::const_iterator(), Const>("end", &Container::end);
			cls.Function("size", &Container::size);
			cls.Function("empty", &Container::empty);
			cls.Function<std::pair<Container::iterator, bool>(const value_type&)>("Add", &Container::insert);
		};
	};

	template<typename T>
	struct AutoReflectImplemented<T, std::enable_if_t<TypeUtilities::IsMultiSet<T>>>
	{
		static const bool value = true;
		static void Reflect(const DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer& typeLibrary)
		{
			using namespace DNVS::MoFa::Reflection::Classes;
			using Container = std::remove_const_t<std::remove_reference_t<T>>;
			if (typeLibrary->IsTypeReflected(typeid(Container)))
				return;
			typeLibrary->SetTypeReflected(typeid(Container));
			using value_type = typename Container::value_type;
			DNVS::MoFa::Reflection::AutoReflect<value_type>::Reflect(typeLibrary);

			Class<Container::const_iterator, DNVS::MoFa::Reflection::IgnoreAutoReflector> const_iterator(typeLibrary, "__ConstIterator__");
			const_iterator.Operator(++This);
			const_iterator.Operator(--This);
			const_iterator.Operator(This++);
			const_iterator.Operator(This--);
			const_iterator.Operator(This.Const == This.Const);
			const_iterator.Operator(This.Const != This.Const);
			const_iterator.Operator(*This.Const);

			Class<Container::iterator, Public<Container::const_iterator>, DNVS::MoFa::Reflection::IgnoreAutoReflector> iterator(typeLibrary, "__Iterator__");
			iterator.Operator(++This);
			iterator.Operator(--This);
			iterator.Operator(This++);
			iterator.Operator(This--);
			iterator.Operator(This.Const == This.Const);
			iterator.Operator(This.Const != This.Const);
			iterator.Operator(*This.Const);

			Class<Container, DNVS::MoFa::Reflection::IgnoreAutoReflector, std::unique_ptr<Container>> cls(typeLibrary, "__MultiSet__");
			cls.AddAttribute<ContainerAttribute>(DNVS::MoFa::Reflection::Types::TypeId<value_type>(), true);
			cls.Constructor();
            Operators::ConditionalOperator<TypeUtilities::HasEqualTo<value_type>>::Operator(cls, This.Const == This.Const);
            Operators::ConditionalOperator<TypeUtilities::HasNotEqualTo<value_type>>::Operator(cls, This.Const != This.Const);
            cls.Function<Container::iterator()>("begin", &Container::begin);
			cls.Function<Container::const_iterator(), Const>("begin", &Container::begin);
			cls.Function<Container::iterator()>("end", &Container::end);
			cls.Function<Container::const_iterator(), Const>("end", &Container::end);
			cls.Function("size", &Container::size);
			cls.Function("empty", &Container::empty);
			cls.Function<std::pair<Container::iterator, bool>(const value_type&)>("Add", &Container::insert);
		};
	};
}}}

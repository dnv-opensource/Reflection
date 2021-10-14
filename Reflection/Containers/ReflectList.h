#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <list>
#include "Reflection/AutoReflect.h"
#include "Reflection/Classes/Class.h"
#include "Reflection/Attributes/ContainerAttribute.h"
#include "../Operators/ConditionallyAddOperator.h"
#include "TypeUtilities/HasOperator.h"

namespace DNVS { namespace MoFa { namespace Reflection {
    template<typename T>
    struct AutoReflectImplemented<T, std::enable_if_t<TypeUtilities::IsList<T>>> 
    {
        static const bool value = true;
		static void Reflect(const TypeLibraries::TypeLibraryPointer& typeLibrary)
		{
			using namespace Classes;
			using Container = std::remove_const_t<std::remove_reference_t<T>>;
			if (typeLibrary->IsTypeReflected(typeid(Container)))
				return;
			typeLibrary->SetTypeReflected(typeid(Container));
			using value_type = typename Container::value_type;
			AutoReflect<value_type>::Reflect(typeLibrary);

			Class<Container::const_iterator, IgnoreAutoReflector> const_iterator(typeLibrary, "__ConstIterator__");
			const_iterator.Operator(++This);
			const_iterator.Operator(--This);
			const_iterator.Operator(This++);
			const_iterator.Operator(This--);
			const_iterator.Operator(This.Const == This.Const);
			const_iterator.Operator(This.Const != This.Const);
			const_iterator.Operator(*This.Const);

			Class<Container::iterator, Public<Container::const_iterator>, IgnoreAutoReflector> iterator(typeLibrary, "__Iterator__");
			iterator.Operator(++This);
			iterator.Operator(--This);
			iterator.Operator(This++);
			iterator.Operator(This--);
			iterator.Operator(This.Const == This.Const);
			iterator.Operator(This.Const != This.Const);
			iterator.Operator(*This.Const);

			Class<Container, IgnoreAutoReflector, std::unique_ptr<Container>> cls(typeLibrary, "__List__");
			cls.AddAttribute<ContainerAttribute>(Types::TypeId<value_type>(), false);
			cls.Constructor();
            Operators::ConditionalOperator<TypeUtilities::HasEqualTo<value_type>>::Operator(cls, This.Const == This.Const);
            Operators::ConditionalOperator<TypeUtilities::HasNotEqualTo<value_type>>::Operator(cls, This.Const != This.Const);
            cls.Function<Container::iterator()>("begin", &Container::begin);
			cls.Function<Container::const_iterator(), Const>("begin", &Container::begin);
			cls.Function<Container::iterator()>("end", &Container::end);
			cls.Function<Container::const_iterator(), Const>("end", &Container::end);
			cls.Function("size", &Container::size);
			cls.Function("empty", &Container::empty);
			cls.Function<void(const value_type&)>("Add", &Container::push_back);
		}
    };
}}}

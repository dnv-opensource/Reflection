#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"
#include "Reflect.h"
#include <list>
#include <set>
#include <vector>
#include <map>
#include "TypeUtilities/IsList.h"
#include "TypeUtilities/IsVector.h"
#include "TypeUtilities/IsSet.h"
#include "TypeUtilities/IsMap.h"
namespace boost {
    template<typename T>
    class optional;
}

namespace DNVS { namespace MoFa { namespace Reflection {
    template<typename T, typename EnableIf = void>
    class AutoReflect
    {
    public:
        static const bool RequiresReflection = false;
        static void Reflect(const TypeLibraries::TypeLibraryPointer& typeLibrary)
        {}
    };
    template<typename T, typename EnableIf = void>
    struct AutoReflectImplemented 
    {
        static const bool value = false;
    };
    struct SignatureVisitor
    {
    public:
        SignatureVisitor(const TypeLibraries::TypeLibraryPointer& typeLibrary)
            : m_typeLibrary(typeLibrary) {}
        template<typename T, size_t Index>
        void Visit() {
            AutoReflect<T>::Reflect(m_typeLibrary);
        }
    private:
        const TypeLibraries::TypeLibraryPointer& m_typeLibrary;
    };

    struct AutoReflectorBase {};

    struct AutoReflector : public AutoReflectorBase
    {
        //Goes through all arguments of the function and calls auto reflect on all of them. Including the return type.
        template<typename Signature>
        static void AutoReflectSignature(const TypeLibraries::TypeLibraryPointer& typeLibrary)
        {
            TypeUtilities::FunctionTraits<Signature>::Visit(SignatureVisitor(typeLibrary));
            AutoReflect<typename TypeUtilities::FunctionTraits<Signature>::ReturnType>::Reflect(typeLibrary);
        }
        template<typename BaseT, typename T>
        static void ReflectBase(const TypeLibraries::TypeLibraryPointer& typeLibrary, const std::allocator<T>& allocator)
        {
            Reflection::Reflect<BaseT>(typeLibrary);
        }
        template<typename BaseT, typename T>
        static void ReflectBase(const TypeLibraries::TypeLibraryPointer& typeLibrary, const T& allocator)
        {
            Reflection::Reflect<BaseT>(typeLibrary, allocator);
        }
    };
    struct IgnoreAutoReflector : public AutoReflectorBase
    {
        template<typename Signature>
        static void AutoReflectSignature(const TypeLibraries::TypeLibraryPointer& typeLibrary)
        {
        }
        template<typename BaseT, typename T>
        static void ReflectBase(const TypeLibraries::TypeLibraryPointer& typeLibrary, const T& allocator)
        {
        }
    };
    
    template<typename T>
    class AutoReflect<T, std::enable_if_t<TypeUtilities::IsList<T>>>
    {
    public:
        static const bool RequiresReflection = true;
        static void Reflect(const TypeLibraries::TypeLibraryPointer& typeLibrary)
        {            
            static_assert(AutoReflectImplemented<T>::value, "Missing include: \n#include \"Reflection/Containers/ReflectList.h\"\n");
			AutoReflectImplemented<T>::Reflect(typeLibrary);
        }
    };

    template<typename T>
    class AutoReflect<T, std::enable_if_t<TypeUtilities::IsVector<T>>>
    {
    public:
        static const bool RequiresReflection = true;
        static void Reflect(const TypeLibraries::TypeLibraryPointer& typeLibrary)
        {
            static_assert(AutoReflectImplemented<T>::value, "Missing include: \n#include \"Reflection/Containers/ReflectVector.h\"\n");
			AutoReflectImplemented<T>::Reflect(typeLibrary);
        }
    };

	template<typename T>
	class AutoReflect<T, std::enable_if_t<TypeUtilities::IsSet<T>>>
	{
	public:
		static const bool RequiresReflection = true;
		static void Reflect(const TypeLibraries::TypeLibraryPointer& typeLibrary)
		{
			static_assert(AutoReflectImplemented<T>::value, "Missing include: \n#include \"Reflection/Containers/ReflectSet.h\"\n");
			AutoReflectImplemented<T>::Reflect(typeLibrary);
		}
	};

	template<typename T>
	class AutoReflect<T, std::enable_if_t<TypeUtilities::IsMultiSet<T>>>
	{
	public:
		static const bool RequiresReflection = true;
		static void Reflect(const TypeLibraries::TypeLibraryPointer& typeLibrary)
		{
			static_assert(AutoReflectImplemented<T>::value, "Missing include: \n#include \"Reflection/Containers/ReflectSet.h\"\n");
			AutoReflectImplemented<T>::Reflect(typeLibrary);
		}
	}; 

	template<typename T>
	class AutoReflect<T, std::enable_if_t<TypeUtilities::IsMap<T>>>
	{
	public:
		static const bool RequiresReflection = true;
		static void Reflect(const TypeLibraries::TypeLibraryPointer& typeLibrary)
		{
			static_assert(AutoReflectImplemented<T>::value, "Missing include: \n#include \"Reflection/Containers/ReflectMap.h\"\n");
			AutoReflectImplemented<T>::Reflect(typeLibrary);
		}
	};

	template<typename T>
	class AutoReflect<T, std::enable_if_t<TypeUtilities::IsMultiMap<T>>>
	{
	public:
		static const bool RequiresReflection = true;
		static void Reflect(const TypeLibraries::TypeLibraryPointer& typeLibrary)
		{
			static_assert(AutoReflectImplemented<T>::value, "Missing include: \n#include \"Reflection/Containers/ReflectMap.h\"\n");
			AutoReflectImplemented<T>::Reflect(typeLibrary);
		}
	};
	
	template<typename FirstT, typename SecondT>
	class AutoReflect<const std::pair<FirstT, SecondT>&, void>
	{
	public:
		static const bool RequiresReflection = true;
		static void Reflect(const TypeLibraries::TypeLibraryPointer& typeLibrary)
		{
			static_assert(AutoReflectImplemented<std::pair<FirstT, SecondT>>::value, "Missing include: \n#include \"Reflection/Containers/ReflectPair.h\"\n");
			Reflection::Reflect<std::pair<FirstT, SecondT>>(typeLibrary);
		}
	};

	template<typename FirstT, typename SecondT>
	class AutoReflect<std::pair<FirstT, SecondT>, void>
	{
	public:
		static const bool RequiresReflection = true;
		static void Reflect(const TypeLibraries::TypeLibraryPointer& typeLibrary)
		{
			static_assert(AutoReflectImplemented<std::pair<FirstT, SecondT>>::value, "Missing include: \n#include \"Reflection/Containers/ReflectPair.h\"\n");
			Reflection::Reflect<std::pair<FirstT, SecondT>>(typeLibrary);
		}
	};

	template<typename... Args>
	class AutoReflect<const std::tuple<Args...>&, void>
	{
	public:
		static const bool RequiresReflection = true;
		static void Reflect(const TypeLibraries::TypeLibraryPointer& typeLibrary)
		{
			static_assert(AutoReflectImplemented<std::tuple<Args...>>::value, "Missing include: \n#include \"Reflection/Containers/ReflectTuple.h\"\n");
			Reflection::Reflect<std::tuple<Args...>>(typeLibrary);
		}
	};

	template<typename... Args>
	class AutoReflect<std::tuple<Args...>, void>
	{
	public:
		static const bool RequiresReflection = true;
		static void Reflect(const TypeLibraries::TypeLibraryPointer& typeLibrary)
		{
			static_assert(AutoReflectImplemented<std::tuple<Args...>>::value, "Missing include: \n#include \"Reflection/Containers/ReflectTuple.h\"\n");
			Reflection::Reflect<std::tuple<Args...>>(typeLibrary);
		}
	};
    template<typename T>
    class AutoReflect<boost::optional<T>, void>
    {
    public:
        static const bool RequiresReflection = true;
        static void Reflect(const TypeLibraries::TypeLibraryPointer& typeLibrary)
        {
            static_assert(AutoReflectImplemented<boost::optional<T>>::value, "Missing include: \n#include \"Reflection/Utilities/ReflectOptional.h\"\n");
            Reflection::Reflect<boost::optional<T>>(typeLibrary);
        }
    };

	template<typename... Args>
	class AutoReflectVariadic
	{
	};
	template<typename T, typename... Args>
	class AutoReflectVariadic<T, Args...>
	{
	public:
		static void Reflect(const TypeLibraries::TypeLibraryPointer& typeLibrary)
		{
			AutoReflect<T>::Reflect(typeLibrary);
			AutoReflectVariadic<Args...>::Reflect(typeLibrary);
		}
	};
	template<>
	class AutoReflectVariadic<>
	{
	public:
		static void Reflect(const TypeLibraries::TypeLibraryPointer& typeLibrary)
		{
		}
	};

}}}

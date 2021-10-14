#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <memory>
#ifndef GET_POINTER_FOR_STD_SHARED_PTR
#define GET_POINTER_FOR_STD_SHARED_PTR
namespace std {
    template<typename T>
    T* get_pointer(std::unique_ptr<T> const& p) { return p.get(); }
    template<typename T>
    T * get_pointer(std::shared_ptr<T> const& p) { return p.get(); }
}
namespace boost {
    template<class T>
    class optional;
}
#endif 

namespace DNVS { namespace MoFa {namespace TypeUtilities {
    template<typename T>
    struct is_smart_pointer {
        //We need this complex setup in order to ensure that the function compiles even if get_pointer is not implemented for a given type.
        //In this case, this overload is excluded using SFINAE rules.
        template<typename U>
        static auto test(const U& u)->decltype(get_pointer(u));
        static const T& test(...);

        static constexpr bool value = !std::is_same_v<decltype(test(std::declval<T>())), const T&>;
        typedef bool value_type;
        typedef std::integral_constant<value_type, value> type;
    };

    template<>
    struct is_smart_pointer<void> : std::integral_constant<bool, false> {};

    template<typename T>
    struct is_smart_pointer<T&> : std::integral_constant<bool, false> {};

    template<typename T>
    struct is_smart_pointer<T*> : std::integral_constant<bool, false> {};

    template<typename T, size_t Size>
    struct is_smart_pointer<T[Size]> : std::integral_constant<bool, false> {};

    template<typename T>
    struct is_smart_pointer<T[]> : std::integral_constant<bool, false> {};

    template<typename T>
    struct is_smart_pointer<boost::optional<T>> : std::integral_constant<bool, false> {};

    template<class _Ty>
    constexpr bool is_smart_pointer_v = is_smart_pointer<_Ty>::value;

    template<typename T>
    struct is_aliasing_smart_pointer {
        //Use SFINAE to test whether T has a constructor taking a smart pointer and a pointer.
        template<typename U>
        static auto test(const U& u)->decltype(T(u, get_pointer(u)));
        struct Dummy {};
        static Dummy test(...);

        static const bool value = !std::is_same<decltype(test(std::declval<T>())), Dummy>::value;
        typedef bool value_type;
        typedef std::integral_constant<value_type, value> type;
    };

    template<>
    struct is_aliasing_smart_pointer<void> : std::integral_constant<bool, false> {};

    template<typename T>
    struct is_aliasing_smart_pointer<T&> : std::integral_constant<bool, false> {};

    template<typename T>
    struct is_aliasing_smart_pointer<T*> : std::integral_constant<bool, false> {};

    template<typename T, size_t Size>
    struct is_aliasing_smart_pointer<T[Size]> : std::integral_constant<bool, false> {};

    template<typename T>
    struct is_aliasing_smart_pointer<T[]> : std::integral_constant<bool, false> {};

	template<typename T>
	struct is_aliasing_smart_pointer<boost::optional<T>> : std::integral_constant<bool, false> {};

    template<class _Ty>
    constexpr bool is_aliasing_smart_pointer_v = is_aliasing_smart_pointer<_Ty>::value;

}}}
#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <type_traits>

namespace DNVS { namespace MoFa { namespace TypeUtilities {
    /************************************************************************/
    /* IsConstructible checks whether a class C can be constructed          */
    /* with the input argument types given.                                 */
    /* Example:                                                             */
    /* - IsConstructible<std::string, const char*>::value is true           */
    /* - IsConstructible<std::string, double, double>::value is false       */
    /* We can use IsConstructible compile time to select which function to  */
    /* run.                                                                 */
    /************************************************************************/
    template <typename T> // if std::declval is not supported by VS10
    typename std::add_rvalue_reference<T>::type DeclVal();

    template<typename C>
    struct IsConstructible0
    {
        //If T is not constructible with no arguments, this overload will be removed using SFINAE.
        template <class T>
        static decltype(new T()) Test(int = 0);

        template <class T>
        static void Test(...);
        
        //Select one of the two Test overloads above.
        typedef std::is_same<decltype(Test<C>(0)), C*> type;
    };
    

    template<typename C, typename A0>
    struct IsConstructible1
    {
        template <class T>
        static decltype(new T(DeclVal<A0>())) Test(int = 0);

        template <class T>
        static void Test(...);
        typedef std::is_same<decltype(Test<C>(0)), C*> type;
    };

    template<typename C, typename A0, typename A1>
    struct IsConstructible2
    {
        template <class T>
        static decltype(new T(DeclVal<A0>(), DeclVal<A1>())) Test(int = 0);

        template <class T>
        static void Test(...);
        typedef std::is_same<decltype(Test<C>(0)), C*> type;
    };

    template<typename C, typename A0, typename A1, typename A2>
    struct IsConstructible3
    {
        template <class T>
        static decltype(new T(DeclVal<A0>(), DeclVal<A1>(), DeclVal<A2>())) Test(int = 0);

        template <class T>
        static void Test(...);
        typedef std::is_same<decltype(Test<C>(0)), C*> type;
    };

    template<typename C, typename A0 = void, typename A1 = void, typename A2 = void>
    struct IsConstructible 
        :   IsConstructible3<C, A0, A1, A2>::type
    {

    };
    template<typename C>
    struct IsConstructible<C, void, void, void>
        :   IsConstructible0<C>::type
    {

    };

    template<typename C, typename A0>
    struct IsConstructible<C, A0, void, void>
        :   IsConstructible1<C, A0>::type
    {

    };

    template<typename C, typename A0, typename A1>
    struct IsConstructible<C, A0, A1, void>
        :   IsConstructible2<C, A0, A1>::type
    {

    };

}}}
#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypeList.h"

namespace DNVS {namespace MoFa {namespace TypeUtilities {
    /*
    InvokeImpl will convert a container of arguments to a converted list of arguments and invoke callback with these arguments.
    Let us look at an example of calling the function:
    double MyFunction(double a, int b, bool c);
    args = a vector of variants (objects that can be converted to any C++ type)
    callback == MyFunction.
    typeList = TypeList<double,int,bool> (the arguments for MyFunction)
    convertedArgs is empty

    Let us write out the call stack at every step in the process:
    1. InvokeImpl(args, MyFunction, TypeList<double,int,bool>); //We haven't converted any arguments, so convertedArgs contains nothing.
       - This matches overload 2.
       - T == [double]
       - Args... == [int, bool]
       - ConvertedArgs&&... == []         
       - sizeof...(ConvertedArgs) == 0 (number of arguments converted)
         This function is recursive. It calls itself, but will convert one argument at a time:
       - args.Get<T,sizeof...(ConvertedArgs)>() == args.Get<double,0>() will convert the first value in args to double
         So the arguments for InvokeImpl will be:
    2. InvokeImpl(args, MyFunction, TypeList<int,bool>, double convertedArgs0);
       - Note that the first argument in TypeList has been removed (this is according to the implementation in Overload 2, 
         where we call the first argument in the TypeList T, and convert it.
         This again matches overload 2.
       - T == [int]
       - Args... = [bool]
       - ConvertedArgs&&... == [double]
       - sizeof...(ConvertedArgs) == 1
       - args.Get<T,sizeof...(ConvertedArgs)>() == args.Get<int,1>()
         Again, we call InvokeImpl:
    3. InvokeImpl(args, MyFunction, TypeList<bool>, double convertedArgs0, int convertedArgs1);
       - Again, we choose overload 2.
       - T == [bool]
       - Args... = []
       - ConvertedArgs&&... == [double, int]
       - sizeof...(ConvertedArgs) == 2
       - args.Get<T,sizeof...(ConvertedArgs)>() == args.Get<bool,2>()
       Again we call InvokeImpl:
    4. InvokeImpl(args, MyFunction, TypeList<>, double convertedArgs0, int convertedArgs1, bool convertedArgs2);
       - This time, InvokeImpl does NOT match overload 2, since TypeList contains no elements.
         Instead, it matches overload 1, which is:
         return callback(std::forward<ConvertedArgs>(convertedArgs)...);
         This translates to:
         return MyFunction(convertedArgs0, convertedArgs1, convertedArgs2); 
         and our mission is done.
    */

    //Overload 1, all arguments converted
    template<typename ArgumentsT, typename CallbackT, typename... ConvertedArgs>
    decltype(auto) InvokeImpl(ArgumentsT& args, const CallbackT& callback, TypeList<> typeList, ConvertedArgs&&... convertedArgs)
    {
        return callback(std::forward<ConvertedArgs>(convertedArgs)...);
    }

    //Overload 2, still more arguments to convert.
    template<typename ArgumentsT, typename CallbackT, typename T, typename... Args, typename... ConvertedArgs>
    decltype(auto) InvokeImpl(ArgumentsT& args, const CallbackT& callback, TypeList<T, Args...> typeList, ConvertedArgs&&... convertedArgs)
    {
        return InvokeImpl(args, callback, TypeList<Args...>(), std::forward<ConvertedArgs>(convertedArgs)..., args.Get<T, sizeof...(ConvertedArgs)>());
    }
}}}
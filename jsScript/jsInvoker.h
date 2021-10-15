#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "jsConversions.h"
#include "jsTypeLibrary.h"
#include "Reflection/Types/DecoratedTypeInfo.h"

template<typename... Args>
struct jsTypeList {};
struct ParamVisitor {
    ParamVisitor(std::string& result, const jsTypeLibrary& typeLibrary, int index)
        : m_typeLibrary(typeLibrary)
        , m_index(index)
        , m_result(result)
    {}
    template<typename Arg, int Index>
    void Visit()
    {
        if (m_index == Index)
            m_result = m_typeLibrary.GetTypeName(typeid(Arg));
    }
private:
    std::string& m_result;
    const jsTypeLibrary& m_typeLibrary;
    int m_index;
};

struct ParamVisitorType {
    ParamVisitorType(DNVS::MoFa::Reflection::Types::DecoratedTypeInfo& result, const jsTypeLibrary& typeLibrary, int index)
        : m_typeLibrary(typeLibrary)
        , m_index(index)
        , m_result(result)
    {}
    template<typename Arg, int Index>
    void Visit()
    {
        if (m_index == Index)
            m_result = DNVS::MoFa::Reflection::Types::TypeId<Arg>();
    }
private:
    DNVS::MoFa::Reflection::Types::DecoratedTypeInfo& m_result;
    const jsTypeLibrary& m_typeLibrary;
    int m_index;
};

struct IsVariadicVisitor {
    IsVariadicVisitor(bool& result, const jsTypeLibrary& typeLibrary, int index)
        : m_typeLibrary(typeLibrary)
        , m_index(index)
        , m_result(result)
    {}
    template<typename Arg, int Index>
    void Visit()
    {
        if (m_index == Index)
            m_result = DNVS::MoFa::Reflection::Types::TypeId<Arg>().GetTypeInfo() == typeid(jsValue::Params);
    }
private:
    bool& m_result;
    const jsTypeLibrary& m_typeLibrary;
    int m_index;
};

struct jsInvoker {
public:
    template <typename R, typename T, typename... Args>
    static jsValue* call(R(T::*pmf)(Args...), const jsValue::Params& params)
    {
        VerifyArgumentCount(0, jsTypeList<Args...>(), params.size() - 1);
        int iarg = 0;
        try {
            auto callback = [pmf](T& target, Args... arguments) -> jsValue*
            {
                if (jsStack::stack()->dummyMode()) {
                    if (jsValue* value = jsStack::stack()->getDummy(typeid(R), typeid(T)))
                        return value;
                }
                return toJScript((target.*pmf)(std::forward<Args>(arguments)...));
            };
            return InvokeImpl(params, iarg, callback, jsTypeList<T&, Args...>());
        }
        catch (bad_type_conversion& e) {
            if (iarg > sizeof...(Args)) throw e;
            else throw invalid_argument(iarg, e);
        }
    }
    template <typename R, typename T, typename... Args>
    static jsValue* call(R(T::*pmf)(Args...) const, const jsValue::Params& params)
    {
        VerifyArgumentCount(0, jsTypeList<Args...>(), params.size() - 1);
        int iarg = 0;
        try {
            auto callback = [pmf](T& target, Args... arguments) -> jsValue*
            {
                return toJScript((target.*pmf)(std::forward<Args>(arguments)...));
            };
            return InvokeImpl(params, iarg, callback, jsTypeList<T&, Args...>());
        }
        catch (bad_type_conversion& e) {
            if (iarg > sizeof...(Args)) throw e;
            else throw invalid_argument(iarg, e);
        }
    }
    template <typename R, typename... Args>
    static jsValue* call(R(*pf)(Args...), const jsValue::Params& params) {
        VerifyArgumentCount(0, jsTypeList<Args...>(), params.size() - 1);
        int iarg = 1;
        try {
            auto callback = [pf](Args... arguments) -> jsValue*
            {
                if (jsStack::stack()->dummyMode()) {
                    if (jsValue* value = jsStack::stack()->getDummy(typeid(R)))
                        return value;
                }
                return toJScript(pf(std::forward<Args>(arguments)...));
            };
            return InvokeImpl(params, iarg, callback, jsTypeList<Args...>());
        }
        catch (bad_type_conversion& e) {
            if (iarg > sizeof...(Args)) throw e;
            else throw invalid_argument(iarg, e);
        }
    }
    template <typename T, typename... Args>
    static jsValue* call(void(T::*pmf)(Args...), const jsValue::Params& params)
    {
        VerifyArgumentCount(0, jsTypeList<Args...>(), params.size() - 1);
        int iarg = 0;
        try {
            auto callback = [pmf](T& target, Args... arguments) -> jsValue*
            {
                if (!jsStack::stack()->dummyMode())
                    (target.*pmf)(std::forward<Args>(arguments)...);
                return nullptr;
            };
            return InvokeImpl(params, iarg, callback, jsTypeList<T&, Args...>());
        }
        catch (bad_type_conversion& e) {
            if (iarg > sizeof...(Args)) throw e;
            else throw invalid_argument(iarg, e);
        }
    }
    template <typename T, typename... Args>
    static jsValue* call(void(T::*pmf)(Args...) const, const jsValue::Params& params)
    {
        VerifyArgumentCount(0, jsTypeList<Args...>(), params.size() - 1);
        int iarg = 0;
        try {
            auto callback = [pmf](T& target, Args... arguments) -> jsValue*
            {
                (target.*pmf)(std::forward<Args>(arguments)...);
                return nullptr;
            };
            return InvokeImpl(params, iarg, callback, jsTypeList<T&, Args...>());
        }
        catch (bad_type_conversion& e) {
            if (iarg > sizeof...(Args)) throw e;
            else throw invalid_argument(iarg, e);
        }
    }
    template <typename... Args>
    static jsValue* call(void(*pf)(Args...), const jsValue::Params& params) {
        VerifyArgumentCount(0, jsTypeList<Args...>(), params.size() - 1);
        int iarg = 1;
        try {
            auto callback = [pf](Args... arguments) -> jsValue*
            {
                if (!jsStack::stack()->dummyMode())
                    pf(std::forward<Args>(arguments)...);
                return nullptr;
            };
            return InvokeImpl(params, iarg, callback, jsTypeList<Args...>());
        }
        catch (bad_type_conversion& e) {
            if (iarg > sizeof...(Args)) throw e;
            else throw invalid_argument(iarg, e);
        }
    }
    template <typename R, typename... Args>
    static jsValue* construct(jsType<R(Args...)>, const jsValue::Params& params) {
        VerifyArgumentCount(0, jsTypeList<Args...>(), params.size() - 1);
        int iarg = 1;
        try {
            auto callback = [](Args... arguments) -> jsValue*
            {
                if (jsStack::stack()->dummyMode())
                {
                    if (jsValue* value = jsStack::stack()->getDummy(typeid(R))) 
                        return value;
                }
                return new R(arguments...);
            };
            return InvokeImpl(params, iarg, callback, jsTypeList<Args...>());
        }
        catch (bad_type_conversion& e) {
            if (iarg > sizeof...(Args)) throw e;
            else throw invalid_argument(iarg, e);
        }
    }
private:
    static void VerifyArgumentCount(size_t count, jsTypeList<>, size_t expectedCount)
    {
        if (expectedCount != count) throw incorrect_number_of_arguments(expectedCount);
    }
    static void VerifyArgumentCount(size_t count, jsTypeList<const jsValue::Params&>, size_t expectedCount)
    {
        if (expectedCount < count) throw incorrect_number_of_arguments(expectedCount);
    }
    template<typename T, typename... Args>
    static void VerifyArgumentCount(size_t count, jsTypeList<T, Args...>, size_t expectedCount)
    {
        VerifyArgumentCount(count + 1, jsTypeList<Args...>(), expectedCount);
    }

    template<typename CallbackT, typename... ConvertedArgs>
    static jsValue* InvokeImpl(const jsValue::Params& params, int& iarg, const CallbackT& callback, jsTypeList<> typeList, ConvertedArgs&&... args)
    {
        return callback(std::forward<ConvertedArgs>(args)...);
    }

    template<typename CallbackT, typename... ConvertedArgs>
    static jsValue* InvokeImpl(const jsValue::Params& params, int& iarg, const CallbackT& callback, jsTypeList<const jsValue::Params&> typeList, ConvertedArgs&&... args)
    {
        jsValue::Params vararg;
        paramsFromJScript(params, iarg, vararg, jsType<const jsValue::Params&>());
        return InvokeImpl(params, iarg, callback, jsTypeList<>(), std::forward<ConvertedArgs>(args)..., vararg);
    }

    template<typename CallbackT, typename T, typename... Args, typename... ConvertedArgs>
    static jsValue* InvokeImpl(const jsValue::Params& params, int& iarg, const CallbackT& callback, jsTypeList<T, Args...> typeList, ConvertedArgs&&... args)
    {
        T currentArg = fromJScript(params[iarg], jsType<T>());
        ++iarg;
        return InvokeImpl(params, iarg, callback, jsTypeList<Args...>(), std::forward<ConvertedArgs>(args)..., std::forward<T>(currentArg));
    }
};
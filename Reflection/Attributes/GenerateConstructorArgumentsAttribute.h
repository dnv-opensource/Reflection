#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include "TypeUtilities/FunctionTraitsVariadic.h"
#include "Reflection/Config.h"
#include "Reflection/Objects/Object.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    template<typename T>
    class GenerateConstructorArgumentsCallback;

    template<typename T, typename... Arguments>
    class GenerateConstructorArgumentsCallback<std::tuple<T, Arguments...>>
    {
    public:
        typedef std::function<bool(T, Arguments... arguments)> InputCallback;        
        GenerateConstructorArgumentsCallback(const InputCallback& callback) : m_inputCallback(callback) {}
        bool operator()(const Objects::Object& objectToInspect, std::vector<Variants::Variant>& arguments)
        {
            if (objectToInspect.IsConvertibleTo<T>())
            {
                std::tuple<std::remove_reference_t<Arguments>...> argumentTuple;                
                if (!InvokeFunction(objectToInspect, argumentTuple))
                    return false;
                PushBackTuple(arguments, argumentTuple);
                return true;
            }
            return false;
        }
    private:
        template<typename... Args,std::size_t... I>
        bool InvokeFunctionImpl(const Objects::Object& objectToInspect, std::tuple<Args...>& args, std::index_sequence<I...>)
        {
            return m_inputCallback(objectToInspect.As<T>(), std::get<I>(args)...);
        }
        template<typename... Args>
        bool InvokeFunction(const Objects::Object& objectToInspect, std::tuple<Args...>& args)
        {
            return InvokeFunctionImpl(objectToInspect, args, std::index_sequence_for<Args...>{});
        }

        void PushBackParameterPack(std::vector<Variants::Variant>& variants)
        {}

        template<typename Head, typename... Args>
        void PushBackParameterPack(std::vector<Variants::Variant>& variants, Head head, Args... args)
        {
            variants.push_back(Variants::VariantService::ReflectType<Head>(head));
            PushBackParameterPack(variants, std::forward<Args>(args)...);
        }

        template<typename... Args, std::size_t... I>
        void PushBackTupleWithIndices(std::vector<Variants::Variant>& variants, const std::tuple<Args...>& args, std::index_sequence<I...>)
        {
            PushBackParameterPack(variants, std::get<I>(args)...);
        }

        template<typename... Args>
        void PushBackTuple(std::vector<Variants::Variant>& variants, const std::tuple<Args...>& args)
        {
            PushBackTupleWithIndices(variants, args, std::index_sequence_for<Args...>{});
        }
        InputCallback m_inputCallback;
    };

    /*
    Allows registering a function that will inspect an object and return the arguments used to construct that object for a given constructor.
    Example:
    //plane3d has two constructors.
    plane3d(pos3d pos, vec3d normal);
    plane3d(pos3d p1, pos3d p2, pos3d p3);
    //In addition it has a couple of static methods for creating special planes.
    plane3d XPlane3d(double x);
    plane3d YPlane3d(double y);
    plane3d ZPlane3d(double z);
    
    In some situations, we want to, given a plane3d, deduce the correct constructor arguments for for example XPlane3d.
    This we can do using the GenerateConstructorArgumentsAttribute.
    First we need a method that will, given a plane3d, try to deduce this information:
    bool DeduceXPlaneConstructorArgument(const plane3d& plane, double& x)
    {
        if(!isEqual(plane.normal().dot(vec3d(1,0,0)),1.0)) 
            return false;
        pos3d p;
        plane.ComputeOnePoint(p);
        x = p.x();
        return true;
    }
    Then we need to register this information:
    Class<plane3d> cls(...);
    cls.NamedConstructor("XPlane3d", XPlane3d)
       .AddAttribute<GenerateConstructorArgumentsAttribute>(DeduceXPlaneConstructorArgument);
       
    Now, autogenerated GUI will automatically select the best constructor for Plane3d and populate it with data.
    */
    class GenerateConstructorArgumentsAttribute {
    public:
        typedef std::function<bool(const Objects::Object&, std::vector<Variants::Variant>&)> Callback;
        template<typename CallbackT>
        GenerateConstructorArgumentsAttribute(const CallbackT& callback)
        {
            #define CONSTRUCTOR_ARGUMENTS_ATTRIBUTE_ERROR_MESSAGE "The function signature must look like this: bool (const T&, A0&, A1&, <etc>), where T is the type you want to make constructor arguments for and A0 and A1 etc. are the arguments for the constructor."
            static_assert(TypeUtilities::FunctionTraits<CallbackT>::Arity >= 1, CONSTRUCTOR_ARGUMENTS_ATTRIBUTE_ERROR_MESSAGE);
            static_assert(std::is_same_v<typename TypeUtilities::FunctionTraits<CallbackT>::ReturnType, bool>, CONSTRUCTOR_ARGUMENTS_ATTRIBUTE_ERROR_MESSAGE);
            #undef CONSTRUCTOR_ARGUMENTS_ATTRIBUTE_ERROR_MESSAGE
            m_callback = GenerateConstructorArgumentsCallback<typename TypeUtilities::FunctionTraitsVariadic<CallbackT>::Arguments>(callback);
        }
        bool GenerateConstructorArguments(const Objects::Object& objectToInspect, std::vector<Variants::Variant>& arguments) const
        {
            return m_callback(objectToInspect, arguments);
        }
    private:
        Callback m_callback;
    };
}}}}
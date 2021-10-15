#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <jsScript/jsScript_config.h>

#include <jsScript/jsProperty.h>
#include <jsScript/jsFunction.h>
#include <jsScript/jsConstructor.h>
#include <typeinfo>

#include <Reflection/Members/Modifier.h>
#include <Reflection/Types/DecoratedTypeInfo.h>

#include "jsReflectionHelpers.h"
#include "jsEnum.h"
#include "jsTypeLibrary.h"
#include "jsVTBL.h"
#include "Reflection/TypeLibraries/ITypeLibrary.h"

class jsVTBL;

class JSSCRIPT_IMPORT_EXPORT jsBaseClass
{
public:
    jsBaseClass(jsTypeLibrary& typeLibrary, const type_info& info, const type_info& parent, const std::string& name);
    jsBaseClass(const jsBaseClass& cls);
    jsBaseClass& operator=(const jsBaseClass& cls);
    virtual ~jsBaseClass();
    jsValue* add(const std::string& identifier, jsValue* value);
    void addGlobal(const std::string& identifier, jsValue* value);
    jsValue* lookup(const std::string& identifier);
    void unite(const type_info& type);
    jsTypeInfo* typeInfo();
    bool reinit();
    void enableScripting(bool value);
    bool isScriptingEnabled() const;
    jsValue* getDummyValue();
    jsTypeLibrary& GetTypeLibrary() { return m_typeLibrary; }
private:
    jsVTBL* m_vtbl;
    jsTypeLibrary& m_typeLibrary;
    bool m_reinit;
    bool m_exposeToScripting;
};

/// Define new scripting objects using this class
/**
   Use jsClass to define the functions of an object derived from jsScriptable.
   Example:

   <PRE>
   class Point : public jsScriptable {
   public:
      Point(double x,double y,double z);
      double get_x();
      void put_x(double x);
      void set(double x,double y,double z);

      //Initialisation function:
      static void init(jsTypeLibrary& typeLibrary);
   private:
      ...
   }
   </PRE>
   The implementation of init should look something like this:
   <PRE>
   void Point::init(jsTypeLibrary& typeLibrary) {
      // Define the class type information
      jsClass<Point> cls(typeLibrary);

      // Define a constructor with 3 double parameters
      cls.Constructor<double,double,double>();

      // Define the function void set(double,double,double) as a scriptable function named "set"
      cls.Function("set",set);

      // Define the property x:
      cls.Set("x",put_x);
      cls.Get("x",get_x);

      // "Point" can now be used as a scripting object.
   }
   </PRE>
*/

template <class P, class TypeLibraryT>
void CallInit(jsTypeLibrary& typeLibrary, void(TypeLibraryT&))
{
    TypeLibraryT* concreteTypelibrary = dynamic_cast<TypeLibraryT*>(&typeLibrary);

    if (concreteTypelibrary)
    {
        P::init(*concreteTypelibrary);
    }
}

template <class P>
void CallInit(jsTypeLibrary& typeLibrary, ...)
{
}

template <class C, class P>
class RegisterBaseClass
{
public:
    RegisterBaseClass()
    {
    }

    RegisterBaseClass(jsTypeLibrary& typeLibrary)
    {
        DNVS::MoFa::Reflection::AddClassWithBase<C, P>(typeLibrary.GetReflectionTypeLibrary()->GetConversionGraph());
        CallInit<P>(typeLibrary, P::init);
    }
};

template <class C>
class RegisterBaseClass<C, C>
{
public:
    RegisterBaseClass()
    {
    }

    RegisterBaseClass(jsTypeLibrary& typeLibrary)
    {
        DNVS::MoFa::Reflection::AddClassWithBase<C, jsValue>(typeLibrary.GetReflectionTypeLibrary()->GetConversionGraph());
    }
};

template <class C, class P = C>
class jsTClass : public RegisterBaseClass<C, P>, public jsBaseClass
{
public:
    jsTClass(jsTypeLibrary& typeLibrary, const std::string& name = "")
        : RegisterBaseClass<C, P>(typeLibrary), jsBaseClass(typeLibrary, typeid(C), typeid(P), name)
    {
        DNVS::MoFa::Reflection::RegisterConversionToJsValueIfNot<std::is_abstract<C>::value>::Apply<C>(
            typeLibrary.GetReflectionTypeLibrary()->GetConversionGraph());
    }

    jsTClass(const jsTClass& cls) : jsBaseClass(cls)
    {
    }

    virtual ~jsTClass()
    {
    }

    // Define constructor by providing a function

    template <class Signature>
    jsTypeInfo& Constructor(Signature function, bool isUserConversion = true)
    {
        std::string name = GetTypeLibrary().GetTypeName(typeid(C));

        if (isScriptingEnabled())
        {
            DNVS::MoFa::Reflection::AddConstructorFunctionConditionalIfArityOne<C, Signature>::Apply(
                function, GetTypeLibrary().GetReflectionTypeLibrary()->GetConversionGraph(), isUserConversion);

            auto constructorObject = new jsTConstructorFunction<Signature>(GetTypeLibrary(), function, name);

            add(name, constructorObject);
            addGlobal(name, constructorObject);

            return *constructorObject->typeInfo();
        }
        else
        {
            GetTypeLibrary().addReservedWord(name);

            return *getDummyValue()->typeInfo();
        }
    }

    template <class Signature>
    jsTypeInfo& Constructor(const char* name, Signature function)
    {
        return Constructor(std::string(name), function);
    }

    template <class Signature>
    jsTypeInfo& Constructor(const std::string& name, Signature function)
    {
        if (isScriptingEnabled())
        {
            auto constructorObject = new jsTConstructorFunction<Signature>(GetTypeLibrary(), function, name);

            add(name, constructorObject);
            addGlobal(name, constructorObject);

            return *constructorObject->typeInfo();
        }
        else
        {
            GetTypeLibrary().addReservedWord(name);

            return *getDummyValue()->typeInfo();
        }
    }

    template <class Signature>
    jsTypeInfo& Constructor(const std::string& name, Signature function,
                         DNVS::MoFa::Reflection::Members::ModifierNamespace::Explicit)
    {
        if (isScriptingEnabled())
        {
            auto constructorObject = new jsTConstructorFunction<Signature>(GetTypeLibrary(), function, name);

            add(name, constructorObject);
            addGlobal(name, constructorObject);

            return *constructorObject->typeInfo();
        }
        else
        {
            GetTypeLibrary().addReservedWord(name);

            return *getDummyValue()->typeInfo();
        }
    }

    // Define constructor by specifying argument list

    jsTypeInfo& Constructor()
    {
        return CreateConstructorWithSignature<C()>();
    }

    jsTypeInfo& Constructor(const char* name)
    {
        return CreateConstructorWithSignature<C()>(std::string(name));
    }

    jsTypeInfo& Constructor(const std::string& name)
    {
        return CreateConstructorWithSignature<C()>(name);
    }

    template <typename... Args>
    jsTypeInfo& Constructor()
    {
        return CreateConstructorWithSignature<C(Args...)>();
    }

    template <typename... Args>
    jsTypeInfo& Constructor(const std::string& name)
    {
        return CreateConstructorWithSignature<C(Args...)>(name);
    }

    /// Define a new function
    template <class Signature>
    jsTypeInfo& Function(const char* name, Signature function)
    {
        return Function(std::string(name), function);
    }

    template <class Signature>
    jsTypeInfo& Function(const std::string& name, Signature function)
    {
        if (isScriptingEnabled())
        {
            jsValue* functionObject = new jsTFunction<Signature>(GetTypeLibrary(), function, name);

            add(name, functionObject);

            return *functionObject->typeInfo();
        }
        else
            return *getDummyValue()->typeInfo();
    }

    /**
    \brief Defines a static function
    \param name The name of the static function
    \param fn The function pointer
    \return The new static function object, use this when adding documentation.
    */
    template <class Fn>
    jsTypeInfo& StaticFunction(const std::string& name, Fn fn)
    {
        if (isScriptingEnabled())
        {
            jsValue* function = new jsTFunction<Fn>(GetTypeLibrary(), fn, name);

            addGlobal(name, function);

            return *function->typeInfo();
        }
        else
            return *getDummyValue()->typeInfo();
    }

    template <typename ReturnType>
    void ImplicitConversion(ReturnType (C::*ConversionFunction)() const, bool isUserConversion = true)
    {
        DNVS::MoFa::Reflection::AddImplicitConversion<const C*, ReturnType>(GetTypeLibrary().GetReflectionTypeLibrary()->GetConversionGraph(),
                                                                            ConversionFunction, isUserConversion);
    }

    template <typename ReturnType>
    void ImplicitConversion(ReturnType (C::*ConversionFunction)(), bool isUserConversion = true)
    {
        DNVS::MoFa::Reflection::AddImplicitConversion<C*, ReturnType>(GetTypeLibrary().GetReflectionTypeLibrary()->GetConversionGraph(),
                                                                      ConversionFunction, isUserConversion);
    }

    template <typename ReturnType>
    void ImplicitConversion(ReturnType (*ConversionFunction)(C*), bool isUserConversion = true)
    {
        DNVS::MoFa::Reflection::AddImplicitConversion<C*, ReturnType>(GetTypeLibrary().GetReflectionTypeLibrary()->GetConversionGraph(),
                                                                      ConversionFunction, isUserConversion);
    }

    template <typename ReturnType>
    void ImplicitConversion(ReturnType (*ConversionFunction)(const C*), bool isUserConversion = true)
    {
        DNVS::MoFa::Reflection::AddImplicitConversion<const C*, ReturnType>(GetTypeLibrary().GetReflectionTypeLibrary()->GetConversionGraph(),
                                                                            ConversionFunction, isUserConversion);
    }

    template <typename A0>
    void ImplicitConstructorConversion(bool isUserConversion = true)
    {
        DNVS::MoFa::Reflection::AddConstructorConditionalIfArityOne<C(A0)>::Apply(
            GetTypeLibrary().GetReflectionTypeLibrary()->GetConversionGraph(), isUserConversion);
    }

    template <typename Signature>
    void ImplicitConstructorConversion(Signature signature, bool isUserConversion = true)
    {
        DNVS::MoFa::Reflection::AddConstructorFunction(GetTypeLibrary().GetReflectionTypeLibrary()->GetConversionGraph(), signature,
                                                       isUserConversion);
    }

    /// Define a property Get method
    template <class Signature>
    jsTypeInfo& Get(const char* name, Signature getter)
    {
        return Get(std::string(name), getter);
    }

    template <class Signature>
    jsTypeInfo& Get(const std::string& name, Signature getter)
    {
        if (isScriptingEnabled())
            return *add(name, new jsTPropertyGet<Signature>(GetTypeLibrary(), getter))->typeInfo();
        else
            return *getDummyValue()->typeInfo();
    }

    /// Define a property Put method
    template <class Signature>
    jsTypeInfo& Set(const char* name, Signature setter)
    {
        return Set(std::string(name), setter);
    }

    template <class Signature>
    jsTypeInfo& Set(const std::string& name, Signature setter)
    {
        if (isScriptingEnabled())
            return *add(name, new jsTPropertyPut<Signature>(GetTypeLibrary(), setter))->typeInfo();
        else
            return *getDummyValue()->typeInfo();
    }

    template <class Enum>
    jsTypeInfo& defEnum(jsType<Enum>, const std::string& identifier)
    {
        jsEnum<Enum>::init(GetTypeLibrary(), identifier);

        return *GetTypeLibrary().lookup(typeid(jsEnum<Enum>))->typeInfo();
    }

    template <class Enum>
    jsTypeInfo& defConstant(Enum enumValue, const std::string& identifier)
    {
        jsEnum<Enum>* value = new jsEnum<Enum>(GetTypeLibrary(), identifier, enumValue);

        jsValue* prototype = GetTypeLibrary().lookup(typeid(jsEnum<Enum>));
        if (!prototype)
        {
            std::string error = "Enumeration '" + std::string(typeid(jsEnum<Enum>).name()) + "' must be defined.";
            error += " Use the function 'defEnum' to define the enumeration";
            throw std::exception(error.c_str());
        }

        add(identifier, value);
        addGlobal(identifier, value);

        jsTClass<jsEnum<Enum>> cls(GetTypeLibrary());
        cls.Get(identifier, value);

        return *value->typeInfo();
    }

    /// Unite this class to an existing class.
    template <class UniteClass>
    void unite(jsType<UniteClass>)
    {
        std::string identifier = "_unite_";
        identifier += jsStack::stack()->typeName(typeid(UniteClass));

        jsValue* val = new jsTConstructor<C()>(GetTypeLibrary());
        jsBaseClass::unite(typeid(UniteClass));

        add(identifier, val);

        DNVS::MoFa::Reflection::AddUniteConversion<UniteClass, C>(GetTypeLibrary().GetReflectionTypeLibrary()->GetConversionGraph());
    }

    jsTClass& operator=(const jsTClass& cls)
    {
        jsBaseClass::operator=(cls);
        return *this;
    }

    jsTypeInfo& AddDocumentation(const std::string& text)
    {
        return typeInfo()->AddDocumentation(text);
    }

    jsExampleInfo& AddExample(const std::string& text)
    {
        return typeInfo()->AddExample(text);
    }

    jsTypeInfo& AddSignature(const std::string& doc, const std::string& args)
    {
        return typeInfo()->AddSignature(doc, args);
    }

    // TODO: Deal with this
    jsArgInfo* returned(const std::string& name)
    {
        return typeInfo()->returned(name);
    }

private:
    template<typename Signature>
    jsTypeInfo& CreateConstructorWithSignature(bool isExplicit = false)
    {
        std::string name = GetTypeLibrary().GetTypeName(typeid(C));

        return CreateConstructorWithSignature<Signature>(name, isExplicit);
    }

    template<typename Signature>
    jsTypeInfo& CreateConstructorWithSignature(const std::string& name, bool isExplicit = false)
    {
        if (isScriptingEnabled())
        {
            DNVS::MoFa::Reflection::AddConstructorConditionalIfArityOne<Signature>::Apply(
                GetTypeLibrary().GetReflectionTypeLibrary()->GetConversionGraph());

            auto constructor = new jsTConstructor<Signature>(GetTypeLibrary(), name, isExplicit);

            addGlobal(name, constructor);
            add(name, constructor);

            return *constructor->typeInfo();
        }
        else
        {
            GetTypeLibrary().addReservedWord(name);

            return *getDummyValue()->typeInfo();
        }
    }

};

template <class C, class P = C>
class jsClass : public jsTClass<C, P>
{
public:
    jsClass(jsTypeLibrary& typeLibrary, const std::string& name = "") : jsTClass<C, P>(typeLibrary, name)
    {
        if (reinit())
            return;

        jsStack::stack()->addDummy(typeid(C), new C(jsAutomation()));
    }

    jsClass(const jsClass& cls) : jsTClass<C, P>(cls)
    {
    }

    jsClass& operator=(const jsClass& cls)
    {
        jsTClass<C, P>::operator=(cls);

        return *this;
    }

    virtual ~jsClass()
    {
    }
};

template <class Fn, class C, class P>
void defFunction(jsClass<C, P>& cls, Fn f, const std::string& identifier)
{
    if (!cls.enableScripting())
        return;

    jsValue* function = new jsTFunction<Fn>(cls.GetTypeLibrary(), f, identifier);

    cls.add(identifier, function);
}

template <class R, class C, class C1, class P1>
void defProperty(jsClass<C1, P1>& cls, void (C::*put)(R), R (C::*get)() const, const std::string& identifier)
{
    if (!cls.enableScripting())
        return;

    jsValue* property = new jsTProperty<C, R>(cls.GetTypeLibrary(), put, get);

    cls.add(identifier, property);
}

template <class EnumT>
class enum_ : public jsTClass<jsEnum<EnumT>>
{
public:
    enum_(jsTypeLibrary& typeLibrary, const std::string& name, int trimLeft = 0, bool smartSpaces = false)
        : jsTClass<jsEnum<EnumT>>(typeLibrary, name), m_trimLeft(trimLeft), m_smartSpaces(smartSpaces)
    {
        typeLibrary.SetTypeName(typeid(EnumT), name);

        ImplicitConstructorConversion<EnumT>(false);
        ImplicitConversion(&jsEnum<EnumT>::operator EnumT, false);

        DNVS::MoFa::Reflection::AddClassWithBase<jsEnum<EnumT>, jsEnumBase>(GetTypeLibrary().GetReflectionTypeLibrary()->GetConversionGraph());
    }

    jsTypeInfo& value(const std::string& identifier, EnumT var)
    {
        jsEnum<EnumT>* value = new jsEnum<EnumT>(GetTypeLibrary(), identifier, var, m_trimLeft, m_smartSpaces);

        add(identifier, value);
        addGlobal(identifier, value);

        return *value->typeInfo();
    }

    jsTypeInfo& value(const std::string& identifier, EnumT var, int trimLeft, bool smartSpaces)
    {
        jsEnum<EnumT>* value = new jsEnum<EnumT>(GetTypeLibrary(), identifier, var, trimLeft, smartSpaces);

        add(identifier, value);
        addGlobal(identifier, value);

        return *value->typeInfo();
    }

private:
    int m_trimLeft;
    bool m_smartSpaces;
};

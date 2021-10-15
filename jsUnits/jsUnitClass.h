#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsScript/jsClass.h>
#include <jsScript/jsUnitValue.h>
#include "Reflection/Classes/Class.h"
#include "Units/Quantity.h"
#include "jsTQuantity.h"

#pragma warning(disable:4275)

template<typename DimensionT, typename Rhs>
typename std::enable_if<std::is_base_of<jsQuantity, Rhs>::value, bool>::type operator<(const DNVS::MoFa::Units::Quantity<DimensionT>& lhs, const Rhs& rhs) {
    return DNVS::MoFa::Units::Runtime::DynamicQuantity(lhs) < rhs.GetQuantity();
}
template<typename DimensionT, typename Rhs>
typename std::enable_if<std::is_base_of<jsQuantity, Rhs>::value, bool>::type operator>(const DNVS::MoFa::Units::Quantity<DimensionT>& lhs, const Rhs& rhs) {
    return DNVS::MoFa::Units::Runtime::DynamicQuantity(lhs) > rhs.GetQuantity();
}
template<typename DimensionT, typename Rhs>
typename std::enable_if<std::is_base_of<jsQuantity, Rhs>::value, bool>::type operator<=(const DNVS::MoFa::Units::Quantity<DimensionT>& lhs, const Rhs& rhs) {
    return DNVS::MoFa::Units::Runtime::DynamicQuantity(lhs) <= rhs.GetQuantity();
}
template<typename DimensionT, typename Rhs>
typename std::enable_if<std::is_base_of<jsQuantity, Rhs>::value, bool>::type operator>=(const DNVS::MoFa::Units::Quantity<DimensionT>& lhs, const Rhs& rhs) {
    return DNVS::MoFa::Units::Runtime::DynamicQuantity(lhs) >= rhs.GetQuantity();
}
template<typename DimensionT, typename Rhs>
typename std::enable_if<std::is_base_of<jsQuantity, Rhs>::value, bool>::type operator==(const DNVS::MoFa::Units::Quantity<DimensionT>& lhs, const Rhs& rhs) {
    return DNVS::MoFa::Units::Runtime::DynamicQuantity(lhs) == rhs.GetQuantity();
}
template<typename DimensionT, typename Rhs>
typename std::enable_if<std::is_base_of<jsQuantity, Rhs>::value, bool>::type operator!=(const DNVS::MoFa::Units::Quantity<DimensionT>& lhs, const Rhs& rhs) {
    return DNVS::MoFa::Units::Runtime::DynamicQuantity(lhs) != rhs.GetQuantity();
}


template<typename DimensionT, typename Lhs>
typename std::enable_if<std::is_base_of<jsQuantity, Lhs>::value, bool>::type operator< (const Lhs& lhs, const DNVS::MoFa::Units::Quantity<DimensionT>& rhs) {
    return lhs.GetQuantity() < DNVS::MoFa::Units::Runtime::DynamicQuantity(rhs);
}
template<typename DimensionT, typename Lhs>
typename std::enable_if<std::is_base_of<jsQuantity, Lhs>::value, bool>::type operator> (const Lhs& lhs, const DNVS::MoFa::Units::Quantity<DimensionT>& rhs) {
    return lhs.GetQuantity() > DNVS::MoFa::Units::Runtime::DynamicQuantity(rhs);
}
template<typename DimensionT, typename Lhs>
typename std::enable_if<std::is_base_of<jsQuantity, Lhs>::value, bool>::type operator<=(const Lhs& lhs, const DNVS::MoFa::Units::Quantity<DimensionT>& rhs) {
    return lhs.GetQuantity() <= DNVS::MoFa::Units::Runtime::DynamicQuantity(rhs);
}
template<typename DimensionT, typename Lhs>
typename std::enable_if<std::is_base_of<jsQuantity, Lhs>::value, bool>::type operator>=(const Lhs& lhs, const DNVS::MoFa::Units::Quantity<DimensionT>& rhs) {
    return lhs.GetQuantity() >= DNVS::MoFa::Units::Runtime::DynamicQuantity(rhs);
}
template<typename DimensionT, typename Lhs>
typename std::enable_if<std::is_base_of<jsQuantity, Lhs>::value, bool>::type operator==(const Lhs& lhs, const DNVS::MoFa::Units::Quantity<DimensionT>& rhs) {
    return lhs.GetQuantity() == DNVS::MoFa::Units::Runtime::DynamicQuantity(rhs);
}
template<typename DimensionT, typename Lhs>
typename std::enable_if<std::is_base_of<jsQuantity, Lhs>::value, bool>::type operator!=(const Lhs& lhs, const DNVS::MoFa::Units::Quantity<DimensionT>& rhs) {
    return lhs.GetQuantity() != DNVS::MoFa::Units::Runtime::DynamicQuantity(rhs);
}

using DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer;
//Dummy registration of this class.
inline void DoReflect(TypeLibraryPointer typeLibrary,  jsQuantity**)
{}
//Dummy registration of this class.
template<typename JavascriptQuantity>
inline void DoReflect(TypeLibraryPointer typeLibrary, jsTQuantity<JavascriptQuantity>**)
{}

template<class JavascriptQuantity,typename QuantityT>
class jsUnitClass : public jsTClass<JavascriptQuantity,jsQuantity> {
public:
    typedef typename QuantityT::DimensionType DimensionType;
    typedef DNVS::MoFa::Units::Quantity<DimensionType> QuantityType;
    template<typename EquivalentQuantity>
    static EquivalentQuantity* ConstructEquivalentQuantity(const JavascriptQuantity& quantity) {
        return new EquivalentQuantity((double)quantity);
    }
    static JavascriptQuantity* ConstructJavascriptQuantity(const QuantityType& q) {
        return new JavascriptQuantity(q);
    }
    static QuantityType ConvertToQuantity(const JavascriptQuantity* q) {
        return q->GetQuantity().As<QuantityType>();
    }
    static QuantityT ConvertToQuantityPhenomenon(const JavascriptQuantity* q)
    {
        return q->GetQuantity().As<QuantityT>();
    }
    static std::string ConvertToString(QuantityType quantity) {
        return JavascriptQuantity(quantity);
    }

    jsUnitClass(jsTypeLibrary& typeLibrary) : jsTClass<JavascriptQuantity,jsQuantity>(typeLibrary,JavascriptQuantity::GetPhenomenon().GetName())
    {
       if (reinit())
           return;

       std::string unitName = JavascriptQuantity::GetPhenomenon().GetName();
       std::string argument = unitName;

       argument[0] = tolower(argument[0]);

       jsStack::stack()->addType(typeid(JavascriptQuantity), unitName);

       AddDocumentation(unitName + " has the signature: " + jsTQuantity<JavascriptQuantity>().signature());

       using DNVS::MoFa::Reflection::Members::Arg;
       Constructor(jsTQuantity<JavascriptQuantity>::construct_1)
           .AddDocumentation("Construct a new " + unitName + " from a double value with the default input unit for " +
                             unitName)
           .AddSignature((Arg(argument)));

       Constructor(jsTQuantity<JavascriptQuantity>::construct_2, false)
           .AddDocumentation("Construct a new " + unitName + " unit from a unit value")
           .AddSignature((Arg(argument)));

       Constructor(ConstructJavascriptQuantity, false)
           .AddDocumentation("Construct a new " + unitName + " unit from a quantity")
           .AddSignature((Arg(argument)));

       ImplicitConstructorConversion<const JavascriptQuantity>();

       Constructor<double, const std::string&>()
           .AddDocumentation("Construct a new " + unitName + " unit explicitly")
           .AddSignature((Arg(argument), "unitName"));

       Constructor(jsTQuantity<JavascriptQuantity>::construct_4)
           .AddDocumentation("Construct a new " + unitName + " based on a unit string")
           .AddSignature((Arg("unitString")));

       ImplicitConversion(ConvertToQuantity);
       ImplicitConversion(ConvertToQuantityPhenomenon);

       using namespace DNVS::MoFa::Reflection::Classes;

       Class<jsTQuantity<JavascriptQuantity>, Public<jsQuantity>> jsParentCls(typeLibrary.GetReflectionTypeLibrary(), "");
       Class<JavascriptQuantity, Public<jsTQuantity<JavascriptQuantity>>> jsCls(typeLibrary.GetReflectionTypeLibrary(), "");
       jsCls.Operator(QuantityType() < This.Const);
       jsCls.Operator(QuantityType() > This.Const);
       jsCls.Operator(QuantityType() <= This.Const);
       jsCls.Operator(QuantityType() >= This.Const);
       jsCls.Operator(QuantityType() != This.Const);
       jsCls.Operator(QuantityType() == This.Const);
       jsCls.Operator(This.Const < This.Const);
       jsCls.Operator(This.Const > This.Const);
       jsCls.Operator(This.Const <= This.Const);
       jsCls.Operator(This.Const >= This.Const);
       jsCls.Operator(This.Const != This.Const);
       jsCls.Operator(This.Const == This.Const);
       jsCls.Operator(This.Const <  QuantityType());
       jsCls.Operator(This.Const >  QuantityType());
       jsCls.Operator(This.Const <= QuantityType());
       jsCls.Operator(This.Const >= QuantityType());
       jsCls.Operator(This.Const != QuantityType());
       jsCls.Operator(This.Const == QuantityType());

       Class<QuantityType> cls(typeLibrary.GetReflectionTypeLibrary(), unitName);
       cls.Function("toDouble", &JavascriptQuantity::toDouble);
       cls.Function("toString", &ConvertToString);
       cls.ImplicitConversion(&ConvertToString);
   }
};

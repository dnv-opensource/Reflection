#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection\Classes\Class.h"
#include "..\ReflectOperators.h"
#include "Units\Runtime\DatabaseUnitSystem.h"
#include "Reflection\Attributes\PostfixAttribute.h"
#include "Reflection\Utilities\DefaultRegistration.h"
#include "Units\Runtime\Unit.h"
#include <type_traits>
#include "Reflection\Functions\RegisterToStringFunction.h"
#include "boost\optional.hpp"
#include "Reflection\Attributes\ParserAttribute.h"
#include "Reflection\Attributes\UndefinedAttribute.h"

namespace DNVS {namespace MoFa {namespace Units {
    //Since Class<A, Public<B>> automatically registers B, we need to have this method in place to eliminate an error message.
    template<typename DimensionType>
    void DoReflect(DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer typeLibrary, Quantity<DimensionType>**, std::enable_if_t<!std::is_base_of_v<IPhenomenon, DimensionType>, void*> = nullptr)
    {

    }
}}}
namespace DNVS {namespace MoFa {namespace Units { namespace Reflection {

    template<typename PhenomenonT>
    std::string ToString(const Quantity<PhenomenonT>& other)
    {
        Runtime::Unit unit = Runtime::GetInputUnitSystem().GetUnit(PhenomenonT());
        return DNVS::MoFa::Reflection::Utilities::DefaultRegistration::ToString(other.GetValue()/unit.GetConversionFactor()) + " " + unit.GetUnitName();
    }

    template<typename PhenomenonT>
    boost::optional<Quantity<PhenomenonT>> ParseQuantity(const std::string& argument, const DNVS::MoFa::Formatting::FormattingService& service)
    {
        try {
            double value = atof(argument.c_str());
            Quantity<PhenomenonT> input(DNVS::MoFa::Units::Runtime::GetInputUnitSystem().ConvertFromUnitSystem<Quantity<PhenomenonT>>(value));
            return input;
        }
        catch (...)
        {
            return boost::none;
        }
    }
    template<typename PhenomenonT>
    void ReflectPhenomenon( DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer typeLibrary )
    {
        using namespace DNVS::MoFa::Reflection::Classes;
        using namespace DNVS::MoFa::Reflection::Functions;
        typedef typename PhenomenonT::DimensionType DimensionType;
        ReflectAllOperators<Quantity<DimensionType>>(typeLibrary);
        Class<Quantity<PhenomenonT>, Quantity<PhenomenonT>, Public<Quantity<DimensionType> > > cls(typeLibrary,PhenomenonT::GetName());
        cls.AddAttribute<UndefinedAttribute>([](const Quantity<PhenomenonT>& q) {return !!_isnan(q.GetValue()); });
        cls.AddAttribute<PostfixAttribute>([]() 
        {
            return Runtime::GetInputUnitSystem().GetUnit(PhenomenonT()).GetUnitName();
        });
        cls.AddAttribute<ParserAttribute>(ParseQuantity<PhenomenonT>);
        cls.Constructor<const Quantity<DimensionType>&>();
        cls.Constructor([](double value) {return DNVS::MoFa::Units::Runtime::GetInputUnitSystem().ConvertFromUnitSystem<Quantity<PhenomenonT>>(value); });
        cls.Operator(This.Const == This.Const);
        cls.Operator(This.Const != This.Const);
        cls.Operator(This.Const < This.Const);
        cls.Operator(This.Const > This.Const);
        cls.Operator(This.Const <= This.Const);
        cls.Operator(This.Const >= This.Const);
        RegisterToStringFunction(cls);
    }

}}}}
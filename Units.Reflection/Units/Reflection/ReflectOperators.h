#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection\TypeLibraries\ITypeLibrary.h"
#include "Reflection\Variants\VariantService.h"
#include "Reflection\Classes\Class.h"
#include "Units\Length.h"
#include "Units\Area.h"
#include "Units\Volume.h"
#include "Units\Time.h"
#include "Units\Angle.h"
#include "Units\Force.h"
#include "Units\Mass.h"
#include "Units\TempDiff.h"
#include "Units\ForcePerUnitArea.h"
#include "Units\ForcePerUnitLength.h"
#include "Units\Acceleration.h"
#include "Units\MassDensity.h"

namespace DNVS {namespace MoFa {namespace Units {
    template<typename DimensionT>
    DNVS::MoFa::Reflection::Variants::Variant operator^(Quantity<DimensionT> lhs, int rhs)
    {
        using namespace DNVS::MoFa::Reflection::Variants;
        switch(rhs) 
        {
        case -4:return VariantService::Reflect(Pow<-4>(lhs));
        case -3:return VariantService::Reflect(Pow<-3>(lhs));
        case -2:return VariantService::Reflect(Pow<-2>(lhs));
        case -1:return VariantService::Reflect(Pow<-1>(lhs));
        case 0:return VariantService::Reflect(Pow<0>(lhs));
        case 1:return VariantService::Reflect(Pow<1>(lhs));
        case 2:return VariantService::Reflect(Pow<2>(lhs));
        case 3:return VariantService::Reflect(Pow<3>(lhs));
        case 4:return VariantService::Reflect(Pow<4>(lhs));
        default:throw std::runtime_error("Unsupported operator");
        }
    }    
}}}

namespace DNVS {namespace MoFa {namespace Units { namespace Reflection {
    template<typename QuantityT>
    void ReflectAllOperators(DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer typeLibrary) 
    {
        using namespace DNVS::MoFa::Reflection::Classes;
        Class<QuantityT,QuantityT> quantity(typeLibrary,"");
        quantity.Operator(This.Const * Quantity<LengthDimension>());
        quantity.Operator(This.Const / Quantity<LengthDimension>());
        quantity.Operator(-This.Const);

        quantity.Operator(This.Const * Quantity<AreaDimension>());
        quantity.Operator(This.Const / Quantity<AreaDimension>());

        quantity.Operator(This.Const * Quantity<VolumeDimension>());
        quantity.Operator(This.Const / Quantity<VolumeDimension>());

        quantity.Operator(This.Const / Quantity<TimeDimension>());
        quantity.Operator(This.Const * Quantity<TimeDimension>());
        quantity.Operator(This.Const / Pow<2>(Quantity<TimeDimension>()));
        quantity.Operator(This.Const * Pow<2>(Quantity<TimeDimension>()));
        quantity.Operator(This.Const * Pow<-2>(Quantity<TimeDimension>()));
        quantity.Operator(This.Const * Pow<-1>(Quantity<TimeDimension>()));

        quantity.Operator(This.Const * Pow<-3>(Quantity<LengthDimension>()));
        quantity.Operator(This.Const * Pow<-2>(Quantity<LengthDimension>()));
        quantity.Operator(This.Const * Pow<-1>(Quantity<LengthDimension>()));

        quantity.Operator(This.Const * Quantity<AngleDimension>());
        quantity.Operator(This.Const / Quantity<AngleDimension>());

        quantity.Operator(This.Const / Quantity<ForceDimension>());
        quantity.Operator(This.Const * Quantity<ForceDimension>());

        quantity.Operator(This.Const / Quantity<MassDimension>());
        quantity.Operator(This.Const * Quantity<MassDimension>());

        quantity.Operator(This.Const / Quantity<TempDiffDimension>());
        quantity.Operator(This.Const * Quantity<TempDiffDimension>());

        quantity.Operator(double() / This.Const);
        quantity.Operator(This.Const^int());
        quantity.Operator(This.Const * Quantity<ForcePerUnitAreaDimension>());
        quantity.Operator(This.Const * Quantity<ForcePerUnitLengthDimension>());
        quantity.Operator(This.Const * Quantity<Dimension<-2, 2, 1, 0, 0> >());

        quantity.Operator(This.Const * Quantity<AccelerationDimension>());
        quantity.Operator(This.Const / Quantity<AccelerationDimension>());

        quantity.Operator(This.Const * Quantity<MassDensityDimension>());
        quantity.Operator(This.Const / Quantity<MassDensityDimension>());
    }
}}}}
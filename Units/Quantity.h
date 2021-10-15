#pragma once 
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "QuantityBase.h"
#include "Formatting\FormattingService.h"
#include "Runtime\DynamicDimension.h"
#include "Formatting\DefaultUnitFormatter.h"

namespace DNVS {namespace MoFa {namespace Units {

    /**
    Implementation of a Quantity of a given dimension.
    Dimensions are given as an instance of the Dimension template type, where Dimension has five template arguments,
    int Length,int Time,int Force,int TemperatureDifference,int Angle,
    So Dimension<1, 0, 0, 0, 0> will be the dimension of length, while
    Dimension<2, 0, 0, 0, 0> is the dimension of Area.
    Dimension<-1, 2, 1, 0, 0> is the dimension of Mass.
   */
    template<typename DimensionT, typename EnableIf>
    class Quantity 
        :   public QuantityBase<DimensionT>
    {
    public:
        Quantity(const Quantity& other) : BaseClass(other) {}
        Quantity(const QuantityBase<DimensionT>& other) : BaseClass(other) {}
        explicit Quantity(double value) : BaseClass(value) {}
        Quantity(){}
        Quantity& operator=(const Quantity& other) {BaseClass::operator=(other); return *this; }
    };

    template<>
    class Quantity<NoDimension> : public QuantityBase<NoDimension> 
    {
    public:
        Quantity(const Quantity& other) : BaseClass(other) {}
        Quantity(const QuantityBase<NoDimension>& other) : BaseClass(other) {}
        Quantity(double value) : BaseClass(value) {}
        Quantity(){}
        Quantity& operator=(const Quantity& other) {BaseClass::operator=(other); return *this; }
        ///Implicit conversion to double for NoDimension quantity
        operator double() const {return GetValue(); }
    };

    //Template metamethod for determining whether a Quantity has a phenomenon or just a dimension.
    template<typename QuantityT>
    struct IsQuantityWithDimension : std::bool_constant<false> {};

    template<int Length, int Time, int Force, int TemperatureDifference, int Angle>
    struct IsQuantityWithDimension<Quantity<Dimension<Length,Time,Force,TemperatureDifference, Angle>>> 
        : std::bool_constant<true> {};

    //Implements ToString for quantity. The last argument indicates how the phenomenon should be formatted.
    template<typename DimensionT>
    std::string ToString(const Quantity<DimensionT>& quantity, const DNVS::MoFa::Formatting::FormattingService& service, std::enable_if_t<IsQuantityWithDimension<Quantity<DimensionT>>::value, void*> = nullptr)
    {
        auto formatter = service.GetFormatterOrDefault<Formatting::IUnitFormatter, Formatting::DefaultUnitFormatter>();
        return formatter->FormatQuantity(quantity.GetValue(), Runtime::DynamicDimension(DimensionT()), service);
    }
}}}

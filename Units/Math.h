#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Quantity.h"
#include "Angle.h"
#include <cmath>
#include "Runtime/DynamicQuantity.h"


namespace DNVS {namespace MoFa {namespace Units {
    template<typename T>
    Quantity<T> Abs(const Quantity<T>& value)
    {
        return Quantity<T>(std::abs(value.GetValue()));
    }
    
    template<typename T>
    bool IsNaN(const Quantity<T>& arg)
    {
        return std::isnan(arg.GetValue());
    }
    inline double sin(const Angle& angle)
    {
        return std::sin(angle.GetValue());
    }
    inline double cos(const Angle& angle)
    {
        return std::cos(angle.GetValue());
    }
    inline double tan(const Angle& angle)
    {
        return std::tan(angle.GetValue());
    }
    template<typename T>
    auto sqrt(const Quantity<T>& quantity)
    {
        using DimensionType = typename Quantity<T>::DimensionType;
        return Quantity<typename DivideDimension<DimensionType, 2>::type>(std::sqrt(quantity.GetValue()));
    }
    namespace Runtime {
        inline DynamicQuantity sqrt(const DynamicQuantity& quantity)
        {
            auto dimension = quantity.GetSimplifiedUnit().GetDimension() / 2;
            return DynamicQuantity(std::sqrt(quantity.GetNeutralValue()), SimplifiedUnit(1, dimension));
        }
        inline double cos(const DynamicQuantity& quantity)
        {
            if (quantity.GetSimplifiedUnit().GetDimension() == AngleDimension())
                return std::cos(quantity.GetNeutralValue());
            else
                throw std::runtime_error("cos is only implemented for angles");
        }
        inline double sin(const DynamicQuantity& quantity)
        {
            if (quantity.GetSimplifiedUnit().GetDimension() == AngleDimension())
                return std::sin(quantity.GetNeutralValue());
            else
                throw std::runtime_error("sin is only implemented for angles");
        }
        inline double tan(const DynamicQuantity& quantity)
        {
            if (quantity.GetSimplifiedUnit().GetDimension() == AngleDimension())
                return std::tan(quantity.GetNeutralValue());
            else
                throw std::runtime_error("tan is only implemented for angles");
        }
    }
    template<typename T>
    Quantity<T> abs(const Quantity<T>& quantity)
    {
        if (quantity.GetValue() < 0)
            return -quantity;
        else
            return quantity;
    }
    namespace Runtime {
        inline DynamicQuantity abs(const DynamicQuantity& quantity)
        {
            if (quantity.GetValue() < 0)
                return -quantity;
            else
                return quantity;
        }
    }

}}}
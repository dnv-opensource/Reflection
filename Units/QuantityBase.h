#pragma once 
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Dimension.h"

namespace DNVS {namespace MoFa {namespace Units {
    template<typename DimensionT, typename EnableIf = void>
    class Quantity;

    /**
    Base class for all quantities. Implements fundamental operations such as comparison and arithmetics.
    All operations will preserve the dimension of the input type.
    Supported operations:
    Area a = Length(1)*Length(1); will compile, while
    Area a = Mass(1)/Length(1); will not compile, since the dimensions here are incorrect.
    */
    template<typename DimensionT>
    class QuantityBase 
    {
    public:
        typedef DimensionT DimensionType;
        QuantityBase(const QuantityBase& other) : m_value(other.m_value) {}
        QuantityBase(double value) : m_value(value) {}
        QuantityBase() : m_value(0.0) {}
        QuantityBase& operator=(const QuantityBase& other) {m_value = other.m_value; return *this; }
        double GetValue() const {return m_value; }

        bool operator==(QuantityBase rhs) const {return GetValue() == rhs.GetValue(); }
        bool operator!=(QuantityBase rhs) const {return GetValue() != rhs.GetValue(); }
        bool operator<(QuantityBase rhs) const {return GetValue()<rhs.GetValue(); }
        bool operator>(QuantityBase rhs) const {return GetValue()>rhs.GetValue(); }
        bool operator<=(QuantityBase rhs) const {return GetValue() <= rhs.GetValue(); }
        bool operator>=(QuantityBase rhs) const {return GetValue() >= rhs.GetValue(); }

        ///Adds two dimensionally equivalent quantities.
        Quantity<DimensionT> operator+(QuantityBase rhs) const
        {
            return Quantity<DimensionT>(GetValue() + rhs.GetValue());
        }
        ///Subtracts two dimensionally equivalent quantities.
        Quantity<DimensionT> operator-(QuantityBase rhs) const
        {
            return Quantity<DimensionT>(GetValue()-rhs.GetValue());
        }
        ///Multiplies two quantities. Returns a new quantity with the correct dimension.
        template<typename OtherDimensionT>
        Quantity<typename AddDimensions<DimensionT, OtherDimensionT>::type> operator*(QuantityBase<OtherDimensionT> rhs) const
        {
            return Quantity<typename AddDimensions<DimensionT, OtherDimensionT>::type>(GetValue()*rhs.GetValue());
        }
        ///Scales a quantity by a scalar.
        Quantity<DimensionT> operator*(double rhs) const  { return Quantity<DimensionT>(GetValue()*rhs); }
        ///Scales a quantity by a scalar.
        friend Quantity<DimensionT> operator*(double lhs, QuantityBase rhs)  { return Quantity<DimensionT>(lhs*rhs.GetValue()); }
        ///NOOP.
        Quantity<DimensionT> operator +() const  { return Quantity<DimensionT>(GetValue()); }

        ///Negates a quantity.
        Quantity<DimensionT> operator -() const  { return Quantity<DimensionT>(-GetValue()); }
        ///Divides two quantities. Returns a new quantity with the correct dimension.
        template<typename OtherDimensionT>
        Quantity<typename SubDimensions<DimensionT, OtherDimensionT>::type> operator/(QuantityBase<OtherDimensionT> rhs) const
        {
            return Quantity<typename SubDimensions<DimensionT, OtherDimensionT>::type>(GetValue() / rhs.GetValue());
        }
        ///Divides quantity with a scalar.
        Quantity<DimensionT> operator/(double rhs) const
        {
            return Quantity<DimensionT>(GetValue() / rhs);
        }
        ///Divides a scalar with a quantity. Inverts the dimension.
        friend Quantity<typename SubDimensions<NoDimension, DimensionT>::type> operator/(double lhs, QuantityBase rhs)
        {
            return Quantity<typename SubDimensions<NoDimension, DimensionT>::type>(lhs / rhs.GetValue());
        }
        template<int Exponent>
        friend Quantity<typename ScaleDimension<DimensionT, Exponent>::type> Pow(QuantityBase subject)
        {
            return Quantity<typename ScaleDimension<DimensionT, Exponent>::type>(pow(subject.GetValue(), Exponent));
        }

        Quantity<DimensionT>& operator+=(QuantityBase rhs)  
        {
            m_value += rhs.GetValue();
            return *static_cast<Quantity<DimensionT>*>(this);
        }
        Quantity<DimensionT>& operator-=(QuantityBase rhs)
        {
            m_value -= rhs.GetValue();
            return *static_cast<Quantity<DimensionT>*>(this);
        }

        Quantity<DimensionT>& operator*=(double rhs) 
        {
            m_value *= rhs;
            return *static_cast<Quantity<DimensionT>*>(this);
        }

        Quantity<DimensionT>& operator/=(double rhs)
        {
            m_value /= rhs;
            return *static_cast<Quantity<DimensionT>*>(this);
        }

        template<typename CallbackT>
        static void ForEachNamedUnit(CallbackT& callback)
        {}

        typedef QuantityBase<DimensionT> BaseClass;
        double ConvertTo(const QuantityBase<DimensionT>& quantity) const
        {
            return m_value / quantity.GetValue();
        }
    private:
        double m_value;
    };


}}}

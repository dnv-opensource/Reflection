#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Quantity.h"
#include <type_traits>
#include "NamedQuantity.h"
#include "Formatting\FormattingService.h"
#include "Runtime\DynamicPhenomenon.h"
#include "Formatting\DefaultUnitFormatter.h"

namespace DNVS {namespace MoFa {namespace Units {
    /**
    IPhenomenon is the base class for all phenomenons.
    Two phenomenons can have the same dimension but different interpretations, such as 
    Energy and MomentOfForce has the same dimension: Dimension<1, 0, 1, 0, 0>
    However, Energy should have the unit J, while MomentOfForce should have the unit Nm.
    The Phenomenon class knows this. It can return a name and can combine fundamental units in order to indicate
    what the final unit of this phenomenon should be.
    Compare:
    class EnergyPhenomenon : public IPhenomenon
    {
    public:
        typedef EnergyDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return callback(EnergyDimension());
        }
        static std::string GetName() {return "Energy";}
    };
    and
    class MomentOfForcePhenomenon : public IPhenomenon
    {
    public:
        typedef MomentOfForceDimension DimensionType;
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return callback(ForceDimension()) * callback(LengthDimension());
        }
        static std::string GetName() {return "MomentOfForce";}
    };

    IPhenomenon must have a static method GetName() that returns the name of the phenomenon.
    
    It must also have a static method ComposeUnit which specifies how you want to display the unit, here
    EnergyPhenomenon will display the current energy unit, such as kJ or similar, while
    MomentOfForcePhenomenon will display the current force unit * the current length unit, such as N*m

    In addition, it must have a typedef <Dimension> DimensionType;
    */
    class IPhenomenon {};

    /*
    Here, we create a class Quantity which has a phenomenon, and let it inherit from a Quantity with dimension. 
    This should probably be rewritten to something like:
    template<typename PhenomenonT>
    class QuantityWithPhenomenon : public QuantityWithDimension<PhenomenonT::DimensionType>
    {};
    */
    template<typename PhenomenonT>
    class Quantity<PhenomenonT, typename std::enable_if<std::is_base_of<IPhenomenon,PhenomenonT>::value>::type>
        :   public Quantity<typename PhenomenonT::DimensionType>
    {
    public:
        typedef Quantity<typename PhenomenonT::DimensionType> BaseClass;
        typedef typename PhenomenonT::DimensionType DimensionType;
        typedef typename PhenomenonT PhenomenonType;

        Quantity(const Quantity& other) : BaseClass(other) {}        
        Quantity(const QuantityBase<DimensionType>& other) : BaseClass(other) {}
        Quantity(const Quantity<DimensionType>& other) : BaseClass(other) {}
        Quantity(const NamedQuantity<DimensionType>& other) : BaseClass(other) {}
        explicit Quantity(double value) : BaseClass(value) {}
        Quantity() {}  
        Quantity& operator=(const Quantity& other) {BaseClass::operator=(other); return *this; }
        Quantity& operator=(const Quantity<DimensionType>& other) {BaseClass::operator=(other); return *this; }
    };
    //Template metamethod for determining whether a Quantity has a phenomenon or just a dimension.
    template<typename QuantityT>
    struct IsQuantityWithPhenomenon : std::bool_constant<false> {};
    
    template<typename PhenomenonT>
    struct IsQuantityWithPhenomenon<Quantity<PhenomenonT>> : std::is_base_of<IPhenomenon, PhenomenonT> {};

    template<typename QuantityT>
    using IsQuantity = std::integral_constant<bool, IsQuantityWithPhenomenon<QuantityT>::value || IsQuantityWithDimension<QuantityT>::value>;

    template<typename QuantityT>
    constexpr bool IsQuantity_v = IsQuantity<QuantityT>::value;

    //Implements ToString for quantity. The last argument indicates how the phenomenon should be formatted.
    template<typename PhenomenonT>
    std::string ToString(const Quantity<PhenomenonT>& phenomenon, const DNVS::MoFa::Formatting::FormattingService& service, std::enable_if_t<IsQuantityWithPhenomenon<Quantity<PhenomenonT>>::value, void*> = nullptr)
    {
        auto formatter =service.GetFormatterOrDefault<Formatting::IUnitFormatter, Formatting::DefaultUnitFormatter>();
        return formatter->FormatQuantity(phenomenon.GetValue(), Runtime::DynamicPhenomenon(PhenomenonT()), service);        
    }
}}}

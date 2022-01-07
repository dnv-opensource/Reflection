#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Units/Stream/StreamUnitSystem.h"
#include "Units/Stream/ShowUnitString.h"
#include "Units/Stream/UseUnitSystemFormatter.h"
#include "Units/Runtime/Unit.h"
#include "Units/Runtime/UnitSystem.h"

namespace DNVS {namespace MoFa {namespace Units {

    template<typename T>
    std::ostream& operator<<(std::ostream& stream, const Quantity<T>& quantity)
    {        
        Runtime::UnitSystem* system = Stream::StreamUnitSystem::GetFromStream(stream);
        if(system)
        {
            Runtime::Unit unit = system->GetUnit(T());
            double value = quantity.GetValue() / unit.GetConversionFactor();
            stream << value;
            if(Stream::ShowUnitString::GetFromStream(stream))
                stream << " " << unit.GetUnitName();
        }
        else 
            stream << quantity.GetValue();
        return stream;
    }

    template<typename Dimension>
    std::ostream& operator<<(std::ostream& stream, const QuantityBase<Dimension>& quantity)
    {
        return stream << Quantity<Dimension>(quantity);
    }

    inline std::ostream& operator<<(std::ostream& stream, const Quantity<NoDimension>& quantity)
    {       
        return stream << quantity.GetValue();
    }
}}}
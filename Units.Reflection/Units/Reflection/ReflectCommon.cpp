//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ReflectCommon.h"
#include "Reflection\Classes\Class.h"
#include "Units\Quantity.h"
#include "Units\ForAllDimensions.h"
#include "Reflection\Attributes\UndefinedAttribute.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Reflection {
    struct CommonOperations 
    {
        CommonOperations(DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer typeLibrary) : m_typeLibrary(typeLibrary) {}
        template<typename DimensionT>
        void Apply() 
        {
            using namespace DNVS::MoFa::Reflection::Classes;
            Class<Quantity<DimensionT>, Quantity<DimensionT> > cls(m_typeLibrary, "");
            cls.AddAttribute<UndefinedAttribute>([](const Quantity<DimensionT>& q) {return !!_isnan(q.GetValue()); });
            cls.Operator(This.Const + This.Const);
            cls.Operator(This.Const - This.Const);
            cls.Operator(This.Const / This.Const);
            cls.Operator(This.Const / double());
            cls.Operator(This.Const * double());
            cls.Operator(double() * This.Const);
            cls.Operator(-This.Const);
            cls.Operator(+This.Const);
            cls.Operator(This.Const<This.Const);
            cls.Operator(This.Const <= This.Const);
            cls.Operator(This.Const>This.Const);
            cls.Operator(This.Const >= This.Const);
            cls.Operator(This.Const == This.Const);
            cls.Operator(This.Const != This.Const);
        }
    private:
        DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer m_typeLibrary;
    };

    void ReflectCommon( DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer typeLibrary )
    {
        ForAllUsedDimensions(CommonOperations(typeLibrary));
    }

}}}}
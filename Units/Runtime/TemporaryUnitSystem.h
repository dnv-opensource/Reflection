#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "UnitSystem.h"
#include "../Config.h"
#include "IInputUnitProvider.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {
    class UNITS_IMPORT_EXPORT TemporaryInputUnitSystem
    {
    public:
        enum UnitCopyOption
        {
            CopyNone,
            CopyAllUnits,
            CopyFundamentalUnits,//Length,Force,Time,TempDiff and Angle
            PostponedInitialization,
        };
        TemporaryInputUnitSystem(UnitCopyOption option = PostponedInitialization);
        ~TemporaryInputUnitSystem();
        //Throws unless option passed to constructor was PostponedInitialization.
        //Can only be called once per instance.
        void Initialize(UnitCopyOption option);
        bool Initialized() const;
        void Reset();
    private:
        std::shared_ptr<IInputUnitProvider> m_oldService;
        UnitCopyOption m_option;
    };

}}}}
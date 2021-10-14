//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TemporaryUnitSystem.h"
#include "DefaultInputUnitProvider.h"
#include "DatabaseUnitSystem.h"
#include "..\Length.h"
#include "..\Force.h"
#include "..\Time.h"
#include "..\Angle.h"
#include "..\TempDiff.h"
#include "Services\ServiceProvider.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {

    TemporaryInputUnitSystem::TemporaryInputUnitSystem(UnitCopyOption option)
        : m_option(PostponedInitialization)
    {
        Initialize(option);
    }

    TemporaryInputUnitSystem::~TemporaryInputUnitSystem()
    {
        Reset();
    }

    void TemporaryInputUnitSystem::Initialize(UnitCopyOption option)
    {
        if (option == PostponedInitialization)
            return;
        else if (Initialized())
            throw std::runtime_error("Cannot initialize TemporaryUnitSystem more than once!");

        m_option = option;
        auto oldInputUnits = GetInputUnitSystem();
        m_oldService = Services::ServiceProvider::Instance().TryGetService<IInputUnitProvider>();
        if (m_oldService)
            Services::ServiceProvider::Instance().UnregisterService<IInputUnitProvider>();
        Services::ServiceProvider::Instance().RegisterService<IInputUnitProvider>(std::make_shared<DefaultInputUnitProvider>());

        if (m_option == CopyFundamentalUnits)
        {
            GetInputUnitSystem().SetUnit<Length>(oldInputUnits.GetUnit<Length>());
            GetInputUnitSystem().SetUnit<Force>(oldInputUnits.GetUnit<Force>());
            GetInputUnitSystem().SetUnit<Time>(oldInputUnits.GetUnit<Time>());
            GetInputUnitSystem().SetUnit<Angle>(oldInputUnits.GetUnit<Angle>());
            GetInputUnitSystem().SetUnit<TempDiff>(oldInputUnits.GetUnit<TempDiff>());
        }
        else if (m_option == CopyAllUnits)
        {
            GetInputUnitSystem().Reset(oldInputUnits);
        }
    }

    void TemporaryInputUnitSystem::Reset()
    {
        if (Initialized())
        {
            Services::ServiceProvider::Instance().UnregisterService<IInputUnitProvider>();
            if (m_oldService)
                Services::ServiceProvider::Instance().RegisterService(m_oldService);
        }
        m_option = PostponedInitialization;
    }

    bool TemporaryInputUnitSystem::Initialized() const
    {
        return m_option != PostponedInitialization;
    }

}}}}


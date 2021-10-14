//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DefaultInputUnitProvider.h"
#include "Units\Angle.h"
#include "DefaultUnitProvider.h"
namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {


    DefaultInputUnitProvider::DefaultInputUnitProvider()
        : DefaultInputUnitProvider(std::make_shared<DefaultUnitProvider>())
    {
    }

    DefaultInputUnitProvider::DefaultInputUnitProvider(const std::shared_ptr<IUnitProvider>& unitProvider)
        : m_unitProvider(unitProvider)
    {
        m_unitProvider->SetPhenomenonUnit(AnglePhenomenon(), _deg);
    }

    DefaultInputUnitProvider::~DefaultInputUnitProvider()
    {
    }

    std::shared_ptr<IUnitProvider> DefaultInputUnitProvider::GetUnitProvider() const
    {
        return m_unitProvider;
    }

}}}}
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DefaultDatabaseUnitProvider.h"
#include "Units\Angle.h"
#include "DefaultUnitProvider.h"
namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {


    DefaultDatabaseUnitProvider::DefaultDatabaseUnitProvider()
        : m_unitProvider(std::make_shared<DefaultUnitProvider>())
    {
    }

    DefaultDatabaseUnitProvider::~DefaultDatabaseUnitProvider()
    {
    }

    std::shared_ptr<IUnitProvider> DefaultDatabaseUnitProvider::GetUnitProvider() const
    {
        return m_unitProvider;
    }

}}}}
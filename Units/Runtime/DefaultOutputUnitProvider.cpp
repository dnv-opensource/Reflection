//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DefaultOutputUnitProvider.h"
#include "Units\Angle.h"
#include "DefaultUnitProvider.h"
namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {


    DefaultOutputUnitProvider::DefaultOutputUnitProvider()
        : DefaultOutputUnitProvider(std::make_shared<DefaultUnitProvider>())
    {
    }

    DefaultOutputUnitProvider::DefaultOutputUnitProvider(const std::shared_ptr<IUnitProvider>& unitProvider)
        : m_unitProvider(unitProvider)
    {
    }

    DefaultOutputUnitProvider::~DefaultOutputUnitProvider()
    {
    }

    std::shared_ptr<IUnitProvider> DefaultOutputUnitProvider::GetUnitProvider() const
    {
        return m_unitProvider;
    }

}}}}
#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IOutputUnitProvider.h"
#include <map>
#include "..\Config.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {
#pragma warning(push)
#pragma warning(disable:4275) //non dll-interface class 'Base' used as base for dll-interface class 'Class'
    class UNITS_IMPORT_EXPORT DefaultOutputUnitProvider : public IOutputUnitProvider
    {
    public:
        DefaultOutputUnitProvider(const std::shared_ptr<IUnitProvider>& unitProvider);
        DefaultOutputUnitProvider();
        virtual ~DefaultOutputUnitProvider();
        virtual std::shared_ptr<IUnitProvider> GetUnitProvider() const;
    private:
        std::shared_ptr<IUnitProvider> m_unitProvider;
    };
#pragma warning(pop)
}}}}
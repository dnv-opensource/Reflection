#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IWorkspacePathService.h"
#include "Config.h"
#include <memory>

namespace DNVS {namespace MoFa {namespace Services {
    class SERVICES_IMPORT_EXPORT ParentWorkspacePathService : public IWorkspacePathService
    {
    public:
        ParentWorkspacePathService(const std::shared_ptr<IWorkspacePathService>& service);
        std::string GetWorkspacePath() const override;
    private:
        std::shared_ptr<IWorkspacePathService> m_service;
    };
}}}
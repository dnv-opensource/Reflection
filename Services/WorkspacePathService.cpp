//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "WorkspacePathService.h"

namespace DNVS {namespace MoFa {namespace Services {
    WorkspacePathService::WorkspacePathService(const std::string& path):m_workspacePath(path) {}

    std::string WorkspacePathService::GetWorkspacePath() const
    {
        return m_workspacePath;
    }
}}}
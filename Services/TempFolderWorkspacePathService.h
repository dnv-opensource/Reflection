#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IWorkspacePathService.h"
#include "Config.h"

namespace DNVS {namespace MoFa {namespace Services {
    class SERVICES_IMPORT_EXPORT TempFolderWorkspacePathService : public IWorkspacePathService
    {
    public:
        std::string GetWorkspacePath() const override;
    };
}}}
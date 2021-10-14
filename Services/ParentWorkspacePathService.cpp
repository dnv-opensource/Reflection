//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ParentWorkspacePathService.h"
#include <filesystem>
namespace std { namespace experimental { namespace filesystem { inline namespace v1 {
    class path;
}}}}
namespace fs = std::experimental::filesystem::v1;
namespace DNVS {namespace MoFa {namespace Services {

    ParentWorkspacePathService::ParentWorkspacePathService(const std::shared_ptr<IWorkspacePathService>& service)
        : m_service(service)
    {

    }

    std::string ParentWorkspacePathService::GetWorkspacePath() const
    {
        fs::path path(m_service->GetWorkspacePath());
        if(path.stem().generic_string() == ".")
            return path.parent_path().parent_path().generic_string();
        else
            return path.parent_path().generic_string();
    }

}}}


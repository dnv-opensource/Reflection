#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include "Config.h"
#include <functional>


namespace DNVS { namespace MoFa { namespace Services {
    class SERVICES_IMPORT_EXPORT TempFolderPathUtility {
    public:
        std::string GetWritablePath();
        std::string GetWindowsTempFolderPath();
        std::string GetExecutabePath();
        std::string GetWorkspacePath();
        TempFolderPathUtility();
        TempFolderPathUtility(const std::function<bool(const std::string& str)>& IsAccessible);
    private:
        std::string m_executablePath;
        std::string m_workSpacePath;
        std::string m_windowsTempPath;
    };
}}}
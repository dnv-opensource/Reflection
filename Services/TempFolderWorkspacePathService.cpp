//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TempFolderWorkspacePathService.h"
#include <windows.h> 

namespace DNVS {namespace MoFa {namespace Services {

    std::string TempFolderWorkspacePathService::GetWorkspacePath() const
    {
        TCHAR tempBuffer[MAX_PATH];
        // Getting windows temporary folder path
        if (GetTempPath(MAX_PATH, tempBuffer) != 0)
        {
            return std::string(tempBuffer);
        }
        return "";
    }

}}}


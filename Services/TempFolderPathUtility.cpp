//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TempFolderPathUtility.h"
#include <string>
#include <windows.h> 
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include "ServiceProvider.h"
#include "IWorkspacePathService.h"
#include "TempFolderWorkspacePathService.h"
namespace DNVS { namespace MoFa { namespace Services {
    std::string TempFolderPathUtility::GetWritablePath()
    {
        if (!m_workSpacePath.empty())
            return m_workSpacePath;
        else if (!m_executablePath.empty())
            return m_executablePath;
        return m_windowsTempPath;
    }

    std::string TempFolderPathUtility::GetWindowsTempFolderPath()
    {
        return m_windowsTempPath;
    }

    std::string TempFolderPathUtility::GetExecutabePath()
    {
        return m_executablePath;
    }

    std::string TempFolderPathUtility::GetWorkspacePath()
    {
        return m_workSpacePath;
    }

    TempFolderPathUtility::TempFolderPathUtility() :TempFolderPathUtility([](const std::string& str) {return _access(str.c_str(), 6) != -1; })
    {
    }

    TempFolderPathUtility::TempFolderPathUtility(const std::function<bool(const std::string&)>& IsAccessible)
    {
        TCHAR tempBuffer[MAX_PATH];
        m_workSpacePath = "";
        m_executablePath = "";
        m_windowsTempPath = "";

        // Getting current directory path
        auto service = Services::ServiceProvider::Instance().TryGetService<Services::IWorkspacePathService>();
        if (service && IsAccessible(service->GetWorkspacePath().c_str()))
        {
            m_workSpacePath = service->GetWorkspacePath();
        }
        // Getting executable path.
        if (GetModuleFileName(nullptr, tempBuffer, MAX_PATH) != 0)
        {
            m_executablePath = std::string(tempBuffer);
            m_executablePath = m_executablePath.substr(0, m_executablePath.find_last_of("\\"));
            if (!IsAccessible(m_executablePath))
                m_executablePath = "";
        }
        m_windowsTempPath = TempFolderWorkspacePathService().GetWorkspacePath();
        if (!IsAccessible(m_windowsTempPath))
            m_windowsTempPath = "";
    }

}}}
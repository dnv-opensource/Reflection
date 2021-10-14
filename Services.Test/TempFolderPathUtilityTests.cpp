#include "gtest/gtest.h"
#include <windows.h> 
#include "Services/TempFolderPathUtility.h"
#include "Services/ScopedServiceProvider.h"
#include "Services/WorkspacePathService.h"
#include "Services/ParentWorkspacePathService.h"
namespace DNVS { namespace MoFa { namespace Services {

    TEST(TempFolderPathUtilityTests, CheckExecutablePath)
    {
        TCHAR tempBuffer[MAX_PATH];
        std::string testPath;
        EXPECT_TRUE(GetModuleFileName(nullptr, tempBuffer, MAX_PATH));
        testPath = std::string(tempBuffer);
        testPath = testPath.substr(0, testPath.find_last_of("\\"));
        TempFolderPathUtility tempFolderPathUtility([](const std::string & str) {return true; });
        EXPECT_EQ(testPath, tempFolderPathUtility.GetWritablePath());
    }
    TEST(TempFolderPathUtilityTests, CheckWorkSpacePathService)
    {

        std::string testPath="C:\\test\\folder";
        ScopedServiceProvider serviceProvider;
        std::shared_ptr<WorkspacePathService> service(std::make_shared<WorkspacePathService>(testPath));
        serviceProvider.RegisterService<IWorkspacePathService>(service);
        TempFolderPathUtility tempFolderPathUtility([](const std::string & str) {return true; });
        EXPECT_EQ(testPath, tempFolderPathUtility.GetWritablePath());
        EXPECT_EQ("C:\\test\\folder", tempFolderPathUtility.GetWritablePath());
    }

    TEST(TempFolderPathUtilityTests, CheckParentFolderPath)
    {

        std::string testPath = "C:\\test\\folder";
        std::shared_ptr<IWorkspacePathService> service(std::make_shared<ParentWorkspacePathService>(std::make_shared<WorkspacePathService>(testPath)));
        EXPECT_EQ(service->GetWorkspacePath(), "C:/test");
    }
    TEST(TempFolderPathUtilityTests, CheckParentFolderPath_RemoveLeafSlash)
    {

        std::string testPath = "C:\\test\\folder\\";
        std::shared_ptr<IWorkspacePathService> service(std::make_shared<ParentWorkspacePathService>(std::make_shared<WorkspacePathService>(testPath)));
        EXPECT_EQ(service->GetWorkspacePath(), "C:/test");
    }
}}}

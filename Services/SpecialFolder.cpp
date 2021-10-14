//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <Services/SpecialFolder.h>
#include <filesystem>
#include <ShlObj.h>
#include "Services/TempFolderPathUtility.h"
inline std::string PathToString(const fs::path& p)
{
    return p.generic_string();
}

using namespace std;

fs::path GetTempFolderName()
{
    DNVS::MoFa::Services::TempFolderPathUtility tempFolderPathUtility;
    return tempFolderPathUtility.GetWritablePath();
}

string GetUniqueTempFolderNameForExecutable()
{
    TCHAR buffer[MAX_PATH];
    buffer[0] = 0;

    if (GetModuleFileName(nullptr, buffer, MAX_PATH) == 0)
    {
        return string();
    }

    basic_string<TCHAR> tcharString(buffer);
    fs::path modulePath(string(tcharString.begin(), tcharString.end()));
    fs::path exeName = fs::path(modulePath).stem();
    fs::path tempPath = GetTempFolderName();
    if (!tempPath.empty() && fs::is_directory(tempPath))
    {
        fs::path fullPath = tempPath / exeName;
        fs::create_directory(fullPath);
        return PathToString(fullPath);
    }
    else {
        fs::path fullPath = modulePath.parent_path() / exeName;
        try {
            fs::create_directory(fullPath);
            return PathToString(fullPath);
        }
        catch (std::exception)
        {
            return "";
        }
    }
}

namespace DNVS { namespace MoFa { namespace Services {

SpecialFolder::SpecialFolder(FolderName name, bool fCreate)
:   m_name(name)
,   m_fCreate(fCreate)
{
}

int SpecialFolder::NameToCSIDL(FolderName name) const
{
    switch(name) {
        case Personal:return CSIDL_PERSONAL;
        case MyMusic:return CSIDL_MYMUSIC;
        case AppData:return CSIDL_APPDATA;
        case LocalAppData:return CSIDL_LOCAL_APPDATA;
        case InternetCache:return CSIDL_INTERNET_CACHE;
        case Cookies:return CSIDL_COOKIES;
    }
    return 0;
}

SpecialFolder::operator fs::path() const
{
    return GetName();
}

string SpecialFolder::GetName() const
{
    string folderPath;

    if(m_name == Temp)
    {
        folderPath = GetUniqueTempFolderNameForExecutable();
    }
    else
    {
        TCHAR buffer[MAX_PATH];

        if (!SHGetSpecialFolderPath(nullptr, buffer, NameToCSIDL(m_name), m_fCreate == true))
        {
            return string();
        }

        basic_string<TCHAR> tcharString(buffer);

        folderPath = string(tcharString.begin(), tcharString.end());
    }

    folderPath += "/";

    return folderPath;
}

/*            History,
            CommonAppData,  // All Users\Application Data
            InternetCache,
            Cookies,
            History,
            CommonAppData,
            Windows,        // GetWindowsDirectory()
            System,         // GetSystemDirectory()
            ProgramFiles,   // C:\Program Files
            MyPictures,     // My Pictures, new for Win2K
            ProgramFilesCommon,// C:\Program Files\Common 
            CommonDocuments,// All Users\Documents
            Resources,      // %windir%\Resources\, For theme and other windows resources.
            ResourcesLocalized,// %windir%\Resources\<LangID>, for theme and other windows specific resources.
            CommonAdminTools,// All Users\Start Menu\Programs\Administrative Tools
            AdminTools,     // <user name>\Start Menu\Programs\Administrative Tools*/

// 
// #define CSIDL_PERSONAL                  0x0005        // My Documents
// #define CSIDL_FAVORITES                 0x0006        // <user name>\Favorites
// #define CSIDL_STARTUP                   0x0007        // Start Menu\Programs\Startup
// #define CSIDL_RECENT                    0x0008        // <user name>\Recent
// #define CSIDL_SENDTO                    0x0009        // <user name>\SendTo
// #define CSIDL_BITBUCKET                 0x000a        // <desktop>\Recycle Bin
// #define CSIDL_STARTMENU                 0x000b        // <user name>\Start Menu
// #define CSIDL_MYDOCUMENTS               0x000c        // logical "My Documents" desktop icon
// #define CSIDL_MYMUSIC                   0x000d        // "My Music" folder
// #define CSIDL_MYVIDEO                   0x000e        // "My Videos" folder
// #define CSIDL_DESKTOPDIRECTORY          0x0010        // <user name>\Desktop
// #define CSIDL_DRIVES                    0x0011        // My Computer
// #define CSIDL_NETWORK                   0x0012        // Network Neighborhood (My Network Places)
// #define CSIDL_NETHOOD                   0x0013        // <user name>\nethood
// #define CSIDL_FONTS                     0x0014        // windows\fonts
// #define CSIDL_TEMPLATES                 0x0015
// #define CSIDL_COMMON_STARTMENU          0x0016        // All Users\Start Menu
// #define CSIDL_COMMON_PROGRAMS           0X0017        // All Users\Start Menu\Programs
// #define CSIDL_COMMON_STARTUP            0x0018        // All Users\Startup
// #define CSIDL_COMMON_DESKTOPDIRECTORY   0x0019        // All Users\Desktop
// #define CSIDL_APPDATA                   0x001a        // <user name>\Application Data
// #define CSIDL_PRINTHOOD                 0x001b        // <user name>\PrintHood
// 
// #ifndef CSIDL_LOCAL_APPDATA
// #define CSIDL_LOCAL_APPDATA             0x001c        // <user name>\Local Settings\Applicaiton Data (non roaming)
// #endif // CSIDL_LOCAL_APPDATA
// 
// #define CSIDL_ALTSTARTUP                0x001d        // non localized startup
// #define CSIDL_COMMON_ALTSTARTUP         0x001e        // non localized common startup
// #define CSIDL_COMMON_FAVORITES          0x001f
// 
// #ifndef _SHFOLDER_H_
// #define CSIDL_INTERNET_CACHE            0x0020
// #define CSIDL_COOKIES                   0x0021
// #define CSIDL_HISTORY                   0x0022
// #define CSIDL_COMMON_APPDATA            0x0023        // All Users\Application Data
// #define CSIDL_WINDOWS                   0x0024        // GetWindowsDirectory()
// #define CSIDL_SYSTEM                    0x0025        // GetSystemDirectory()
// #define CSIDL_PROGRAM_FILES             0x0026        // C:\Program Files
// #define CSIDL_MYPICTURES                0x0027        // C:\Program Files\My Pictures
// #endif // _SHFOLDER_H_
// 
// #define CSIDL_PROFILE                   0x0028        // USERPROFILE
// #define CSIDL_SYSTEMX86                 0x0029        // x86 system directory on RISC
// #define CSIDL_PROGRAM_FILESX86          0x002a        // x86 C:\Program Files on RISC
// 
// #ifndef _SHFOLDER_H_
// #define CSIDL_PROGRAM_FILES_COMMON      0x002b        // C:\Program Files\Common
// #endif // _SHFOLDER_H_
// 
// #define CSIDL_PROGRAM_FILES_COMMONX86   0x002c        // x86 Program Files\Common on RISC
// #define CSIDL_COMMON_TEMPLATES          0x002d        // All Users\Templates
// 
// #ifndef _SHFOLDER_H_
// #define CSIDL_COMMON_DOCUMENTS          0x002e        // All Users\Documents
// #define CSIDL_COMMON_ADMINTOOLS         0x002f        // All Users\Start Menu\Programs\Administrative Tools
// #define CSIDL_ADMINTOOLS                0x0030        // <user name>\Start Menu\Programs\Administrative Tools
// #endif // _SHFOLDER_H_
// 
// #define CSIDL_CONNECTIONS               0x0031        // Network and Dial-up Connections
// #define CSIDL_COMMON_MUSIC              0x0035        // All Users\My Music
// #define CSIDL_COMMON_PICTURES           0x0036        // All Users\My Pictures
// #define CSIDL_COMMON_VIDEO              0x0037        // All Users\My Video
// #define CSIDL_RESOURCES                 0x0038        // Resource Direcotry
// 
// #ifndef _SHFOLDER_H_
// #define CSIDL_RESOURCES_LOCALIZED       0x0039        // Localized Resource Direcotry
// #endif // _SHFOLDER_H_
// 
// #define CSIDL_COMMON_OEM_LINKS          0x003a        // Links to All Users OEM specific apps
// #define CSIDL_CDBURN_AREA               0x003b        // USERPROFILE\Local Settings\Application Data\Microsoft\CD Burning
//     // unused                               0x003c
// #define CSIDL_COMPUTERSNEARME           0x003d        // Computers Near Me (computered from Workgroup membership)
// 
// #ifndef _SHFOLDER_H_
// #define CSIDL_FLAG_CREATE               0x8000        // combine with CSIDL_ value to force folder creation in SHGetFolderPath()
// #endif // _SHFOLDER_H_
// 
// #define CSIDL_FLAG_DONT_VERIFY          0x4000        // combine with CSIDL_ value to return an unverified folder path
// #define CSIDL_FLAG_NO_ALIAS             0x1000        // combine with CSIDL_ value to insure non-alias versions of the pidl
// #define CSIDL_FLAG_PER_USER_INIT        0x0800        // combine with CSIDL_ value to indicate per-user init (eg. upgrade)
// #define CSIDL_FLAG_MASK                 0xFF00        // mask for all possible flag values
// }
// m_path[0]=0;
// HRESULT result=SHGetSpecialFolderPath(NULL,m_path,csidl,fCreate==true);

}}}

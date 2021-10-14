#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include <Services/Config.h>
namespace std { namespace experimental { namespace filesystem { inline namespace v1 {class path;}}}}
namespace fs = std::experimental::filesystem::v1;

namespace DNVS { namespace MoFa { namespace Services {
    class SERVICES_IMPORT_EXPORT SpecialFolder {
    public:
        enum FolderName {
            Personal,       //My Documents
            MyMusic,        //My Music
            AppData,        //Application Data
            LocalAppData,   // non roaming, user\Local Settings\Application Data
            InternetCache,
            Cookies,
            History,
            CommonAppData,  // All Users\Application Data
            Windows,        // GetWindowsDirectory()
            System,         // GetSystemDirectory()
            ProgramFiles,   // C:\Program Files
            MyPictures,     // My Pictures, new for Win2K
            ProgramFilesCommon,// C:\Program Files\Common 
            CommonDocuments,// All Users\Documents
            Resources,      // %windir%\Resources\, For theme and other windows resources.
            ResourcesLocalized,// %windir%\Resources\<LangID>, for theme and other windows specific resources.
            CommonAdminTools,// All Users\Start Menu\Programs\Administrative Tools
            AdminTools,     // <user name>\Start Menu\Programs\Administrative Tools
            Temp,
        };
        SpecialFolder(FolderName name, bool fCreate=true);
        operator fs::path() const;
        std::string GetName() const;
    private:
        int NameToCSIDL(FolderName name) const;
        FolderName m_name;
        bool m_fCreate;
    };
}}}
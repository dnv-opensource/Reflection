//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Module.h"
#include <Windows.h>
#include <tchar.h>

namespace DNVS {namespace MoFa {namespace Services {

    const std::string Module::FileName() const
    {
        std::string path=PathName();
        char filename[_MAX_FNAME];
        char ext[_MAX_EXT];
        _splitpath_s(path.c_str(),
            0,0,
            0,0,
            filename,_MAX_FNAME,
            ext,_MAX_EXT);
        return std::string(filename)+ext;
    }

    const std::string Module::DirectoryName() const
    {
        std::string path=PathName();
        char drive[_MAX_DRIVE];  //  Disk drive (e.g., "A:")
        char dir[_MAX_DIR];      //  File subdirectory specification
        _splitpath_s(path.c_str(),
            drive,_MAX_DRIVE,
            dir,_MAX_DIR,
            0,0,
            0,0);
        return std::string(drive)+dir;
    }

    const std::string Module::PathName() const
    {
        TCHAR szPath [MAX_PATH + 1] = "";
        HMODULE hModule=HMODULE(m_handle);
        GetModuleFileName (hModule, szPath, MAX_PATH);
        std::basic_string<TCHAR> wfile_name(szPath);
        return std::string(wfile_name.begin(),wfile_name.end());
    }
    bool Module::GetFileResource(unsigned short resource_id,std::string& file_content) const
    {
        HINSTANCE hInstance = (HINSTANCE)m_handle;
        HRSRC hResource = ::FindResource(hInstance, MAKEINTRESOURCE(resource_id), RT_HTML);
        if(!hResource) return false;
        HGLOBAL hHTML = LoadResource(hInstance, hResource);
        DWORD size=SizeofResource(hInstance,hResource);
        if(!hHTML) return false;
        file_content=std::string(reinterpret_cast<char*>(hHTML),size);
        return true;
    }
    Module::Module(const std::string& file_name)
        : m_handle(size_t(GetModuleHandleA(file_name.c_str())))
    {
    }

    size_t Detail::ModuleHandleImpl(void* ptr)
    {
        MEMORY_BASIC_INFORMATION mbiInfo = { 0 };
        SIZE_T dwRet = 0;

        dwRet = VirtualQuery(((LPCVOID)(ptr)), &mbiInfo,
            ((DWORD)(sizeof(MEMORY_BASIC_INFORMATION))));
        if (dwRet)
        {
            return size_t(mbiInfo.AllocationBase);
        }
        else return 0;
    }

}}}
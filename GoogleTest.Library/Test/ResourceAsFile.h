#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <Services/Module.h>
#include <Services/SpecialFolder.h>
#include <filesystem>
#include <gtest/gtest.h>

#include <windows.h>
#include <fstream>

namespace DNVS { namespace MoFa { namespace GoogleTest {
    class ResourceAsFile : public Services::Module {
    public:
        inline ResourceAsFile(unsigned short resource_id,const std::string& file_name) : Services::Module() {
            if(!file_name.empty()) {
                DeleteFileA(file_name.c_str());
            }
            if (fs::is_directory(file_name))
                fs::remove(file_name);
            fs::path path = fs::path(file_name).parent_path();
            if(!path.empty() && !fs::is_directory(path))
                fs::create_directories(path);
            DWORD result = GetFileAttributesA(file_name.c_str());
            //file_name already exists. This is an error.
            EXPECT_EQ(0xfffffffful,result); 
            
            std::string file_content;
            EXPECT_TRUE(GetFileResource(resource_id,file_content));
            auto size = file_content.size();
            auto content = file_content.c_str();
            std::ofstream myFile (file_name.c_str(), std::ios::out | std::ios::binary);
            myFile.write (content, size);
            myFile.close();
            m_fileName=file_name;
        }
        inline ~ResourceAsFile()
        {
            if(!m_fileName.empty()) {
                DeleteFileA(m_fileName.c_str());
            }
        }
    private:
        std::string m_fileName;
    };

#   define MOFA_TEST_RESOURCE_AS_FILE(name,ext) DNVS::MoFa::GoogleTest::ResourceAsFile rai##name(IDR_##name,MOFA_TEST_RESOURCE_FILE_NAME(name,ext));
#   define MOFA_TEST_RESOURCE_FILE_NAME(name,ext) (DNVS::MoFa::Services::SpecialFolder(DNVS::MoFa::Services::SpecialFolder::Temp).GetName()+#name"."#ext)
#   define MOFA_TEST_RESOURCE_AS_FILE_RELATIVE(relative, name, ext) DNVS::MoFa::GoogleTest::ResourceAsFile rai##name(IDR_##name,MOFA_TEST_RESOURCE_FILE_NAME_RELATIVE(relative, name, ext));
#   define MOFA_TEST_RESOURCE_FILE_NAME_RELATIVE(relative, name, ext) (DNVS::MoFa::Services::SpecialFolder(DNVS::MoFa::Services::SpecialFolder::Temp).GetName() + #relative + "/" + #name"."#ext)
}}}
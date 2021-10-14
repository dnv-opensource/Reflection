#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include "Config.h"

namespace DNVS {namespace MoFa {namespace Services {
    namespace Detail {
        namespace {
            int memory_on_stack;
        }
        SERVICES_IMPORT_EXPORT size_t ModuleHandleImpl(void* ptr);
        inline size_t ModuleHandle() {
            void* memory = (void*)&memory_on_stack;
            return ModuleHandleImpl(memory);
        }
        inline size_t ModuleHandle(const type_info& info) {
            void* memory = (void*)&info;
            return ModuleHandleImpl(memory);
        }
    }

    class Module {     
    public:
        inline Module() :m_handle(Detail::ModuleHandle()) {}
        Module(size_t handle) : m_handle(handle) {}
        SERVICES_IMPORT_EXPORT Module(const std::string& file_name);
        size_t Handle() const {return m_handle;}
        SERVICES_IMPORT_EXPORT const std::string FileName() const;
        SERVICES_IMPORT_EXPORT const std::string DirectoryName() const;
        SERVICES_IMPORT_EXPORT const std::string PathName() const;
        SERVICES_IMPORT_EXPORT bool GetFileResource(unsigned short resource_id,std::string& file_content) const;
    private:
        size_t m_handle;
    };

    class ApplicationModule : public Module
    {
    public:
        ApplicationModule() : Module(0) {}
    };
}}}
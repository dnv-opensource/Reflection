#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <list>
#include <string>

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    class FileType {
    public:
        FileType(const std::string& extension, const std::string& description)
            : m_extension(extension)
            , m_description(description)
        {}
        const std::string& GetExtension() const { return m_extension; }
        const std::string& GetDescription() const { return m_description; }
    private:
        std::string m_extension;
        std::string m_description;
    };
    namespace Impl {
        struct JournallingRequired {};
    }
    //Enforce scripting of the save command. Otherwise, we would show a check box for whether the command should be scripted.
    constexpr Impl::JournallingRequired JournallingRequired;
    /*
    Indicates that a function represents a Save or Export statement.
    Usdage:
    Class<FemExporter> cls(typeLibrary,"FemExporter");
    cls.Constructor();
    cls.Function("DoExport")
    .AddAttribute(FileType("FEM", "Finite Element Model"), FileType("SIN", "Binary representation"),JournallingRequired);    
    */
    class SaveFileAttribute
    {
    public:
        SaveFileAttribute()
            : m_journallingRequired(false)
        {}
        template<typename... Args>
        SaveFileAttribute(const FileType& fileType, const Args&... args)
            : SaveFileAttribute(args...)
        {
            m_fileTypes.push_front(fileType);
        }
        template<typename... Args>
        SaveFileAttribute(const Impl::JournallingRequired& journallingRequired, const Args&... args)
            : SaveFileAttribute(args...)
        {
            m_journallingRequired = true;
        }
        template<typename... Args>
        SaveFileAttribute(const std::function<std::string()>& initialFolderCallback, const Args&... args)
            : SaveFileAttribute(args...)
        {
            m_initialFolderCallback = initialFolderCallback;
        }
        bool GetJournallingRequired() const { return m_journallingRequired; }
        const std::list<FileType>& GetFileTypes() const { return m_fileTypes; }
        const std::function<std::string()>& GetInitialFolderCallback() const { return m_initialFolderCallback; }
    private:
        std::list<FileType> m_fileTypes;
        bool m_journallingRequired;
        std::function<std::string()> m_initialFolderCallback;
    };
}}}}

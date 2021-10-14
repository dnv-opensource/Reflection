#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include "Reflection/Layout/ILayoutElement.h"
#include "Reflection/Layout/Header/Header.h"
#include "Reflection/Layout/Image.h"
#include "Reflection/Layout/Caption.h"
#include "IAttribute.h"


namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {
    /*
    Represents the documentation for a type or member.
    */
    class DocumentationAttribute : public IAttribute
    {
    public:
        DocumentationAttribute() {}
        template<typename... Args>
        DocumentationAttribute(const Layout::Header& header, Args&&... args)
            : DocumentationAttribute(std::forward<Args>(args)...)
        {
            m_docElements.push_front(std::make_shared<Layout::Header>(header));
        }
        template<typename... Args>
        DocumentationAttribute(const Layout::Image& image, Args&&... args)
            : DocumentationAttribute(std::forward<Args>(args)...)
        {
            m_docElements.push_front(std::make_shared<Layout::Image>(image));
        }
        template<typename... Args>
        DocumentationAttribute(const Layout::Caption& caption, Args&&... args)
            : DocumentationAttribute(std::forward<Args>(args)...)
        {
            m_docElements.push_front(std::make_shared<Layout::Caption>(caption));
        }
        template<typename... Args>
        DocumentationAttribute(const std::string& doc, Args&&... args)            
            : DocumentationAttribute(std::forward<Args>(args)...)
        {
            m_docElements.push_front(std::make_shared<Layout::Caption>(doc));
        }
        std::string GetDocumentation() const
        {
            std::string doc;
            for (const auto& element : m_docElements)
            {
                if (!doc.empty())
                    doc += "\n";
                if (auto caption = std::dynamic_pointer_cast<Layout::Caption>(element))
                    doc += caption->GetCaption();
                else if (auto header = std::dynamic_pointer_cast<Layout::Header>(element))
                    doc += "*" + header->GetColumns()[0]->GetHeader(nullptr, typeid(void), false) + "*";
                else if (auto image = std::dynamic_pointer_cast<Layout::Image>(element))
                    doc += "<Image>";
            }
            return doc;
        }
        const std::list<std::shared_ptr<Layout::ILayoutElement>>& GetDocumentationElements() const
        {
            return m_docElements;
        }
    private:
        std::list<std::shared_ptr<Layout::ILayoutElement>> m_docElements;
    };
}}}}
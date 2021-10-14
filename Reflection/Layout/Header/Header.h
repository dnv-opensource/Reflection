#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IColumn.h"
#include "DefaultColumn.h"
#include "CallbackColumn.h"
#include "PropertyColumn.h"
#include <vector>
#include "Reflection/Layout/ILayoutElement.h"
#include "Reflection/Layout/TypedLayoutElement.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Layout {
#pragma warning(push)
#pragma warning(disable:4275)
    class REFLECTION_IMPORT_EXPORT Header : public TypedLayoutElement<Header, ILayoutElement>
    {
    public:
        Header();
        Header(const Header&) = default;
        Header(Header&&) = default;
        Header& operator=(const Header&) = default;
        Header& operator=(Header&&) = default;
        explicit Header(const std::vector<std::shared_ptr<IColumn>>& columns, bool showHeader = true);
        template<typename... Columns>
        explicit Header(const Columns&... columns)
        {
            Initialize(columns...);
        }
        const std::vector<std::shared_ptr<IColumn>>& GetColumns() const;
        bool ShowHeader() const;
        template<typename TColumn>
        std::enable_if_t<std::is_base_of_v<IColumn, TColumn>> AddColumn(const TColumn& column)
        {
            m_columns.push_back(std::make_shared<TColumn>(column));
        }
        void SetShowHeader(bool flag) { m_showHeader = flag; }
    private:
        bool m_showHeader;
        std::vector<std::shared_ptr<IColumn>> m_columns;
    private:
        template<typename TColumn, typename... Columns>
        std::enable_if_t<std::is_base_of_v<IColumn, TColumn>> Initialize(const TColumn& column, const Columns&... columns)
        {
            m_columns.push_back(std::make_shared<TColumn>(column));
            Initialize(columns...);
        }
        template<typename... Columns>
        void Initialize(const std::string& column, const Columns&... columns)
        {
            Initialize(DefaultColumn(column), columns...);
        }
        template<typename... Columns>
        void Initialize(const char* column, const Columns&... columns)
        {
            Initialize(DefaultColumn(column), columns...);
        }
        void Initialize(bool showHeader)
        {
            m_showHeader = showHeader;
        }
        void Initialize()
        {
            m_showHeader = true;
        }
    };
#pragma warning(pop)
}}}}

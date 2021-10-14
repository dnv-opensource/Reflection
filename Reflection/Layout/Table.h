#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlElement.h"
#include "AutoControl.h"
#include "Header\Header.h"
#include "TypeUtilities/Tribool.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class Table : public TypedLayoutElement<Table, ControlElement>
    {
    public:
        Table(const Table&) = default;
        Table(Table&&) = default;
        Table& operator=(const Table&) = default;
        Table& operator=(Table&&) = default;

        Table(const AutoControl& other) : TypedLayoutElement<Table, ControlElement>(other), m_initialNumberOfRows(6), m_hasSorting(TypeUtilities::Indeterminate) {}
        explicit Table(const std::string& name, const Header& header, int initlaiNumberOfRows = 6, TypeUtilities::Tribool hasSorting = TypeUtilities::Indeterminate)
            : Table(name, "__USENAME__", header, initlaiNumberOfRows, hasSorting)
        {}
        explicit Table(const Header& header, int initlaiNumberOfRows = 6, TypeUtilities::Tribool hasSorting = TypeUtilities::Indeterminate)
            : Table("", "__USENAME__", header, initlaiNumberOfRows, hasSorting)
        {}
        explicit Table(const std::string& name, const std::string& caption, const Header& header, int initlaiNumberOfRows = 6, TypeUtilities::Tribool hasSorting = TypeUtilities::Indeterminate)
            : TypedLayoutElement<Table, ControlElement>(name, caption)
            , m_header(header)
            , m_initialNumberOfRows(initlaiNumberOfRows)
            , m_hasSorting(hasSorting)
        {}
        size_t GetInitialNumberOfRows() const { return m_initialNumberOfRows; }
        const Header& GetHeader() const { return m_header; }
        TypeUtilities::Tribool HasSorting() const { return m_hasSorting; }

        void SetHeader(const Header& header) { m_header = header; }
        void SetInitialNumberOfRows(size_t initialNumberOfRows) { m_initialNumberOfRows = initialNumberOfRows; }
        void SetHasSorting(TypeUtilities::Tribool hasSorting) { m_hasSorting = hasSorting; }
    private:
        size_t m_initialNumberOfRows;
        Header m_header;
        TypeUtilities::Tribool m_hasSorting;
    };
}}}}

#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ILayoutElementGroupCreator.h"
#include "Reflection/Attributes/RecordAttribute.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class RecordAttributeGroupCreator : public ILayoutElementGroupCreator
    {
    public:
        RecordAttributeGroupCreator(const Attributes::RecordAttribute& recordAttribute);
        void AddLayoutElement(const std::shared_ptr<ILayoutElement>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode) override;
        std::shared_ptr<ILayoutElement> GetGroup() const override;
    private:
        std::shared_ptr<ILayoutElement> GetAt(size_t row, size_t col) const;
        void SetAt(size_t row, size_t col, const std::shared_ptr<ILayoutElement>& layoutElement);
        size_t GetIndex(size_t row, size_t col) const;
        size_t GetRowIndex(const Objects::Object& row) const;
        size_t GetColumnIndex(const Objects::Object& col) const;

        std::vector<std::shared_ptr<ILayoutElement>> m_grid;
        std::vector<Variants::Variant> m_rows;
        std::vector<Variants::Variant> m_columns;
        const Attributes::RecordAttribute& m_recordAttribute;
        std::string m_rowIndexName;
        std::string m_columnIndexName;
    };
}}}}
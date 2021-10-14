#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "CollectionOfElements.h"
#include "TypedLayoutElement.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    class GridElements : public TypedLayoutElement<GridElements, CollectionOfElements>
    {
    public:
        GridElements(GridElements&& other) = default;
        GridElements(const GridElements& other) = default;
        GridElements& operator=(GridElements&& other) = default;
        GridElements& operator=(const GridElements& other) = default;

        template<typename... ElementsT>
        explicit GridElements(size_t rows, size_t cols, ElementsT&&... args)
            : TypedLayoutElement<GridElements, CollectionOfElements>(std::forward<ElementsT>(args)...)
        {
            m_rows = rows;
            m_cols = cols;
        }
        size_t GetRows() const { return m_rows; }
        size_t GetCols() const { return m_cols; }
    private:
        size_t m_rows;
        size_t m_cols;
    };
}}}}
